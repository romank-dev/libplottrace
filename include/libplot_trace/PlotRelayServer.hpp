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


#pragma once
#include <map>
#include <libnet/libnet.hpp>
#include <libcommon/libcommon.hpp>
#include <libplot_trace/PlotRelayProtocol.hpp>

/**
 * @class PlotRelayServer
 * @brief A broker between plot data publishers and plot data clients.
 *
 * The `PlotRelayServer` class manages TCP client connections that request
 * graph data, listens on the plot UNIX socket for packets from plot publishers,
 * and distributes plot data that is relevant to each client.
 */
class PlotRelayServer final : NonCopyable
{
    public:
        /**
        * @brief Constructs a new `PlotRelayServer` object.
        */
        PlotRelayServer();

        /**
         * @brief Destroys the `PlotRelayServer` object.
         */
        ~PlotRelayServer() = default;

        /**
         * @brief Starts the main server loop.
         */
        void run_loop();

        /**
         * @brief Stops the server loop by signalling the quit event
         */
        void stop();

    private:
        void get_image();
        void tcp_server_loop(Thread&);
        void handle_packet(const PlotPacket& packet);
        void refresh_cache();

        static constexpr int graph_TIMEOUT_MS = 5000;

    /**
     * @class ClientInfo
     * @brief Holds information about a connected client.
     * This class is managed by the `PlotRelayServer` to keep track of each client's
     * socket, requested graphs, and last send times.
     */
    class ClientInfo
    {
        public:
            /**
             * @brief Constructs a `ClientInfo` object.
             * @param socket The TCP socket associated with the client.
             * @param graphs The list of graph names requested by the client.
             */
            ClientInfo(const TcpSocket::sptr& socket, const std::vector<std::string>& graphs);

            /**
             * @brief Gets the TCP socket associated with the client.
             * @return A reference to the client's TCP socket.
             */
            TcpSocket::sptr& socket();

            /**
             * @brief Gets the list of graph names requested by the client.
             * @return A reference to the vector of requested graph names.
             */
            std::vector<std::string>& requested_graphs();

            /**
             * @brief Gets the last send time for the client.
             * @return A reference to the last send time in milliseconds.
             */
            uint32_t& last_send_time();

            /**
             * @brief Gets the last send duration for the client.
             * @return A reference to the last send duration in milliseconds.
             */
            uint32_t& last_send_duration();

        private:
            TcpSocket::sptr             _socket;
            std::vector<std::string>    _requested_graphs;
            uint32_t                    _last_send_time;
            uint32_t                    _last_send_duration;
    };

    private:
        Event                                       _quit_event;
        Stopwatch                                   _timer;
        UnixDatagramSocket                          _listener;
        ThreadSafe<std::vector<ClientInfo>>         _clients;
        TcpServerSocket                             _server;
        ThreadSafe<std::map<std::string, uint32_t>> _graphs_cache; // name -> time
        Thread                                      _acceptor_thread;
};

