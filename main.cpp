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
#include "main.h"
#include "drive.h"
#include "navigate.h"

/* Define Functions */
void Main::initialize() {
/* Initialize Serial */
  Serial.begin(9600);

  /* Define Propulsion Pins */
  struct Propel propulsion = {
    {10, 11}, //10LF, 11LB
    {6, 7}, //6LF, 7LF
    {12, 13}, //12RF, 13RB
    {8, 9} //8RF, 9RB
  };

  /* Define Vision Data */
  struct Vision vision = {
    {{19, 20, 21}},
  };

  /* Initialize the Drive Object */
  Drive drive(propulsion);
  drive.forward(10);

  /* Initialize Navigation */
  Navigate navigate(vision);

  while (1 == 1) {
    /* Perform Infinite Loop */
    if (navigate.ulsonic_ping(0) < 5) drive.brake();
    else drive.forward(10);
  }
}