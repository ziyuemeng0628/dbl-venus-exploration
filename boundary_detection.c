#include<Servo.h>

#define steps 4
#define stepSize 100
#define IRpin 2

Servo servo_left;
Servo servo_right;

void servo_attach(char a) {
  if(a == 'r'){
    servo_right.attach(12);
  }
  if(a == 'l'){
    servo_left.attach(13);
  }
  if(a == 'a'){
    servo_right.attach(12);
    servo_left.attach(13);
  }
}

void servo_deatach() {
  servo_right.detach();
  servo_left.detach();
}

void goForward() {
  servo_attach('r');
  servo_attach('l');
  servo_right.write(0);
  servo_left.write(stepSize);
  delay(stepSize);
  servo_deatach();
}

void goBackward() {
  servo_attach('r');
  servo_attach('l');
  servo_right.write(stepSize);
  servo_left.write(0);
  delay(stepSize);
  servo_deatach();
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  delay(100);
  int i = 0;
  while(digitalRead(IRpin) != 1 && i < steps){
    goForward();
    i = i + 1;
    delay(100);
    Serial.print(digitalRead(IRpin));
  }
  while(i > 0){
    goBackward();
    i = i - 1;
  }
}
