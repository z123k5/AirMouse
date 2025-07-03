// air_mouse_with_click
#include <Arduino.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Mouse.h>
#include <Wire.h>

MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;
int vx, vy, vx_prec, vy_prec;
int count = 0;

void setup() {
    Serial.begin(115200);
    Mouse.begin();
    Wire.begin();
    mpu.initialize();
    if (!mpu.testConnection()) {
        while (1)
            ;
    }
    pinMode(PB0, INPUT_PULLUP);
    pinMode(PB1, INPUT_PULLUP);
}

bool lpressed = false;
bool rpressed = false;

void loop() {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    vx = (gx + 300) / 200;
    vy = -(gz - 100) / 200;

    Mouse.move(vx, vy);

    // if ((vx_prec - 5) <= vx && vx <= vx_prec + 5 && (vy_prec - 5) <= vy &&
    //     vy <= vy_prec + 5) { // for checking the cursor doesn't move too much from its actual
    //                          // position: (in this case a 10 pixel square)
    //     count++;
    //     if (count ==
    //         100) { // the click will happen after 2 seconds the pointer has stopped in the 10px square
    //         if (!Mouse.isPressed(MOUSE_LEFT)) {
    //             Mouse.press(MOUSE_LEFT);
    //             count = 0;
    //         }
    //     } else {
    //         if (Mouse.isPressed(MOUSE_LEFT)) {
    //             Mouse.release(MOUSE_LEFT);
    //         }
    //     }
    // } else {
    //     vx_prec = vx; // updating values to check the position of the pointer and allow the click
    //     vy_prec = vy;
    //     count = 0;
    // }

    if (digitalRead(PB0) == LOW) {
        lpressed = true;
        Mouse.press(MOUSE_LEFT);
    } else if (lpressed) {
        Mouse.release(MOUSE_LEFT);
        lpressed = false;
    }

    if (digitalRead(PB1) == LOW) {
        rpressed = true;
        Mouse.press(MOUSE_RIGHT);
    } else if (rpressed) {
        Mouse.release(MOUSE_RIGHT);
        rpressed = false;
    }

    delay(5);
}