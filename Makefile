#  Copyright 2025 Roman Kudinov. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

PROJECT = $(shell basename $(CURDIR))
SRC_DIR = src

SRC_CONSOLE_RELAY_CLIENT = src/console_plot_client.cpp src/PlotRelayClient.cpp
SRC_GUI_RELAY_CLIENT = src/cv_plot_client.cpp src/CvPlot.cpp src/CvPlotRenderer.cpp src/PlotRelayClient.cpp
SRC_RELAY_SERVER = src/plot_relay_server.cpp src/PlotRelayServer.cpp 
SRC_PUBLISH_TESTER = src/publish_test.cpp src/PlotRelayPublisher.cpp src/PlotRelayGlobal.cpp 
SRC_PLOT_TESTER = src/cv_plot_test.cpp src/CvPlot.cpp
SRC_LIB = src/PlotRelayGlobal.cpp src/PlotRelayPublisher.cpp src/PlotRelayClient.cpp

OBJ_CONSOLE_RELAY_CLIENT = $(SRC_RELAY_CLIENT:$(SRC_DIR)/%.cpp=$(G_OBJ)/$(PROJECT)/%.o)
OBJ_GUI_RELAY_CLIENT = $(SRC_PLOT_CLIENT:$(SRC_DIR)/%.cpp=$(G_OBJ)/$(PROJECT)/%.o)
OBJ_RELAY_SERVER = $(SRC_RELAY_SERVER:$(SRC_DIR)/%.cpp=$(G_OBJ)/$(PROJECT)/%.o)
OBJ_PUBLISH_TESTER = $(SRC_PUBLISH_TESTER:$(SRC_DIR)/%.cpp=$(G_OBJ)/$(PROJECT)/%.o)
OBJ_PLOT_TESTER = $(SRC_PLOT_TESTER:$(SRC_DIR)/%.cpp=$(G_OBJ)/$(PROJECT)/%.o)
OBJ_LIB = $(SRC_LIB:$(SRC_DIR)/%.cpp=$(G_OBJ)/$(PROJECT)/%.o)

DEP_CONSOLE_RELAY_CLIENT = $(OBJ_RELAY_CLIENT:%.o=%.d)
DEP_GUI_RELAY_CLIENT = $(OBJ_PLOT_CLIENT:%.o=%.d)
DEP_LIB = $(OBJ_LIB:%.o=%.d)
DEP_RELAY_SERVER = $(OBJ_RELAY_SERVER:%.o=%.d)
DEP_PUBLISH_TESTER = $(OBJ_PUBLISH_TESTER:%.o=%.d)
DEP_PLOT_TESTER = $(OBJ_PLOT_TESTER:%.o=%.d)

INCLUDE_DIRS = 	-Iinclude -I$(G_SRC)/libcommon/include -I$(G_SRC)/libnet/include -I$(G_SRC)/liblogtrace/include $(INCLUDE_OPENCV)

LIBRARY_DIRS = -L$(G_BIN)
CXXFLAGS = -fPIC -MMD $(INCLUDE_DIRS)
LDFLAGS = $(LIBRARY_DIRS) -lcommon

.PHONY: all
ifdef INCLUDE_OPENCV
all: $(G_BIN)/$(PROJECT)/plot_relay $(G_BIN)/$(PROJECT)/client_gui \
	$(G_BIN)/$(PROJECT)/test_plot_publish $(G_BIN)/$(PROJECT)/test_gui \
	$(G_BIN)/libplot_relay.so $(G_BIN)/$(PROJECT)/client_console 
	@echo "\033[0;97m [DONE] $(PROJECT)"
else
all: $(G_BIN)/$(PROJECT)/plot_relay $(G_BIN)/$(PROJECT)/test_plot_publish $(G_BIN)/libplot_relay.so $(G_BIN)/$(PROJECT)/client_console 
	@echo "\033[0;97m [DONE] $(PROJECT) (OpenCV not installed - skipping UI)"
endif

$(G_BIN)/$(PROJECT)/client_gui: $(SRC_GUI_RELAY_CLIENT)
	@echo "\033[0;32m [LINK] $@ \033[0;0m"
	@g++ -o $@ $^ $(LDFLAGS) $(G_LDFLAGS) $(LDFLAGS_OPENCV) -lnet
	
$(G_BIN)/$(PROJECT)/client_console: $(OBJ_CONSOLE_RELAY_CLIENT)
	@echo "\033[0;32m [LINK] $@ \033[0;0m"
	@g++ -o $@ $^ $(LDFLAGS) $(G_LDFLAGS) -lnet
	
$(G_BIN)/$(PROJECT)/plot_relay: $(OBJ_RELAY_SERVER)
	@echo "\033[0;32m [LINK] $@ \033[0;0m"
	@g++ -o $@ $^ -lnet -llogtrace $(LDFLAGS)

$(G_BIN)/$(PROJECT)/test_gui: $(OBJ_PLOT_TESTER)
	@echo "\033[0;32m [LINK] $@ \033[0;0m"
	@g++ -o $@ $^ $(LDFLAGS) $(G_LDFLAGS) $(LDFLAGS_OPENCV) -lnet
	
$(G_BIN)/$(PROJECT)/test_plot_publish: $(OBJ_PUBLISH_TESTER)
	@echo "\033[0;32m [LINK] $@ \033[0;0m"
	@g++ -o $@ $^ $(LDFLAGS) $(G_LDFLAGS) $(LDFLAGS_OPENCV) -lnet

$(G_BIN)/libplot_relay.so: $(OBJ_LIB)
	@echo "\033[0;32m [LINK] $@ \033[0;0m"
	@g++ -shared -o $@ $^ -lnet $(LDFLAGS) 

-include $(DEP_CONSOLE_RELAY_CLIENT)
-include $(DEP_GUI_RELAY_CLIENT)
-include $(DEP_LIB)
-include $(DEP_RELAY_SERVER)
-include $(DEP_PUBLISH_TESTER)
-include $(DEP_PLOT_TESTER)

$(G_OBJ)/$(PROJECT)/%.o: $(SRC_DIR)/%.cpp
	@echo "\033[0;93m [CC] $@ \033[0;0m"
	g++ -c $< -o $@ $(CXXFLAGS) $(G_CXXFLAGS) -std=c++11

$(shell mkdir -p $(G_BIN)/$(PROJECT))
$(shell mkdir -p $(G_OBJ)/$(PROJECT))
