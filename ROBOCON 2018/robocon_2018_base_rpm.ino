
//pin 1 connect to 2(int0)
//pin 2 connect to 9
//pin 3(final count) connect to 3(int1)

long count1 = 0;
long pcount1 = 0;
long count2 = 0;
long pcount2 = 0;
long count3 = 0;
long pcount3 = 0;
long count4 = 0;
long pcount4 = 0;
unsigned long prev_t;
void update_c1 () {
  if(digitalRead(22)==HIGH)//(PORTH&B00100000==B00100000) // pin no 8
   count1++;
  else 
   count1--;
  
} 
void update_c2 () {
  if(digitalRead(23)==HIGH)//(PORTH&B01000000==B01000000)  //pin no 9
   count2++;
  else 
   count2--;
  
}
void update_c3 () {
  if(digitalRead(24)==HIGH)//(PORTB&B00010000==B00010000)  // 10
   count3++;
  else 
   count3--;
  
}
void update_c4 () {
  if(digitalRead(25)==HIGH)//(PORTB&B00100000==B00100000)  // 11
   count4++;
  else 
   count4--;
  
}


void setup() {

  // put your setup code here, to run once:
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(21), update_c1, RISING);
  attachInterrupt(digitalPinToInterrupt(20), update_c2, RISING);
  attachInterrupt(digitalPinToInterrupt(19), update_c3, RISING);
  attachInterrupt(digitalPinToInterrupt(18), update_c4, RISING);
}

void loop() {
 if(millis()-prev_t>=1000)
 {  
  Serial.println();
  detachInterrupt(digitalPinToInterrupt(21));
  detachInterrupt(digitalPinToInterrupt(20));
  detachInterrupt(digitalPinToInterrupt(19));
  detachInterrupt(digitalPinToInterrupt(18));
  float rpm1 = (abs(count1-pcount1)/96)*60;
  float rpm2 = (abs(count2-pcount2)/96)*60;
  float rpm3 = (abs(count3-pcount3)/96)*60;
  float rpm4 = (abs(count4-pcount4)/96)*60;
  Serial.print("1= ");
  Serial.print(rpm1);
  Serial.print(" 2= ");
  Serial.print(rpm2);
  Serial.print(" 3= ");
  Serial.print(rpm3);
  Serial.print(" 4= ");
  Serial.print(rpm4);
  pcount1=count1;
  pcount2=count2;
  pcount3=count3;
  pcount4=count4;
  attachInterrupt(digitalPinToInterrupt(21), update_c1, RISING);
  attachInterrupt(digitalPinToInterrupt(20), update_c2, RISING);
  attachInterrupt(digitalPinToInterrupt(19), update_c3, RISING);
  attachInterrupt(digitalPinToInterrupt(18), update_c4, RISING);
  prev_t=millis();
 }

}


