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

#include "navigate.hpp"
#include "middleware.hpp"

/* Define Constrcutors */
Navigate::Navigate(Vision vision_obj) {
  /* Update the Instance Variable */
  vision = vision_obj;

  /* Initialize the Pins */
  for (int *sensor : vision.ulsonic_pins) {
    /* Pins and Pwr */
    pinMode(sensor[0], OUTPUT);
    digitalWrite(sensor[0], HIGH);
    
    /* Tx, Rx */
    pinMode(sensor[1], OUTPUT);
    pinMode(sensor[2], INPUT);
  }
}

long Navigate::ulsonic_ping(int sensor_index) {
  /* Get Sensor Pwr, Tx, Rx Pins from Vision Struct */
  Middleware middleware;
  int *sensor = vision.ulsonic_pins[sensor_index];
  
  /*for (int pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    vision.ulsonic_sweep.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5);                       // waits 15ms for the servo to reach the position
  }

  delay(500);

  for (int pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    vision.ulsonic_sweep.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5);                       // waits 15ms for the servo to reach the position
  }*/

  /* Trigger Ultrasonic Sensor Pins */
  digitalWrite(sensor[1], LOW);
  delayMicroseconds(2);
  digitalWrite(sensor[1], HIGH);
  delayMicroseconds(10);
  digitalWrite(sensor[1], LOW);

  /* Read Duration and return Distance */
  long duration = pulseIn(sensor[2], HIGH);
  return middleware.ulsonic_parse(duration);
}