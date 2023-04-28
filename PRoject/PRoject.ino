#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <TridentTD_LineNotify.h>

#define SSID        "Rito"   // ชื่อ Wifi
#define PASSWORD    "jimmyjimmy"   // รหัส Wifi
#define LINE_TOKEN  "U82Hpg0zZ5Apjy1NfQsha2QA1db7VLAAkd2ulmmWL3o"   // TOKEN

const int buzzer = D4;
int mesure_distance_cm(byte trig_pin,byte dist_pin)
{
  digitalWrite(trig_pin,HIGH);
  delayMicroseconds(1000);
  digitalWrite(trig_pin,LOW);
  int value=(pulseIn(dist_pin,HIGH)/2)/29.1+2;
  if(value>255) {value=255; }
  delay(20);
  return value;
}
void setup() {
  Serial.begin(115200);
  Serial.println(LINE.getVersion());
  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting ",  SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());
  pinMode(D1, OUTPUT); //Ultrasonic triger
  pinMode(D2, INPUT); //Ultrasonic distance
  pinMode(buzzer, OUTPUT);; //ลำโพง
  pinMode(D7, INPUT); // Emergency_button

}

void loop() {
  if(digitalRead(D7) == HIGH)
  {
  Serial.println("ส่งข้อความ");
  LINE.setToken(LINE_TOKEN);
  LINE.notify("สัญญาณขอความช่วยเหลือจากไม้เท้า");
  LINE.notify("โปรดผู้ดูแลรีบให้การช่วยเหลือโดยด่วน");
  // ส่งสติกเกอร์
  LINE.notifySticker(1, 422); 
  for(int i = 0; i < 20; i++)
  {
  tone(buzzer, 600); // Send 600Hz sound signal...
  delay(500);        // ...for 1/2 sec
  noTone(buzzer);     // Stop sound...
  delay(500);        // ...for 1/2 sec
  }
  }
  delay(1);
  if(mesure_distance_cm(D1,D2)<=80)
  {
    tone(buzzer,400);
    delay(100);
    tone(buzzer,380);
    delay(100);
    tone(buzzer,400);
    noTone(buzzer);
    delay(500);
    Serial.println("<80");
  }
  else if(mesure_distance_cm(D1,D2)<=150)
  {
    tone(buzzer,350);
    delay(250);
    tone(buzzer,370);
    delay(250);
    noTone(buzzer);
    delay(2000);
    Serial.println("<150");
  }
  else if(mesure_distance_cm(D1,D2)<=250)
  {
    tone(buzzer,230);
    delay(250);
    tone(buzzer,200);
    delay(250);
    noTone(buzzer);
    delay(3000);
    Serial.println("<250");
  }
  else
  {
    noTone(buzzer);
    Serial.println(">250");
  }
}


