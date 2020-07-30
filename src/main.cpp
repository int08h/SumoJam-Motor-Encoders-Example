/* 
 * Shows how to read the encoders on the Zumo 32U4. The encoders can 
 * tell you how far, and in which direction each motor has turned.
 * 
 * Press and hold button A to drive both motors forward at full speed.  
 * 
 * Press and hold button C to drive both motors in reverse at full speed.
 * 
 * Encoder counts are printed to the LCD and to the serial monitor.
 * 
 * On the LCD, the top line shows the counts from the left encoder,
 * and the bottom line shows the counts from the right encoder.
 * Encoder errors should not happen, but if one does happen then the
 * buzzer will beep and an exclamation mark will appear temporarily
 * on the LCD.
 * 
 * In the serial monitor, the first and second numbers represent
 * counts from the left and right encoders, respectively. The third
 * and fourth numbers represent errors from the left and right
 * encoders, respectively. 
 * 
 * Adapted from Zumo32U4 library example
 * https://github.com/pololu/zumo-32u4-arduino-library/blob/master/examples/Encoders/Encoders.ino
 */

#include <Zumo32U4.h>

namespace {
  Zumo32U4Motors motors = Zumo32U4Motors{};
  Zumo32U4Encoders encoders = Zumo32U4Encoders{};

  Zumo32U4LCD lcd = Zumo32U4LCD{};

  Zumo32U4ButtonA buttonA = Zumo32U4ButtonA{};
  Zumo32U4ButtonC buttonC = Zumo32U4ButtonC{};

  uint8_t lastDisplayTime = 0;
  uint8_t displayErrorLeftCountdown = 0;
  uint8_t displayErrorRightCountdown = 0;
} // namespace 

void setup()
{
  // Initialize the LCD display
  lcd.init();

  // Uncomment this if the motors of your Zumo are backwards
  // motors.flipLeftMotor();
  // motors.flipRightMotor();
}

void loop()
{
  if ((uint8_t)(millis() - lastDisplayTime) >= 100)
  {
    lastDisplayTime = millis();

    int16_t countsLeft = encoders.getCountsLeft();
    int16_t countsRight = encoders.getCountsRight();

    bool errorLeft = encoders.checkErrorLeft();
    bool errorRight = encoders.checkErrorRight();

    if (encoders.checkErrorLeft()) {
      // An error occurred on the left encoder channel.
      // Display it on the LCD for the next 10 iterations and
      // also beep.
      displayErrorLeftCountdown = 10;
    }

    if (encoders.checkErrorRight()) {
      // An error occurred on the left encoder channel.
      // Display it on the LCD for the next 10 iterations and
      // also beep.
      displayErrorRightCountdown = 10;
    }

    // Update the LCD with encoder counts and error info.
    lcd.clear();
    lcd.print(countsLeft);
    lcd.gotoXY(0, 1);
    lcd.print(countsRight);

    if (displayErrorLeftCountdown) {
      // Show an exclamation point on the first line to
      // indicate an error from the left encoder.
      lcd.gotoXY(7, 0);
      lcd.print('!');
      displayErrorLeftCountdown--;
    }

    if (displayErrorRightCountdown) {
      // Show an exclamation point on the second line to
      // indicate an error from the left encoder.
      lcd.gotoXY(7, 1);
      lcd.print('!');
      displayErrorRightCountdown--;
    }

    // Send the information to the serial monitor also.
    char report[32] = {};
    snprintf(report, sizeof(report), "%6d %6d %1d %1d",
        countsLeft, countsRight, errorLeft, errorRight);

    Serial.println(report);
  }

  if (buttonA.isPressed()) {
    motors.setSpeeds(400, 400);
  }
  else if (buttonC.isPressed()) {
    motors.setSpeeds(-400, -400);
  }
  else {
    motors.setSpeeds(0, 0);
  }
}