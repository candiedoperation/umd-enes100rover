/* Header Definition */
#ifndef drive_h
#define drive_h

/* Include Required Libraries */
#include <Arduino.h>

/* Define Classes */
class Drive {
  private:
    int drivePin01, drivePin02, drivePin03, drivePin04;

  public:
    void forward(int distance);
    void backward(int distance);
    void angled(int theta);
}