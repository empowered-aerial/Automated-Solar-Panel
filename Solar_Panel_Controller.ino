#include <Servo.h>

// Servo instances
Servo horizontal;
Servo vertical;

int horizontalPos = 90;
int verticalPos = 90;

const int horizontalLimitHigh = 180;
const int horizontalLimitLow = 0;
const int verticalLimitHigh = 180;
const int verticalLimitLow = 0;

// LDR pins
const int ldrTopLeft = A0;
const int ldrTopRight = A3;
const int ldrBottomLeft = A1;
const int ldrBottomRight = A2;

void setup() {
  horizontal.attach(2);
  vertical.attach(13);

  Serial.begin(9600);

  horizontal.write(horizontalPos);
  vertical.write(verticalPos);
  delay(1000);
}

void loop() {
  int topLeft = analogRead(ldrTopLeft);
  int topRight = analogRead(ldrTopRight);
  int bottomLeft = analogRead(ldrBottomLeft);
  int bottomRight = analogRead(ldrBottomRight);

  int avgTop = (topLeft + topRight) / 2;
  int avgBottom = (bottomLeft + bottomRight) / 2;
  int avgLeft = (topLeft + bottomLeft) / 2;
  int avgRight = (topRight + bottomRight) / 2;

  int verticalDifference = avgTop - avgBottom;
  int horizontalDifference = avgLeft - avgRight;

  int tolerance = 10;

  if (abs(verticalDifference) > tolerance) {
    verticalPos += (avgTop > avgBottom) ? 1 : -1;
    verticalPos = constrain(verticalPos, verticalLimitLow, verticalLimitHigh);
    vertical.write(verticalPos);
  }

  if (abs(horizontalDifference) > tolerance) {
    horizontalPos += (avgLeft > avgRight) ? 1 : -1;
    horizontalPos = constrain(horizontalPos, horizontalLimitLow, horizontalLimitHigh);
    horizontal.write(horizontalPos);
  }

  delay(200);
}
