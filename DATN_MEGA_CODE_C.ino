#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver mod_1 = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN  90 //độ dài xung tối thiểu; xung PPM
#define SERVOMAX  600
int gtmacdinh2=1;
int giatri2;
int dem2;
int cb,cbb;
const int IR_PIN = 12;
const int IR_PIN2 = 13;
bool detected = false ;
bool detected2 = false ;
int ctht1 = 45;
int ctht2 = 43;
int t=0;
int t2=0;
int start=0;
//DC
int LPWM = 4;
int RPWM = 7;
//STEP
const int dirPin1 = 5;
const int stepPin1 = 2;
const int dirPin3 = 6;
const int stepPin3 = 3;
int stepCount = 0;// Biến đếm số bước di chuyển
#define EN 8
int acceleration = 9;       // Giá trị gia tốc
int maxSpeedDelay = 1100;    // Thời gian trễ tối đa (tốc độ tối đa)
int decelerationStartStep = 200;  // Số bước di chuyển để bắt đầu giảm tốc
int currentDelay = maxSpeedDelay;  // Thời gian trễ hiện tại
//LED
int relay1 = 39;
int relay2 = 41;

int topulse(int goc) //chuyển góc thành xung
{
  int xung = map(goc, 0, 180, SERVOMIN, SERVOMAX);
  return xung;
}

void step_u(int tocdo){
  digitalWrite(EN,LOW);
  digitalWrite(dirPin1, LOW);
  for (int stepCount = 0; stepCount < tocdo; stepCount++) {
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(currentDelay);
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(currentDelay);
    if (stepCount < 50) {
      currentDelay -= acceleration;
      if (currentDelay < maxSpeedDelay) {
        currentDelay = maxSpeedDelay;
      }
    }
    else {
      currentDelay += acceleration;
      if (currentDelay > maxSpeedDelay) {
        currentDelay = maxSpeedDelay;
      }
    }
  }
  digitalWrite(EN, HIGH);
}
void step_d(int tocdo){
   digitalWrite(EN,LOW);
   digitalWrite(dirPin1, HIGH);
  for (int stepCount = 0; stepCount < tocdo; stepCount++) {
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(currentDelay);
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(currentDelay);

    if(digitalRead(ctht1)==0){
      tocdo=stepCount;
      digitalWrite(EN, HIGH);
    }
    if (stepCount < 50) {
      currentDelay -= acceleration;
      if (currentDelay < maxSpeedDelay) {
        currentDelay = maxSpeedDelay;
      }
    }
    else {
      currentDelay += acceleration;
      if (currentDelay > maxSpeedDelay) {
        currentDelay = maxSpeedDelay;
      }
    }
  }
  digitalWrite(EN, HIGH);
}
void step_r(int tocdo){
  digitalWrite(EN,LOW);
  digitalWrite(dirPin3, LOW);
  for (int stepCount = 0; stepCount < tocdo; stepCount++) {
    digitalWrite(stepPin3, HIGH);
    delayMicroseconds(900);
    digitalWrite(stepPin3, LOW);
    delayMicroseconds(900);
    if(digitalRead(ctht2)==0){
      tocdo=stepCount;
      digitalWrite(EN, HIGH);
    }
    if (stepCount < 50) {
      currentDelay -= acceleration;
      if (currentDelay < maxSpeedDelay) {
        currentDelay = maxSpeedDelay;
      }
    }
    else {
      currentDelay += acceleration;
      if (currentDelay > maxSpeedDelay) {
        currentDelay = maxSpeedDelay;
      }
    }
  }
  digitalWrite(EN, HIGH);  // Tắt động cơ sau khi hoàn thành vòng lặp
}
void step_l(int tocdo){
  digitalWrite(EN,LOW);
  digitalWrite(dirPin3, HIGH);
	for(int x = 0; x < tocdo; x++)//380
	{
		digitalWrite(stepPin3, HIGH);
		delayMicroseconds(900);
		digitalWrite(stepPin3, LOW);
		delayMicroseconds(900);
    if (stepCount < decelerationStartStep) {
      currentDelay -= acceleration;
      if (currentDelay < 50) {
        currentDelay = maxSpeedDelay;
      }
    }
    else {
      currentDelay += acceleration;
      if (currentDelay > maxSpeedDelay) {
        currentDelay = maxSpeedDelay;
      }
    }
    char receivedChar = Serial.read();
    if (receivedChar == '9') {
          digitalWrite(EN,HIGH);
          //Serial.println('6');  
          delay(1000);
    }
	}
}
void cb1(){
  if(cb==1){
  if(digitalRead(IR_PIN) == 0 && !detected){ 
  detected = true;
  t=1;
  delay(100);
  }
  }
}
void cb2() {
  if(cbb==1){
  int sensorValue = digitalRead(IR_PIN2);  // Đọc giá trị từ cảm biến
   if (sensorValue == 1) {
      giatri2 = 1;
   } else {
      giatri2 = 0;
   }

   if (giatri2 != gtmacdinh2) {
      if (giatri2 == 0) {
        t2=1;
        dem2++;
      }
      gtmacdinh2 = giatri2;
   }
}
}
 void but1() {
  for (int goc = 300; goc >= 0; goc -= 10) {
    mod_1.setPWM(15, 0, topulse(goc));
    delay(5);
  }
  delay(10);
  for (int goc = 0; goc <= 300; goc += 10) {
    mod_1.setPWM(15, 0, topulse(goc));
    delay(5);
  }
}

