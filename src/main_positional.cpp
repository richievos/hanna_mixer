// #include <Arduino.h>

// #ifdef ESP8266
// // for basic arduinos, including ESP8266s (what I used before I ran out)
// #include <Servo.h>
// #else
// #include <ESP32Servo.h>
// #endif

// #define SERVO_PIN 4
// // default = 544, 2400
// // https://www.amazon.com/Micro-Helicopter-Airplane-Remote-Control/dp/B072V529YD says 2500
// #define SERVO_MIN_PWM 544
// // #define SERVO_MAX_PWM 2500
// #define SERVO_MAX_PWM 2400

// #define SERVO_MIN_ANGLE 0
// #define SERVO_MAX_ANGLE 170

// #define SERVO_TIME_TO_FULLY_MOVE_MS 2000
// #define SERVO_TIME_TO_FULLY_MOVE_MS 500

// #define SECS_TO_MS(mins) mins * 1000
// #define MINS_TO_MS(mins) SECS_TO_MS(mins) * 60

// #define MIX_TIME_MINUTES 2
// #define MIX_TIME_MS MINS_TO_MS(MIX_TIME_MINUTES)

// Servo mixer;  // create servo object to control a servo
// // twelve servo objects can be created on most boards

// void setup() {
//     Serial.begin(115200);
//     mixer.attach(SERVO_PIN, SERVO_MIN_PWM, SERVO_MAX_PWM);
//     mixer.write(SERVO_MIN_ANGLE);
// }

// void mixOnce() {
//     mixer.write(SERVO_MIN_ANGLE);
//     delay(SERVO_TIME_TO_FULLY_MOVE_MS);

//     mixer.write(SERVO_MAX_ANGLE);
//     delay(SERVO_TIME_TO_FULLY_MOVE_MS);
// }

// // Routine to make some noise so user is aware that unit is still energized after completing mixing cycle,
// // ie to remind user to turn off unit if not using
// void wiggleOnce() {
//     mixer.write(60);
//     delay(750);
//     mixer.write(30);
//     delay(750);
// }

// void mixerLoop() {
//     Serial.println("Initializing real mixer loop");
//     mixer.write(0);        // Move mixer into storage position and wait
//     delay(SECS_TO_MS(5));  // Wait to allow any manual messing around

//     const unsigned long endTime = millis() + MIX_TIME_MS;

//     Serial.println("Beginning real mix loop");
//     unsigned long curTime = millis();
//     while (endTime > curTime) {
//         const long timeLeft = lround((endTime - curTime) / 1000.0);
//         Serial.print("Time left=");
//         Serial.print(timeLeft);
//         Serial.println("s");

//         mixOnce();
//         curTime = millis();
//     }

//     // After mixing cycle, return mixer to storage position and wait 15 seconds for user to remove sample and turn off
//     Serial.println("Finished");
//     mixer.write(0);
//     delay(SECS_TO_MS(15));

//     // if still on, start shaking periodically as a reminder to get turned off
//     Serial.println("Reminding time is up");
//     while (true) {
//         wiggleOnce();
//     }
// }

// void debugLoop() {
//     Serial.println("Running debug loop");
//     mixOnce();
//     delay(1000);
//     wiggleOnce();
//     delay(2000);
// }

// #define LOOP_FUNC mixerLoop
// // #define LOOP_FUNC debugLoop

// void loop() {
//     LOOP_FUNC();
// }