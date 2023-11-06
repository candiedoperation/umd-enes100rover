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

#include "drive.hpp"
#include "remote.hpp"
#include "navigate.hpp"
#include "middleware.hpp"
#include "VisionSystemClient.hpp"

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
  
  for (int pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    vision.ulsonic_sweep.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5);                       // waits 15ms for the servo to reach the position
  }

  delay(500);

  for (int pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    vision.ulsonic_sweep.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5);                       // waits 15ms for the servo to reach the position
  }

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

RemoteCoords Navigate::get_position(Remote *remote) {
  /* Uses the Remote Backend to get X, Y, Theta */
  RemoteCoords rmc;
  rmc.X = remote->backend->getX();
  rmc.Y = remote->backend->getY();
  rmc.XY = (remote->backend->getTheta()) * 57.2958; /* Convert Rads to Degrees */
  return rmc;
}

void Navigate::mission_site(Remote *remote, Drive *drive) {
  /*
    According to the Mission Specifications,
    Position straight by Turning (Theta - 0)
    if the Y Coordinate is < 1.00, go Left
    if the Y Coordinate is > 1.00, go Right
  */

  /* Define Regime Coordinates */
  const float BEGIN_XCOORD = 0.53;
  
  /* Orient to Mission Site */
  if (this->get_position(remote).X < (BEGIN_XCOORD + 0.2) || this->get_position(remote).X > (BEGIN_XCOORD - 0.2)) {
    /* We are within 0.2 of the Begin Line, Orient Straight */
    drive->angled(this->get_position(remote).XY - 0);

    /* Mission site is opposite to begin Point */
    if (this->get_position(remote).Y < 1.00) {
        Serial.println("Go Left");
        drive->angled(-90);
    } else {
        Serial.println("Go Right");
        drive->angled(90);
    }

    /* Move to Mission Site */
    /*while (this->get_position(remote).Y < 1.2) {
      drive->forward(20);
      delay(150);
      drive->brake();
    }*/

    /* Orient Straight, facing Concrete Blocks */
    //drive->angled(this->get_position(remote).XY - 0);

    /*while (this->get_position(remote).Y < 1.84) {
      drive->forward(20);
      delay(150);
      drive->brake();
    }*/

    /* Check which Concrete Block we're closer to and turn opposite */
    /*rmc = this->get_position(remote);
    if (rmc.Y < 1.00) {
        Serial.println("Go Left");
        drive->angled(-90);
    } else {
        Serial.println("Go Right");
        drive->angled(90);
    }*/

    /* Drive Forward for Delta Distance */
    /*while (this->get_position(remote).Y <= 1.0) {
      drive->forward(20);
      delay(150);
      drive->brake();
    }*/

    /* Orient Straight, facing the Logs */
    //drive->angled(this->get_position(remote).XY - 0);

    /* Drive Forward for Delta Distance */
    /*while (this->get_position(remote).X <= 2.9) {
      drive->forward(20);
      delay(150);
      drive->brake();
    }*/

    /* Turn Right for Log */
    //drive->angled(90);

    /* Move Towards Log Center */
    /*while (this->get_position(remote).Y <= 0.43) {
      drive->forward(20);
      delay(150);
      drive->brake();
    }*/

    /* Orient Straight, facing the Log */
    //drive->angled(this->get_position(remote).XY - 0);

    /* Climb Over Log */
    /*drive->forward(85);
    delay(1000);
    drive->brake();*/
  }
}