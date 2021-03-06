                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       #include <SoftwareSerial.h>
SoftwareSerial LSAser1(26,27);
SoftwareSerial LSAser2(28,29);
SoftwareSerial LSAser3(30,31);
SoftwareSerial LSAser4(32,33);
//---------------LSA08--------------------------
#define uen1 50
#define uen2 51
#define uen3 52
#define uen4 53
#define jp1 40 
#define jp2 41
#define jp3 42
#define jp4 45
#define an1 A0
#define an2 A1
#define an3 A2
#define an4 A3
char addr = 0;
//---------------MOTORS-------------------------
#define m1_p  7  
#define m1_e  6
#define m2_p 12 
#define m2_e 11
#define m3_p  8
#define m3_e  9
#define m4_p  4       
#define m4_e  5
//RPM
#define intr1 19
#define intr2 18
#define intr3 2
#define intr4 3
//----------------related to isr---------------
int count1=0, count2=0, count3=0, count4=0;
int pcount1=0, pcount2=0, pcount3=0, pcount4=0;
//----------------lsa pid----------------------
int sval1, sval2,sval3, sval4;
int psval1, psval2,psval3, psval4;
int stPtlsa1=330,stPtlsa2=350,stPtlsa3=320,stPtlsa4=350;
int errlsa1,errlsa2,errlsa3,errlsa4;
int preverrlsa1, preverrlsa2, preverrlsa3, preverrlsa4;
int difflsa1,difflsa2,difflsa3,difflsa4;
int corr,speed1, speed2, speed3, speed4;
int basespeed = 70;
float kpl = 0.1;
float kdl = 0.05;
int path_count=0;
//---------------------rp pid-------------------------
float rpm1,rpm2,rpm3,rpm4;
unsigned long prev_t,currt_t,prevpid_t;
int setpoint1 = 0;
int setpoint2 = 0;
int setpoint3 = 0;
int setpoint4 = 0;
int prevsetpoint1 = 0;
int prevsetpoint2 = 0;
int prevsetpoint3 = 0;
int prevsetpoint4 = 0;
boolean flag = false,flagsw = false;
float error1,error2,error3,error4;
float intge1,intge2,intge3,intge4;
float preve1,preve2,preve3,preve4;
float diffe1,diffe2,diffe3,diffe4;
int pwm1,pwm2,pwm3,pwm4;
int fpwm1,fpwm2,fpwm3,fpwm4;
float kp = 0.18;//0.15
float ki = 0.001;//0.001;
float kd = 0.0001;//0.5;
//----------------------------------------------------------------
void update_c1 () {
  //if (digitalRead(22)==HIGH)// (PORTH & B00100000==B00100000) // pin no 8
   count1++;
} 
void update_c2 () {
  
   count2++;
}
void update_c3 () {
  
   count3++;
}
void update_c4 () {
  
   count4++;
}
void rpm_f()
 {

  if(millis()-prev_t>10)
  {
    Serial.println();
    currt_t = millis();
    detachInterrupt(digitalPinToInterrupt(intr1));
    detachInterrupt(digitalPinToInterrupt(intr2));
    detachInterrupt(digitalPinToInterrupt(intr3));
    detachInterrupt(digitalPinToInterrupt(intr4));
     rpm1 = ((count1-pcount1)/(float)(currt_t-prev_t))*60000.f;
     rpm2 = ((count2-pcount2)/(float)(currt_t-prev_t))*60000.f;
     rpm3 = ((count3-pcount3)/(float)(currt_t-prev_t))*60000.f;
     rpm4 = ((count4-pcount4)/(float)(currt_t-prev_t))*60000.f;
     rpm1 = (rpm1/96.0);
     rpm2 = (rpm2/96.0);
     rpm3 = (rpm3/96.0);
     rpm4 = (rpm4/96.0);
    /*
    Serial.print(" DELTA_T =  "); 
    Serial.println((float)(currt_t-prev_t));
    Serial.print("Count 1 : ");
    Serial.println(count1);
    Serial.print("Prev_Count 1 : ");
    Serial.println(pcount1);
    Serial.print("Count 2 : ");
    Serial.println(count2);
    Serial.print("Prev_Count 2");
    Serial.println(pcount2);
    Serial.print("Count 3 ");
    Serial.println(count3);
    Serial.print("Prev_Count 3 ");
    Serial.println(pcount3);
    Serial.print("Count 4 ");
    Serial.println(count4);
    Serial.print("Prev_Count 4 ");
    Serial.println(pcount4);
    Serial.println(" ");
    */
     pcount1=count1;
     pcount2=count2;
     pcount3=count3;
     pcount4=count4;
     prev_t=millis();
    attachInterrupt(digitalPinToInterrupt(intr1), update_c1, RISING);
    attachInterrupt(digitalPinToInterrupt(intr2), update_c2, RISING);
    attachInterrupt(digitalPinToInterrupt(intr3), update_c3, RISING);
    attachInterrupt(digitalPinToInterrupt(intr4), update_c4, RISING);
    Serial.print(" RPM1 = ");
    Serial.print(rpm1);
    Serial.print(" ");
    Serial.print(" RPM2 = ");
    Serial.print(rpm2);
    Serial.print(" ");
    Serial.print(" RPM3 = ");
    Serial.print(rpm3);
    Serial.print(" ");
    Serial.print(" RPM4 = ");
    Serial.println(rpm4);  
  }
}
void pid()
{
 if(millis() - prevpid_t>10){
//    if((flag == true )&&( prevup == ps2.readButton(PS2_JOYSTICK_RIGHT_UP) ||prevdown == ps2.readButton(PS2_JOYSTICK_RIGHT_DOWN) ||prevleft == ps2.readButton(PS2_JOYSTICK_RIGHT_LEFT) ||prevright == ps2.readButton(PS2_JOYSTICK_RIGHT_RIGHT) ))
//   {
//    setpoint1 = setpoint2 = setpoint3 = setpoint4 = min(min(rpm1,rpm2),min(rpm3,rpm4));   
//   }
//   else
//   {
//    flag = false;
//   }
      error1=setpoint1-rpm1;
      error2=setpoint2-rpm2;
      error3=setpoint3-rpm3;
      error4=setpoint4-rpm4;
      intge1+=error1;
      intge2+=error2;
      intge3+=error3;
      intge4+=error4;
      diffe1=error1-preve1;
      diffe2=error2-preve2;
      diffe3=error3-preve3;
      diffe4=error4-preve4;
      pwm1=kp*error1+ki*intge1+kd*diffe1;
      pwm2=kp*error2+ki*intge2+kd*diffe2;
      pwm3=kp*error3+ki*intge3+kd*diffe3;
      pwm4=kp*error4+ki*intge4+kd*diffe4;
      preve1=error1;
      preve2=error2;
      preve3=error3;
      preve4=error4;
      fpwm1 = fpwm1+pwm1;
      fpwm2 = fpwm2+pwm2;
      fpwm3 = fpwm3+pwm3;
      fpwm4 = fpwm4+pwm4;
           
      Serial.print("setpoint = ");
      Serial.print(setpoint1);
      Serial.print(" ");
      Serial.print(setpoint2);
      Serial.print(" ");
      Serial.print(setpoint3);
      Serial.print(" ");
      Serial.print(setpoint4);
      Serial.print(" ");
      //Serial.println();
      /*
      Serial.print("pwm = ");
      Serial.print(fpwm1);
      Serial.print(" ");
      Serial.print(fpwm2);
      Serial.print(" ");
      Serial.print(fpwm3);
      Serial.print(" ");
      Serial.print(fpwm4);
      Serial.print(" ");
      Serial.println();
      */
    if(max(max(fpwm1,fpwm2),max(fpwm3,fpwm4))>=255)
    {
      flag = true;
      fpwm1=constrain(fpwm1,0,255);
      fpwm2=constrain(fpwm2,0,255);
      fpwm3=constrain(fpwm3,0,255);
      fpwm4=constrain(fpwm4,0,255);
    }
    if(min(min(fpwm1,fpwm2),min(fpwm3,fpwm4))<=0)
    {
      fpwm1=constrain(fpwm1,0,255);
      fpwm2=constrain(fpwm2,0,255);
      fpwm3=constrain(fpwm3,0,255);
      fpwm4=constrain(fpwm4,0,255);
    }
motor_speed(fpwm1,fpwm2,fpwm3,fpwm4);
prevpid_t = millis();
}
}
void lsa_send(char command,char data)
{
  char checksum;
  checksum=(command+data+addr);
  LSAser1.write(addr);
  LSAser1.flush();
  LSAser1.write(command);
   LSAser1.flush();
  LSAser1.write(data);
   LSAser1.flush();
  LSAser1.write(checksum);
   LSAser1.flush();
  LSAser2.write(addr);
   LSAser2.flush();
  LSAser2.write(command);
   LSAser2.flush();
  LSAser2.write(data);
   LSAser2.flush();
  LSAser2.write(checksum);
   LSAser2.flush();
  LSAser3.write(addr);
   LSAser3.flush();
  LSAser3.write(command);
  LSAser3.flush();
  LSAser3.write(data);
   LSAser3.flush();
  LSAser3.write(checksum);
   LSAser3.flush();
  LSAser4.write(addr);
   LSAser4.flush();
  LSAser4.write(command);
   LSAser4.flush();
  LSAser4.write(data);
   LSAser4.flush();
  LSAser4.write(checksum);
   LSAser4.flush();
}

