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

/* Header Definition */
#ifndef drive_hpp
#define drive_hpp

/* Include Required Libraries */
#include <Arduino.h>
#include "middleware.hpp"

/* Define Structs */
struct Propel {
  int leftDrivePWM[2];
  int leftDriveDirection[2];
  int rightDrivePWM[2];
  int rightDriveDirection[2];
};

/* Define Classes */
class Drive {
  private:
    Propel propulsion;
    Middleware middleware;

  public:
    Drive(Propel propulsion);
    void brake();
    void forward(int speed);
    void backward(int speed);
    void angled(float theta);
};

#endif