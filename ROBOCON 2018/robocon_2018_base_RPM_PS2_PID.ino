/*  manual bot robocon 2018 
 *  microntroller-> arduino meaga 2560
 *   movements  base ->done
 *   rack piston -> done 
 *  stepper arm -> not done
 *  abbrivations m4-> motor front left
 *               m1->motor front right
 *               m3->motor rear left 
 *               m2->motor rear right 
 *               p ->positive
 *               n->negetive
 *               e-> enable
 *               nu1->pneumatic
 *               
 */

#include <SoftwareSerial.h>
#include <Cytron_PS2Shield.h>

Cytron_PS2Shield ps2(50,51);  //rx and tx 
//.............................motor.............................

#define m1_p  7  
#define m1_e  6
#define m2_p 12 
#define m2_e 11
#define m3_p  8
#define m3_e  9
#define m4_p  4       
#define m4_e  5
//rpm
#define intr1 21
#define intr2 20
#define intr3 2
#define intr4 3

float rpm1,rpm2,rpm3,rpm4;
//-------------------------------------------------------------
volatile long count1 = 0;
long pcount1 = 0;
volatile long count2 = 0;
long pcount2 = 0;
volatile long count3 = 0;
long pcount3 = 0;
volatile long count4 = 0;
long pcount4 = 0;
//-------------------------------------------------------------
unsigned long prev_t,currt_t,prevpid_t;
int setpoint1 = 0;
int setpoint2 = 0;
int setpoint3 = 0;
int setpoint4 = 0;
int prevsetpoint1 = 0;
int prevsetpoint2 = 0;
int prevsetpoint3 = 0;
int prevsetpoint4 = 0;
boolean flag = false;
int prevup,prevdown,prevleft,prevright;
//------------------------------------------------------------
float error1,error2,error3,error4;
float intge1,intge2,intge3,intge4;
float preve1,preve2,preve3,preve4;
float diffe1,diffe2,diffe3,diffe4;
int pwm1,pwm2,pwm3,pwm4;
int fpwm1,fpwm2,fpwm3,fpwm4;
float kp = 0.2;
float ki = 0.002;
float kd = 0.02;
//-------------------------------------------------------------
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

void setup() 
{


ps2.begin(9600);  // begin ps2
Serial.begin(9600); // begin serail communicarion 
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
digitalWrite(intr1,HIGH);
digitalWrite(intr2,HIGH);
digitalWrite(intr3,HIGH);
digitalWrite(intr4,HIGH);

attachInterrupt(digitalPinToInterrupt(intr1), update_c1, RISING);
attachInterrupt(digitalPinToInterrupt(intr2), update_c2, RISING);
attachInterrupt(digitalPinToInterrupt(intr3), update_c3, RISING);
attachInterrupt(digitalPinToInterrupt(intr4), update_c4, RISING);

}
//.................................main body........................................
void loop()
{   
 if((ps2.readButton(PS2_JOYSTICK_LEFT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)<50))   
   {Serial.println("frwd");
    while((ps2.readButton(PS2_JOYSTICK_LEFT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)<50))  
        {
          motor_control(1,1,1,1);  //forward tested
          
          if(!ps2.readButton(PS2_JOYSTICK_LEFT))
            setpoint1 = setpoint2 = setpoint3 = setpoint4  = map(ps2.readButton(PS2_JOYSTICK_LEFT_UP),0,100,0,200); //Speed
          else
            setpoint1 = setpoint2 = setpoint3 = setpoint4  = map(ps2.readButton(PS2_JOYSTICK_LEFT_UP),0,100,0,400); //Speed
          
          pid();
          rpm_f();
          
        }
        motor_speed(0,0,0,0);
        setpoint1= setpoint2 = setpoint3 = setpoint4 =0;
        fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
   }
 else if((ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)<50))   
   {Serial.println("bkwd");
    while((ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)<50))  
         {
          motor_control(2,2,2,2);  //back tested
         if(!ps2.readButton(PS2_JOYSTICK_LEFT))
            setpoint1 = setpoint2 = setpoint3 = setpoint4  = map(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN),0,100,0,150); //Speed
          else
            setpoint1 = setpoint2 = setpoint3 = setpoint4  = map(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN),0,100,0,300); //Speed
          
          pid();
          rpm_f();
          }
          motor_speed(0,0,0,0);
          setpoint1= setpoint2 = setpoint3 = setpoint4 =0;
          fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;

   }
