#include <Arduino.h>
#include "calculator.h"
#include <Bounce2.h>

#define BUTTON 27
#define BLUE 5
#define RED 12
Bounce debouncer = Bounce();
int cnt = 5000;
int status = 0;

void Button_LED (void *param) {
    while (1) {
        debouncer.update();
        if (debouncer.fell()) {
            digitalWrite(RED, 1);
        }
    }
}

void Dim_LED (void *param) {
    while (1) {
        for (int i = 0; i < 255; i++) {
            ledcWrite(0, i);
            delay(5);
        }
        for (int i = 255; i > 0; i--) {
            ledcWrite(0, i);
            delay(5);
        }
        delay(1000);
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("BUTTON");
    debouncer.attach(BUTTON, INPUT_PULLUP);
    debouncer.interval(25); 
    //Connect_Wifi();
    
    pinMode(RED, OUTPUT);
    digitalWrite(RED, LOW);

    ledcSetup(0, 5000, 8);
    ledcAttachPin(BLUE, 0);

    xTaskCreatePinnedToCore(Button_LED, "Button_LED", 10000, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(Dim_LED, "Dim_LED", 10000, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(calculator, "calculator", 10*1024, NULL, 1, NULL, 1);
}

void loop() {
}
