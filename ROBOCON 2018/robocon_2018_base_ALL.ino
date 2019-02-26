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
#define intr3 19
#define intr4 18

float rpm1,rpm2,rpm3,rpm4;

long count1 = 0;
long pcount1 = 0;
long count2 = 0;
long pcount2 = 0;
long count3 = 0;
long pcount3 = 0;
long count4 = 0;
long pcount4 = 0;
unsigned long prev_t;
int setpoint;
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
            setpoint = map(ps2.readButton(PS2_JOYSTICK_LEFT_UP),0,100,0,150); //Speed
          else
            setpoint = map(ps2.readButton(PS2_JOYSTICK_LEFT_UP),0,100,0,255); //Speed
          motor_speed(setpoint,setpoint,setpoint,setpoint);  
          
          rpm_f();
        }
   }
 else if((ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)<50))   
   {Serial.println("bkwd");
    while((ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)<50))  
        {
          motor_control(2,2,2,2);  //back tested
         if(!ps2.readButton(PS2_JOYSTICK_LEFT))
            setpoint = map(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN),0,100,0,150); //Speed
          else
            setpoint = map(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN),0,100,0,255); //Speed
          motor_speed(setpoint,setpoint,setpoint,setpoint);  
          
          rpm_f();
        }
   }
else if((ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_UP)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)<50))
   {Serial.println("left");
    while((ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_UP)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)<50))
        {
          motor_control(2,1,2,1);  //left tested
          if(!ps2.readButton(PS2_JOYSTICK_LEFT))
            setpoint = map(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT),0,100,0,150); //Speed
          else
            setpoint = map(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT),0,100,0,255); //Speed
          motor_speed(setpoint,setpoint,setpoint,setpoint);  
          
          rpm_f();
        }
   }            
else if((ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_UP)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)<50))
   {
    while((ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_UP)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)<50))
        {
          
          motor_control(1,2,1,2);  //right tested
          
          if(!ps2.readButton(PS2_JOYSTICK_LEFT))
            setpoint = map(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT),0,100,0,150); //Speed
          else
            setpoint = map(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT),0,100,0,255); //Speed
          motor_speed(setpoint,setpoint,setpoint,setpoint);  
          
          rpm_f();
        }
   }
else if((ps2.readButton(PS2_JOYSTICK_LEFT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)>50))
   {Serial.println("45deg");
    while((ps2.readButton(PS2_JOYSTICK_LEFT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)>50))
        {
                
          motor_control(0,1,0,1); // 45 degree  tested 
          if(!ps2.readButton(PS2_JOYSTICK_LEFT))
            motor_speed(0,150,0,150); //Speed
          else
            motor_speed(0,250,0,250); //Speed
          
          rpm_f();
         }
    }   
         
else if((ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)>50))
    {Serial.println("135deg");
     while((ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)>50))
         {
          
          motor_control(2,0,2,0); // 135 degree  tested 
          if(!ps2.readButton(PS2_JOYSTICK_LEFT))
            motor_speed(150,0,150,0); //Speed
          else
            motor_speed(250,0,250,0); //Speed
          
         }
    }    
else if((ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)>50))
    {Serial.println("225deg");
     while((ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)>50))
         {
          
          motor_control(0,2,0,2); // 225 degree  tested 
          if(!ps2.readButton(PS2_JOYSTICK_LEFT))
            motor_speed(0,150,0,150); //Speed
          else
            motor_speed(0,250,0,250); //Speed
          
          rpm_f();
         }
    }    
else if((ps2.readButton(PS2_JOYSTICK_LEFT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)>50))
     {Serial.println("315deg");
      while((ps2.readButton(PS2_JOYSTICK_LEFT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)>50))
         {
          
          motor_control(1,0,1,0); // 315 degree  tested 
          if(!ps2.readButton(PS2_JOYSTICK_LEFT))
            motor_speed(150,0,150,0); //Speed
          else
            motor_speed(250,0,250,0); //Speed
          
          rpm_f();
         }
     }
else if(ps2.readButton(PS2_LEFT_1)==0)  // rotate left
    {
     while(ps2.readButton(PS2_LEFT_1)==0)
         { 
          motor_speed(250,250,250,250); //Speed);
          motor_control(2,2,1,1);
          Serial.println("R L");
          rpm_f();
         }
    }    
else if(ps2.readButton(PS2_RIGHT_1)==0)  // rotate right
    {
     while(ps2.readButton(PS2_RIGHT_1)==0)
         { 
         
          motor_speed(250,250,250,250); //Speed);
          motor_control(1,1,2,2);
          Serial.println("R R");
          rpm_f();
         }
    }   
else
    {
      motor_control(0,0,0,0);
      Serial.println("NOTHING");
    }
   rpm_f(); 
}
void rpm_f()
 {
  if(millis()-prev_t>=1000)
   {  
    Serial.println();
    detachInterrupt(digitalPinToInterrupt(intr1));
    detachInterrupt(digitalPinToInterrupt(intr2));
    detachInterrupt(digitalPinToInterrupt(intr3));
    detachInterrupt(digitalPinToInterrupt(intr4));
     rpm1 = count1-pcount1;
     rpm2 = count2-pcount2;
     rpm3 = count3-pcount3;
     rpm4 = count4-pcount4;
     rpm1 = (rpm1/96)*60;
     rpm2 = (rpm2/96)*60;
     rpm3 = (rpm2/96)*60;
     rpm4 = (rpm2/96)*60;
    Serial.print("RPM1 = ");
    Serial.print(rpm1);
    Serial.print("RPM2 = ");
    Serial.print(rpm2);
    Serial.print("RPM3 = ");
    Serial.print(rpm3);
    Serial.print("RPM4 = ");
    Serial.print(rpm4);
    Serial.println(" ");
    /*Serial.print(count1);Serial.print("  ");
    Serial.print(pcount1);
    Serial.println(" ");
    Serial.print(count2);Serial.print("  ");
    Serial.print(pcount2);
    Serial.println(" ");
    Serial.print(count3);Serial.print("  ");
    Serial.print(pcount3);
    Serial.println(" ");
    Serial.print(count4);Serial.print("  ");
    Serial.print(pcount4);
    Serial.println(" ");*/
    pcount1=count1;
    pcount2=count2;
    pcount3=count3;
    pcount4=count4;
    attachInterrupt(digitalPinToInterrupt(intr1), update_c1, RISING);
    attachInterrupt(digitalPinToInterrupt(intr2), update_c2, RISING);
    attachInterrupt(digitalPinToInterrupt(intr3), update_c3, RISING);
    attachInterrupt(digitalPinToInterrupt(intr4), update_c4, RISING);
    prev_t=millis();
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


  
