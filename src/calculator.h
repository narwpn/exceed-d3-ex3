#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>

const String baseUrl = "https://exceed-hardware-stamp465.koyeb.app/";
int a = 0, b = 0, result = 0;
String op = "", questionId = "";

void POST_result(String questionId, int result) {
    Serial.printf("result: %d\n", result);
    Serial.printf("questionId: %s\n", questionId);
    DynamicJsonDocument doc(2048);
    doc["questionId"] = questionId;
    doc["result"] = result;
    String json;
    serializeJson(doc, json);
    String url = baseUrl + "answer";
    HTTPClient http;
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(json);
    if  (httpCode >= 200 && httpCode < 300) {
        String res = http.getString();
        Serial.println(res);
    } else {
        Serial.println("POST ERROR");
    }
}

bool GET_question() {
    String url = baseUrl + "question";
    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode >= 200 && httpCode < 300) {
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, http.getString());
        a = doc["a"].as<int>();
        b = doc["b"].as<int>();
        op = doc["op"].as<String>();
        questionId = doc["questionId"].as<String>();
        Serial.printf("a: %d\n", a);
        Serial.printf("b: %d\n", b);
        Serial.printf("op: %s\n", op);
        Serial.printf("questionId: %s\n", questionId);
        return true;
    } else {
        Serial.println("GET ERROR");
        return false;
    }
}

void calculator(void *param) {
    while (true) {
        if (GET_question()) {
            if (op == "+") {
                result = a + b;
            } else if (op == "-") {
                result = a - b;
            } else if (op == "*") {
                result = a * b;
            }
            POST_result(questionId, result);
        }
        vTaskDelay(5000/portTICK_PERIOD_MS);
    }
}