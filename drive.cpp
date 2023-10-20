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
#include "drive.h"
#include "middleware.h"

/* Constructor */
Drive::Drive(Propel propel) {
  /* Update the Private Propulsion Object */
  propulsion = propel;

  /* Initialize the Pins */
  for (int pin : propulsion.leftDrivePWM) { pinMode(pin, OUTPUT); }
  for (int pin : propulsion.rightDrivePWM) { pinMode(pin, OUTPUT); }
  for (int pin : propulsion.leftDriveDirection) { pinMode(pin, OUTPUT); }
  for (int pin : propulsion.rightDriveDirection) { pinMode(pin, OUTPUT); }
}

void Drive::brake() {
  /* Cut Power to All Drive Pins */
  for (int pin : propulsion.leftDrivePWM) { digitalWrite(pin, LOW); }
  for (int pin : propulsion.rightDrivePWM) { digitalWrite(pin, LOW); }
}

void Drive::forward(int speed) {
  /* Take Speed as percent of 255 */
  Middleware middleware;
  int pwm_speed = middleware.pwmp_parse(speed);

  /* Power Direction Pins */
  for (int pin : propulsion.leftDriveDirection) { digitalWrite(pin, HIGH); }
  for (int pin : propulsion.rightDriveDirection) { digitalWrite(pin, HIGH); }

  /* Power Drive Pins */
  for (int pin : propulsion.leftDrivePWM) { analogWrite(pin, pwm_speed); }
  for (int pin : propulsion.rightDrivePWM) { analogWrite(pin, pwm_speed); }
}

void Drive::backward(int speed) {
  /* Take Speed as percent of 255 */
  Middleware middleware;
  int pwm_speed = middleware.pwmp_parse(speed);

  /* Power Direction Pins */
  for (int pin : propulsion.leftDriveDirection) { digitalWrite(pin, LOW); }
  for (int pin : propulsion.rightDriveDirection) { digitalWrite(pin, LOW); }

  /* Power Drive Pins */
  for (int pin : propulsion.leftDrivePWM) { analogWrite(pin, pwm_speed); }
  for (int pin : propulsion.rightDrivePWM) { analogWrite(pin, pwm_speed); }
}

void Drive::angled(int theta) {

}