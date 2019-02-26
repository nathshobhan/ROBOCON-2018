#include <SoftwareSerial.h>
#include <Cytron_PS2Shield.h>
#include<Servo.h>

Servo claw;
Cytron_PS2Shield ps2(25,29);  //rx and tx 


#define mp 43
#define me 10
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
#define clawpin 6
#define status_led 13
int dc_count=0;
int stepp_count1=0;
int stepp_count2=0;
int pos=0;
boolean flag = false;
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);  
ps2.begin(115200);
pinMode(pul1, OUTPUT);
pinMode(pul2, OUTPUT);
pinMode(dir1, OUTPUT);
pinMode(dir2, OUTPUT);
pinMode(ena1, OUTPUT);
pinMode(ena2, OUTPUT);
pinMode(clawpin, OUTPUT);
pinMode(mp, OUTPUT);
pinMode(me, OUTPUT);
pinMode(mcha, INPUT);
pinMode(mcha, INPUT);
pinMode(status_led, OUTPUT);
Serial.println("READY");
digitalWrite(ena1,HIGH);
digitalWrite(ena2,HIGH);

}

void loop() {
  // put your main code here, to run repeatedly:  
//if((ps2.readButton(PS2_JOYSTICK_LEFT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)<50))   
//   {
//    while((ps2.readButton(PS2_JOYSTICK_LEFT_UP)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)<50))  
//        {
//          analogWrite(me, 200);
//          digitalWrite(mp,HIGH);
//          if(digitalRead(mcha)==HIGH)
//          {
//            while(digitalRead(mcha));
//            dc_count++;
//          }
//        }
//     analogWrite(me, 0);
//   }
//
// if((ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)<50))   
//   {
//    while((ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)<50))  
//         {
//          //dwn
//          analogWrite(me, 150);
//          digitalWrite(mp,LOW);
//          if(digitalRead(mcha)==HIGH)
//            {
//             while(digitalRead(mcha));
//             dc_count--;
//            }
//          
//          }
//          analogWrite(me, 0);
//          
//   }
//
// if((ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_UP)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)<50))
//   {digitalWrite(dir1,HIGH);
//    while((ps2.readButton(PS2_JOYSTICK_LEFT_LEFT)>50))
//        {
//         if(flag==false)
//           {
digitalWrite(dir1,HIGH);
              for (int i=0; i<(6400/252)*90; i++)    
             {         //(0=clockwise  //  1=anticlockwise)
               if(i<100)
               {digitalWrite(pul1,HIGH); //(high to low on this pin generates 1 step on stepper motor )
               delayMicroseconds(1000-i);
               digitalWrite(pul1,LOW);
               delayMicroseconds(1000-i);}
               else
               {
               digitalWrite(pul1,HIGH); //(high to low on this pin generates 1 step on stepper motor )
               delayMicroseconds(500);
               digitalWrite(pul1,LOW);
               delayMicroseconds(500);
               }
               stepp_count1--;
             }
             delay(1000);
//           }
//         else
//         { 
//           for (int i=0; i<97; i++)    
//           {   //(0=clockwise  //  1=anticlockwise)
//             digitalWrite(pul1,HIGH); //(high to low on this pin generates 1 step on stepper motor )
//             delayMicroseconds(500);
//             digitalWrite(pul1,LOW);
//             delayMicroseconds(500);
//             stepp_count1--;
//           }
//         }
//         Serial.println("LEFT");
//        }
//        
//   }
//
// if((ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)>50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_UP)<50)&&(ps2.readButton(PS2_JOYSTICK_LEFT_DOWN)<50))
//   {digitalWrite(dir1,LOW);
//    while((ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)>50))
//        { 
//         if(flag==false)
//          {
digitalWrite(dir1,LOW);
for (int i=0; i<(6400/252)*90; i++)    
           {         //(0=clockwise  //  1=anticlockwise)
             if(i<100)
               {digitalWrite(pul1,HIGH); //(high to low on this pin generates 1 step on stepper motor )
               delayMicroseconds(1000-i);
               digitalWrite(pul1,LOW);
               delayMicroseconds(1000-i);
               }
             else
               {
               digitalWrite(pul1,HIGH); //(high to low on this pin generates 1 step on stepper motor )
               delayMicroseconds(500);
               digitalWrite(pul1,LOW);
               delayMicroseconds(500);
               }
               stepp_count1++;
             }
             delay(1000);
//          }
//         else
//         { 
//           for (int i=0; i<97; i++)    
//           {   //(0=clockwise  //  1=anticlockwise)
//             digitalWrite(pul1,HIGH); //(high to low on this pin generates 1 step on stepper motor )
//             delayMicroseconds(50);
//             digitalWrite(pul1,LOW);
//             delayMicroseconds(50);
//             stepp_count1++;
//           }
//         }
//        while((ps2.readButton(PS2_JOYSTICK_LEFT_RIGHT)>50));
//        Serial.println("LEFT");
//       }
//        
//   }
// if(ps2.readButton(PS2_SQUARE)==0)  
//    {
//     while(ps2.readButton(PS2_SQUARE)==0)
//         {         
//         digitalWrite(dir2,HIGH);
//           if(flag)
//             {
digitalWrite(dir2,HIGH);
               for (int i=0; i<(6400/7)*5; i++)    
               {         
                 digitalWrite(pul2,HIGH); //(high to low on this pin generates 1 step on stepper motor )
                 delayMicroseconds(100);
                 digitalWrite(pul2,LOW);
                 delayMicroseconds(100);
                 stepp_count2--;
               }
               delay(1000);
//             } 
//           else
//             { 
//             for (int i=0; i<(6400/7)*0.1; i++)    
//               {
//                 digitalWrite(pul2,HIGH);
//                 delayMicroseconds(50);
//                 digitalWrite(pul2,LOW);
//                 delayMicroseconds(50);
//                 stepp_count2--;
//               }
//             }
////        while(!ps2.readButton(PS2_SQUARE));
//         Serial.println("FORWARD");
//         }
//         
//    }
//
// if(ps2.readButton(PS2_CIRCLE)==0)  
//    {
//     while(ps2.readButton(PS2_CIRCLE)==0)
//       {  Serial.println("x");       
//          digitalWrite(dir2,LOW);
//          if(flag)
//           {
digitalWrite(dir2,LOW);
             for (int i=0; i<(6400/7)*5; i++)    
             {         
               digitalWrite(pul2,HIGH); //(high to low on this pin generates 1 step on stepper motor )
               delayMicroseconds(100);
               digitalWrite(pul2,LOW);
               delayMicroseconds(100);
               stepp_count2++;
             }
             delay(1000);
//           } 
//           else
//           { 
//           for (int i=0; i<(6400/7)*0.1; i++)    
//             {
//               digitalWrite(pul2,HIGH);
//               delayMicroseconds(50);
//               digitalWrite(pul2,LOW);
//               delayMicroseconds(50);
//               stepp_count2++;
//             }
//           }
//         }
//     }
//// if(ps2.readButton(PS2_RIGHT_2)==0)  
////    {
////     while(ps2.readButton(PS2_RIGHT_2)==0)
////         {         
////          //CLAW
////          claw.attach(clawpin);
////          if(claw.read()==0)
////          for (pos = 0; pos <= 180; pos++) 
////            { 
////            claw.write(pos);              // tell servo to go to position in variable 'pos'
////            delay(15);                   // waits 15ms for the servo to reach the position
////            }
////          else if(claw.read()==180)
////            for (pos = 180; pos >= 0; pos--) 
////            { 
////            claw.write(pos);              // tell servo to go to position in variable 'pos'
////            delay(15);                   // waits 15ms for the servo to reach the position
////            }
////          claw.detach();
////         }
////         
////    }  
////if(ps2.readButton(PS2_SELECT)==0&&ps2.readButton(PS2_START)==0)
////    {delay(100);
////      while(ps2.readButton(PS2_SELECT)==0);
////      flag =!flag;
////      digitalWrite(status_led,flag);   
////    }
Serial.println("NOTHING");
}
  
