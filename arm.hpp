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

#ifndef arm_hpp
#define arm_hpp

/* Include Required Libraries */
#include <Arduino.h>
#include <Servo.h>

/* Define Required Structs */
struct ArmComponents {
  int ActuatorPin; /* Arm Actuator Servo */
  int SignalPosPin; /* Singal Measure +ve Pin */
  int MagnetInfPin; /* Magnet Detect Info Pin */
  void *MagnetInterrupt; /* Interrupt for Magnet Detection */
};

/* Define Required Classes */
class Arm {
  private:
    Servo arm_actuator;
    ArmComponents armc;

  public:
    Arm(ArmComponents armc_object);
    void deploy(int percent);
    int read_dcycle();
    bool detect_magnet();
};

#endif