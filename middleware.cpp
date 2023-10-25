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
#include "middleware.hpp"

/* Define Functions */
long Middleware::ulsonic_parse(long ms) {
  /*
    See: https://www.parallax.com/package/ping-ultrasonic-distance-sensor-downloads/
    According to Parallax's datasheet for the PING))), there are 73.746
    microseconds per inch (i.e. sound travels at 1130 feet per second).
    This gives the distance travelled by the ping, outbound and return,
    so we divide by 2 to get the distance of the obstacle.
  */

  return (ms / 74 / 2);
}

int Middleware::pwmp_parse(int percent) {
  /* Convert a percent speed value to PWM 0 - 255 range */
  return (((float) (100 - percent) / 100) * 255);
}