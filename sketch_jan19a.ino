#include <Servo.h>
#include <SoftwareSerial.h>

#define BT_RX 7
#define BT_TX 8
#define SERVO 9

SoftwareSerial HM10(BT_RX,BT_TX); //RX(7 pin)-> HM10(TX), TX(8 pin) -> HM10(RX)
Servo mServo; //서보 변수 선언
String text = "";
void setup() {
  Serial.begin(9600);
  HM10.begin(9600);
  mServo.attach(SERVO);
}

void loop() {
  if(HM10.available()){
    char c = (char) HM10.read();
    //만약 문자열의 끝을 알리는 '.'이 온다면, 문자를 그만 읽고 각도를 출력한다.
    if(c == '.'){
      String degree = text.substring(0,text.length());
      int realDegree = (180 - degree.toInt());
      Serial.println(realDegree);
      text = "";
      //각도를 조절하는 함수
      mServo.write(realDegree);
      delay(900);

      //각도 조절이 끝나면 ack를 전송한다.
      String ACK = degree + "/ack";
      HM10.write((uint8_t *)ACK.c_str(), ACK.length());

      //각도 조절이 끝나면 각도를 초기화 한다.
      degree = "";
      // 끝이 아니라면 문자를 계속 읽어 문자열을 만든다.
    }else{
      text += c;
    }
  }

  if(Serial.available()){
    HM10.write(Serial.read());
  }
}
