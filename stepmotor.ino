//스텝모터 함수

#include <Stepper.h>
const float step1 = 205;
int Pumptime = 12;// 정수기 아두이노로 타이머로 정한 시간이 되면 신호를 보내는 핀
int sendCheck = 13; // 정수기 아두이노에 연결되었다는 신호를 보내는 핀
int BUTTON1 = 8; // 타이머로 부터 시간이 되면 신호를 받는 핀
int DRUG = 10;//약 인식 센서 번호
Stepper mystepper1(step1, 5,3,4,2 );//스텝모터 구동 함수 
int re1;
int re;
int timer;
int state = 0;

int BUTTON2 = A1; // 1번 스텝모터 대응 버튼 입력핀
int count =0;

unsigned long time = millis();


void setup() {
  pinMode(sendCheck, OUTPUT);
  pinMode(Pumptime, OUTPUT);
  pinMode(BUTTON1, INPUT);
  pinMode(DRUG, INPUT);
  Serial.begin(9600);
  pinMode(BUTTON2, INPUT);
  mystepper1.setSpeed(10); //모터 회전 속도

}

void loop() {
  digitalWrite(sendCheck, HIGH);// 다른 아두이노에 연결 신호를 보내기 위해 항시 HIGH로 출력
  digitalWrite(Pumptime, LOW);
  Serial.println(digitalRead(Pumptime));
  Serial.println(digitalRead(BUTTON1));
  if(digitalRead(BUTTON1) == HIGH){ //타이머로 정한 시간이 됨
    digitalWrite(Pumptime, HIGH);// 정한 시간이 되면 정수기 아두이노로 신호를 보냄
    Serial.println(digitalRead(Pumptime));
    mystepper1.step(step1);
    Serial.println("hiyo");
  }
  //약이 적외선 센서에 인식 되면
  if(digitalRead(DRUG) == LOW){
    tone(6, 252, 1000);
    Serial.println("alarm");
  }
  else if(digitalRead(DRUG) == HIGH){
    noTone(6);
    
    Serial.println("alarm");
  }
    

}
