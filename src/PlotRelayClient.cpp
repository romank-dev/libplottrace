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

#include <sys/socket.h>
#include <sys/types.h>
#include <cstring>
#include <sys/un.h>
#include <unistd.h>
#include <sstream>
#include <libplot_trace/PlotRelayClient.hpp>


#define CLIENT_RECEIVE_TIMEOUT_MS 1000

using namespace std;

PlotRelayClient::PlotRelayClient(const string& target_ip, const vector<string>& topics) :
    _client(IpSocketAddress(target_ip, DEFAULT_PLOT_RELAY_PORT), 1000)
{
    _client.set_receive_timeout(1000);
    string graphs_str;
    for(const auto& topic : topics)
    {
        graphs_str += topic.c_str() + string(" ");
    }

    int graph_str_size = graphs_str.size();
    _client.send(&graph_str_size, sizeof(graph_str_size));
    _client.send(graphs_str.c_str(), graphs_str.size());

    _client.receive(&graph_str_size, sizeof(graph_str_size));
    vector<char> remote_graphs(graph_str_size);
    _client.receive(remote_graphs.data(), remote_graphs.size());

    string remote_graphs_str(remote_graphs.data(), remote_graphs.size());
    std::stringstream ss(remote_graphs_str);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, ' '))
    {
        _graphs_snapshot.push_back(item);
    }
}

const vector<string>& PlotRelayClient::get_graphs()
{
    return _graphs_snapshot;
}

void PlotRelayClient::receive_graph(PlotPacket& pkt)
{
    _client.receive(&pkt, sizeof(pkt));
}

