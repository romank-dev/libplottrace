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

/**
 * This file defines the global (hard-coded) settings for the plot trace library
 */

/**
 * The TCP port that the Plot Relay listens on for plot client apps.
 */
#define DEFAULT_PLOT_RELAY_PORT ((uint16_t)18778)

/**
 * Length of the name of a curve in a graph. Affects packet size & throughput.
 */
#define TEXT_LEN size_t(32)

/**
 * UNIX datagram socket file system path
 */
#define PLOT_PIPE_PATH "/tmp/__plot_relay__"

/**
 * Structure of a single plot packet.
 * If a curve name is empty (starts with '0x0') then that curve value is not to be considered.
 */
struct PlotPacket
{
    char graph_name[TEXT_LEN];
    float stamp;
    char curve_1[TEXT_LEN];
    float value_1;
    char curve_2[TEXT_LEN];
    float value_2;
    char curve_3[TEXT_LEN];
    float value_3;
    char curve_4[TEXT_LEN];
    float value_4;
} __attribute__((packed));
