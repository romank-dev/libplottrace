/* Copyright 2025 Roman Kudinov. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstring>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <libplot_trace/PlotRelayServer.hpp>
#include "logging.hpp"

using namespace std;
using std::placeholders::_1;

PlotRelayServer::PlotRelayServer() :
    _server(IpSocketAddress(IpAddress::AnyAddress, DEFAULT_PLOT_RELAY_PORT), true),
    _acceptor_thread(bind(&PlotRelayServer::tcp_server_loop, this, _1)),
    _pipe_fd(socket(AF_UNIX, SOCK_DGRAM, 0))
{
    CHECK_THROW_POSIX(_pipe_fd >= 0, "socket() failed!");

    FileSystemPath pipe_path(PLOT_PIPE_PATH);
    if(pipe_path.exists())
    {
        CHECK_THROW_POSIX(!remove(pipe_path.str().c_str()), "remove() failed");
    }

    struct sockaddr_un name;
    name.sun_family = AF_UNIX;
    strcpy(name.sun_path, PLOT_PIPE_PATH);

    CHECK_THROW_POSIX(!bind(_pipe_fd, (struct sockaddr *) &name, sizeof(struct sockaddr_un)), "bind() failed");

}

PlotRelayServer::~PlotRelayServer()
{
    SAFE_DESTRUCTOR
    (
        close(_pipe_fd);
    )
}

void PlotRelayServer::refresh_cache()
{
    _graphs_cache.get([&](map<string, uint32_t>& cache)
    {
        for(auto it = cache.begin(); it != cache.end(); ++it)
        {
            auto diff_ms = _timer.get_ms() - it->second;
            if(diff_ms > graph_TIMEOUT_MS)
            {
                cache.erase(it);
                it = cache.begin();
            }
        }
    });
}

void PlotRelayServer::handle_packet(const PlotPacket& packet)
{
    _clients.get([&](vector<ClientInfo>& clients)
    {
        for(int i = 0; i < clients.size(); ++i)
        {
            auto& client = clients[i];
            for(const auto& graph : client.requested_graphs())
            {
                if(!strncmp(graph.c_str(), packet.graph_name, sizeof(packet.graph_name)))
                {
                    // the if statement below protects us from overloading the client's send queue, which can
                    // happen if his network connection reached it's peak performance. Overloading the send-Q
                    // will cause send() to hang until data is sent and finally throw exception, destroying the connection.
                    if(client.last_send_time() > 0 && (_timer.get_ms() - client.last_send_time() < client.last_send_duration()))
                    {
                        continue; // do not send any more data to the client who can't receive our data on time
                    }

                    try
                    {
                        Stopwatch send_time;
                        auto& client_sock = client.socket();
                        client_sock->send(&packet, sizeof(packet));
                        client.last_send_time() = _timer.get_ms();
                        client.last_send_duration() = send_time.get_ms();
                    }
                    catch(const std::exception& ex)
                    {
                        clients.erase(clients.begin()+i);
                        i--;
                    }
                    break;
                }
            }
        }
    });
}

void PlotRelayServer::run_loop()
{
    PlotPacket packet;
    while(true)
    {
        int actual_image_size;
        int num_read;
        CHECK_THROW_POSIX((num_read = read(_pipe_fd, &packet, sizeof(packet))) > 0, "read() failed");
        _graphs_cache.get([&](map<string, uint32_t>& cache)
        {
            cache[string(packet.graph_name)] = _timer.get_ms();
        });

        handle_packet(packet);

        refresh_cache();
    }
}

void PlotRelayServer::tcp_server_loop(Thread& t)
{
    while(!t.should_quit())
    {
        TcpSocket::sptr client_sock = _server.accept_connection();
        static const int SMALL_TOLERANCE_FOR_SEND = 500;
        client_sock->set_send_timeout(SMALL_TOLERANCE_FOR_SEND);
        client_sock->set_receive_timeout(SMALL_TOLERANCE_FOR_SEND);
        int flag = 1;
        client_sock->set_sock_opt(IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));
        vector<string> requested_graphs_list;
        try
        {
            int graph_str_size;
            client_sock->receive(&graph_str_size, sizeof(graph_str_size));
            vector<char> requested_graphs(graph_str_size);
            client_sock->receive(requested_graphs.data(), graph_str_size);

            string requested_graphs_str(requested_graphs.data(), requested_graphs.size());

            std::stringstream ss(requested_graphs_str);
            std::string item;
            std::vector<std::string> elems;
            while (std::getline(ss, item, ' '))
            {
                requested_graphs_list.push_back(item);
            }

            string result_graphs = "";
            _graphs_cache.get([&](map<string, uint32_t>& cache)
            {
                for(auto it = cache.begin(); it != cache.end(); ++it)
                {
                    string graph = it->first;
                    result_graphs += graph + " ";
                }
            });

            graph_str_size = result_graphs.size();
            client_sock->send(&graph_str_size, sizeof(graph_str_size));
            client_sock->send(result_graphs.c_str(), graph_str_size);
        }
        catch(const Exception& e)
        {
            TRACE_ERR("Exception during client registration: %s",e.full_message().c_str());
            continue;
        }

        _clients->get().push_back(ClientInfo(client_sock, requested_graphs_list));
    }
}


PlotRelayServer::ClientInfo::ClientInfo(const TcpSocket::sptr& socket, const vector<std::string>& graphs) :
    _socket(socket),
    _requested_graphs(graphs),
    _last_send_time(0),
    _last_send_duration(0)
{}


TcpSocket::sptr& PlotRelayServer::ClientInfo::socket()
{
    return _socket;
}

std::vector<std::string>& PlotRelayServer::ClientInfo::requested_graphs()
{
    return _requested_graphs;
}

uint32_t& PlotRelayServer::ClientInfo::last_send_time()
{
    return _last_send_time;
}

uint32_t& PlotRelayServer::ClientInfo::last_send_duration()
{
    return _last_send_duration;
}




















