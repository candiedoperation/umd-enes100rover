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
  delay(100);

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

int Navigate::get_lane(Remote *remote) {
  float pos_y = this->get_position(remote).Y;
  if (pos_y >= LANE_LEFTEAST && pos_y <= LANE_LEFTWEST) {
    /* Lane 1, Leftmost Lane */
    return 1;
  } else if (pos_y > LANE_RIGHTWEST && pos_y < LANE_LEFTEAST) {
    /* Lane 2, Middle Lane */
    return 2;
  } else if (pos_y >= LANE_RIGHTEAST && pos_y <= LANE_RIGHTWEST) {
    /* Lane 3, Rightmost Lane */
    return 3;
  } else {
    /* False Case */
    return NULL;
  }
}

void Navigate::set_lane(Remote *remote, Drive *drive, int lane) {
  /* Orient Straight */
  delay(500);
  drive->angled(this->get_position(remote).XY);

  if (lane == 1 && this->get_lane(remote) != 1) {
    /* Calculate Lane Average */
    float lane_avg = (LANE_LEFTEAST + LANE_LEFTWEST) / 2;

    /* Orient Towards Lane Path */
    if (this->get_position(remote).Y < lane_avg) { this->precision_turn(remote, drive, 90); }
    else { this->precision_turn(remote, drive, -90); }

    /* Move Forward Until Lane Center is Reached */
    delay(500);
    while (this->get_position(remote).Y < (lane_avg - 0.2)) {
        drive->forward(25);
        delay(150);
        drive->brake();
    }

    /* Orient Straight */
    delay(500);
    drive->angled(this->get_position(remote).XY);
  } else if (lane == 2 && this->get_lane(remote) != 2) {
    /* Calculate Lane Average */
    float lane_avg = (LANE_LEFTEAST + LANE_RIGHTWEST) / 2;

    /* Orient Towards Lane Path */
    if (this->get_position(remote).Y < lane_avg) { this->precision_turn(remote, drive, 90); }
    else { this->precision_turn(remote, drive, -90); }

    /* Move Forward Until Lane Center is Reached */
    delay(500);
    while (abs(this->get_position(remote).Y - lane_avg) > 0.03) {
        drive->forward(25);
        delay(150);
        drive->brake();
    }

    /* Orient Straight */
    delay(500);
    drive->angled(this->get_position(remote).XY);
  } else if (lane == 3 && this->get_lane(remote) != 3) {
    /* Calculate Lane Average */
    float lane_avg = (LANE_RIGHTEAST + LANE_RIGHTWEST) / 2;

    /* Orient Towards Lane Path */
    if (this->get_position(remote).Y < lane_avg) { this->precision_turn(remote, drive, 90); }
    else { this->precision_turn(remote, drive, -90); }

    /* Move Forward Until Lane Center is Reached */
    delay(500);
    while (this->get_position(remote).Y > (lane_avg)) {
        drive->forward(25);
        delay(150);
        drive->brake();
    }

    /* Orient Straight */
    delay(500);
    drive->angled(this->get_position(remote).XY);
  }
}

void Navigate::obstacle_avoid(Remote *remote, Drive *drive) {
    /* Orient Straight, facing Concrete Blocks */
    delay(500);
    drive->angled(this->get_position(remote).XY);

    /* Test */
    //this->set_lane(remote, drive, 3);
    //delay(1000);
    //this->set_lane(remote, drive, 1);
    //delay(1000);
    //this->set_lane(remote, drive, 2);

    while (this->get_position(remote).X < 2.4) {
      /* Perform Obstacle Avoidance Until System reaches Log */
      
      /* Test */
      //Serial.print("Lane ");
      //Serial.println(this->get_lane(remote));

      while (this->ulsonic_read(0) > 7 && this->get_position(remote).X < 2.4) {
        drive->forward(100);
        delay(20);
        drive->brake();
      }

      /*
        Obstacle Detected,
        Apply Obstacle Avoidance Logic:
          - Move to Lane 1, Check Obstacle
          - Move to Lane 2, Check Obstacle
          - Move to Lane 3, Check Obstacle
      */

      /* Either Lanes would be free */
      if (this->ulsonic_read(0) <= 7) {
        this->set_lane(remote, drive, 2); 
        delay(1000);

        if (this->ulsonic_read(0) <= 7) {
          this->set_lane(remote, drive, 3); 
          delay(1000);

          if (this->ulsonic_read(0) <= 7) {
            this->set_lane(remote, drive, 1); 
            delay(1000);
          }
        }
      }

      /* Set Loop Delay to 300ms */
      delay(300);
    }

    /* Orient Straight */
    delay(1000);
    drive->angled(this->get_position(remote).XY);
}

void Navigate::end_site(Remote *remote, Drive *drive) {
    /* Move to Lane 3 */
    delay(500);
    this->set_lane(remote, drive, 3);

    /* Move Forward */
    delay(500);
    while (this->get_position(remote).X < 3.75) {
      drive->forward(100);
      delay(20);
      drive->brake();
    }
}

void Navigate::precision_turn(Remote *remote, Drive *drive, float angle) {
  const int PRECISION_TURN_LIMIT = 8;
  int precision_turns = 0;
  while ((precision_turns <= PRECISION_TURN_LIMIT) && (this->get_position(remote).XY < (angle - 0.5) || this->get_position(remote).XY > (angle + 0.5))) {
    precision_turns++;
    drive->angled(this->get_position(remote).XY - angle);
    delay(350);
  }
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
    //int min_angle = this->ulsonic_sweep(0, ULSONIC_SWEEP_MIN);
    //drive->angled(min_angle);
  }
}