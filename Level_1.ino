/*
MCU1: PushButton Puzzle & LCD Display (HP & # Attempts), 
  sends elevator signal to MCU2
Green LED * 4
Red LED * 1
Pushbutton * 4
LCD * 1
Buzzer * 1 (Optional, not added yet) 
*/
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#define RxD 2
#define TxD 3 //3, 2 for mcu2
SoftwareSerial MCU1(RxD, TxD);

const int rs = 6, en = 7, d4 = 5, d5 = 4, d6 = 13, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int corr_seq=0;
const int button1= 11;
const int button2= 10;
const int button3= 9;
const int button4= 8;

const int gLed1 = A0;
const int gLed2 = A1;
const int gLed3 = A2;
const int gLed4 = A3;
const int rLed = A4;

char sequence[4] = {'4', '2', '3', '1'};//define correct sequence
char buttonCur = ' ';
int i = 0;//iterator of current digit in sequence

int HP = 100;
short int attemptNum = 1; 
int secPerDecay = 100;

int lv2Clear = 0;

void setup() {
  pinMode(RxD, INPUT); 
  pinMode(TxD, OUTPUT); 
  MCU1.begin(9600); 

  lcd.begin(16, 2);

  pinMode(button1,INPUT);
  pinMode(button2,INPUT);
  pinMode(button3,INPUT);
  pinMode(button4,INPUT);

  pinMode(gLed1,OUTPUT);
  pinMode(gLed2,OUTPUT);
  pinMode(gLed3,OUTPUT);
  pinMode(gLed4,OUTPUT);
  pinMode(rLed,OUTPUT);
  Serial.begin(9600);

  lcd.setCursor(0, 0);//column, row
  lcd.print("MA2012 Project");
  delay(3000);
  lcd.clear();

  digitalWrite(gLed1, HIGH);
  digitalWrite(gLed2, HIGH);
  digitalWrite(gLed3, HIGH);
  digitalWrite(gLed4, HIGH);
  digitalWrite(rLed, HIGH);
}

void printCurState(){//prints current state on LCD  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("HP: ");
  lcd.setCursor(4, 0);
  lcd.print(HP);
  if(corr_seq==0){
    lcd.setCursor(0, 1);
    lcd.print("Attempt ");
    lcd.setCursor(8, 1);
    lcd.print(attemptNum);
  }
  else if(corr_seq==1 && secPerDecay == 21){
    lcd.setCursor(0, 1);
    lcd.print("Radiation Clear");
  }
  if(secPerDecay==30&&lv2Clear==0){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Lvl 2 Clear!");
    delay(3000);
    lv2Clear = 1;
  }
}
void successSequence(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Lvl 1 Clear!");
  if (attemptNum<=3)
    MCU1.write(30);//send signal, elevator goes down to lv3
  else if (attemptNum<=10)
    MCU1.write(20);//send signal, elevator goes down to lv2 with hazmat
  else
    MCU1.write(10);//send signal, elevator goes down to lv2 without hazmat
  digitalWrite(gLed1,LOW);
  digitalWrite(gLed2,LOW);
  digitalWrite(gLed3,LOW);
  digitalWrite(gLed4,LOW);
  delay(3000);
}
int currentMillis, previousMillis=0;
void loop() {
  printCurState();
  
  //reading decay rate & deducting HP
  currentMillis = millis();
  if((currentMillis - previousMillis >= secPerDecay*1000)&& corr_seq){
    HP-=1;
    previousMillis = currentMillis;
  }
  else if(secPerDecay>=20) previousMillis = currentMillis;
  if(MCU1.available() && corr_seq==1){
    secPerDecay = MCU1.read();
    Serial.println(secPerDecay);
  }

  
  if(HP<=0){
    lcd.setCursor(0, 0);
    lcd.print("Game Over");
  }
  //perform if any button is pressed
  int ledToLight = gLed1;
  if(!digitalRead(button1)||!digitalRead(button2)||!digitalRead(button3)||!digitalRead(button4)&&corr_seq==0){
    if(!digitalRead(button1)) 
      {buttonCur = '1'; ledToLight = gLed1;}
    else if(!digitalRead(button2)) 
      {buttonCur = '2'; ledToLight = gLed2;}
    else if(!digitalRead(button3)) 
      {buttonCur = '3'; ledToLight = gLed3;}
    else 
      {buttonCur = '4'; ledToLight = gLed4;}
    //if current button pressed is the correct one as defined by the sequence,
    //move to the next digit in sequence, light up corresponding LED
    /*Serial.println(buttonCur);
    Serial.println(sequence[i]);
    Serial.println(ledToLight);
    Serial.println();*/
    if (buttonCur==sequence[i]) {
      i++; 
      digitalWrite(ledToLight,LOW);
      digitalWrite(rLed,HIGH);
      //last digit entered correctly
      if(i>=4){
        i=0;
        corr_seq=1;
        successSequence();
      }
    }
    //else, light up red LED, deduct HP, close all green LED
    else{
      digitalWrite(rLed,LOW);
      HP -= 5;//deduct 10 HP each wrong attempt
      attemptNum += 1;
      digitalWrite(gLed1,HIGH);digitalWrite(gLed2,HIGH);digitalWrite(gLed3,HIGH);digitalWrite(gLed4,HIGH);
      i=0;//reset i
    }
    
  }
  delay(220);

//reset problem, elevator?




  


}
