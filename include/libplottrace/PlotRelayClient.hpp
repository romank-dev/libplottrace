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

#include <libplottrace/PlotRelayProtocol.hpp>
#include <libnet/TcpSocket.hpp>

/**
 * @class PlotRelayClient
 * @brief A client for communicating with the Plot Relay server.
 *
 * This class is responsible for connecting to a Plot Relay server, subscribing to specific topics,
 * and receiving graph data packets.
 */
class PlotRelayClient final : NonCopyable
{
    public:
        /**
         * @brief Constructs a PlotRelayClient instance.
         *
         * @param target_ip The IP address of the target Plot Relay server.
         * @param topics A list of topics to subscribe to.
         */
        explicit PlotRelayClient(const std::string& target_ip, const std::vector<std::string>& topics);

        /**
        * @brief Retrieves the current snapshot of available graphs.
        *
        * @return A reference to a vector containing the names of the graphs.
        */
        const std::vector<std::string>& get_graphs();

        /**
         * @brief Receives a graph data packet from the server.
         *
         * @param pkt A reference to a PlotPacket object to store the received data.
         */
        void receive_graph(PlotPacket& pkt);

    private:
        TcpSocket                   _client;
        std::vector<std::string>    _graphs_snapshot;
};
