#include<HTTPClient.h>
#include<ArduinoJson.h>

#include "shieldsio.hpp"

ShieldsIO::ShieldsIO(String endpoint) {
  this->endpoint = endpoint;
}

void ShieldsIO::fetch() {
  HTTPClient http;
  http.begin(this->endpoint);
  int status = http.GET();
  if(status != 200) return;
  Stream *resp = http.getStreamPtr();
  StaticJsonDocument<256> json;
  deserializeJson(json, *resp);
  this->label = json["label"].as<String>();
  this->color = json["color"].as<String>();
  this->message = json["message"].as<String>();
  http.end();
}
