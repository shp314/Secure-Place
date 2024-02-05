#include <SoftwareSerial.h> //serial comm
#include <Wire.h> //I2C for communication with 2 slaves   

#define BluetoothRxD 2
#define BluetoothTxD 3

SoftwareSerial blueToothSerial(BluetoothRxD,BluetoothTxD); 

int x; //random password
int y; //random potentiometer values
String x_temp;

#define NO_OF_CHAR 4
char password[NO_OF_CHAR+1];

void setup() {
  Serial.begin(9600);

  Wire.begin();

  blueToothSerial.begin(9600);
  pinMode(BluetoothRxD, INPUT);                             
  pinMode(BluetoothTxD, OUTPUT); 
}

void loop() {
  x=random(1000,9999);
  x_temp=String(x);
  blueToothSerial.println(x);
  Serial.println(x);
  x_temp.toCharArray(password,5); 
  Wire.beginTransmission(1); 
  Wire.write(x >> 8); // sends the most significant byte
  Wire.write(x & 0xFF); // sends the least significant byte
  for(int i=0;i<4;i++){                                                                                                                    
    Serial.println(password[i]);
  }
  Wire.endTransmission();  
  delay(8000);
  resetPassword();

}

void resetPassword() {
  for (int k = 0;k<(NO_OF_CHAR);k++) password[k] = ' ';
  password[NO_OF_CHAR] = 0;
}
