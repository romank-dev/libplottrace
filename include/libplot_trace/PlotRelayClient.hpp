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

#include <libplot_trace/PlotRelayProtocol.hpp>
#include <libnet/TcpSocket.hpp>

class PlotRelayClient final : NonCopyable
{
    public:
        explicit PlotRelayClient(const std::string& target_ip, const std::vector<std::string>& topics);

        const std::vector<std::string>& get_graphs();

        void receive_graph(PlotPacket& pkt);

    private:
        TcpSocket                   _client;
        std::vector<std::string>    _graphs_snapshot;
};
