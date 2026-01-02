# libplottrace
This repository implements a high-frequency logger for time-series numerical data, enabling efficient capture, processing, and visualization of rapidly changing numeric values.

The idea came out of necessity to read application logs with mostly numerical content, at high frequency.
Since the classic log is not a suitable platform of visualising quickly changing numerical data (e.g. vehicle speed), 
I have developed a logging solution tailored specifically towards frequently changing numerical data. 
This platform makes it easy to consume the numeric data and process it, or plot it on a graph (hence the name).

## Building
* **liblogtrace** is built using the [workspace](https://github.com/romank-dev/workspace) build system. 
**For building with a different system, see [this link](https://github.com/romank-dev/workspace?tab=readme-ov-file#deep-dive--building-projects-without-workspace).**
* The manifest [URL](manifest.txt) may be used by the Workspace **init.sh** script to pull this project and it's dependencies.
## Overview
The current implementation is very minimal and is a basis for expanion of the numeric data loggign concept:
1. We introduce a **graph** which is an XY (cartesian) plot that has a name.
2. A graph holds up to 4 **curves**, where the Y axis is user-defined and the X axis is the **sample** number (usually corresponds to time).
3. Each curve has a name and is made from a collection of samples submitted by an application.
4. The application periodically submits a sample of numeric values for up to 4 curves to a graph.
5. A consumer application (such as a visualizer) can query the available graphs and start consuming samples that are being submitted.

Here is an example of an application submitting numeric data:<br/>
```
float speed_gps, speed_vo;
...
TRACE_PLOT("speed_graph", time_stamp++, "spd_gps", speed_gps, "spd_vo", speed_vo);
```
This code will submit the values speed_gps and speed_vo to the curves "spd_gps" and "spd_vo" in the graph "speed_graph".
In the future there should be support for extra metadata for different kinds of graphs, etc.<br/>
A GUI visualizer may visualize the data in a convenient way:<br/>
<img width="350" height="234" alt="Screenshot from 2026-01-02 19-04-30" src="https://github.com/user-attachments/assets/28c68f95-4f75-4caf-8ba4-2c368b5ea046" />

This is a comparison between estimated GPS speed and speed calculated using Visual Odometry.

## Technical Info
* The libary uses datagram domain sockets to send packets with numeric sample data.
* Plot Relay program receives these packets and redistributes to clients via TCP.
* A TCP client (Implemented in class PlotRelayClient) connects to the relay server, performs handshake during which he receives the names of available graphs and requests which graph information to pull for the remainder of the connection.
