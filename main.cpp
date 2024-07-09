#define ENA 13
#define ENB 12
#define IN1  5
#define IN2  4
#define IN3  16
#define IN4  17
#define sensor1 27
#define sensor2 33
#define sensor3 25
#define sensor4 26
#define sensor5 14
int IR;
int S;
int A;
int T;
int Z;
int basespeedleft = 200;
int basespeedright = 200  ;
const int frequency = 500;
const int pwm_channelA = ENA;
const int pwm_channelB = ENB;
const int resolution = 8;
int pos = 0;
int error = 0;
int setpoint = 0;
int Kp = 80;
int Ki = 0.7;
int Kd =1.15;
int U ; //control signal
int Maxspeedleft = 200;
int Minispeedleft = 0;
int Maxspeedright = 200;
int Minispeedright = 0;
int Mleft;
int Mright;
int lasterror = 0;
int I=0;
int D=0;
void setup() {
  pinMode (ENA, OUTPUT);
  pinMode (ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
  pinMode(sensor4, INPUT);
  pinMode(sensor5, INPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  Serial.begin(115200);
  ledcSetup(pwm_channelA, frequency, resolution); 
  ledcSetup(pwm_channelB, frequency, resolution); 
  ledcAttachPin(ENA, pwm_channelA);
  ledcAttachPin(ENB, pwm_channelB);
  //Serial.println("Setup complete."); // Debug statement
}

void loop() {
  IR = digitalRead(sensor1);
  S = digitalRead(sensor2);
  A = digitalRead(sensor3);
  T = digitalRead(sensor4);
  Z = digitalRead(sensor5);
  Serial.print(IR);
  Serial.print(S);
  Serial.print(A);
  Serial.print(T);
  Serial.println(Z);
  if (IR == HIGH && S == HIGH && A == LOW && T == HIGH && Z == HIGH ) //11011
  { 
   pos = 0;
  }
  else if (IR == HIGH && S == HIGH && A == LOW && T == LOW && Z == HIGH ) //11001
  { 
   pos = 1;
  }
  else if (IR == HIGH && S == HIGH && A == HIGH && T == LOW && Z == HIGH ) //11101
  { 
   pos = 2;
  }
  else if (IR == HIGH && S == HIGH && A == HIGH && T == LOW && Z == LOW ) //11100
  { 
   pos = 7 ;
  }
   else if (IR == HIGH && S == HIGH && A == HIGH && T == HIGH && Z == LOW ) //11110
  { 
   pos = 13 ;
  }
  else if (IR == HIGH && S == HIGH && A == LOW && T == LOW && Z == LOW ) //11000
  { 
   pos = 32 ;
  }
   else   if (IR == HIGH && S == HIGH && A == HIGH && T == HIGH && Z == HIGH ) //11111
  { 
    pos = 0;
  }
  else if (IR == HIGH && S == LOW && A == LOW && T == HIGH && Z == HIGH ) //10011
  { 
  pos = -1; 
  }
   else if (IR == HIGH && S == LOW && A == HIGH && T == HIGH && Z == HIGH ) //10111
  { 
   pos =-2;
  }
   else if (IR == LOW && S == LOW && A == HIGH && T == HIGH && Z == HIGH ) //00111
  { 
  pos = -7;
   }
  
  else if (IR == LOW && S == HIGH && A == HIGH && T == HIGH && Z == HIGH ) //01111
  { 
   pos = -13;
  }
    else if (IR == LOW && S == LOW && A == LOW && T == HIGH && Z == HIGH ) //00011
  { 
   pos = -32 ;
  }
  
 // Serial.println(pos);
error = setpoint - pos;
I = I+error;
D = error - lasterror;
lasterror = error;
U = Kp*error+Ki*I+Kd*D;
Serial.println(U);
//Serial.println(pos);
Mleft = basespeedleft+U;
if(Mleft > Maxspeedleft)
{
  Mleft = Maxspeedleft;
}
else if(Mleft < Minispeedleft)
{
  Mleft = Minispeedleft;
}
Mright = basespeedright-U;
if(Mright > Maxspeedright)
{
  Mright = Maxspeedright;
}
if(Mright < Minispeedright)
{
  Mright = Minispeedright;
}
        ledcWrite(pwm_channelA, Mleft);
         digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        ledcWrite(pwm_channelB, Mright);
         digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
      delay(20);
    //  Serial.println(Mleft);
    //  Serial.println(Mright);
}