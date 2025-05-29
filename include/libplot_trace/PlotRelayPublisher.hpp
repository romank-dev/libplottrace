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
#include <libplot_trace/PlotRelayProtocol.hpp>

class PlotRelayPublisher final : NonCopyable
{
    public:
        PlotRelayPublisher();
        ~PlotRelayPublisher();

        void publish(const char* graph_name, uint32_t graph_name_len, float stamp, const char* curve_1, uint32_t curve_1_len, float value_1);
        void publish(const char* graph_name, uint32_t graph_name_len, float stamp, const char* curve_1, uint32_t curve_1_len, float value_1, const char* curve_2, uint32_t curve_2_len, float value_2);
        void publish(const char* graph_name, uint32_t graph_name_len, float stamp, const char* curve_1, uint32_t curve_1_len, float value_1, const char* curve_2, uint32_t curve_2_len, float value_2, const char* curve_3, uint32_t curve_3_len, float value_3);
        void publish(const char* graph_name, uint32_t graph_name_len, float stamp, const char* curve_1, uint32_t curve_1_len, float value_1, const char* curve_2, uint32_t curve_2_len, float value_2, const char* curve_3, uint32_t curve_3_len, float value_3, const char* curve_4, uint32_t curve_4_len, float value_4);

    private:
        int     _fd;
};
