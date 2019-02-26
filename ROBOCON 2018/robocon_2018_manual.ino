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
#include<Servo.h>.
Servo claw;
Cytron_PS2Shield ps2(12,15);  //rx and tx 
//.............................motor.............................

#define m1_p  24 
#define m1_e  7
#define m2_p 36 
#define m2_e 10
#define m3_p  28
#define m3_e  8
#define m4_p  32       
#define m4_e  9
//rpm
#define intr1 2
#define intr2 19
#define intr3 3
#define intr4 18

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
boolean flagsw = false;
int prevup,prevdown,prevleft,prevright;
//------------------------------------------------------------
float error1,error2,error3,error4;
float intge1,intge2,intge3,intge4;
float preve1,preve2,preve3,preve4;
float diffe1,diffe2,diffe3,diffe4;
int pwm1,pwm2,pwm3,pwm4;
int fpwm1,fpwm2,fpwm3,fpwm4;
float kp = 0.1;//0.15;
float ki = 0.001;//0.005;
float kd = 0.0;//0.5;
//-------------------------------------------------------------
#define mp 40
#define me 11
#define mcha 44
#define mchb 45
//----------------
#define pul2 45
#define dir2 49
#define ena2 53
//----------------
#define pul1 33
#define dir1 37
#define ena1 41
//----------------
#define clawpin 52  
#define rack_r1 48
#define rack_r2 44
#define status_led 13
int oang = 50,cang = 150;
int dc_count=0;
int stepp_count1=0;
int stepp_count2=0;
int pos=0;
boolean flag = false;
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
//---------------------------------------
pinMode(pul1, OUTPUT);
pinMode(pul2, OUTPUT);
pinMode(dir1, OUTPUT);
pinMode(dir2, OUTPUT);
pinMode(ena1, OUTPUT);
pinMode(ena2, OUTPUT);
pinMode(clawpin, OUTPUT);
pinMode(rack_r1, OUTPUT);
pinMode(rack_r2, OUTPUT);
pinMode(mp, OUTPUT);
pinMode(me, OUTPUT);
pinMode(mcha, INPUT);
pinMode(mcha, INPUT);
pinMode(status_led, OUTPUT);

digitalWrite(ena1,HIGH);
digitalWrite(ena2,HIGH);
digitalWrite(rack_r1,HIGH);
digitalWrite(rack_r2,HIGH);
//---------------------------
digitalWrite(intr1,HIGH);
digitalWrite(intr2,HIGH);
digitalWrite(intr3,HIGH);
digitalWrite(intr4,HIGH);

