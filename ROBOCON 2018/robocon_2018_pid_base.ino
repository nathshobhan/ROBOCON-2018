#define mfr_e 0
#define mrr_e 0
#define mfl_e 0
#define mrl_e 0
int setpoint = 0;
int prevsetpoint = 0;
boolean flag = false;
int rpm1,rpm2,rpm3,rpm4;
int error1,error2,error3,error4;
int intge1,intge2,intge3,intge4;
int preve1,preve2,preve3,preve4;
int diffe1,diffe2,diffe3,diffe4;
int pwm1,pwm2,pwm3,pwm4;
int fpwm1,fpwm2,fpwm3,fpwm4;
float kp = 0;
float ki = 0;
float kd = 0;

void setup() {
  // put your setup code here, to run once:
pinMode(mfr_e,OUTPUT);
pinMode(mrr_e,OUTPUT);
pinMode(mfl_e,OUTPUT);
pinMode(mrl_e,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
pid();
}
void pid()
{
 
   if(flag == true && setpoint == prevsetpoint)
   {
    setpoint = min(min(rpm1,rpm2),min(rpm3,rpm4));   
   }
   else
   {
    flag = false;
   }
      error1=setpoint-rpm1;
      error2=setpoint-rpm2;
      error3=setpoint-rpm3;
      error4=setpoint-rpm4;
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
      prevsetpoint = setpoint;
      fpwm1 = fpwm1+pwm1;
      fpwm2 = fpwm2+pwm2;
      fpwm3 = fpwm3+pwm3;
      fpwm4 = fpwm4+pwm4;
    if(max(max(fpwm1,fpwm2),max(fpwm3,fpwm4))>255)
    {
      flag = true;
      fpwm1=constrain(fpwm1,0,255);
      fpwm2=constrain(fpwm2,0,255);
      fpwm3=constrain(fpwm3,0,255);
      fpwm4=constrain(fpwm4,0,255);
    }
    
drive();
}
void drive()
{
 analogWrite(mfr_e,fpwm1);
 analogWrite(mrr_e,fpwm2);
 analogWrite(mfl_e,fpwm3);
 analogWrite(mrl_e,fpwm4); 
}
