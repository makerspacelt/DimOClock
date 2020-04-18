#include "wifi.h"

const char* ssid = "name";
const char* password = "password";

WiFiUDP ntpUDP;
NTPClient ntp(ntpUDP, "europe.pool.ntp.org");


void setupWifi() {
    Serial.print("Setting up wifi... ");

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
}

void setupNTP() {
    Serial.print("Setting up NTP... ");
    ntp.begin();
    ntp.setTimeOffset(3*60*60);
    ntp.update();
    Serial.println(ntp.getFormattedTime());
}

void updateNTP() {
    ntp.update();
}

NTPClient getNTPClient() {
    return ntp;
}