attachInterrupt(digitalPinToInterrupt(intr1), update_c1, RISING);
attachInterrupt(digitalPinToInterrupt(intr2), update_c2, RISING);
attachInterrupt(digitalPinToInterrupt(intr3), update_c3, RISING);
attachInterrupt(digitalPinToInterrupt(intr4), update_c4, RISING);
Serial.println("READY");
}
//.................................main body........................................
void loop()
{   
 if((ps2.readButton(PS2_JOYSTICK_RIGHT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_RIGHT_LEFT)<50)&&(ps2.readButton(PS2_JOYSTICK_RIGHT_RIGHT)<50))   
   {Serial.println("frwd");
    while((ps2.readButton(PS2_JOYSTICK_RIGHT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_RIGHT_LEFT)<50)&&(ps2.readButton(PS2_JOYSTICK_RIGHT_RIGHT)<50))  
        {
          motor_control(1,1,1,1);  //forward tested
          if(!ps2.readButton(PS2_JOYSTICK_RIGHT))
            {
              if(flagsw==false){
               fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
               flagsw = true; 
               }
            setpoint1 = setpoint2 = setpoint3 = setpoint4  = map(ps2.readButton(PS2_JOYSTICK_RIGHT_UP),0,100,0,50); //Speed
            }
          else
            {
              if(flagsw==true){
               fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
               flagsw = false; 
               } 
             setpoint1 = setpoint2 = setpoint3 =setpoint4  = map(ps2.readButton(PS2_JOYSTICK_RIGHT_UP),0,100,0,50); //Speed
            }
          //motor_speed(setpoint1,setpoint2,setpoint3,setpoint4)   ;           
          pid();
          rpm_f();
           
        }
        motor_speed(0,0,0,0);
        setpoint1= setpoint2 = setpoint3 = setpoint4 =0;
        fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
   }
 else if((ps2.readButton(PS2_JOYSTICK_RIGHT_DOWN)>50)&&(ps2.readButton(PS2_JOYSTICK_RIGHT_LEFT)<50)&&(ps2.readButton(PS2_JOYSTICK_RIGHT_RIGHT)<50))   
   {Serial.println("bkwd");
    while((ps2.readButton(PS2_JOYSTICK_RIGHT_DOWN)>50)&&(ps2.readButton(PS2_JOYSTICK_RIGHT_LEFT)<50)&&(ps2.readButton(PS2_JOYSTICK_RIGHT_RIGHT)<50))  
         {
          motor_control(2,2,2,2);  //back tested
         if(!ps2.readButton(PS2_JOYSTICK_RIGHT))
         {
            if(flagsw==false){
               fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
               flagsw = true; 
               } 
            setpoint1 = setpoint2 = setpoint3 = setpoint4  = map(ps2.readButton(PS2_JOYSTICK_RIGHT_DOWN),0,100,0,100); //Speed
         }
          else
            {
             if(flagsw==true){
               fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
               flagsw = false; 
               }
             setpoint1 = setpoint2 = setpoint3 = setpoint4  = map(ps2.readButton(PS2_JOYSTICK_RIGHT_DOWN),0,100,0,200); //Speed
            }
          
          pid();
          rpm_f();
          }
          motor_speed(0,0,0,0);
          setpoint1= setpoint2 = setpoint3 = setpoint4 =0;
          fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;

   }
else if((ps2.readButton(PS2_JOYSTICK_RIGHT_LEFT)>50)&&(ps2.readButton(PS2_JOYSTICK_RIGHT_UP)<50)&&(ps2.readButton(PS2_JOYSTICK_RIGHT_DOWN)<50))
   {Serial.println("left");
    while((ps2.readButton(PS2_JOYSTICK_RIGHT_LEFT)>50)&&(ps2.readButton(PS2_JOYSTICK_RIGHT_UP)<50)&&(ps2.readButton(PS2_JOYSTICK_RIGHT_DOWN)<50))
        {
          motor_control(1,2,1,2);  //left tested
          if(!ps2.readButton(PS2_JOYSTICK_RIGHT))
          {
           if(flagsw==false){
               fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
               flagsw = true; 
               } 
           setpoint1 = setpoint2 = setpoint3 = setpoint4  = map(ps2.readButton(PS2_JOYSTICK_RIGHT_LEFT),0,100,0,100); //Speed
          }
          else
          {
           if(flagsw==true){
               fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
               flagsw = false; 
               } 
           setpoint1 = setpoint2 = setpoint3 = setpoint4  = map(ps2.readButton(PS2_JOYSTICK_RIGHT_LEFT),0,100,0,200); //Speed
          }
          pid();
          rpm_f();
          
        }
        motor_speed(0,0,0,0);
        setpoint1= setpoint2 = setpoint3 = setpoint4 =0;
        fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
   }            
else if((ps2.readButton(PS2_JOYSTICK_RIGHT_RIGHT)>50)&&(ps2.readButton(PS2_JOYSTICK_RIGHT_UP)<50)&&(ps2.readButton(PS2_JOYSTICK_RIGHT_DOWN)<50))
   {Serial.println("RIGHT");
    while((ps2.readButton(PS2_JOYSTICK_RIGHT_RIGHT)>50)&&(ps2.readButton(PS2_JOYSTICK_RIGHT_UP)<50)&&(ps2.readButton(PS2_JOYSTICK_RIGHT_DOWN)<50))
        {
          
          motor_control(2,1,2,1);  //right tested
          
          if(!ps2.readButton(PS2_JOYSTICK_RIGHT))
          {
            if(flagsw==false){
               fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
               flagsw = true; 
               } 
            setpoint1 = setpoint2 = setpoint3 = setpoint4  = map(ps2.readButton(PS2_JOYSTICK_RIGHT_RIGHT),0,100,0,100); //Speed
          }
          else
          {
            if(flagsw==true){
               fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
               flagsw = false; 
               } 
            setpoint1 = setpoint2 = setpoint3 = setpoint4  = map(ps2.readButton(PS2_JOYSTICK_RIGHT_RIGHT),0,100,0,200); //Speed
          }

          pid();
          rpm_f();
          
        }
    motor_speed(0,0,0,0);
    setpoint1= setpoint2 = setpoint3 = setpoint4 =0;
    fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
   }
else if((ps2.readButton(PS2_JOYSTICK_RIGHT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_RIGHT_RIGHT)>50))
   {Serial.println("45deg");
    while((ps2.readButton(PS2_JOYSTICK_RIGHT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_RIGHT_RIGHT)>50))
        {
                
          motor_control(0,1,0,1); // 45 degree  tested 
          if(!ps2.readButton(PS2_JOYSTICK_RIGHT))
          {
            if(flagsw==false){
               fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
               flagsw = true; 
               }
            setpoint2 = setpoint4 = 100; //Speed
          }
          else
          {
            if(flagsw==true){
               fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
               flagsw = false; 
               }
            setpoint2 = setpoint4 = 200; //Speed
           
          }
           setpoint1 = setpoint3 = -5;
          pid();
          rpm_f();
          }
         motor_speed(0,0,0,0);
         setpoint1= setpoint2 = setpoint3 = setpoint4 =0;
         fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
    }   
         
else if((ps2.readButton(PS2_JOYSTICK_RIGHT_RIGHT)>50)&&(ps2.readButton(PS2_JOYSTICK_RIGHT_DOWN)>50))
    {Serial.println("135deg");
     while((ps2.readButton(PS2_JOYSTICK_RIGHT_RIGHT)>50)&&(ps2.readButton(PS2_JOYSTICK_RIGHT_DOWN)>50))
         {
          
          motor_control(2,0,2,0); // 135 degree  tested 
         if(!ps2.readButton(PS2_JOYSTICK_RIGHT))
          {
            if(flagsw==false){
               fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
               flagsw = true; 
               }
            setpoint1 = setpoint3 = 100; //Speed
          }
          else
          {
            if(flagsw==true){
               fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
               flagsw = false; 
               }
            setpoint1 = setpoint3 = 200; //Speed
          
          }
           setpoint2 = setpoint4 = -5;
          pid();
          rpm_f();
          
         }
         motor_speed(0,0,0,0);
         setpoint1= setpoint2 = setpoint3 = setpoint4 =0;
         fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
    }    
else if((ps2.readButton(PS2_JOYSTICK_RIGHT_DOWN)>50)&&(ps2.readButton(PS2_JOYSTICK_RIGHT_LEFT)>50))
    {Serial.println("225deg");
     while((ps2.readButton(PS2_JOYSTICK_RIGHT_DOWN)>50)&&(ps2.readButton(PS2_JOYSTICK_RIGHT_LEFT)>50))
         {
          
          motor_control(0,2,0,2); // 225 degree  tested 
          if(!ps2.readButton(PS2_JOYSTICK_RIGHT))
          {
            if(flagsw==false){
               fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
               flagsw = true; 
               }
            setpoint2 = setpoint4 = 100; //Speed
           
          }
          else
          {
            if(flagsw==true){
               fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
               flagsw = false; 
               }
            setpoint2 = setpoint4 = 200; //Speed
          
          }
          setpoint1 = setpoint3 = -5;
          pid();
          rpm_f();
          
         }
         motor_speed(0,0,0,0);
         setpoint1= setpoint2 = setpoint3 = setpoint4 =0;
         fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
    }    
else if((ps2.readButton(PS2_JOYSTICK_RIGHT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_RIGHT_LEFT)>50))
     {Serial.println("315deg");
      while((ps2.readButton(PS2_JOYSTICK_RIGHT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_RIGHT_LEFT)>50))
         {
          
          motor_control(1,0,1,0); // 315 degree  tested 
          if(!ps2.readButton(PS2_JOYSTICK_RIGHT))
          {
            if(flagsw==false){
               fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
               flagsw = true; 
               }
            setpoint1 = setpoint3 = 100; //Speed
          
          }
          else
          {
            if(flagsw==true){
               fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
               flagsw = false; 
               }
            setpoint1 = setpoint3 = 200; //Speed
          
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
          if(!ps2.readButton(PS2_JOYSTICK_RIGHT))
          {
          if(flagsw==false){
               fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
               flagsw = true; 
               }
          setpoint1 = setpoint2 = setpoint3 = setpoint4 = 25;//Speed);
          }
          else
          {
           if(flagsw==true){
               fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
               flagsw = false; 
               } 
          setpoint1 = setpoint2 = setpoint3 = setpoint4 = 50;//Speed);
          }          
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
          if(!ps2.readButton(PS2_JOYSTICK_RIGHT))
          {
           if(flagsw==false){
               fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
               flagsw = true; 
               }
          setpoint1 = setpoint2 = setpoint3 = setpoint4 = 25;//Speed);
          }
          else
          {
            if(flagsw==true){
               fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
               flagsw = false; 
               }
          setpoint1 = setpoint2 = setpoint3 = setpoint4 = 50;//Speed);
          }     
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
      motor_control(1,1,1,1);
      motor_speed(0,0,0,0);
      Serial.println("NOTHING");
      setpoint1=0, setpoint2 =0, setpoint3 =0, setpoint4 =0;
      rpm_f();
      //pid();
      fpwm1 = fpwm2 = fpwm3 = fpwm4 = 0;
    }
//----------------------------------------------------arm---------------------------------------
if((ps2.readButton(PS2_JOYSTICK_LEFT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)<50))   
   {
    while((ps2.readButton(PS2_JOYSTICK_LEFT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)<50))  
        {
//          analogWrite(me, 200);
//          digitalWrite(mp,HIGH);
//          if(digitalRead(mcha)==HIGH)
//          {
//            while(digitalRead(mcha));
//            dc_count++;
//          }
            arm_motor(LOW,200);
        }
          analogWrite(me, 0);
   }

 if((ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)<50))   
   {
    while((ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)<50))  
         {
            arm_motor(HIGH,200);
         }
          analogWrite(me, 0);
          
   }

 if((ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_UP)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)<50))
   {
    while((ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)>50))
        {
         if(flag==false)
           {
              stepper1(HIGH,90);
           }
         else
           { 
              stepper1(HIGH,4);
           }
         Serial.println("LEFT");
        }
        
   }

 if((ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_UP)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)<50))
   {digitalWrite(dir1,LOW);
    while((ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)>50))
        { 
         if(flag==false)
          {

              stepper1(LOW,90);
          }
         else
         { 
              stepper1(LOW,4);
         }
//        while((ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)>50));
        Serial.println("RIGHT");
       }
        
   }
 if(ps2.readButton(PS2_SQUARE)==0)  
    {
     while(ps2.readButton(PS2_SQUARE)==0)
         {                  
           if(flag)
             {
                stepper2(HIGH,5);
             } 
           else
             { 
                stepper2(HIGH,.5);
             }
//        while(!ps2.readButton(PS2_SQUARE));
         Serial.println("FORWARD");
         }
         
    }

 if(ps2.readButton(PS2_CIRCLE)==0)  
    {
     while(ps2.readButton(PS2_CIRCLE)==0)
       {             
          if(flag)
           {
              stepper2(LOW,5);
           } 
           else
           { 
              stepper2(LOW,.5);
           }
         }
     }
 if(ps2.readButton(PS2_TRIANGLE)==0)  
    {
     while(ps2.readButton(PS2_TRIANGLE)==0)
         {         
          //CLAW
          claw.attach(clawpin);
          for (pos = 150; pos <= 180; pos++) 
            { 
            claw.write(180);              // tell servo to go to position in variable 'pos'
            delay(15);                   // waits 15ms for the servo to reach the position
            }
          claw.detach();
          Serial.println(claw.read());
          while(ps2.readButton(PS2_TRIANGLE)==0);
         }
         
    }  
    if(ps2.readButton(PS2_CROSS)==0)  
    {
     while(ps2.readButton(PS2_CROSS)==0)
         {         
          //CLAW
          claw.attach(clawpin);
         
            for (pos = 180; pos >= 150; pos--) 
            { 
            claw.write(150);              // tell servo to go to position in variable 'pos'
            delay(15);                   // waits 15ms for the servo to reach the position
            }
          claw.detach();
          while(ps2.readButton(PS2_CROSS)==0);
         }
         
    }  
