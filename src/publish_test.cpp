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

#include <libcommon/libcommon.hpp>
#include <libgen.h>
#include <thread>
#include <cstdio>
#include <cmath>
#include <libplot_trace/libplot_trace.hpp>

using namespace std;

int safe_main(int argc, char** argv)
{
    Utils::FileSystem::set_local_cwd();

    if(argc == 1)
    {
        printf("USAGE: publish_tester [graph1] [graph2] ...\n");
        return 0;
    }

    vector<string> topics;

    for(int i = 1; i < argc; ++i)
    {
        topics.push_back(string(argv[i]));
    }

    float stamp = 0;
    while(true)
    {
        this_thread::sleep_for(std::chrono::milliseconds(10));
        for(const auto& topic : topics)
        {
            TRACE_PLOT(topic.c_str(), stamp, "sin(timestamp)", sin(stamp), "cos(timestamp)", cos(stamp), "sin(2*timestamp)", sin(2*stamp), "sqrt(cos(t))", sin(cos(stamp)));
        }
        stamp += 0.01f;
    }

    return 0;
}


int main(int argc, char** argv)
{
    try
    {
         return safe_main(argc, argv);
    }
    catch(const Exception& ex)
    {
        printf("Caught Exception: %s\n", ex.full_message().c_str());
    }
    catch(const std::exception& ex)
    {
        printf("Caught std::exception: %s\n", ex.what());
    }
    catch(...)
    {
        printf("Caught unknown exception\n");
    }

    return 0;
}
