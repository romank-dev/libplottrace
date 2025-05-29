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


class CvPlotHandler final : NonCopyable
{
    public:

        enum class Theme
        {
            WHITE,
            DARK,
        };

    public:
        CvPlotHandler(std::string graph_name, int history_size=200, int rate=20);

        void set_legends(std::vector<std::string> legend_strings);
        void set_theme(Theme theme);

        void add_values(float time, float x1, float x2, float x3, float x4);

        void generate_plot(std::pair<float, float> axis_range);
        void set_x_vs_y();
    private:
        int                             _plot_width, _plot_height;
        int                             _history_size;
        std::list<float>                _x_vec;
        std::list<std::vector<float>>   _y_vec;
        int                             _rate;
        std::vector<std::string>        _legend_strings;
        std::string                     _graph_name;
        Theme                           _theme;
        bool                            _enable_stretch;
        bool                            _x_vs_y;
};
