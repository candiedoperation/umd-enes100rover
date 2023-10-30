/*
    ENES100 Autonomous Rover Project, University of Maryland
    Copyright (C) 2023  Atheesh Thirumalairajan

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef remote_hpp
#define remote_hpp

/* Include Required Libraries */
#include <Arduino.h>
#include "VisionSystemClient.hpp"

/* Define Required Structs */
struct Origin {
  char* team_name; // Team Name
  byte team_type; // Team Project
  int aruco_marker; // Aruco Marker Number
  int wifi_tx; // Wi-Fi Module Transmit Pin
  int wifi_rx; // Wi-Fi Module Receive Pin
};

struct RemoteCoords {
  float X;
  float Y;
  float XY;
};

class Remote {
  private:
    VisionSystemClient vision_system;
    Origin origin;

  public:
    /* Define the Constructor */
    Remote(Origin origin_obj);
    VisionSystemClient* getBackend();
};

#endif