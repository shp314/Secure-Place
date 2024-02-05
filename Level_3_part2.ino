#include <Servo.h>

Servo myServo; 

int s1;
int s2;
int s3;
int val1;
int val2;
int val3;
int servo = 2;
int ap1 = A5;
int ap2 = A4;
int ap3 = A3;
int g1 = A2;
int g2 = A1;
int g3 = 3;
int r1 = 12;
int master=11;
unsigned long previousMillis = 0;
const long interval = 15000;
bool v0=false;
bool v1=false;
bool v2=false;
bool v3=false;


void setup() {
  Serial.begin(9600);
  pinMode(ap1, INPUT);
  pinMode(ap2, INPUT);
  pinMode(ap3, INPUT);
  pinMode(g1, OUTPUT);
  pinMode(g2, OUTPUT);
  pinMode(g3, OUTPUT);
  pinMode(r1, OUTPUT);
  pinMode(master,INPUT);
  myServo.attach(servo);
  digitalWrite(g1,HIGH);
  digitalWrite(g2,HIGH);
  digitalWrite(g3,HIGH);
  digitalWrite(r1,LOW);
  myServo.write(135);
}

void loop() {
  // put your main code here, to run repeatedly:
  int master_state=digitalRead(master);
  Serial.println(master_state);
  if(master_state==1){
    digitalWrite(r1,HIGH);
    randomSeed(analogRead(A0));
    s1=random(0,1023);
    s2=random(0,1023);
    s3=random(0,1023);
    Serial.println("After Reset");
    Serial.println(s1);
    Serial.println(s2);
    Serial.println(s3);
    unsigned long currentMillis = millis();
    while(currentMillis-previousMillis <= interval && v0==false){
      val1 = analogRead(ap1);
      val2 = analogRead(ap2);
      val3 = analogRead(ap3);
      if(val3<=s3+10 && val3>=s3-10){
        digitalWrite(g1,LOW);
        v1=true;
      }
      else{
        digitalWrite(g1,HIGH);
        v1=false;
      }
      if(val2<=s2+10 && val2>=s2-10){
        digitalWrite(g2,LOW);
        v2=true;
      }
      else{
        digitalWrite(g2,HIGH);
        v2=false;
      }
      if(val1<=s1+10 && val1>=s1-10){
        digitalWrite(g3,LOW);
        v3=true;
      }
      else{
        digitalWrite(g3,HIGH);
        v3=false;
      }
      currentMillis = millis();
    }
    previousMillis=currentMillis;
    if(v1==true && v2==true && v3==true){
      Serial.println("You Won");
      for (int i=0;i<3;i++){
        digitalWrite(r1,LOW);
        delay(500);
        digitalWrite(r1,HIGH);
        delay(500);
      }
      myServo.write(0);
      delay(2000);
      myServo.write(135);
      v0=true;
      v1=v2=v3=false;
    }
    else{
      previousMillis=currentMillis;
    }
  }
  else{
    digitalWrite(r1,LOW);
  }
}


