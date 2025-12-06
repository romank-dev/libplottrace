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

#include <libcommon/libcommon.hpp>
#include <libplottrace/PlotRelayProtocol.hpp>
#include <libnet/libnet.hpp>

/**
 * @class PlotRelayPublisher
 * @brief A final class responsible for publishing plot data points to the plot relay.
 *
 * This class provides multiple overloaded `publish` methods to send data
 * for up to four curves to a plot relay system. It is non-copyable.
 */
class PlotRelayPublisher final : NonCopyable
{
    public:
        PlotRelayPublisher();
        ~PlotRelayPublisher() = default;

        void publish(const char* graph_name, size_t graph_name_len, float stamp, const char* curve_1, size_t curve_1_len, float value_1);
        void publish(const char* graph_name, size_t graph_name_len, float stamp, const char* curve_1, size_t curve_1_len, float value_1, const char* curve_2, size_t curve_2_len, float value_2);
        void publish(const char* graph_name, size_t graph_name_len, float stamp, const char* curve_1, size_t curve_1_len, float value_1, const char* curve_2, size_t curve_2_len, float value_2, const char* curve_3, size_t curve_3_len, float value_3);
        void publish(const char* graph_name, size_t graph_name_len, float stamp, const char* curve_1, size_t curve_1_len, float value_1, const char* curve_2, size_t curve_2_len, float value_2, const char* curve_3, size_t curve_3_len, float value_3, const char* curve_4, size_t curve_4_len, float value_4);

    private:
        UnixDatagramSocket  _client;
};
