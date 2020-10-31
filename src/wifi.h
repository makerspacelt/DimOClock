#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

void setupWifi();
void setupNTP(int tz);
void updateNTP();

NTPClient getNTPClient();

#endif // WIFI_H