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

#include <cstring>
#include <libplottrace/PlotRelayPublisher.hpp>


using namespace std;

PlotRelayPublisher::PlotRelayPublisher() :
    _client()
{}

void PlotRelayPublisher::publish(const char* graph_name, size_t graph_name_len, float stamp, const char* curve_1, size_t curve_1_len, float value_1, const char* curve_2, size_t curve_2_len, float value_2, const char* curve_3, size_t curve_3_len, float value_3)
{
    publish(graph_name, graph_name_len, stamp, curve_1, curve_1_len, value_1, curve_2, curve_2_len, value_2, curve_3, curve_3_len, value_3, nullptr, 0, 0);
}

void PlotRelayPublisher::publish(const char* graph_name, size_t graph_name_len, float stamp, const char* curve_1, size_t curve_1_len, float value_1, const char* curve_2, size_t curve_2_len, float value_2)
{
    publish(graph_name, graph_name_len, stamp, curve_1, curve_1_len, value_1, curve_2, curve_2_len, value_2, nullptr, 0, 0, nullptr, 0, 0);
}

void PlotRelayPublisher::publish(const char* graph_name, size_t graph_name_len, float stamp, const char* curve_1, size_t curve_1_len, float value_1)
{
    publish(graph_name, graph_name_len, stamp, curve_1, curve_1_len, value_1, nullptr, 0, 0, nullptr, 0, 0, nullptr, 0, 0);
}

void PlotRelayPublisher::publish(const char* graph_name, size_t graph_name_len, float stamp, const char* curve_1, size_t curve_1_len, float value_1, const char* curve_2, size_t curve_2_len, float value_2, const char* curve_3, size_t curve_3_len, float value_3, const char* curve_4, size_t curve_4_len, float value_4)
{
    PlotPacket payload_packet {};
    BufferWriter payload_writer(&payload_packet, sizeof(payload_packet));

    payload_writer.write(graph_name, sizeof(payload_packet.graph_name));
    payload_writer.write(stamp);

    if(curve_1 != nullptr)
    {
        curve_1_len = min(curve_1_len, TEXT_LEN);
        payload_writer.write(curve_1, curve_1_len);
        payload_writer.skip(TEXT_LEN-curve_1_len);
        payload_writer.write(value_1);
    }
    else payload_writer.skip(TEXT_LEN + sizeof(float));

    if(curve_2 != nullptr)
    {
        curve_2_len = min(curve_2_len, TEXT_LEN);
        payload_writer.write(curve_2, curve_2_len);
        payload_writer.skip(TEXT_LEN-curve_2_len);
        payload_writer.write(value_2);
    }
    else payload_writer.skip(TEXT_LEN + sizeof(float));

    if(curve_3 != nullptr)
    {
        curve_3_len = min(curve_3_len, TEXT_LEN);
        payload_writer.write(curve_3, curve_3_len);
        payload_writer.skip(TEXT_LEN-curve_3_len);
        payload_writer.write(value_3);
    }
    else payload_writer.skip(TEXT_LEN + sizeof(float));

    if(curve_4 != nullptr)
    {
        curve_4_len = min(curve_4_len, TEXT_LEN);
        payload_writer.write(curve_4, curve_4_len);
        payload_writer.skip(TEXT_LEN-curve_4_len);
        payload_writer.write(value_4);
    }
    else payload_writer.skip(TEXT_LEN + sizeof(float));


    _client.send(&payload_packet, sizeof(payload_packet), UnixSocketAddress(PLOT_PIPE_PATH, false));
}