void calb()
{
  
 //--------Light Mode---------
 lsa_send('L',0x00);
 //---------Junction Width----
 lsa_send('J',0x05);

 //--------- calibrate--------
 lsa_send('C',0x00);
 pcount1 = count1;
 do{
  motor_control(2,2,1,1);
  motor_speed(50,50,50,50);
  //delay(1000);
  Serial.println("count1");
Serial.println(count1);
 }while((count1 - pcount1)<(90));
  motor_speed(0,0,0,0);
  delay(500);
  pcount1 = count1;
  do
 {
    motor_control(1,1,2,2);
    motor_speed(50,50,50,50);
    Serial.println("count1");
Serial.println(count1);
 }while((count1 - pcount1)<(180));
  motor_speed(0,0,0,0);
  delay(500);
  pcount1 = count1;
 do{
  motor_control(2,2,1,1);
  motor_speed(50,50,50,50);
  Serial.println("count1");
Serial.println(count1);
}while((count1 - pcount1)<(90));
  motor_control(1,1,1,1);
  motor_speed(0,0,0,0);
  delay(500);
}
void motor_control(int m1,int m2,int m3,int m4)     
  {  
     switch(m1)   // motor front right
      {
       case 0:
          analogWrite(m1_e,0);
          break;
       case 1:
          digitalWrite(m1_p,1);
          break; 
       case 2:
          digitalWrite(m1_p,0);
          break;    
       default: break;   
      }
     switch(m2)   // motor rear right
      {
       case 0:
          analogWrite(m2_e,0);
          break;
       case 1:
          digitalWrite(m2_p,1);
          break; 
       case 2:
          digitalWrite(m2_p,0);
          break;    
       default: break;   
      } 
     switch(m3)   // motor rear left
      {
       case 0:
          analogWrite(m3_e,0);
          break;
       case 1:
          digitalWrite(m3_p,1);
          break; 
       case 2:
          digitalWrite(m3_p,0);
          break;    
       default: break;   
      }  
     switch(m4)   // motor front left
      {
       case 0:  
          analogWrite(m4_e,0);
          break;
       case 1:
          digitalWrite(m4_p,1);
          break; 
       case 2:
          digitalWrite(m4_p,0);
          break;    
       default: break;   
      }  
  }
