int L1=A5;
int L1_State;
int L2=A4;
int L2_State;
int in1 =11;
int in2 =12;
int enA =6;
int received3=3;
int received2=4;
int received2_state=0;
int received3_state=0;
int done2=0;
int done3=0;
void setup() {
  // put your setup code here, to run once:
  pinMode(L1,INPUT);
  pinMode(L2,INPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(enA,OUTPUT);
  pinMode(received2,INPUT);
  pinMode(received3,INPUT);
  Serial.begin(9600);
  int received3_state=digitalRead(received3);
  int received2_state=digitalRead(received2);

  while(L1_State==0){
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    digitalWrite(enA,150);
    L1_State=digitalRead(L1);
  }
  if(L1_State==1){
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
    digitalWrite(enA,0);
  }
  delay(3000);
  Serial.println("reset");
}

void loop() {
//   // put your main code here, to run repeatedly:
  L1_State=digitalRead(L1);
  L2_State=digitalRead(L2);
  received3_state=digitalRead(received3);
  received2_state=digitalRead(received2);

  if (received2_state==1 && done2==0){
    lift_to_2();
    delay(3000);
    Serial.println("lift to 2");
    done2=1;
  }

  if(received3_state==1 && done3==0){
    lift_to_3();
    delay(1000);
    Serial.println("lift to 3");
    done3=1;
  }

}

void lift_to_3(){
  delay(2000);
  while(L2_State==0){
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
    digitalWrite(enA,150);
    L2_State=digitalRead(L2);
  }
  if(L2_State==1){
      digitalWrite(in1,LOW);
      digitalWrite(in2,LOW);
      digitalWrite(enA,0);
  }
}

void lift_to_2(){
  delay(2000);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(enA,150);
  delay(2850);
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(enA,0);
}

void lift_to_1(){
  delay(2000);
  while(L1_State==0){
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    digitalWrite(enA,150);
    L1_State=digitalRead(L1);
  }
  if(L1_State==1){
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
    digitalWrite(enA,0);
  }
}