if(!ps2.readButton(PS2_RIGHT_2))
    {delay(200);
      if(!ps2.readButton(PS2_RIGHT_2))
        digitalWrite(rack_r1,LOW);
    }
else if(!ps2.readButton(PS2_LEFT_2)){
      delay(200);
      if(!ps2.readButton(PS2_LEFT_2))
        digitalWrite(rack_r1,HIGH);
    }
    
if(ps2.readButton(PS2_SELECT)==0&&ps2.readButton(PS2_START)==0)
    {delay(100);
      while(ps2.readButton(PS2_SELECT)==0);
      flag =!flag;
      digitalWrite(status_led,flag);   
    }
Serial.println("NOTHING");

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
      prevup = ps2.readButton(PS2_JOYSTICK_RIGHT_UP);
      prevdown = ps2.readButton(PS2_JOYSTICK_RIGHT_DOWN);
      prevleft = ps2.readButton(PS2_JOYSTICK_RIGHT_LEFT);
      prevright = ps2.readButton(PS2_JOYSTICK_RIGHT_RIGHT);
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
      
//      Serial.print("setpoint = ");
//      Serial.print(setpoint1);
//      Serial.print(" ");
//      Serial.print(setpoint2);
//      Serial.print(" ");
//      Serial.print(setpoint3);
//      Serial.print(" ");
//      Serial.print(setpoint4);
//      Serial.print(" ");
//      Serial.println();
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
     rpm1 = (rpm1/70.0);
     rpm2 = (rpm2/70.0);
     rpm3 = (rpm3/70.0);
     rpm4 = (rpm4/70.0);
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
void stepper1(boolean dir,int j)
{j = (6400/252)*j;
int k=0;
  digitalWrite(dir1,dir);
  for(int i = 0;i<j;i++)
  {if(k>1000)
    k=1000;
    if(i<(j/3)){
      digitalWrite(pul1,HIGH); //(high to low on this pin generates 1 step on stepper motor )
      delayMicroseconds(1000-i);
      digitalWrite(pul1,LOW);
      delayMicroseconds(1000-i);
    }
    else if(i>(2*(j/3))){
      digitalWrite(pul1,HIGH); //(high to low on this pin generates 1 step on stepper motor )
      delayMicroseconds(300+k);
      digitalWrite(pul1,LOW);
      delayMicroseconds(300+k);
      k++;
    }
    else
    {
      digitalWrite(pul1,HIGH); //(high to low on this pin generates 1 step on stepper motor )
      delayMicroseconds(300);
      digitalWrite(pul1,LOW);
      delayMicroseconds(300);
    }
//      digitalWrite(pul1,HIGH); //(high to low on this pin generates 1 step on stepper motor )
//      delayMicroseconds(50);
//      digitalWrite(pul1,LOW);
//      delayMicroseconds(50);
  }
}
void stepper2(boolean dir,float j)
{j = (6400/7)*j;
  digitalWrite(dir2,dir);

  for(int i = 0;i<j;i++)
  {
      digitalWrite(pul2,HIGH); //(high to low on this pin generates 1 step on stepper motor )
      delayMicroseconds(205);
      digitalWrite(pul2,LOW);
      delayMicroseconds(205);
  }
}  
void arm_motor(boolean dir,int as)
{
  analogWrite(me, as);
  digitalWrite(mp,dir);
}
void servo(boolean dir)
{
  claw.attach(clawpin);
  if(dir){
    for(int i=cang;i>oang;i--){
      claw.write(i);
      delay(10);
    }
   }
  else{
     for(int i=oang;i<cang;i++){
      claw.write(i);
      delay(10);
    } 
   }
  claw.detach();
}
void control_motors() 
{
 if((ps2.readButton(PS2_JOYSTICK_LEFT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)<50))   
   {
    while((ps2.readButton(PS2_JOYSTICK_LEFT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)<50))  
        {
          //up
        }
        
   }

 if((ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)<50))   
   {
    while((ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)<50))  
         {
          //dwn
          }
          
   }

 if((ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_UP)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)<50))
   {
    while((ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_UP)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)<50))
        {
          //rt turn 
         
        }
        
   }

 if((ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_UP)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)<50))
   {
    while((ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_UP)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)<50))
        {
          // lft turn
          
        }
        
   }
 if(ps2.readButton(PS2_SQUARE)==0)  
    {
     while(ps2.readButton(PS2_SQUARE)==0)
         {         
          //ARM BACKWARDS
         }
         
    }

 if(ps2.readButton(PS2_CIRCLE)==0)  
    {
     while(ps2.readButton(PS2_CIRCLE)==0)
         {         
          //ARM FORWARD
         }
         
    }

 if(ps2.readButton(PS2_RIGHT_2)==0)  
    {
     while(ps2.readButton(PS2_RIGHT_2)==0)
         {         
          //CLAW
         }
         
    }  

}
  
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
