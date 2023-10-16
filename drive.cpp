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

/* Constructor */
Drive::Drive(Propel propel) {
  /* Update the Private Propulsion Object */
  propulsion = propel;

  /* Initialize the Pins */
  for (int pin : propel.leftDrivePWM) { pinMode(pin, OUTPUT); }
  for (int pin : propel.rightDrivePWM) { pinMode(pin, OUTPUT); }
  for (int pin : propel.leftDriveDirection) { pinMode(pin, OUTPUT); }
  for (int pin : propel.rightDriveDirection) { pinMode(pin, OUTPUT); }
}

/* Define Functions */
void Drive::forward(int distance) {
    /* Copyright (C) 2023  Ishan Desai */
}

void Drive::backward(int distance) {

}

void Drive::angled(int theta) {

}