void but2() {
  for (int goc = 300; goc >= 0; goc -= 10) {
    mod_1.setPWM(14, 0, topulse(goc));
    delay(5);
  }
  delay(10);
  for (int goc = 0; goc <= 300; goc += 10) {
    mod_1.setPWM(14, 0, topulse(goc));
    delay(5);
  }
}
  
void but3() {
  for (int goc = 300; goc >= 0; goc -= 10) {
    mod_1.setPWM(13, 0, topulse(goc));
    delay(5);
  }
  delay(10);
  for (int goc = 0; goc <= 300; goc += 10) {
    mod_1.setPWM(13, 0, topulse(goc));
    delay(5);
  }
}

void but4() {
  for (int goc = 300; goc >= 0; goc -= 10) {
    mod_1.setPWM(12, 0, topulse(goc));
    delay(5);
  }
  delay(10);
  for (int goc = 0; goc <= 300; goc += 10) {
    mod_1.setPWM(12, 0, topulse(goc));
    delay(5);
  }
}

void but5() {
  for (int goc = 300; goc >= 0; goc -= 10) {
    mod_1.setPWM(11, 0, topulse(goc));
    delay(5);
  }
  delay(10);
  for (int goc = 0; goc <= 300; goc += 10) {
    mod_1.setPWM(11, 0, topulse(goc));
    delay(5);
  }
}

void but6() {
  for (int goc = 300; goc >= 0; goc -= 10) {
    mod_1.setPWM(10, 0, topulse(goc));
    delay(5);
  }
  delay(10);
  for (int goc = 0; goc <= 300; goc += 10) {
    mod_1.setPWM(10, 0, topulse(goc));
    delay(5);
  }
}

void but7() {
  for (int goc = 300; goc >= 0; goc -= 10) {
    mod_1.setPWM(9, 0, topulse(goc));
    delay(5);
  }
  delay(10);
  for (int goc = 0; goc <= 300; goc += 10) {
    mod_1.setPWM(9, 0, topulse(goc));
    delay(5);
  }
}
void but8() {
  for (int goc = 300; goc >= 0; goc -= 10) {
    mod_1.setPWM(8, 0, topulse(goc));
    delay(5);
  }
  delay(10);
  for (int goc = 0; goc <= 300; goc += 10) {
    mod_1.setPWM(8, 0, topulse(goc));
    delay(5);
  }
}

void but9() {
  for (int goc = 300; goc >= 0; goc -= 10) {
    mod_1.setPWM(7, 0, topulse(goc));
    delay(5);
  }
  delay(10);
  for (int goc = 0; goc <= 300; goc += 10) {
    mod_1.setPWM(7, 0, topulse(goc));
    delay(5);
  }
}

