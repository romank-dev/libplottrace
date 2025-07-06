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

#include <libgen.h>
#include <thread>
#include <cstdio>
#include <libplot_trace/PlotRelayClient.hpp>
#include <libplot_trace/CvPlotRenderer.hpp>
#include <libnet/Detector.hpp>

using namespace std;

void usage()
{
    printf("USAGE: cv_plot_client [OPTIONS]\n"\
            "\t-h\tspecify host (don't use detector)\n" \
            "\t-q\tquery available plots\n" \
            "\t-s [G]\t show single graph with name G\n" \
            "\n\tOPTIONAL:\n" \
            "\t-d \t\t\tfor dark theme\n" \
            "\t-v [idx_1, idx_2] \tshow for example x_loc vs y_loc\n" \
            "\t-l [history-amount] \t\thow many values to keep[default 1000 samples]\n" \
            "\t-f [index] \t\tDefine which index to show\n" \
            "\t-r [y1 y2] \t\tDefine the range values of the y-axis between y1 and y2\n");
}

int main(int argc, char** argv)
{
    Utils::FileSystem::set_local_cwd();

    if(argc < 2)
    {
        usage();
        return 0;
    }

    string host = "";
    string verb = "";
    string graph = "";
    int history = 1000;
    vector<int> idx;
    int only_idx = -1;
    pair<float, float> range(0, 0);

    CvPlotRenderer::Theme theme = CvPlotRenderer::Theme::WHITE;

    for(int i = 1; i < argc; i++)
    {
        string arg(argv[i]);
        if(arg == "-q")
        {
            if(!verb.empty())
            {
                printf("Error in args (%s %s)\n", verb.c_str(), arg.c_str());
                return 0;
            }
            verb = arg;
        }
        else if(arg == "-h")
        {
            if(!host.empty() || i == argc-1)
            {
                printf("Error in host arg\n");
                return 0;
            }
            host = argv[++i];
        }
        else if(arg == "-s")
        {
            if(!verb.empty())
            {
                printf("Error in args (%s %s)\n", verb.c_str(), arg.c_str());
                return 0;
            }
            if(!graph.empty() ||  i == argc-1)
            {
                printf("Error in topics arg\n");
                return 0;
            }
            graph = argv[++i];
            verb = "-s";
        }
        else if (arg == "-d")
        {
            theme = CvPlotRenderer::Theme::DARK;
        }
        else if (arg == "-v")
        {
            idx.push_back(atoi(argv[++i]));
            idx.push_back(atoi(argv[++i]));
        }
        else if (arg == "-r")
        {
            range.first = atof(argv[++i]);
            range.second = atof(argv[++i]);
        }
        else if (arg == "-l")
        {
            history = atoi(argv[++i]);
        }
        else if (arg == "-f")
        {
            only_idx = atoi(argv[++i]);
            if (only_idx > 3)
            {
                usage();
                return 0;
            }
        }
        else
        {
            printf("Arg parse error at %s\n", arg.c_str());
            return 0;
        }
    }

    if(host.empty())
    {
        Detector d;
        auto results = d.scan_and_collect(250);
        if(results.empty())
        {
            printf("failed to auto-detect server\nFeel free to use plot_relay_client [IP]\n");
            return 0;
        }
        printf("Auto-detected server at %s\n", results[0].second.c_str());
        host = results[0].second;
    }

    if(verb == "-q")
    {
        PlotRelayClient client(host, {});
        auto graphs = client.get_graphs();
        printf("Graphs: \n");

        File f(FileSystemPath("/tmp/plot_opt"), File::Mode::Write);
        for(const auto& graph : graphs)
        {
            char name[32]{};
            int n = sprintf(name, " %s", graph.c_str());
            printf("\t%s\n", name);
            f.write(name, n);
        }
        return 0;
    }
    else if(verb == "-s")
    {
        std::unique_ptr<PlotRelayClient> client{new PlotRelayClient(host, {graph})};
        if (!idx.empty())
        {
            printf("%d Vs %d\n", idx[0], idx[1]);
        }
        CvPlotRenderer plot_h(graph, history);
        plot_h.set_theme(theme);
        bool first = true;

        while(1)
        {
            PlotPacket pkt;
            try
            {
                client->receive_graph(pkt);
            }
            catch(const Exception& e)
            {
                client.reset(new PlotRelayClient(host, {graph}));
                first = true;
                printf("Not receiving data...\n");
                continue;
            }

            if (first)
            {
                first = false;
                if(idx.empty())
                {
                    vector<string> legend;
                    string s1(pkt.curve_1);
                    if (!s1.empty() && (only_idx == -1 || only_idx == 0))
                        legend.push_back(s1);
                    string s2(pkt.curve_2);
                    if (!s2.empty() && (only_idx == -1 || only_idx == 1))
                        legend.push_back(s2);
                    string s3(pkt.curve_3);
                    if (!s3.empty() && (only_idx == -1 || only_idx == 2))
                        legend.push_back(s3);
                    string s4(pkt.curve_4);
                    if (!s4.empty() && (only_idx == -1 || only_idx == 3))
                        legend.push_back(s4);
                    plot_h.set_legends(legend);
                }
                else
                {
                    plot_h.set_x_vs_y();
                }
            }
            if (idx.empty())
            {
                if (only_idx == -1)
                    plot_h.add_values(pkt.stamp, pkt.value_1, pkt.value_2, pkt.value_3, pkt.value_4);
                else
                {
                    switch(only_idx)
                    {
                        case 0:
                            plot_h.add_values(pkt.stamp, pkt.value_1, 0, 0, 0);
                            break;
                        case 1:
                            plot_h.add_values(pkt.stamp, 0, pkt.value_2, 0, 0);
                            break;
                        case 2:
                            plot_h.add_values(pkt.stamp, 0, 0, pkt.value_3, 0);
                            break;
                        case 3:
                            plot_h.add_values(pkt.stamp, 0, 0, 0, pkt.value_4);
                            break;

                    }
                }
            }
            else
            {
                float x;
                float y;
                switch(idx[0])
                {
                    case 0:
                        x = pkt.value_1;
                        break;
                    case 1:
                        x = pkt.value_2;
                        break;
                    case 2:
                        x = pkt.value_3;
                        break;
                    case 3:
                        x = pkt.value_4;
                        break;
                    default:
                        printf("WTF???? what kind of index did you give me?\n");
                        return 0;
                }
                switch(idx[1])
                {
                    case 0:
                        y = pkt.value_1;
                        break;
                    case 1:
                        y = pkt.value_2;
                        break;
                    case 2:
                        y = pkt.value_3;
                        break;
                    case 3:
                        y = pkt.value_4;
                        break;
                    default:
                        printf("WTF???? what kind of index did you give me?\n");
                        return 0;
                }
                plot_h.add_values(x, y, 0, 0, 0);
            }
            plot_h.generate_plot(range);
        }
    }
    else usage();

    return 0;
}
