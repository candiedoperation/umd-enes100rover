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

/* Include Headers */
#include "remote.hpp"

Remote::Remote(Origin origin_obj, VisionSystemClient *backend_ref) {
  /* Update Instance Variables */;
  backend = backend_ref;
  origin = origin_obj;
  
  /* Initialize Pins */
  pinMode(origin.wifi_tx, OUTPUT);
  pinMode(origin.wifi_rx, INPUT);

  /* Connect to Backend */
  backend->begin(
    origin.team_name, 
    origin.team_type, 
    origin.aruco_marker, 
    origin.wifi_tx, 
    origin.wifi_rx
  );

  /* We're Connected! */
  Serial.println("wtf?");
};