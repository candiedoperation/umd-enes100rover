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

/* Include Required Libraries */
#include "arm.hpp"

Arm::Arm(ArmComponents armc_object) {
  /* Update the Private Variable */
  armc = armc_object;
  
  /* Initialize the Pins */
  arm_actuator.attach(armc.ActuatorPin);
  pinMode(armc.SignalPosPin, INPUT);
  pinMode(armc.MagnetInfPin, INPUT);

  /* Retract the Arm */
  arm_actuator.write(180);
  delay(50);
}

bool Arm::detect_magnet() {
  /* Read the Magnetic Hall Sensor Pin */
  return (digitalRead(armc.MagnetInfPin) == 0) ? true : false;
}

int Arm::read_dcycle() {
  /* Define Required Variables */
  double cycletime_high, cycletime_low;
  cycletime_high = pulseIn(armc.SignalPosPin, HIGH);
  cycletime_low = pulseIn(armc.SignalPosPin, LOW);

  /* Return Duty Cycle */
  double duty_cycle = ((cycletime_high / (cycletime_high + cycletime_low)) * 100);
  return (int) duty_cycle;
}

void Arm::deploy(int percent) {
  int dest = (180 - ((percent/100) * 85));
  if (dest <= 180 && dest >= 95) {
    /* Retract the Arm */
    for (int pos = arm_actuator.read(); pos <= 180; pos += 1) {
      arm_actuator.write(pos);
      delay(50);
    }

    for (int pos = 180; pos >= dest; pos -= 1) {
      arm_actuator.write(pos);
      delay(50);
    }
  }
}