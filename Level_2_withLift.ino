  /*
MCU2: Elevator & Nuclear Reactor Control, sends HP duduction rate to MCU1
Slotted Opto Switch * 1
Buzzer * 1
Servo * 2
Potentialmeter * 2
*/
#include <SoftwareSerial.h>
#define RxD 3
#define TxD 2 //2, 3 for mcu1
SoftwareSerial MCU2(RxD, TxD);
const int toLift = 9; //comms with lift MCU
const int fromLift = 10; //comms with lift MCU
SoftwareSerial MCU23(fromLift,toLift);

#include <Servo.h>
Servo Servo1;
Servo Servo2;
int servoPos1;
int servoPos2;
float potReading1;
float potReading2;

const int motorPin1 = A3;
const int motorPin2 = A4;

const int servo1Pin = 4;
const int servo2Pin = 5;
const int potMeter1 = A0;
const int potMeter2 = A1;

const int optoSwitch = A2;
const int buzzerPin = 8;

const int endSwitch = 7; // toggle switch

const int L1= A5;//A5
const int L3= A4;//A4
int L1_State;
int L3_State;
int in1 = 11 ;//2
int in2 = 12;//4
int enA = 6;//3

int beepInterval = 100;
unsigned long currentMillis=0, previousMillis1=0, previousMillis2 = 0;
float secPerDecay = 1000;
int hasmat = 0;
int liftHere = 0; // state variable
int initialPos; // to check initial position of toggle switch

void setup() {
  pinMode(RxD, INPUT); 
  pinMode(TxD, OUTPUT); 
  pinMode(toLift, OUTPUT);
  MCU2.begin(9600);
  Servo1.attach(servo1Pin);
  Servo2.attach(servo2Pin);
  pinMode(endSwitch, INPUT);
  pinMode(optoSwitch, INPUT);
  pinMode(buzzerPin, OUTPUT);
  //pinMode(liftButton1, INPUT);
  //pinMode(liftButton2, INPUT);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  Serial.begin(9600);
  initialPos = digitalRead(endSwitch);
  //L1_State=digitalRead(L1);
  //L3_State=digitalRead(L3);
  //liftReset();
}

/*void liftReset(){
  while(L1_State==0){
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    digitalWrite(enA,150);
    L1_State=digitalRead(L1);
    L3_State=digitalRead(L1); 
  }
  if(L1_State==1){
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
    digitalWrite(enA,0);
  }
  delay(2000);
}
void liftFloor2(){
  //delay(2000);
  liftHere=1;
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(enA,150);
  delay(2000);
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(enA,0);
}
void liftFloor3(){
  delay(2000);
  while(L3_State==0){
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
    digitalWrite(enA,150);
    L3_State=digitalRead(L3);
    L1_State=digitalRead(L1);
  }
  if(L3_State==1){
      digitalWrite(in1,LOW);
      digitalWrite(in2,LOW);
      digitalWrite(enA,0);
  }
}*/
int received = 0;
int canEndLevel = 1;
int liftSent = 0;
void loop() {
  //Serial.println()
  //Serial.println(digitalRead(endSwitch));
  if(canEndLevel == 1 && (digitalRead(endSwitch)!=initialPos) && liftSent == 0){
    MCU23.write(3);
    liftHere = 0;
    liftSent = 1;

    if(beepInterval!=200){//puzzle not solved
      liftHere = 1;
    }    
  }

  //communications
  if(MCU2.available()&&received == 0){
    received = MCU2.read();
    if(received==10){//floor 2 without hasmat
      MCU23.write(2);
      hasmat = 0;
      liftHere = 1;
    }  
    else if (received==20){//floor 2 with hasmat
       MCU23.write(2);
      hasmat = 1;
      liftHere = 1;
    }
    else if (received==30){
      MCU23.write(3);
      liftHere = 0;
    }
    else{
      liftHere = 0;
      received = 0;
    }

  }
  
  //beeping control
  currentMillis = millis();
  if((currentMillis - previousMillis1 >= beepInterval)&&liftHere&&canEndLevel){
    tone(buzzerPin, 200, 5);//beeping
    previousMillis1 = currentMillis;
  }
  
  if(servoPos1+servoPos2<=70) 
    canEndLevel = 0;//unable to end level
  else
    canEndLevel = 1;
  
  if((currentMillis - previousMillis2 >= 1000)){
    if((liftHere==0 && liftSent==0)||canEndLevel==0)
      MCU2.write(20);
    if(liftHere==1 && liftSent==0 && beepInterval==200)
      MCU2.write(21);
    if(liftHere==1 && liftSent==0 && beepInterval!=200)
      MCU2.write(secPerDecay);
    if (liftSent==1){
      MCU2.write(30);
      Serial.println("Finish Message sent");
    }
    previousMillis2 = currentMillis;
  }
    
  //servo position
  if(liftSent==0){
    potReading1 = analogRead(potMeter1);
    potReading2 = analogRead(potMeter2);
  }
  servoPos1 = potReading1*(150.0/1023.0)+15;//range: 15-165 degrees
  servoPos2 = potReading2*(150.0/1023.0)+15;
  Servo1.write(servoPos1);
  Servo2.write(servoPos2);

  int error = 10;//error in degrees
  //radiation calculation
  int deltaPos1 = abs(servoPos1-45), deltaPos2 = abs(servoPos2-95);//change this to change solution
  if(deltaPos1<=error && deltaPos2<=error){
    beepInterval = 200;
  }
  else{
    beepInterval = 10 + 100/(1+0.001*(deltaPos1*deltaPos2));
  }
  
  if(hasmat==1 && digitalRead(optoSwitch)==1)//with hasmat suit
    secPerDecay = 0.04 * beepInterval;
  else 
    secPerDecay = 0.02 * beepInterval;
  //Serial.println(beepInterval);

}