void motor_control1(int m1,int m4)     
  {  
     switch(m1)   // motor front right
      {
       case 0:
          analogWrite(m1_e,0);
          break;
       case 1:
          digitalWrite(m1_p,1);
          break; 
       case 2:
          digitalWrite(m1_p,0);
          break;    
       default: break;   
      }
     
     switch(m4)   // motor front left
      {
       case 0:  
          analogWrite(m4_e,0);
          break;
       case 1:
          digitalWrite(m4_p,1);
          break; 
       case 2:
          digitalWrite(m4_p,0);
          break;    
       default: break;   
      }  
  }
void motor_control3(int m2,int m3)     
  {  
     
     switch(m2)   // motor rear right
      {
       case 0:
          analogWrite(m2_e,0);
          break;
       case 1:
          digitalWrite(m2_p,1);
          break; 
       case 2:
          digitalWrite(m2_p,0);
          break;    
       default: break;   
      } 
     switch(m3)   // motor rear left
      {
       case 0:
          analogWrite(m3_e,0);
          break;
       case 1:
          digitalWrite(m3_p,1);
          break; 
       case 2:
          digitalWrite(m3_p,0);
          break;    
       default: break;   
      }  
     
}
void motor_speed(int s1,int s2,int s3,int s4)
  {
  analogWrite(m1_e,s1);
  analogWrite(m2_e,s2);
  analogWrite(m3_e,s3);
  analogWrite(m4_e,s4);
  }
  void motor_speed1(int s1,int s4)
  {
  analogWrite(m1_e,s1);
  analogWrite(m4_e,s4);
  }
  void motor_speed2(int s1,int s2)
  {
  analogWrite(m1_e,s1);
  analogWrite(m2_e,s2);
  }
  void motor_speed3(int s2,int s3)
  {
  analogWrite(m2_e,s2);
  analogWrite(m3_e,s3);
  }
  void motor_speed4(int s3,int s4)
  {
  
  analogWrite(m3_e,s3);
  analogWrite(m4_e,s4);
  }
  void readsensor()
{
sval1 = analogRead(an1);
sval2 = analogRead(an2);
sval3 = analogRead(an3);
sval4 = analogRead(an4);
Serial.print("ls1 = ");
Serial.println(sval1);
Serial.print("ls3 = ");
Serial.println(sval3);
Serial.print("ls2 = ");
Serial.println(sval2);
Serial.print("ls4 = ");
Serial.println(sval4);
}
void pid1(boolean sw)
  {float corr;
  sval1 = analogRead(an1);
    errlsa1=stPtlsa1-sval1;
    difflsa1=errlsa1-preverrlsa1;
    corr=kpl*errlsa1+kdl*difflsa1;
    preverrlsa1=errlsa1; 
    if(sw)
    {
    speed1  = constrain(basespeed+corr,0,255);
    speed4  = constrain(basespeed-corr,0,255);
    }
    else
    {
    speed1  = constrain(basespeed-corr,0,255);
    speed4  = constrain(basespeed+corr,0,255);
    }
    motor_speed1(speed1,speed4);
  }

