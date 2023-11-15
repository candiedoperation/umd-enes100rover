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
#include "main.hpp"
#include "arm.hpp"
#include "drive.hpp"
#include "navigate.hpp"
#include "remote.hpp"
#include "Enes100.h"
#include <Servo.h>

/* Define Functions */
void Main::initialize() {
  /* Initialize Serial */
  Serial.begin(9600);

  /* Define Propulsion Pins */
  struct Propel propulsion = {
    {9, 10}, //10LF, 11LB
    {4, 5}, //6LF, 7LB
    {11, 12}, //12RF, 13RB
    {6, 7} //8RF, 9RB
  };
  
  /* Define Origin Data */
  struct Origin origin = {
    "Ein, the Data Dog!",
    DATA,
    11, /* 58 is the Mascot */
    52,
    50
  };

  /* Define Ultrasonic Sweep servo on Pin 2 */
  Servo ulsonic_sweep;
  ulsonic_sweep.attach(2);

  /* Define Vision Data */
  struct Vision vision = {
    {{19, 20, 21}},
    ulsonic_sweep,
    NULL
  };

  /* Define Arm Data */
  struct ArmComponents armc = {
    3, /* Arm Servo Pin */
    A0, /* Signal Input Pin */
    8, /* Magnetic Hall Sensor */
  };

  /* Initialize Arm */
  Arm arm(armc);
  arm.deploy(100);
  delay(2500);

  for (int i = 0; i < 5; i++) {
    Serial.print("DUTY CYCLE: ");
    Serial.println(arm.read_dcycle());
    Serial.print("MAGNET PRESENCE: ");
    Serial.println(arm.detect_magnet());
    Serial.println("");
    delay(250);
  }

  //Serial.print("Duty Cycle: ");
  //Serial.println(arm.read_dcycle());

  delay(2500);
  arm.deploy(0);

  /* Initialize Navigation */
  Navigate navigate(vision);

  /* Initialize the Drive Object */
  Drive drive(propulsion);

  /* Test */
  //int ang = navigate.ulsonic_sweep(0, ULSONIC_SWEEP_MIN);
  //Serial.println("Minimum Dist at: ");
  //Serial.print(ang);
  //drive.angled(ang);

  /* Initialize Remote */
  Remote remote(origin, &Enes100);
  Enes100.println("Hello! I'm Ein, the Data Dog!");

  /* Initialize Navigation */
  //delay(800);
  //navigate.mission_site(&remote, &drive);

  while (1 == 1) {
    //navigate.ulsonic_ping(0);

    /*Serial.println(dist);
    if (dist < 10) {
      drive.brake();
    } else {
      drive.forward(40);
    }*/
    
    delay(300);
  }
}