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

#include <iostream>
#include <libplot_trace/CvPlot.hpp>
#include <opencv2/opencv.hpp>
#include <libcommon/libcommon.hpp>
using namespace cv;
using namespace std;

int PlotGraph(Mat & x, Mat& y) {

    //converting the Mat to CV_64F

    Mat plot_result;

    Ptr<plot::Plot2d> plot = plot::Plot2d::create(x, y);
    vector<string> strings = {"asdf", "qwer"};
    plot->setLegendStrings(strings);
    plot->render(plot_result);

    imshow("Graph", plot_result);
    waitKey(10);

    return 0;
}

int main()
{
    Stopwatch t;
    while (t.get_ms() < 20000)
    {
        Mat x(1000, 1, CV_64F);
        Mat y(1000, 2, CV_64F);
        for (int i = 0; i < x.rows; i++)
        {
            double time = 0.001*((float)t.get_ms() + i);
            x.at<double>(i) = time;
            y.at<double>(i, 0) = 4*sin(time*4);
            y.at<double>(i, 1) = 1*cos(time*4);
        }
        PlotGraph(x, y);
//        std::this_thread::sleep_for(chrono::milliseconds(10));
//        printf("fdasdf\n");
    }
    return 0;
}
