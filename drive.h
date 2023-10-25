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
#ifndef drive_h
#define drive_h

/* Include Required Libraries */
#include <Arduino.h>
#include "middleware.h"

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
    void forward(int distance);
    void backward(int distance);
    void angled(int theta);
};

#endif