else if((ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_UP)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)<50))
   {Serial.println("left");
    while((ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_UP)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)<50))
        {
          motor_control(1,2,1,2);  //left tested
          if(!ps2.readButton(PS2_JOYSTICK_LEFT))
           setpoint1 = setpoint2 = setpoint3 = setpoint4  = map(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT),0,100,0,150); //Speed
          else
           setpoint1 = setpoint2 = setpoint3 = setpoint4  = map(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT),0,100,0,300); //Speed
          
          pid();
          rpm_f();
          
        }
        motor_speed(0,0,0,0);
        setpoint1= setpoint2 = setpoint3 = setpoint4 =0;
        fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
   }            
else if((ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_UP)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)<50))
   {
    while((ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_UP)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)<50))
        {
          
          motor_control(2,1,2,1);  //right tested
          
          if(!ps2.readButton(PS2_JOYSTICK_LEFT))
            setpoint1 = setpoint2 = setpoint3 = setpoint4  = map(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT),0,100,0,150); //Speed
          else
            setpoint1 = setpoint2 = setpoint3 = setpoint4  = map(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT),0,100,0,300); //Speed

          pid();
          rpm_f();
          
        }
        motor_speed(0,0,0,0);
        setpoint1= setpoint2 = setpoint3 = setpoint4 =0;
        fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
   }
else if((ps2.readButton(PS2_JOYSTICK_LEFT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)>50))
   {Serial.println("45deg");
    while((ps2.readButton(PS2_JOYSTICK_LEFT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)>50))
        {
                
          motor_control(0,1,0,1); // 45 degree  tested 
          if(!ps2.readButton(PS2_JOYSTICK_LEFT))
          {setpoint2 = setpoint4 = 150; //Speed
          
          }
          else
          {setpoint2 = setpoint4 = 250; //Speed
           
          }
           setpoint1 = setpoint3 = -5;
          pid();
          rpm_f();
          
         }
         motor_speed(0,0,0,0);
         setpoint1= setpoint2 = setpoint3 = setpoint4 =0;
         fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
    }   
         
else if((ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)>50))
    {Serial.println("135deg");
     while((ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)>50))
         {
          
          motor_control(2,0,2,0); // 135 degree  tested 
         if(!ps2.readButton(PS2_JOYSTICK_LEFT))
          {setpoint1 = setpoint3 = 150; //Speed
          
          }
          else
          {setpoint1 = setpoint3 = 250; //Speed
          
          }
           setpoint2 = setpoint4 = -5;
          pid();
          rpm_f();
          
         }
         motor_speed(0,0,0,0);
         setpoint1= setpoint2 = setpoint3 = setpoint4 =0;
         fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
    }    
else if((ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)>50))
    {Serial.println("225deg");
     while((ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)>50))
         {
          
          motor_control(0,2,0,2); // 225 degree  tested 
          if(!ps2.readButton(PS2_JOYSTICK_LEFT))
          {setpoint2 = setpoint4 = 150; //Speed
           
          }
          else
          {setpoint2 = setpoint4 = 250; //Speed
          
          }
          setpoint1 = setpoint3 = -5;
          pid();
          rpm_f();
          
         }
         motor_speed(0,0,0,0);
         setpoint1= setpoint2 = setpoint3 = setpoint4 =0;
         fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
    }    
