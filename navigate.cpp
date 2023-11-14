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

long Navigate::ulsonic_read(int sensor_index) {
  /* Get Sensor Pwr, Tx, Rx Pins from Vision Struct */
  Middleware middleware;
  int *sensor = vision.ulsonic_pins[sensor_index];

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

int Navigate::ulsonic_sweep(int sensor_index, bool type) {
  /* Sweep for Minimum Angle and Distance */
  int angle = 0;
  long distance = (type == ULSONIC_SWEEP_MIN) ? 500 : 0;
  
  /* Center the Servo */
  vision.ulsonic_sweep.write(180);
  delay(200);

  for (int pos = 180; pos >= 0; pos -= 1) {
    vision.ulsonic_sweep.write(pos);
    delay(15);

    /* Read Distance */
    long dist = this->ulsonic_read(sensor_index);

    /* Update Minimum Distance Values */
    if (type == ULSONIC_SWEEP_MIN) {
      if (dist <= distance) {
        distance = dist;
        angle = (pos - 90); /* -90 as servo range is 0 to 180, Turn Fn. is -90 to 90 */
      }
    } else {
      if (dist >= distance) {
        distance = dist;
        angle = (pos - 90); /* -90 as servo range is 0 to 180, Turn Fn. is -90 to 90 */
      }
    }
  }

  /* Center the Servo */
  vision.ulsonic_sweep.write(90);
  return angle;
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
  bool payload_direction = false; /* False is Left, True is Right */
  const int PRECISION_TURN_LIMIT = 8;
  const float PAYLOAD_RIGHTC = 0.89;
  const float PAYLOAD_LEFTC = 1.08;
  const float BEGIN_XCOORD = 0.53;
  const float BEGIN_XCOORDC = 0.6;

  /* Orient to Mission Site */
  if (this->get_position(remote).X < (BEGIN_XCOORD + 0.2) || this->get_position(remote).X > (BEGIN_XCOORD - 0.2)) {
    /* We are within 0.2 of the Begin Line, Orient Straight */
    drive->angled(this->get_position(remote).XY);

    /* Align to Centerline */
    delay(350);
    if (this->get_position(remote).X > BEGIN_XCOORDC) {
      while (this->get_position(remote).X > BEGIN_XCOORDC) {
        drive->backward(15);
        delay(50);
        drive->brake();
      }
    } else if (this->get_position(remote).X < BEGIN_XCOORDC) {
      while (this->get_position(remote).X < BEGIN_XCOORDC) {
        drive->forward(15);
        delay(50);
        drive->brake();
      }
    }

    /* Mission site is opposite to begin Point */
    delay(350);
    if (this->get_position(remote).Y < 1.00) {
        /* Go Left */
        int precision_turns = 0;
        payload_direction = false;
        while ((precision_turns <= PRECISION_TURN_LIMIT) && (this->get_position(remote).XY < 90.5 || this->get_position(remote).XY > 90.5)) {
          precision_turns++;
          drive->angled(this->get_position(remote).XY - 90);
          delay(250);
        }
    } else {
        /* Go Right */
        int precision_turns = 0;
        payload_direction = true;
        while ((precision_turns <= PRECISION_TURN_LIMIT) && this->get_position(remote).XY < -90.5 || this->get_position(remote).XY > -90.5) {
          precision_turns++;
          drive->angled(this->get_position(remote).XY - (-90));
          delay(250);
        }
    }

    /* Move to Mission Site */
    delay(350);
    if (payload_direction == false) {
      /* Payload is in the Left */
      while (this->get_position(remote).Y < PAYLOAD_LEFTC) {
        drive->forward(15);
        delay(50);
        drive->brake();
      }
    } else {
      /* Payload is in the Right */
      while (this->get_position(remote).Y > PAYLOAD_RIGHTC) {
        drive->forward(15);
        delay(50);
        drive->brake();
      }
    }

    /* Use Ultrasonic Sensor Precision */
    int min_angle = this->ulsonic_sweep(0, ULSONIC_SWEEP_MIN);
    drive->angled(min_angle);

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