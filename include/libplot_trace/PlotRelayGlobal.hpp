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

#include <libplot_trace/PlotRelayPublisher.hpp>

extern PlotRelayPublisher g_plot_pub;

#define GET_MACRO(_1,_2,_3,_4,_5,_6,_7,_8,NAME,...) NAME
#define TRACE_PLOT(graph_name, stamp, ...) GET_MACRO(__VA_ARGS__, TRACE_PLOT8, TRACE_PLOT7, TRACE_PLOT6, TRACE_PLOT5, TRACE_PLOT4, TRACE_PLOT3, TRACE_PLOT2)(graph_name, stamp, ##__VA_ARGS__)

#define TRACE_PLOT2(graph_name, stamp, name1, value1) g_plot_pub.publish(graph_name, sizeof(graph_name), stamp, name1, sizeof(name1), value1)
#define TRACE_PLOT4(graph_name, stamp, name1, value1, name2, value2) g_plot_pub.publish(graph_name, sizeof(graph_name), stamp, name1, sizeof(name1), value1, name2, sizeof(name2), value2)
#define TRACE_PLOT6(graph_name, stamp, name1, value1, name2, value2, name3, value3) g_plot_pub.publish(graph_name, sizeof(graph_name), stamp, name1, sizeof(name1), value1, name2, sizeof(name2), value2, name3, sizeof(name3), value3)
#define TRACE_PLOT8(graph_name, stamp, name1, value1, name2, value2, name3, value3, name4, value4) g_plot_pub.publish(graph_name, sizeof(graph_name), stamp, name1, sizeof(name1), value1, name2, sizeof(name2), value2, name3, sizeof(name3), value3, name4, sizeof(name4), value4)
