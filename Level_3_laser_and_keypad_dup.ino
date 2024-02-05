#include <Wire.h> // I2C
#include <LiquidCrystal.h> //LCD
const int rs = 6, en = 7, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int KB_DataAvailable=A0;
int KB_A=10;
int KB_B=11;
int KB_C=12;
int KB_D=13;


char keys[] = {'1','2','3','F',
               '4','5','6','E',
               '7','8','9','D',
               'A','0','B','C'};
            
#define NO_OF_CHAR 4
char expression[NO_OF_CHAR+1];
char password[NO_OF_CHAR+1];

int laser=8;
int laser_sensor=9;
int laser_state;

int buzzer=A1;

int write_pin=A2; //communicating with third arduino
int write_pin2=A3;

bool password_checker=false;

String x_temp;

void setup() {
  Serial.begin(9600);

  Wire.begin(1);  
  Wire.onReceive(joinPassword);

  pinMode(laser,OUTPUT);
  pinMode(laser_sensor,INPUT);

  pinMode(A2,OUTPUT);
  pinMode(A3,OUTPUT);

  pinMode(KB_DataAvailable,INPUT); //LCD
  pinMode(KB_A,INPUT);
  pinMode(KB_B,INPUT);
  pinMode(KB_C,INPUT);
  pinMode(KB_D,INPUT);
  lcd.begin(16, 2);
  for(int j = 0;j < NO_OF_CHAR;j++) expression[j] = ' '; 

  analogWrite(write_pin,LOW);
  //analogWrite(write_pin2,LOW);
}

void loop() {
  lcd.setCursor(0,0);
  lcd.print("Enter PIN:");
  digitalWrite(laser,HIGH);
  laser_state=digitalRead(laser_sensor);
  if (laser_state==HIGH){
    tone(buzzer,1000);
    //analogWrite(write_pin,255);
  }
  /*else if (laser_state==LOW){
    noTone(buzzer);
  }*/
  Serial.println(password);
  if(digitalRead(KB_DataAvailable)){
    KB_Read();
    delay(250);
  lcd.setCursor(10,0);
  lcd.print(expression);
  }
}

void KB_Read() {
  int ka,kb,kc,kd,i;
  char k;
  ka=digitalRead(KB_A); 
  kb=digitalRead(KB_B); 
  kc=digitalRead(KB_C); 
  kd=digitalRead(KB_D); 

  k=ka+kb*2+kc*4+kd*8; 
  
  for(i=0;i<(NO_OF_CHAR-1);i++) expression[i]=expression[i+1];
  expression[NO_OF_CHAR-1]=keys[k]; 

  if(expression[NO_OF_CHAR-4]!=' '){ //check if typed password matches generated password
    for (int i=0;i<4;i++){
      if (expression[i]==password[i]){
        password_checker=true; 
      }
      else{
        password_checker=false;
        break;
      }
    }
    if (password_checker==true){
      digitalWrite(laser,LOW);
      noTone(buzzer);
      analogWrite(write_pin,255);
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Access Granted");
      Serial.print("correct");
      resetExpression();
      lcd.setCursor(10,0);
      lcd.print(expression);
      delay(15000);
      lcd.setCursor(0,0);
      lcd.print("Enter PIN:");
      ;lcd.clear();
    }
    else if (password_checker==false){
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Access Denied");
      Serial.print("wrong");
      delay(1000);
      lcd.clear();
      resetExpression();
    }
  }
}

void resetExpression() {
  for (int l = 0;l<(NO_OF_CHAR);l++) expression[l] = ' ';
  expression[NO_OF_CHAR] = 0;
}

void resetPassword() {
  for (int k = 0;k<(NO_OF_CHAR);k++) password[k] = ' ';
  password[NO_OF_CHAR] = 0;
}

void joinPassword(int howMany){
  if (Wire.available()){
    int x = (Wire.read() << 8) | Wire.read(); // read the two bytes and combine 
    x_temp=String(x);
    x_temp.toCharArray(password,5); 
    } 
  }

