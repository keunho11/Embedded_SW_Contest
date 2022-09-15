#include<Servo.h> //Servo 라이브러리를 추가

Servo servo1;      //Servo 클래스로 servo객체 생성
Servo servo2;
Servo servo3;
Servo servo4;

#define X_STEP_PIN 54
#define X_DIR_PIN 55
#define X_ENABLE_PIN 38
#define X_MIN_PIN 3
#define X_MAX_PIN 2

#define Servo_1 11
#define Servo_2 6
#define Servo_3 5
#define Servo_4 4

char move[4];
int index = 0;
int A_angle = 40;
int B_angle = 50;
int C_angle = 60;
int D_angle = 90;
int R_Step = 0;
int L_Step = 0;
bool flag = false;

void setup() {
  Serial.begin(9600);

  pinMode(X_STEP_PIN , OUTPUT);
  pinMode(X_DIR_PIN , OUTPUT);
  pinMode(X_ENABLE_PIN , OUTPUT);
  digitalWrite(X_ENABLE_PIN , LOW);

  servo1.attach(Servo_1);
  servo1.write(0);
  servo2.attach(Servo_2);
  servo2.write(0);
  servo3.attach(Servo_3);
  servo3.write(0);
  servo4.attach(Servo_4);
  servo4.write(0);
}
// ex)'A.45.E.R.100.\n' : 서보선택/각도/스텝선택/스텝방향/각도
void loop() {
  if (flag == true)
  {
    //angle = (int)move[1];

    switch (move[0]) {
      case 'A':
        SER_1();
        break;
      case 'B':
        SER_2();
        break;
      case 'C':
        SER_3();
        break;
      case 'D':
        SER_4();
        break;
      case 'E':
        Step_X();
        break;
      case '0':
        SET_ON();
        break;
      case '1':
        SET_OFF();
        break;
    }
    //delay(10);
    //Serial.write("ok\n");
    flag = false;
  }
}

void serialEvent()
{
  char myChar = (char)Serial.read();
  move[index] = myChar;
  index++;
  if (myChar == '\n')
  {
    flag = true;
    index = 0;

    //Serial.print(move[0]);
    //Serial.print(move[1]);
    //Serial.print(move[2]);
    //Serial.print(move[3]);
  }
}
void SET_ON()
{
  servo2.write(50);
  delay(2000);
  servo1.write(40);
  servo3.write(60);
  servo4.write(90);

  A_angle = 40;
  B_angle = 50;
  C_angle = 60;
  D_angle = 90;
}
void SET_OFF()
{
  servo4.write(0);
  delay(2000);
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
  
  if(R_Step < L_Step)
  {
    int E_Step = (L_Step - R_Step) * 21;
    digitalWrite(X_DIR_PIN , HIGH);
    for (int i = 0; i <= E_Step; i++)
    {
      digitalWrite(X_STEP_PIN , HIGH);
      delayMicroseconds(2800);             //속도 설정
      digitalWrite(X_STEP_PIN , LOW);
    }
  }
  else
  {
    int E_Step = (R_Step - L_Step) * 21;
    digitalWrite(X_DIR_PIN , LOW);
    for (int i = 0; i <= E_Step; i++)
    {
      digitalWrite(X_STEP_PIN , HIGH);
      delayMicroseconds(2800);             //속도 설정
      digitalWrite(X_STEP_PIN , LOW);
    }
  }
  R_Step = 0;
  L_Step = 0;
  A_angle = 0;
  B_angle = 0;
  C_angle = 0;
  D_angle = 0;
}
void Step_X()
{
  if (move[2] == 'R') {
    digitalWrite(X_DIR_PIN , HIGH);
    R_Step++;
  }
  else if (move[2] == 'L') {
    digitalWrite(X_DIR_PIN , LOW);
    L_Step++;
  }

  //E_angle = map(E_angle, 0, 255, 0, 3200);
  for (int i = 0; i <= 20; i++)
  {
    digitalWrite(X_STEP_PIN , HIGH);
    delayMicroseconds(2800);             //속도 설정
    digitalWrite(X_STEP_PIN , LOW);
  }
}

/*CHANGE------------------------------------------------------------*/
void SER_1()
{
  if(move[1] == 'i') {
    A_angle++;
    if(A_angle > 110) A_angle = 110;
    servo1.write(A_angle);
  }
  else if(move[1] == 'j') {
    A_angle--;
    if(A_angle < 20) A_angle = 20;
    servo1.write(A_angle);
  }
}
void SER_2()
{
  if(move[1] == 'i') {
    B_angle++;
    if(B_angle > 120) B_angle = 120;
    servo2.write(B_angle);
  }
  else if(move[1] == 'j') {
    B_angle--;
    if(B_angle < 0) B_angle = 0;
    servo2.write(B_angle);
  }
}
void SER_3()
{
  if(move[1] == 'i') {
    //C_angle++;
    if(B_angle < 100) C_angle++;
    servo3.write(C_angle);
  }
  else if(move[1] == 'j') {
    //C_angle--;
    if(B_angle > 0) C_angle--;
    servo3.write(C_angle);
  }
}
void SER_4()
{
  if(move[1] == 'i') {
    D_angle++;
    //if(D_angle > 180) D_angle = 179;
    servo4.write(D_angle);
  }
  else if(move[1] == 'j') {
    D_angle--;
    //if(D_angle < 0) D_angle = 0;
    servo4.write(D_angle);
  }
}