void pid3(boolean sw)
  { float corr;
   sval3 = analogRead(an3);
    errlsa3=stPtlsa3-sval3;
    difflsa3=errlsa3-preverrlsa3;
    corr=kpl*errlsa3+kdl*difflsa3;
    preverrlsa3=errlsa3; 
    if(sw){
    speed2  = constrain(basespeed+corr,0,255);
    speed3  = constrain(basespeed-corr,0,255);
    }
    else
    {
    speed2  = constrain(basespeed-corr,0,255);
    speed3  = constrain(basespeed+corr,0,255);
    }
    motor_speed3(speed2,speed3);
  }
 void pid2(boolean sw)
  {float corr;
  sval2 = analogRead(an2);
    errlsa2=stPtlsa2-sval2;
    difflsa2=errlsa2-preverrlsa2;
    corr=kpl*errlsa2+kdl*difflsa2;
    preverrlsa2=errlsa2; 
    if(sw)
    {speed1  = constrain(basespeed-corr,0,255);
    speed2  = constrain(basespeed+corr,0,255);}
    else
    {speed1  = constrain(basespeed+corr,0,255);
    speed2  = constrain(basespeed-corr,0,255);}
    motor_speed2(speed1,speed2);
  }

void pid4(boolean sw)
  { float corr;
   sval4 = analogRead(an4);
    errlsa4=stPtlsa4-sval4;
    difflsa4=errlsa4-preverrlsa4;
    corr=kpl*errlsa4+kdl*difflsa4;
    preverrlsa4=errlsa4; 
    if(sw){
    speed3  = constrain(basespeed+corr,0,255);
    speed4  = constrain(basespeed-corr,0,255);
    }
    else
    {
    speed3  = constrain(basespeed-corr,0,255);
    speed4  = constrain(basespeed+corr,0,255);
    }
    motor_speed4(speed3,speed4);
  }  
