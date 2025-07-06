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
#include <list>

/**
 * @class CvPlotRenderer
 * @brief A class for rendering plots with customizable themes, legends, and data.
 *
 * This class provides functionality to render plots with a specified history size,
 * update rate, and axis range. It supports adding multiple data points, setting
 * legends, and switching between themes.
 */
class CvPlotRenderer final : NonCopyable
{
    public:
        /**
         * @enum Theme
         * @brief Enum for specifying the plot theme.
         */
        enum class Theme
        {
            WHITE,
            DARK,
        };

    public:
        /**
         * @brief Constructor for CvPlotRenderer.
         *
         * @param graph_name The name of the graph.
         * @param history_size The number of historical data points to store (default: 200).
         */
        CvPlotRenderer(std::string graph_name, int history_size=200);

        /**
         * @brief Set the legends for the plot.
         *
         * @param legend_strings A vector of strings representing the legends.
         */
        void set_legends(std::vector<std::string> legend_strings);

        /**
        * @brief Set the theme for the plot.
        *
        * @param theme The theme to set (Theme::WHITE or Theme::DARK).
        */
        void set_theme(Theme theme);

        /**
         * @brief Add a set of values to the plot.
         *
         * @param time The time value for the x-axis.
         * @param x1 The first data value for the y-axis.
         * @param x2 The second data value for the y-axis.
         * @param x3 The third data value for the y-axis.
         * @param x4 The fourth data value for the y-axis.
         */
        void add_values(float time, float x1, float x2, float x3, float x4);

        /**
         * @brief Generate the plot with a specified axis range and draw it in a window.
         *
         * @param axis_range A pair of floats representing the minimum and maximum values for the axis.
         */
        void generate_plot(std::pair<float, float> axis_range);

        void set_x_vs_y();

    private:
        int                             _plot_width, _plot_height;
        int                             _history_size;
        std::list<float>                _x_vec;
        std::list<std::vector<float>>   _y_vec;
        std::vector<std::string>        _legend_strings;
        std::string                     _graph_name;
        Theme                           _theme;
        bool                            _enable_stretch;
        bool                            _x_vs_y;
};
