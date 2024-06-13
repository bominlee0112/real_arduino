//정수기 코드
// 핀 번호 설정
int irSensorPin = 2;   // 적외선 센서 핀
int relayPin = 3;      // 릴레이 제어 핀
int checkConnect = 7; // 연결확인 핀
int checkTime = 6; // 시간 입력 핀
// 변수 설정
bool cupDetected = false;
int Yellow = 9; //연결확인 용 노란 LED
int Green = 10; // 1단계 초록 LED
int Red = 11; // 2단계 빨간 LEd
int White = 12; // 3단계 하얀 LED

void setup() {
  // 핀 모드 설정
  pinMode(checkConnect, INPUT);
  pinMode(checkTime, INPUT);
  pinMode(irSensorPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(Green, OUTPUT);
  pinMode(Red, OUTPUT);
  pinMode(White, OUTPUT);
  pinMode(Yellow, OUTPUT);
  
  // 초기 상태 설정
  digitalWrite(relayPin, LOW);
  
  // 시리얼 통신 시작 (디버깅용)
  Serial.begin(9600);
}

void loop() {
  
  // 적외선 센서 값 읽기
  Serial.println(digitalRead(checkTime));
  if (digitalRead(checkConnect)==1){
   
  }
  int sensorValue = digitalRead(irSensorPin);
  //Serial.println(analogRead(A0));
  int drug = analogRead(A0);//가변 저항 값 읽기

 
  if(drug < 1024/3 && drug > 10){ 
    digitalWrite(Green, HIGH); //1단계 초록색 LED 점등 
    digitalWrite(Red, LOW);
    digitalWrite(White, LOW);
  } 
  else if(drug < 1024*2/3){ 
    digitalWrite(Green, LOW);
    digitalWrite(Red, HIGH);//2단계 빨간색 LED 점등 
    digitalWrite(White, LOW);
  } 
  else if(drug < 1024){
    digitalWrite(Green, LOW);
    digitalWrite(Red, LOW);
    digitalWrite(White, HIGH); //3단계 흰색 LED 점등 
  } 
  //타이머(스텝모터 구동 아두이노)와 연결 되었을 때
  if(digitalRead(checkConnect) == HIGH){
    digitalWrite(Yellow, HIGH);
    if(digitalRead(checkTime)==HIGH){//시간이 되었을 때만 물 자동 공급
      Serial.println("Hello");
      waterpump();
    }
  }
//타이머와 분리 되었을 때
  else if(digitalRead(checkConnect) == LOW){
    digitalWrite(Yellow, LOW);
    waterpump();
  }
} 

//물펌프 가동 함수
void waterpump(){
  int sensorValue = digitalRead(irSensorPin);
  int drug = analogRead(A0);//가변 저항 값 읽기
if (sensorValue == LOW && !cupDetected) { // LOW는 컵이 감지된 상태
    cupDetected = true;  // 컵 감지 상태로 변경
    Serial.println("Cup detected. Starting pump...");
    
    // 0.5초 대기
    delay(500);
    
    // 펌프 작동
    if(drug < 1024/3){//1단계
    Serial.println("lv.1");
    digitalWrite(relayPin, HIGH); // 릴레이 ON
    delay(5000);  // 5초 동안 펌프 작동
    digitalWrite(relayPin, LOW); // 릴레이 OFF
    }
    else if(drug < 1024*2/3){//2단계
      Serial.println("lv.2");
    digitalWrite(relayPin, HIGH); // 릴레이 ON
    delay(8000);  // 8초 동안 펌프 작동
    digitalWrite(relayPin, LOW); // 릴레이 OFF
    }
    else if(drug < 1024){//3단계
      Serial.println("lv.3");
    digitalWrite(relayPin, HIGH); // 릴레이 ON
    delay(10000);  // 10초 동안 펌프 작동
    digitalWrite(relayPin, LOW); // 릴레이 OFF
    }
    
    Serial.println("Pump stopped.");
    
    // 컵이 제거될 때까지 대기
    while (digitalRead(irSensorPin) == LOW) {
      delay(50);  // 작은 지연 시간으로 무한 루프 방지
    }
    cupDetected = false;  // 컵 감지 상태 초기화
    Serial.println("Cup removed. Ready for next cup.");
  }
  
  // 짧은 지연 시간으로 센서 상태 주기적으로 확인
  delay(100);
}
