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

#ifndef navigate_hpp
#define navigate_hpp

/* Include Required Libraries */
#include <Arduino.h>
#include <Servo.h>
#include "remote.hpp"

/* Define Required Macros */
#define ULSONIC_SWEEP_MIN false
#define ULSONIC_SWEEP_MAX true

/* Define Required Constants */
const double LANE_LEFTWEST = 1.75;
const double LANE_LEFTEAST = 1.35;
const double LANE_RIGHTWEST = 0.83;
const double LANE_RIGHTEAST = 0.29;

/* Define Required Strcuts */
struct Vision {
  /* ulsonic_pins: [Sensor Count][Pwr, Tx, Rx] */
  int ulsonic_pins[1][3];
  Servo ulsonic_sweep;
  Remote* remote;
};

/* Define Class */
class Navigate {
  private:
    Vision vision;
  
  public:
    Navigate(Vision vision_obj);
    void precision_turn(Remote *remote, Drive *drive, float angle);
    void obstacle_avoid(Remote *remote, Drive *drive);
    int get_lane(Remote *remote);
    void set_lane(Remote *remote, Drive *drive, int lane);
    long ulsonic_read(int sensor_index);
    int ulsonic_sweep(int sensor_index, bool type);
    RemoteCoords get_position(Remote *remote);
    void mission_site(Remote *remote, Drive *drive);
};

#endif