void setup()
{
Serial.begin(9600); // begin serail communicarion 
pinMode(13,OUTPUT);
  LSAser1.begin(9600);
  LSAser2.begin(9600);
  LSAser3.begin(9600);
  LSAser4.begin(9600);
pinMode(m4_p,OUTPUT);
pinMode(m4_e,OUTPUT);
pinMode(m1_p,OUTPUT);
pinMode(m1_e,OUTPUT);
pinMode(m3_p,OUTPUT);
pinMode(m3_e,OUTPUT);
pinMode(m2_p,OUTPUT);
pinMode(m2_e,OUTPUT);
pinMode(intr1,INPUT);
pinMode(intr2,INPUT);
pinMode(intr3,INPUT);
pinMode(intr4,INPUT);
pinMode(jp1, INPUT);
pinMode(jp2, INPUT);
pinMode(jp3, INPUT);
pinMode(jp4, INPUT);
pinMode(an1, INPUT);
pinMode(an2, INPUT);
pinMode(an3, INPUT);
pinMode(an4, INPUT);
pinMode(uen1, OUTPUT);
pinMode(uen2, OUTPUT);
pinMode(uen3, OUTPUT);
pinMode(uen4, OUTPUT);
digitalWrite(uen1, LOW);
digitalWrite(uen2, LOW);
digitalWrite(uen3, LOW);
digitalWrite(uen4, LOW);
digitalWrite(intr1,HIGH);
digitalWrite(intr2,HIGH);
digitalWrite(intr3,HIGH);
digitalWrite(intr4,HIGH);
attachInterrupt(digitalPinToInterrupt(intr1), update_c1, RISING);
attachInterrupt(digitalPinToInterrupt(intr2), update_c2, RISING);
attachInterrupt(digitalPinToInterrupt(intr3), update_c3, RISING);
attachInterrupt(digitalPinToInterrupt(intr4), update_c4, RISING);
//calb();

}
void loop()
{
 // readsensor();
//motor_control(2,1,2,1);
//setpoint1 =300; setpoint2 =250; setpoint3 =200; setpoint4 = 150;
//rpm_f();
//pid();
//motor_speed(100,100,100,100);
//Serial.print("JP1 = ");
//Serial.println(digitalRead(jp1));
//Serial.print("JP2 = ");
//Serial.println(digitalRead(jp2));
//Serial.print("JP3 = ");
//Serial.println(digitalRead(jp3));
//Serial.print("JP3 = ");
//Serial.println(digitalRead(jp4));

//followy(1);


switch(path_count)
{
  case 0:{
    p1();
    break;
  }
  case 1:{
    p2();
    break;
  }

   case 2:{
    p3();
    break;
  }
case 3:{
    p4();
    break;
  }
  case 4:{
    p5();
    while(1);
    break;
  }
 case 5:{
    p6();
    break;
 }
 case 6:{
    p7();
    break;
 }
 case 7:{
    p8();
    break;
    
 }
case 8:{
    p9();
    break;
    
 }
  
 
  default:{
    motor_speed(0,0,0,0);
    //followy(1);
    readsensor();
    Serial.println("JP1 = ");
    Serial.println(digitalRead(jp1));
    Serial.println("JP2 = ");
    Serial.println(digitalRead(jp2));
    Serial.println("JP3 = ");
    Serial.println(digitalRead(jp3));
    Serial.println("JP4 = ");
    Serial.println(digitalRead(jp4));
    break;
  }
}
}

