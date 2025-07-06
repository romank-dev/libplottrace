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

#include <libplot_trace/CvPlot.hpp>
#include <libplot_trace/CvPlotRenderer.hpp>

using namespace cv;
using namespace std;


CvPlotRenderer::CvPlotRenderer(string graph_name, int history_size) :
    _plot_width(600), _plot_height(400),
    _history_size(history_size),
    _graph_name(graph_name),
    _theme(Theme::WHITE),
    _enable_stretch(false),
    _x_vs_y(false)
{

}

void CvPlotRenderer::set_x_vs_y()
{
    _x_vs_y = true;
}

void CvPlotRenderer::set_legends(vector<string> legend_strings)
{
    _legend_strings = legend_strings;
}

void CvPlotRenderer::set_theme(CvPlotRenderer::Theme theme)
{
    _theme = theme;
}

void CvPlotRenderer::add_values(float time, float x1, float x2, float x3, float x4)
{
    vector<float> y_vals;
    if (_x_vec.back() > time)
    {
        _x_vec.clear();
        _y_vec.clear();
    }
    _x_vec.push_back(time);
    y_vals.push_back(x1);
    if (_legend_strings.size() >= 2)
        y_vals.push_back(x2);
    if (_legend_strings.size() >= 3)
        y_vals.push_back(x3);
    if (_legend_strings.size() == 4)
        y_vals.push_back(x4);
    _y_vec.push_back(y_vals);
    if (_x_vec.size() > _history_size)
    {
        _x_vec.pop_front();
        _y_vec.pop_front();
    }
}

void CvPlotRenderer::generate_plot(std::pair<float, float> axis_range)
{
    Mat plot_result;
    int n = _x_vec.size();
    int m = _y_vec.begin()->size();
    Mat x(n, 1, CV_64F);
    Mat y(n, m, CV_64F);
    int i = 0;
    for (auto &v : _y_vec)
    {
        for (int j = 0 ; j < m; j++)
            y.at<double>(i, j) = v[j];
        i++;
    }
    i = 0;
    for (auto &v : _x_vec)
        x.at<double>(i++) = v;

    Ptr<plot::Plot2d> plot = plot::Plot2d::create(x, y);
    plot->setPlotSize(_plot_width, _plot_height);
    //plot->setLegendStrings(_legend_strings);
    if (axis_range.first != axis_range.second)
    {
        plot->setMinY(axis_range.first);
        plot->setMaxY(axis_range.second);
    }

    if (_theme == CvPlotRenderer::Theme::DARK)
    {
        plot->setPlotAxisColor(Scalar(200, 200, 200));
        plot->setPlotGridColor(Scalar(155, 155, 155));
        plot->setPlotBackgroundColor(Scalar(0, 0, 0));
        plot->setPlotTextColor(Scalar(255, 255, 255));
    }
    if (_x_vs_y)
        plot->setNeedPlotLine(false);
    plot->render(plot_result);
    if (_enable_stretch)
        namedWindow(_graph_name.c_str(), WINDOW_NORMAL);
    else
        namedWindow(_graph_name.c_str(), WINDOW_AUTOSIZE);
    imshow(_graph_name.c_str(), plot_result);
    int k = waitKey(5);

    if (k == 45) // '-' shrink
    {
        _plot_height = (int)(_plot_height * 0.9);
        _plot_width = (int)(_plot_width * 0.9);
    }
    if (k == 61 || k == 43)
    {
        _plot_height = (int)(_plot_height * 1.1);
        _plot_width = (int)(_plot_width * 1.1);
    }
    if (k == 104) // help
    {
        printf("-\tshrink\n+\tgrow\n\ts\tAllow stretching window\n");
    }
    if (k == 115)
    {
        cv::destroyWindow(_graph_name);
        _enable_stretch = !_enable_stretch;
        if (_enable_stretch)
            printf("Enable stretch\n");
        else
            printf("Disable stretch\n");
    }

}
