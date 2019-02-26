/*  manual bot robocon 2018 
 *  microntroller-> arduino meaga 2560
 *   movements  base ->done
 *   rack piston -> done 
 *  stepper arm -> not done
 *  abbrivations mfl-> motor front left
 *               mfr->motor front right
 *               mrl->motor rear left 
 *               mrr->motor rear right 
 *               p ->positive
 *               n->negetive
 *               e-> enable
 *               nu1->pneumatic
 *               
 */

#include <SoftwareSerial.h>
#include <Cytron_PS2Shield.h>

Cytron_PS2Shield ps2(2,3);  //rx and tx 
//.............................motor.............................
#define mfl_p  4       
#define mfl_e  5
#define mfr_p  7  
#define mfr_e  6
#define mrl_p  8
#define mrl_e  9
#define mrr_p 12 
#define mrr_e 10

void setup() 
{


ps2.begin(9600);  // begin ps2
Serial.begin(9600); // begin serail communicarion 
pinMode(mfl_p,OUTPUT);
pinMode(mfl_e,OUTPUT);
pinMode(mfr_p,OUTPUT);
pinMode(mfr_e,OUTPUT);
pinMode(mrl_p,OUTPUT);
pinMode(mrl_e,OUTPUT);
pinMode(mrr_p,OUTPUT);
pinMode(mrr_e,OUTPUT);

}
//.................................main body........................................
void loop()
{   
 if((ps2.readButton(PS2_JOYSTICK_LEFT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)<50))   
   {
    while((ps2.readButton(PS2_JOYSTICK_LEFT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)<50))  
        {
          motor_control(1,1,1,1);  //forward tested
          if(!ps2.readButton(PS2_JOYSTICK_LEFT))
            motor_speed(map(ps2.readButton(PS2_JOYSTICK_LEFT_UP),0,100,0,150)); //Speed
          else
            motor_speed(map(ps2.readButton(PS2_JOYSTICK_LEFT_UP),0,100,0,255)); //Speed
          Serial.println("frwd");
          
        }
   }
 else if((ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)<50))   
   {
    while((ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)<50))  
        {
          motor_control(2,2,2,2);  //back tested
          if(!ps2.readButton(PS2_JOYSTICK_LEFT))
            motor_speed(map(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN),0,100,0,150)); //Speed
          else
            motor_speed(map(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN),0,100,0,255)); //Speed
          Serial.println("bkwd");
        }
   }
else if((ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_UP)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)<50))
   {
    while((ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_UP)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)<50))
        {
          motor_control(2,1,1,2);  //left tested
          if(!ps2.readButton(PS2_JOYSTICK_LEFT))
            motor_speed(map(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT),0,100,0,150)); //Speed
          else
            motor_speed(map(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT),0,100,0,255)); //Speed
          Serial.println("left");
        }
   }            
else if((ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_UP)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)<50))
   {
    while((ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_UP)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)<50))
        {
          
          motor_control(1,2,2,1);  //right tested
          if(!ps2.readButton(PS2_JOYSTICK_LEFT))
            motor_speed(map(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT),0,100,0,150)); //Speed
          else
            motor_speed(map(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT),0,100,0,255)); //Speed
          Serial.println("right");
        }
   }
else if((ps2.readButton(PS2_JOYSTICK_LEFT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)>50))
   {
    while((ps2.readButton(PS2_JOYSTICK_LEFT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)>50))
        {
                
          motor_control(1,0,0,1); // 45 degree  tested 
          if(!ps2.readButton(PS2_JOYSTICK_LEFT))
            motor_speed(150); //Speed
          else
            motor_speed(250); //Speed
          Serial.println("45deg");
         }
    }   
         
else if((ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)>50))
    {
     while((ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)>50))
         {
          
          motor_control(0,1,1,0); // 135 degree  tested 
          if(!ps2.readButton(PS2_JOYSTICK_LEFT))
          motor_speed(150); //Speed
          else
          motor_speed(250); //Speed
          Serial.println("135deg");
         }
    }    
else if((ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)>50))
    {
     while((ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)>50))
         {
          
          motor_control(2,0,0,2); // 225 degree  tested 
          if(!ps2.readButton(PS2_JOYSTICK_LEFT))
          motor_speed(150); //Speed
          else
          motor_speed(250); //Speed
          Serial.println("225deg");
         }
    }    
else if((ps2.readButton(PS2_JOYSTICK_LEFT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)>50))
     {
      while((ps2.readButton(PS2_JOYSTICK_LEFT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)>50))
         {
          
          motor_control(0,2,2,0); // 315 degree  tested 
          if(!ps2.readButton(PS2_JOYSTICK_LEFT))
          motor_speed(150); //Speed
          else
          motor_speed(250); //Speed
          Serial.println("315deg");
         }
     }
else if(ps2.readButton(PS2_LEFT_1)==0)  // rotate left
    {
     while(ps2.readButton(PS2_LEFT_1)==0)
         { 
          motor_speed(150);
          motor_control(2,1,2,1);
          Serial.println("R L");
         }
    }    
else if(ps2.readButton(PS2_RIGHT_1)==0)  // rotate right
    {
     while(ps2.readButton(PS2_RIGHT_1)==0)
         { 
          motor_speed(150);
          motor_control(1,2,1,2);
          Serial.println("R R");
         }
    }   
else
    {
      motor_control(0,0,0,0);
      Serial.println("NOTHING");
    }
     
}
      
//.....................................motor control.................
void motor_control(int mfl,int mfr,int mrl,int mrr)     
  {  
     switch(mfl)   // motor front left
      {
       case 0:  
          digitalWrite(mfl_e,0);
          break;
       case 1:
          digitalWrite(mfl_p,1);
          break; 
       case 2:
          digitalWrite(mfl_p,0);
          break;    
       default: break;   
      }
     switch(mfr)   // motor front right
      {
       case 0:
          digitalWrite(mfr_e,0);
          break;
       case 1:
          digitalWrite(mfr_p,1);
          break; 
       case 2:
          digitalWrite(mfr_p,0);
          break;    
       default: break;   
      } 
     switch(mrl)   // motor rear left
      {
       case 0:
          digitalWrite(mrl_e,0);
          break;
       case 1:
          digitalWrite(mrl_p,1);
          break; 
       case 2:
          digitalWrite(mrl_p,0);
          break;    
       default: break;   
      }  
     switch(mrr)   // motor rear right
     {
       case 0:
          digitalWrite(mrr_e,0);
          break;
       case 1:
          digitalWrite(mrr_p,1);
          break; 
       case 2:
          digitalWrite(mrr_p,0);
          break;    
       default: break;   
     }  
  }
//.......................................................................
//...................................motor speed.........................
void motor_speed(int s)
  {
  analogWrite(mfl_e,s);
  analogWrite(mfr_e,s);
  analogWrite(mrl_e,s);
  analogWrite(mrr_e,s);
  }
//....................................................................


  