void p1()
{
 basespeed = 75 ;
 bool f=true;
 int x=0;
 while(1)
 {
motor_control(2,1,2,1);
setpoint1 =75; setpoint2 =0; setpoint3 =75; setpoint4 =0;
rpm_f();
pid();

    if(digitalRead(jp1))   
     {     
    while(digitalRead(jp1));
        
            while(f)
              {
                if(analogRead(an3)<600&&analogRead(an1)<600)
                 {f=false;}
              motor_control(2,1,2,1);
              setpoint1 =50; setpoint2 =0; setpoint3 =50; setpoint4 =0;
              rpm_f();
              pid();
             }
           
      motor_speed(0,0,0,0);
      path_count++;
      return;    
     }
      
   } 
      
  /*while(analogRead(an4)>600)
   {Serial.println(analogRead(an4));
   motor_speed(0,100,0,100);
   motor_control(0,2,0,2);
   }
   //motor_speed(0,0,0,0);
   while(analogRead(an4)>700)
   {Serial.println(analogRead(an4));
   motor_speed(0,100,0,100);
   motor_control(0,2,0,2);
   }
   motor_speed(0,0,0,0);
   path_count++;
   return;
 */

}
void p2()
{
  basespeed=50;
  motor_speed(0,0,0,0);
  while(1){
    if(basespeed<100)
    {basespeed+=2;}
    followx(0);
    if(digitalRead(jp3))
    {   basespeed=50;
    while(1){
     followx(0); 
     if((analogRead(an2)<700)&&(analogRead(an4)<700))
    {motor_speed(0,0,0,0);
    path_count++;
      //while(1);
      return;
      }
    }
    
    }
   }
}
void p3()
 {
  basespeed=70;
  while(1)
  {
    if(basespeed<100)
       {basespeed=basespeed+2;}
  followy(1);
  if(digitalRead(jp2))
     {
      while(digitalRead(jp2))
        {
          followy(1);
          
         }
        while(!digitalRead(jp2))
        { 
          basespeed = 60;
      followy(0);
        }
//      while(digitalRead(jp2))
//        {
//          followy(0); 
//        }
        motor_speed(0,0,0,0);
        path_count++;
        delay(2000);
        return;
      }  
  }
   }
   void p4()
    {int flag4 = true;
      int count=0;
     basespeed=75;
      while(1)
      {
      followy(1);
      if(digitalRead(jp2))
        {
          count++;
          while(digitalRead(jp2))
            { 
              followy(1);
              }
          }
        if(count==2) 
          {
            while(1)
              {
                followy(1);
                if(analogRead(an1)<600&&analogRead(an3)<600)
                   {
                     motor_speed(0,0,0,0);
                   shoot();
                    break; 
                   }
               } 
          basespeed=50;     
          while(1)
          {Serial.println("yaaa ho");
            followy(0);
            if(digitalRead(jp2))
            {count++;
            while(digitalRead(jp2))
            {
             followy(0);  
            }
            }
            if(count==3)
            {while(1)
            {
              followy(0);
              if(digitalRead(jp2))
              {
              motor_speed(0,0,0,0);
              path_count++;
              break;
              }
            }
              motor_speed(0,0,0,0);
           //   while(1);  
           return;
            }
          }
        }
        
      }
  }
  void p5(){
    basespeed = 75;
    while(1){
      followy(0);
      if(digitalRead(jp4)){
        basespeed = 50;
        while(digitalRead(jp4))
        {
          followy(0);
        }
        while(1)
        {
          followy(0);
       if(analogRead(an1)<600&&analogRead(an3)<600)
          {motor_speed(0,0,0,0);
            path_count++;
            return;
          }
        }
      }
    }
  }
 void p6()
 {basespeed = 75;
  while(1)
    {
      followx(0);
      if(digitalRead(jp3))
      {
        basespeed=50;
        while(digitalRead(jp3))
           {
            followx(0);
            }  
        while(1)
         {
           followx(0);
          if(analogRead(an2)<600&&analogRead(an4))
             {
              motor_speed(0,0,0,0);
              path_count++;
              return;
              }
           }
        }
    }
  }
 void p7()
 {
  basespeed=50;
  
  while(1)
  {
    if(basespeed<75)
       {basespeed=basespeed+2;}
  followy(1);
  if(digitalRead(jp2))
     {
      while(digitalRead(jp2))
        {
          followy(1);
          
         }
        while(!digitalRead(jp2))
        { 
          basespeed = 60;
      followy(0);
        }
//      while(digitalRead(jp2))
//        {
//          followy(0); 
//        }
        motor_speed(0,0,0,0);
        path_count++;
        //delay(2000);
        return;
      }  
  }
   } 
 void p8()
    {int flag4 = true;
      int count=0;
     basespeed=75;
      while(1)
      {
      followy(1);
      if(digitalRead(jp2))
        {
          count++;
          while(digitalRead(jp2))
            { 
              followy(1);
              }
          }
        if(count==2) 
          {
            while(1)
              {
                followy(1);
                if(analogRead(an1)<600&&analogRead(an3)<600)
                   {
                     motor_speed(0,0,0,0);
                   shoot();
                    break; 
                   }
               } 
          basespeed=50;     
          while(1)
          {Serial.println("yaaa ho");
            followy(0);
            if(digitalRead(jp2))
            {count++;
            while(digitalRead(jp2))
            {
             followy(0);  
            }
            }
            if(count==3)
            {while(1)
            {
              followy(0);
              if(digitalRead(jp2))
              {
              motor_speed(0,0,0,0);
              path_count++;
              break;
              }
            }
              motor_speed(0,0,0,0);
           //   while(1);  
           return;
            }
          }
        }
        
      }
  }