void but10() {
  for (int goc = 300; goc >= 0; goc -= 10) {
    mod_1.setPWM(5, 0, topulse(goc));
    delay(5);
  }
  delay(10);
  for (int goc = 0; goc <= 300; goc += 10) {
    mod_1.setPWM(5, 0, topulse(goc));
    delay(5);
  }
}
void reset_but(){
    for (int goc = 0; goc <= 300; goc += 10)
  {
    mod_1.setPWM(15, 0, topulse(goc));
    mod_1.setPWM(14, 0, topulse(goc));
    mod_1.setPWM(13, 0, topulse(goc));
    mod_1.setPWM(12, 0, topulse(goc));
    mod_1.setPWM(11, 0, topulse(goc));
    mod_1.setPWM(10,0, topulse(goc));
    mod_1.setPWM(9, 0, topulse(goc));
    mod_1.setPWM(8, 0, topulse(goc));
    mod_1.setPWM(7, 0, topulse(goc));
    mod_1.setPWM(5, 0, topulse(goc));
    delay(5);
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(IR_PIN, INPUT);
  pinMode(IR_PIN2, INPUT);
  pinMode(ctht1, INPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(RPWM, OUTPUT);
	pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);
	pinMode(stepPin3, OUTPUT);
	pinMode(dirPin3, OUTPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(EN, OUTPUT);
  //
  digitalWrite(EN,HIGH);
  step_d(4000);
  delay(100);
  step_r(4000);
  //
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, LOW);
  delay(50);
  digitalWrite(relay2, HIGH);
  //
  analogWrite(LPWM,0); 
  analogWrite(RPWM,0);
  //
  mod_1.begin();
  mod_1.setOscillatorFrequency(27000000);
  mod_1.setPWMFreq(60);
  delay(50);
  reset_but();
  //
  cb=0;
  cbb=0;
  dem2=0;
  t=0;
  t2=0;
}
void loop(){
  cb1();
  cb2();
  if (t == 1) {
    analogWrite(LPWM, 0);
    analogWrite(RPWM, 0);  
    Serial.print("10");
    t = 0;
    cb=0;
  } 
    if (t2 == 1&&dem2==1) {
    analogWrite(LPWM, 0);
    analogWrite(RPWM, 0);
    delay(500);  
    t2 = 0;
    cbb=0;
    Serial.print("20");
  }
      if (t2 == 1&&dem2==2) {
    analogWrite(LPWM, 0);
    analogWrite(RPWM, 0);
    delay(500);  
    t2 = 0;
    cbb=0;
    Serial.print("21");
  }
    if (t2 == 1&&dem2==3) {
    analogWrite(LPWM, 0);
    analogWrite(RPWM, 0);
    delay(500);  
    t2 = 0;
    cbb=0;
    Serial.print("22");
  }
    if (t2 == 1&&dem2 == 4) {
    analogWrite(LPWM, 0);
    analogWrite(RPWM, 0);
    delay(500);  
    t2 = 0;
    cbb=0;
    Serial.print("23");
  }
      if (t2 == 1&&dem2==5) {
    analogWrite(LPWM, 0);
    analogWrite(RPWM, 0);
    delay(500);  
    t2 = 0;
    cbb=0;
    Serial.print("24");
  }
      if (t2 == 1&&dem2==6) {
    analogWrite(LPWM, 0);
    analogWrite(RPWM, 0);
    delay(500);  
    t2 = 0;
    cbb=0;
    Serial.print("25");
  }
      if (t2 == 1&&dem2==7) {
    analogWrite(LPWM, 0);
    analogWrite(RPWM, 0);
    delay(500);  
    t2 = 0;
    cbb=0;
    Serial.print("26");
  }
      if (t2 == 1&&dem2==8) {
    analogWrite(LPWM, 0);
    analogWrite(RPWM, 0);
    delay(500);  
    t2 = 0;
    cbb=0;
    Serial.print("27");
  }
      if (t2 == 1&&dem2==9) {
    analogWrite(LPWM, 0);
    analogWrite(RPWM, 0);
    delay(500);  
    t2 = 0;
    cbb=0;
    Serial.print("28");
  }
      if (t2 == 1&&dem2==10) {
    analogWrite(LPWM, 0);
    analogWrite(RPWM, 0);
    delay(500);  
    t2 = 0;
    cbb=0;
    Serial.print("29");
  }
  char receivedChar = Serial.read();
if (receivedChar == 'C'){
    analogWrite(LPWM, 0);
    analogWrite(RPWM, 0);
}
if (receivedChar == 'A') {
    cb=1;
    analogWrite(LPWM, 70);
    analogWrite(RPWM, 0);
} 
else if (receivedChar == '1') {
    digitalWrite(relay1, LOW);
    delay(100);
    digitalWrite(relay1, HIGH);
    step_u(160);
    delay(500);
    step_l(340);
    digitalWrite(EN, HIGH);
    Serial.println('1');
    delay(500);
} else if (receivedChar == '2') {
    step_u(240);
    delay(500);
    digitalWrite(EN, HIGH);
    Serial.println('2');
    delay(500);
} else if (receivedChar == '3') {
    digitalWrite(relay1, LOW);
    delay(100);
    digitalWrite(relay1, HIGH);
    step_u(330);
    delay(500);
    digitalWrite(EN, HIGH);
    Serial.println('3');
    delay(500);
} else if (receivedChar == '4') {
    step_u(210);
    delay(500);
    digitalWrite(EN, HIGH);
    Serial.println('4');
    delay(500);
} else if (receivedChar == '5') {
    digitalWrite(relay1, LOW);
    delay(100);
    digitalWrite(relay1, HIGH);
    step_l(1100);
    delay(500);
    digitalWrite(EN, HIGH);
    Serial.println('5');
    delay(500);
} else if (receivedChar == '6') {
    step_d(190);
    delay(500);
    digitalWrite(EN, HIGH);
    Serial.println('6');
    delay(500);
} else if (receivedChar == '7') {
    digitalWrite(relay1, LOW);
    delay(100);
    digitalWrite(relay1, HIGH);
    step_d(280);
    delay(500);
    digitalWrite(EN, HIGH);
    Serial.println('7');
    delay(500);
} else if (receivedChar == '8') {
    step_d(230);
    delay(500);
    digitalWrite(EN, HIGH);
    Serial.println('8');
    delay(500);
    digitalWrite(relay1, LOW);
    delay(100);
    digitalWrite(relay1, HIGH);
} else if (receivedChar == '9') {
  step_d(4000);
  delay(100);
  step_r(4000);
  //Serial.println('9');
}
  else if (receivedChar == 'K'){
  cbb=1;
  analogWrite(LPWM, 70);
  analogWrite(RPWM, 0);    
} 
else if (receivedChar == 'J') {
  cbb=1;
  analogWrite(LPWM, 70);
  analogWrite(RPWM, 0); 
}
else if (receivedChar == 'H') {
  cbb=1;
  analogWrite(LPWM, 70);
  analogWrite(RPWM, 0); 
}
else if (receivedChar == 'G') {
  cbb=1;
  analogWrite(LPWM, 70);
  analogWrite(RPWM, 0); 
}
else if (receivedChar == 'F') {
  cbb=1;
  analogWrite(LPWM, 70);
  analogWrite(RPWM, 0); 
}
else if (receivedChar == 'D') {
  cbb=1;
  analogWrite(LPWM, 70);
  analogWrite(RPWM, 0); 
}
else if (receivedChar == 'S') {
  cbb=1;
  analogWrite(LPWM, 70);
  analogWrite(RPWM, 0); 
}
else if (receivedChar == 'M') {
  cbb=1;
  analogWrite(LPWM, 70);
  analogWrite(RPWM, 0); 
}
else if (receivedChar == 'N') {
  cbb=1;
  analogWrite(LPWM, 70);
  analogWrite(RPWM, 0); 
}
else if (receivedChar == 'B') {
  cbb=1;
  analogWrite(LPWM, 70);
  analogWrite(RPWM, 0); 
}
else if (receivedChar == 'Q') {but1();} 
else if (receivedChar == 'W') {but2();} 
else if (receivedChar == 'E') {but3();} 
else if (receivedChar == 'R') {but4();} 
else if (receivedChar == 'T') {but5();} 
else if (receivedChar == 'Y') {but6();} 
else if (receivedChar == 'U') {but7();} 
else if (receivedChar == 'I') {but8();} 
else if (receivedChar == 'O') {but9();}
else if (receivedChar == 'P') {but10();} 
else if (receivedChar == 'X') {
  analogWrite(LPWM,0); 
  analogWrite(RPWM,0);
  digitalWrite(relay2, LOW);
  delay(50);
  digitalWrite(relay2, HIGH);
  delay(100);
  step_d(4000);
  delay(100);
  step_r(4000);
  digitalWrite(EN, HIGH);
  reset_but();
  cb=0;
  cbb=0;
  dem2=0;
  t=0;
  t2=0;
}
}
  

