#include<M5StickC.h>
#include<WiFi.h>

#include "shieldsio.hpp"

// Configuration
const String USERS[] = {"tourist", "chokudai", "Makutamoto"};
const char SSID[] = ""; // Required
const char PASSWORD[] = ""; // Required

#define NOF_USERS sizeof(USERS) / sizeof(String)
#define GRAY M5.Lcd.color565(128, 128, 128)

int userIndex;

uint16_t getRatingColor(int rating) {
  const static uint16_t colors[] = {
    GRAY, // gray coder
    M5.Lcd.color565(165, 42, 42), // brown coder
    GREEN, // green coder
    CYAN, // cyan coder
    BLUE, // blue coder
    YELLOW, // yellow coder
    ORANGE, // orange coder
    RED, // red coder
  };
  if(rating < 0) return WHITE;
  return colors[min(rating / 400, 7)];
}

void printRating(String username, ShieldsIO userInfo) {
  M5.Lcd.setTextDatum(TL_DATUM);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(BLACK, GRAY);
  M5.Lcd.drawString(userInfo.label, 0, 0);
  M5.Lcd.setTextColor(getRatingColor(userInfo.message.toInt()));
  M5.Lcd.setTextDatum(BC_DATUM);
  M5.Lcd.drawString("Rating: " + userInfo.message, M5.Lcd.width() / 2, M5.Lcd.height());
  M5.Lcd.setTextDatum(MC_DATUM);
  M5.Lcd.setTextSize(2);
  M5.Lcd.drawString(username, M5.Lcd.width() / 2, M5.Lcd.height() / 2);
}

void printPaging(int current, int whole) {
  M5.Lcd.setTextDatum(TR_DATUM);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(BLACK, GRAY);
  M5.Lcd.drawString(current + String("/") + whole, M5.Lcd.width(), 0);
}

ShieldsIO getRating(String username) {
  ShieldsIO atcoder("https://atcoder-badges.now.sh/api/atcoder/json/" + username);
  atcoder.fetch();
  return atcoder;
}

void drawDisplay(int index, const String users[], size_t size) {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextDatum(MC_DATUM);
  M5.Lcd.setTextSize(1);
  M5.Lcd.drawString("Loading...", M5.Lcd.width() / 2, M5.Lcd.height() / 2);
  printPaging(index + 1, size);
  ShieldsIO userInfo = getRating(users[index]);
  M5.Lcd.fillScreen(BLACK);
  printPaging(index + 1, size);
  printRating(users[index], userInfo);
}

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);
  WiFi.begin(SSID, PASSWORD);
  M5.Lcd.print("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    M5.Lcd.print(".");
    delay(100);
  }
  drawDisplay(0, USERS, sizeof(USERS) / sizeof(String));
}

void loop() {
  M5.update();
  if(M5.BtnA.wasPressed()) {
    userIndex++;
    if(userIndex >= NOF_USERS) userIndex = 0;
  }
  if(M5.BtnA.wasPressed() || M5.BtnB.wasPressed()) drawDisplay(userIndex, USERS, NOF_USERS);
}