void p9()
    {
     int flag4 = true;
      int count=0;
     basespeed=75;
      while(1)
      {
      followy(1);
      if(digitalRead(jp2))
        {
          count++;
          while(digitalRead(jp2))
            { 
              followy(1);
              }
          }
        if(count==4) 
          {
            while(1)
              {
                followy(1);
                if(analogRead(an1)<600&&analogRead(an3)<600)
                   {
                     motor_speed(0,0,0,0);
                   shoot();
                    break; 
                   }
               } 
          basespeed=50;     
          while(1)
          {Serial.println("yaaa ho");
            followy(0);
            if(digitalRead(jp2))
            {count++;
            while(digitalRead(jp2))
            {
             followy(0);  
            }
            }
            if(count==8)
            {while(1)
            {
              followy(0);
              if(digitalRead(jp2))
              {
              motor_speed(0,0,0,0);
              path_count++;
              break;
              }
            }
              motor_speed(0,0,0,0);
              path_count++;
           //   while(1);  
           return;
            }
          }
        }
        
      }
  }   
void shoot()
{
  motor_speed(0,0,0,0);
  digitalWrite(13,1);
  for(int i = 0;i<500;i++)
  Serial.println("SHOOOT");

  return;
  
  
}
void followx(boolean sw)
{

  if(sw)
    motor_control(1,1,1,1);
  else
    motor_control(2,2,2,2);
  readsensor();  
  if(sval1<700)
  {
   psval1 = sval1;
   pid1(sw);
   //if(abs(sval1-psval1)<50)
  }
  else
  {
  if(sw){
   if(psval1<stPtlsa1)
   motor_speed1(basespeed,0);
   else if(psval1>stPtlsa1)
   motor_speed1(0,basespeed);
   else
   motor_speed1(basespeed,basespeed);
   }
  else
   {
   if(psval1<stPtlsa1)
   motor_speed1(0,basespeed);
   else if(psval1>stPtlsa1)
   motor_speed1(basespeed,0);
   else
   motor_speed1(basespeed,basespeed);
   }
  }
 readsensor();
 if(sval3<800)
  {
    psval3 = sval3;
    pid3(sw);
//if(abs(sval3-psval3)<50)
  }
  else
  {
   if(sw){
      if(psval3<stPtlsa3)
      motor_speed3(basespeed,0);
      else if(psval3>stPtlsa3)
      motor_speed3(0,basespeed);
      else
      motor_speed3(basespeed,basespeed);
    }
   else{
      if(psval3<stPtlsa3)
      motor_speed3(0,basespeed);
      else if(psval3>stPtlsa3)
      motor_speed3(basespeed,0);
      else
      motor_speed3(basespeed,basespeed);
   }
  }
//  Serial.println("psval1 = ");
//  Serial.println(psval1);
//  Serial.println("psval3 = ");
//  Serial.println(psval3);
//  
}

void followy(boolean sw)
{
  if(sw)
    motor_control(2,1,2,1);
  else
    motor_control(1,2,1,2);
 readsensor();   
 if(sval2<800)
  {
   psval2 = sval2; 
   pid2(sw);
  }
 else
  {
   if(sw){
     if(psval2>stPtlsa2)
        motor_speed2(basespeed,0);
     else if(psval2<stPtlsa2)
       motor_speed2(0,basespeed);
     else
     motor_speed2(basespeed,basespeed);    
   }
   else{
    if(psval2>stPtlsa2)
    motor_speed2(0,basespeed);
    else if(psval2<stPtlsa2)
    motor_speed2(basespeed,0);
    else
    motor_speed2(basespeed,basespeed); 
   }
  }

readsensor();
 if(sval4<800)
  {  
    psval4 = sval4;
    pid4(sw);
  }
 else
  {
    if(sw)
    {
     if(psval4<stPtlsa4)//-
      motor_speed4(basespeed,0);
     else if(psval4>stPtlsa4)//+
      motor_speed4(0,basespeed);
     else
      motor_speed4(basespeed,basespeed); 
    } 
    else
    {
     if(psval4<stPtlsa4)//-
       motor_speed4(0,basespeed);
     else if(psval4>(stPtlsa4))//+
       motor_speed4(basespeed,0);
     else
     motor_speed4(basespeed,basespeed); 
    }
    
  }
  Serial.println("psval2 = ");
  Serial.println(psval2);
  Serial.println("psval4 = ");
  Serial.println(psval4);
  
}

