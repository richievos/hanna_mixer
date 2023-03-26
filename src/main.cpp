#include <Arduino.h>

#ifdef ESP8266
// for basic arduinos, including ESP8266s (what I used before I ran out)
#include <Servo.h>
#else
#include <ESP32Servo.h>
#endif

const int SERVO_PIN = 4;

const uint32_t SERVO_TIME_TO_FULLY_MOVE_MS = 500;

const int SERVO_STOP = 90;
// the value these are set to will control how fast the servo spins
struct ServoSpeeds {
    int ccw_speed = 45;
    int cw_speed = 135;

    int ccw_speed_slow = 85;
    int cw_speed_slow = 95;
} servoSpeeds;

#define SECS_TO_MS(mins) mins * 1000
#define MINS_TO_MS(mins) SECS_TO_MS(mins) * 60

const unsigned int MIX_TIME_MINUTES = 2;
const unsigned int MIX_TIME_MS = MINS_TO_MS(MIX_TIME_MINUTES);

Servo mixer;  // create servo object to control a servo

void setup() {
    Serial.begin(115200);
    mixer.attach(SERVO_PIN);
    mixer.write(SERVO_STOP);
}

void mixOnce() {
    mixer.write(servoSpeeds.cw_speed);
    delay(SERVO_TIME_TO_FULLY_MOVE_MS);

    mixer.write(servoSpeeds.ccw_speed);
    delay(SERVO_TIME_TO_FULLY_MOVE_MS);
}

// Routine to make some noise so user is aware that unit is still energized after completing mixing cycle,
// ie to remind user to turn off unit if not using
enum WiggleSpeed { WIGGLE_FAST, WIGGLE_SLOW };
void wiggleOnce(const WiggleSpeed wiggleSpeed=WIGGLE_SLOW) {
    const uint32_t wiggleDelay = wiggleSpeed == WIGGLE_SLOW ? 750 : 100;
    mixer.write(servoSpeeds.cw_speed_slow);
    delay(wiggleDelay);
    mixer.write(servoSpeeds.ccw_speed_slow);
    delay(wiggleDelay);
}

void mixerLoop() {
    Serial.println("Initializing real mixer loop");
    wiggleOnce();
    delay(SECS_TO_MS(1));       // Signal it's on
    mixer.write(SERVO_STOP);    // Stop
    delay(SECS_TO_MS(15));      // Wait for tube to be inserted

    const unsigned long endTime = millis() + MIX_TIME_MS;

    const int wiggleEvery = SECS_TO_MS(15);

    Serial.println("Beginning real mix loop");
    unsigned long curTime = millis();
    while (endTime > curTime) {
        const long timeLeft = lround((endTime - curTime) / 1000.0);
        Serial.print("Time left=");
        Serial.print(timeLeft);
        Serial.println("s");

        // _count_ every 15ish seconds to make it easier to keep track of time
        if (timeLeft % wiggleEvery == 0) {
            wiggleOnce(WIGGLE_FAST);
        }

        mixOnce();
        curTime = millis();
    }

    // After mixing cycle, return mixer to storage position and wait 15 seconds for user to remove sample and turn off
    Serial.println("Finished");
    mixer.write(SERVO_STOP);
    delay(SECS_TO_MS(15));

    // if still on, start shaking periodically as a reminder to get turned off
    Serial.println("Reminding time is up");
    while (true) {
        wiggleOnce();
        delay(SECS_TO_MS(5));
    }
}

void debugLoop() {
    Serial.println("Running debug loop");
    mixOnce();
    delay(1000);
    wiggleOnce();
    delay(2000);
}

#define LOOP_FUNC mixerLoop
// #define LOOP_FUNC debugLoop

void loop() {
    LOOP_FUNC();
}