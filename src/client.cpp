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
#include <libcommon/libcommon.hpp>
#include <thread>
#include <cstdio>
#include <libplot_trace/PlotRelayClient.hpp>
#include <libplot_trace/PlotRelayPublisher.hpp>
#include <libnet/Detector.hpp>

using namespace std;

void usage()
{
    printf("USAGE: plot_relay_client [OPTIONS]\n\t-h\tspecify host (don't use detector)\n\t-q\tquery available plots\n\t-s [G]\t show single graph with name G\n");
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
            printf("failed to auto-detect IVO\nFeel free to use plot_relay_client [IP]\n");
            return 0;
        }
        printf("Auto-detected IVO at %s\n", results[0].second.c_str());
        host = results[0].second;
    }

    if(verb == "-q")
    {
        PlotRelayClient client(host, {});
        auto graphs = client.get_graphs();
        printf("Graphs: \n");

        for(const auto& graph : graphs)
        {
            printf("\t%s\n", graph.c_str());
        }
        return 0;
    }
    else if(verb == "-s")
    {
        PlotRelayClient client(host, {graph});
        bool first = true;
        bool paused = false;

        Thread stdin_handler([&paused](Thread& t)
        {
            while(!t.should_quit())
            {
                char c;
                if(Utils::OS::wait_for_read_fd(0, 10))
                {
                    fread(&c, 1, 1, stdin);
                    if(c == '\n')
                    {
                        printf("\r\033[F\033[K");
                        paused = !paused;
                    }
                }
            }
        });

        while(1)
        {
            PlotPacket pkt, sink;
            client.receive_graph(paused ? sink : pkt);

            if(!first)
                printf("\r\033[F\033[K\r\033[F\033[K\r\033[F\033[K");

            printf("stamp: %f %s\n", pkt.stamp, paused ? "[PAUSED]" : "");
            printf("%s%-17s%s%-17s%s%-17s%s%-17s%s\n", fgnd_red, pkt.curve_1, fgnd_green, pkt.curve_2, fgnd_magenta, pkt.curve_3, fgnd_blue, pkt.curve_4, font_reset);
            printf("%s%s%-17f%s%-17f%s%-17f%s%-17f%s\n", fgnd_white, bgnd_red, pkt.value_1, bgnd_green, pkt.value_2, bgnd_magenta, pkt.value_3, bgnd_blue, pkt.value_4, font_reset);
            first = false;
        }
    }
    else usage();

    printf("DONE\n");

    return 0;
}
