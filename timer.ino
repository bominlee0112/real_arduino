//timer

#include <core_build_options.h>
#include <swRTC.h>
#include <LiquidCrystal.h>

#define gotomotor 5
#define switch2 2  // 인터럽트 핀으로 변경
#define switch3 3  // 인터럽트 핀으로 변경

LiquidCrystal lcd(4, 6, 10, 11, 12, 13); //RS 핀, E핀, 데이터 핀 4개
String lcdString = ""; //객체 선언 : 출력 할 글자 저장
swRTC rtc;
int temphour = 0; // 알람이 울릴 시간을 지정해주는 변수... 
int tempmin = 0;
volatile int hourscnt = 0; // 인터럽트 핸들러에서 사용하므로 volatile로 선언
volatile int mincnt = 0; // 인터럽트 핸들러에서 사용하므로 volatile로 선언
int counttime;
volatile unsigned long lastDebounceTimeHour = 0;
volatile unsigned long lastDebounceTimeMinute = 0;
const unsigned long debounceDelay = 200; // 200ms 디바운스 딜레이

//AM PM을 구분해 주는 함수
void Set_AMPM(int hour) {
    if(hour >= 12) {
        lcd.print("PM");
        lcd.print(hour);
    } else {
        lcd.print("AM");
        Set_lowThanTen(hour);
    }
}

//10보다 작은수를 출력할때 앞에 0을 출력하게 하는 함수 8을 08 이런식으로 8시 9분도 08시 09분처럼 
void Set_lowThanTen(int time) {
    lcd.print(time < 10 ? "0" : "");
    lcd.print(time);
}

//알람이 울릴시간인지 체크하는 함수
void checkTheAlarmTime(int alarmHour, int alarmMinute) {
    if(alarmHour == rtc.getHours() && alarmMinute == rtc.getMinutes()) {
        //여기에 모터부분에 신호를 보내자. digitalWrite()값으로 신호보내기. 
        if (rtc.getSeconds() == 3){
          digitalWrite(gotomotor, LOW);
          delay(100);
          digitalWrite(gotomotor, HIGH);
          delay(100);
          digitalWrite(gotomotor, LOW);
          
        }
    }
}

// 인터럽트 핸들러 함수
void incrementHour() {
    unsigned long currentTime = millis();
    if (currentTime - lastDebounceTimeHour > debounceDelay) {
        lastDebounceTimeHour = currentTime;
        hourscnt += 1;
        if (hourscnt == 24) {
            hourscnt = 0;
        }
        temphour = hourscnt;
    }
}

void incrementMinute() {
    unsigned long currentTime = millis();
    if (currentTime - lastDebounceTimeMinute > debounceDelay) {
        lastDebounceTimeMinute = currentTime;
        mincnt += 2;
        if (mincnt == 60){
            mincnt = 0;
        }
        tempmin = mincnt;
    }
}

void setup() {
    lcd.begin(16, 2); //LCD 크기 지정, 2줄 16칸
    lcd.clear(); //화면 초기화
    rtc.stopRTC(); //정지
    rtc.setTime(19, 12, 0); //시간, 분, 초 초기화 현재 시간 입력하는 코드 -> 지금 시간이 몇시인지 
    rtc.setDate(4, 6, 2024); //일, 월, 년 초기화 ->지금 년도가 몇년도인지
    rtc.startRTC(); //시작
    pinMode(gotomotor, OUTPUT);
    digitalWrite(gotomotor, LOW);
    Serial.begin(9600); //시리얼 포트 초기화
    pinMode(switch2, INPUT_PULLUP);
    pinMode(switch3, INPUT_PULLUP);

    // 인터럽트 설정
    attachInterrupt(digitalPinToInterrupt(switch2), incrementHour, RISING);
    attachInterrupt(digitalPinToInterrupt(switch3), incrementMinute, RISING);
}

void loop() {
    // 첫 번째 줄 업데이트 (현재 시간)
    lcd.setCursor(0, 0); //커서를 0,0에 지정
    lcd.clear();
    Set_AMPM(rtc.getHours()); // -> rtc에 있는 hours를 불러와서 ampm인지 구분해서 0,0에 출력을 한다.
    lcd.print(":"); // ex) am08: 현재까지 입력된 값 
    Set_lowThanTen(rtc.getMinutes()); // -> rtc에 있는 minutes를 불러와서 %02d를 사용해서 2자리보다 작으면 0을 붙여서 출력한다. 
    lcd.print(":"); // ex) am08:09: 현재까지 입력된 값
    Set_lowThanTen(rtc.getSeconds()); // -> getminutes처럼 seconds를 불러와서 %02d를 사용해서 2자리보다 작으면 0을 붙여서 출력한다.  
    lcd.print("[");
    Set_lowThanTen(rtc.getMonth());  // -> 똑같이 10보다 크거나 작거나를 구분한다. 출력 
    lcd.print("/");
    Set_lowThanTen(rtc.getDay()); 
    lcd.print("]"); // ex) am08:09 [07/14]

    // 두 번째 줄 업데이트 (알람 시간)
    lcd.setCursor(0, 1); // -> 아래 줄에 lcd 출력을 한다. 
    lcd.print("Alarm "); // ex) Alarm    <공백 있으니 유의>
    Set_AMPM(temphour); //temptime를 생각하자! 12보다 크면 pm00 식으로 나올것이다. 
    lcd.print(":"); // ex) Alarm pm20: 
    Set_lowThanTen(tempmin); // 2030/100의 나머지는 30이므로 이뜻은 30분을 의미한다. 10보다 큰 값이므로 30이 출력되고, 만약 10보다 작으면 08이런식으로 표현된다.
    lcd.print(" ");
  
    // 알람이 울릴 시간인지 체크
    checkTheAlarmTime(temphour, tempmin); //현재 2030으로 설정 됐으므로 변수에 (20, 30)이 들어간 상태이다. 이때 rtc에 저장된 hour과 min의 값이 같으면 motor에 신호를 보낸다. 

    delay(1000); // 시계는 1초마다 값이 바뀌므로 당연한 과정이다. 삭제하고 다시표현 ㅇㅇ
}
