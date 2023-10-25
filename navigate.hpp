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

/* Define Required Strcuts */
struct Vision {
  /* ulsonic_pins: [Sensor Count][Pwr, Tx, Rx] */
  int ulsonic_pins[1][3];
  Servo ulsonic_sweep;
};

/* Define Class */
class Navigate {
  private:
    Vision vision;
  
  public:
    Navigate(Vision vision_obj);
    long ulsonic_ping(int sensor_index);
};

#endif