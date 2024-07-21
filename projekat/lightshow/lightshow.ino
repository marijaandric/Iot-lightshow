#include <SPI.h>
#include <stdio.h>
#include <stdlib.h>

int green = 3;
int yellow = 6;
int rgb_red = 5;
int rgb_green = 7;
int mode = 1;
int duration;
int direction;

void setup() {
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(rgb_red, OUTPUT);
  pinMode(rgb_green, OUTPUT);
  Serial.println("Pokrenut.\n");
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    int pos1 = data.indexOf('M');
    int pos2 = data.indexOf('D');
    int pos3 = data.indexOf('S');
    if (pos1 != -1 && pos2 != -1 && pos3 != -1) {
      mode = data.substring(pos1 + 1, pos2).toInt();
      duration = data.substring(pos2 + 1, pos3).toInt();
      direction = data.substring(pos3 + 1).toInt();
      Serial.println((String)"Poslao sam R " + mode + (String)" Poslao sam G " + duration + (String)" Poslao sam B " + direction);
      Serial.flush();
    }
  }
  executeMode();
}

void executeMode() {
  switch(mode) {
    case 0:
      blinkMode();
      break;
    case 1:
      fadeMode();
      break;
    case 2:
      chaseMode();
      break;
    case 3:
      randomMode();
      break;
    case 4:
      breathingMode();
      break;
    default:
      break;
  }
}

void blinkMode() {
  digitalWrite(rgb_red, HIGH);
  digitalWrite(green, HIGH);
  digitalWrite(yellow, HIGH);
  delay(duration);
  digitalWrite(rgb_red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(yellow, LOW);
  delay(duration);
}

void chaseMode() {
  if (direction == 1)
  {
    digitalWrite(rgb_red, HIGH);
    delay(duration);
    digitalWrite(rgb_red, LOW);
    digitalWrite(green, HIGH);
    delay(duration);
    digitalWrite(green, LOW);
    digitalWrite(yellow, HIGH);
    delay(duration);
    digitalWrite(yellow, LOW);
  }
  else{
    digitalWrite(yellow, HIGH);
    delay(duration);
    digitalWrite(yellow, LOW);
    digitalWrite(green, HIGH);
    delay(duration);
    digitalWrite(green, LOW);
    digitalWrite(rgb_red, HIGH);
    delay(duration);
    digitalWrite(rgb_red, LOW);
  }
}

void randomMode() {
  int leds[] = {rgb_red, green, yellow};
  int randomLed = random(0, 3);
  digitalWrite(leds[randomLed], HIGH);
  delay(duration);
  digitalWrite(leds[randomLed], LOW);
  delay(duration);
}

void fadeMode(){
  if (direction == 1)
  {
    fadeLed(green);
    fadeLed(yellow);
    fadeLed(rgb_red);
  }
  else{
    fadeLed(rgb_red);
    fadeLed(yellow);
    fadeLed(green);
  }
}

void fadeLed(int ledPin) {
  // Fade in
  for (int brightness = 0; brightness <= 255; brightness++) {
    analogWrite(ledPin, brightness);
    delay(10);
  }
  
  // Fade out
  for (int brightness = 255; brightness >= 0; brightness--) {
    analogWrite(ledPin, brightness);
    delay(10);
  }
}

void breathingMode(){
  for (int brightness = 0; brightness <= 255; brightness++) {
    analogWrite(green, brightness);
    analogWrite(yellow, brightness);
    analogWrite(rgb_red, brightness);
    delay(10);
  }
  
  for (int brightness = 255; brightness >= 0; brightness--) {
    analogWrite(green, brightness);
    analogWrite(yellow, brightness);
    analogWrite(rgb_red, brightness);
    delay(10);
  }
}