else if((ps2.readButton(PS2_JOYSTICK_LEFT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)>50))
     {Serial.println("315deg");
      while((ps2.readButton(PS2_JOYSTICK_LEFT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)>50))
         {
          
          motor_control(1,0,1,0); // 315 degree  tested 
          if(!ps2.readButton(PS2_JOYSTICK_LEFT))
          {setpoint1 = setpoint3 = 150; //Speed
          
          }
          else
          {setpoint1 = setpoint3 = 250; //Speed
          
          } 
           setpoint2 = setpoint4 = -5;       
          pid();
          rpm_f();
          
         }
         motor_speed(0,0,0,0);
         setpoint1= setpoint2 = setpoint3 = setpoint4 =0;
         fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
     }
else if(ps2.readButton(PS2_LEFT_1)==0)  // rotate left
    {
     while(ps2.readButton(PS2_LEFT_1)==0)
         { 
          setpoint1 = setpoint2 = setpoint3 = setpoint4 = 250;//Speed);
          motor_control(2,2,1,1);
          Serial.println("R L");
          pid();
          rpm_f();
          
         }
         motor_speed(0,0,0,0);
         setpoint1= setpoint2 = setpoint3 = setpoint4 =0;
         fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
    }    
else if(ps2.readButton(PS2_RIGHT_1)==0)  // rotate right
    {
     while(ps2.readButton(PS2_RIGHT_1)==0)
         {         
          setpoint1 = setpoint2 = setpoint3 = setpoint4 = 250;
          motor_control(1,1,2,2);
          Serial.println("R R");
          pid();
          rpm_f();
         }
         motor_speed(0,0,0,0);
         setpoint1= setpoint2 = setpoint3 = setpoint4 =0;
         fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
    }   
else
    {
      motor_control(0,0,0,0);
      motor_speed(0,0,0,0);
      Serial.println("NOTHING");
      setpoint1= setpoint2 = setpoint3 = setpoint4 =0;
      fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
    }
   rpm_f(); 
}
void pid()
{
 

if(millis() - prevpid_t>10){
//    if((flag == true )&&( prevup == ps2.readButton(PS2_JOYSTICK_LEFT_UP) ||prevdown == ps2.readButton(PS2_JOYSTICK_LEFT_DOWN) ||prevleft == ps2.readButton(PS2_JOYSTICK_LEFT_LEFT) ||prevright == ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT) ))
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
      prevup = ps2.readButton(PS2_JOYSTICK_LEFT_UP);
      prevdown = ps2.readButton(PS2_JOYSTICK_LEFT_DOWN);
      prevleft = ps2.readButton(PS2_JOYSTICK_LEFT_LEFT);
      prevright = ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT);
      fpwm1 = fpwm1+pwm1;
      fpwm2 = fpwm2+pwm2;
      fpwm3 = fpwm3+pwm3;
      fpwm4 = fpwm4+pwm4;
      if(setpoint1<=0)
      { 
        fpwm1=0;
      }
       if(setpoint2<=0)
      { 
        fpwm2=0;
      }
       if(setpoint3<=0)
      { 
        fpwm3=0;
      }
       if(setpoint4<=0)
      { 
        fpwm4=0;
      }
      
      Serial.print("setpoint = ");
      Serial.print(setpoint1);
      Serial.print(" ");
      Serial.print(setpoint2);
      Serial.print(" ");
      Serial.print(setpoint3);
      Serial.print(" ");
      Serial.print(setpoint4);
      Serial.print(" ");
      Serial.println();
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
    Serial.println(rpm1);
    Serial.print(" RPM2 = ");
    Serial.println(rpm2);
    Serial.print(" RPM3 = ");
    Serial.println(rpm3);
    Serial.print(" RPM4 = ");
    Serial.println(rpm4);
    
    
  }
}
      
//.....................................motor control.................
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
//.......................................................................
//...................................motor speed.........................
void motor_speed(int s1,int s2,int s3,int s4)
  {
  analogWrite(m1_e,s1);
  analogWrite(m2_e,s2);
  analogWrite(m3_e,s3);
  analogWrite(m4_e,s4);
  }
//....................................................................


  

