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
#include "drive.hpp"
#include "middleware.hpp"

/* Constructor */
Drive::Drive(Propel propel) {
  /* Update the Private Propulsion Object */
  Middleware middleware;
  propulsion = propel;

  /* Initialize the Pins */
  for (int pin : propulsion.leftDrivePWM) { pinMode(pin, OUTPUT); analogWrite(pin, middleware.pwmp_parse(0)); }
  for (int pin : propulsion.rightDrivePWM) { pinMode(pin, OUTPUT); analogWrite(pin, middleware.pwmp_parse(0)); }
  for (int pin : propulsion.leftDriveDirection) { pinMode(pin, OUTPUT); }
  for (int pin : propulsion.rightDriveDirection) { pinMode(pin, OUTPUT); }
}

void Drive::brake() {
  /* Cut Power to All Drive Pins */
  for (int pin : propulsion.leftDrivePWM) { analogWrite(pin, middleware.pwmp_parse(0)); }
  for (int pin : propulsion.rightDrivePWM) { analogWrite(pin, middleware.pwmp_parse(0)); }
}

void Drive::forward(int speed) {
  /* Take Speed as percent of 255 */
  int pwm_speed = middleware.pwmp_parse(speed);

  /* Power Direction Pins */
  digitalWrite(propulsion.leftDriveDirection[0], LOW);
  digitalWrite(propulsion.rightDriveDirection[0], HIGH);
  digitalWrite(propulsion.leftDriveDirection[1], LOW);
  digitalWrite(propulsion.rightDriveDirection[1], HIGH);

  /* Power Drive Pins */
  for (int pin : propulsion.leftDrivePWM) { analogWrite(pin, pwm_speed); }
  for (int pin : propulsion.rightDrivePWM) { analogWrite(pin, pwm_speed); }
}

void Drive::backward(int speed) {
  /* Take Speed as percent of 255 */
  int pwm_speed = middleware.pwmp_parse(speed);

  /* Power Direction Pins */
  digitalWrite(propulsion.leftDriveDirection[0], HIGH);
  digitalWrite(propulsion.rightDriveDirection[0], LOW);
  digitalWrite(propulsion.leftDriveDirection[1], HIGH);
  digitalWrite(propulsion.rightDriveDirection[1], LOW);

  /* Power Drive Pins */
  for (int pin : propulsion.leftDrivePWM) { analogWrite(pin, pwm_speed); }
  for (int pin : propulsion.rightDrivePWM) { analogWrite(pin, pwm_speed); }
}

void Drive::angled(float theta) {
  /* This Function needs to be calibrated based on use Case */
  
  /* Calibrate the PWM % to requirement. For us -> 90deg is 2020ms. 1deg = 22.444444ms (turn_factor) */
  const long TURN_CALIBRATION = 22.444444; 
  int pwm_speed = middleware.pwmp_parse(30);

  /* Power Direction Pins */
  for (int pin : propulsion.leftDriveDirection) { pinMode(pin, (theta > 0) ? HIGH : LOW); }
  for (int pin : propulsion.rightDriveDirection) { pinMode(pin, (theta > 0) ? HIGH : LOW); }

  /* Power Drive Pins */
  for (int pin : propulsion.leftDrivePWM) { analogWrite(pin, pwm_speed); }
  for (int pin : propulsion.rightDrivePWM) { analogWrite(pin, pwm_speed); }

  /* Stop Driving after Turn Completion */
  delay(TURN_CALIBRATION * abs(theta));
  this->brake();
}