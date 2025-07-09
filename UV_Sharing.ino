#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


#define WIFI_SSID ""
#define WIFI_PASSWORD ""


#define API_KEY ""
#define FIREBASE_PROJECT_ID ""
#define FIRESTORE_COLLECTION "" 


#define UV_SENSOR_PIN 34 // GPIO34 (ADC1_6)

void setup() {
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");
}

void sendToFirestore(float uvIntensity, int rawValue, float voltage) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;

        String url = "https://firestore.googleapis.com/v1/projects/" + String(FIREBASE_PROJECT_ID) + 
                     "/databases/(default)/documents/" + String(FIRESTORE_COLLECTION) + "?key=" + String(API_KEY);

        
        DynamicJsonDocument doc(512);
        doc["fields"]["uv_intensity"]["doubleValue"] = uvIntensity;
        doc["fields"]["uv_voltage"]["doubleValue"] = voltage;
        doc["fields"]["uv_raw"]["integerValue"] = rawValue;
        doc["fields"]["timestamp"]["integerValue"] = millis();

        String jsonPayload;
        serializeJson(doc, jsonPayload);

        http.begin(url);
        http.addHeader("Content-Type", "application/json");

        int httpResponseCode = http.POST(jsonPayload);
        if (httpResponseCode > 0) {
            Serial.print("Firestore response code: ");
            Serial.println(httpResponseCode);
        } else {
            Serial.print("Failed to send data: ");
            Serial.println(http.errorToString(httpResponseCode).c_str());
        }

        http.end();
    } else {
        Serial.println("WiFi disconnected!");
    }
}

void loop() {
    int uvRaw = analogRead(UV_SENSOR_PIN);
    float uvVoltage = uvRaw * (3.3 / 4095.0);  
    float uvIntensity = uvVoltage / 0.1;      

    Serial.printf("UV Raw: %d | Voltage: %.2f V | Intensity: %.2f mW/cm²\n", uvRaw, uvVoltage, uvIntensity);

    sendToFirestore(uvIntensity, uvRaw, uvVoltage);

    delay(5000); 
}
