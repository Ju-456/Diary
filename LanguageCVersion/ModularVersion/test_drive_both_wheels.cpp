#include <Arduino.h>

void setup() {
  // right
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(0, OUTPUT); // analogic pin
  // left
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(1, OUTPUT); //analogic pin

  stop();

  delay(2000);
  front(200);
  delay(2000);
  back(200);

  delay(2000);
  left(200);
  delay(2000);
  right(200);
  delay(2000);

  stop();
}
void stop(){
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);
}

void front(int vts){
  digitalWrite(13, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(11, LOW);
  digitalWrite(10, HIGH);
  analogWrite(0, vts);
  analogWrite(1, vts);
}
void back(int vts){
  digitalWrite(13, HIGH);
  digitalWrite(12, LOW);
  digitalWrite(11, HIGH);
  digitalWrite(10, LOW);
  analogWrite(0, vts);
  analogWrite(1, vts);
}
void left(int vts){
  digitalWrite(13, HIGH);
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  digitalWrite(10, HIGH);
  analogWrite(0, vts);
  analogWrite(1, vts);
}
void right(int vts){
  digitalWrite(13, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(10, LOW);
  analogWrite(0, vts);
  analogWrite(1, vts);
}


void loop() {
  }