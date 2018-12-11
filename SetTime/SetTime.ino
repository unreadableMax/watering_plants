#include <Wire.h>
#include "ds3231.h"
#include "rtc_ds3231.h"


#define BUFF_MAX 128



void setup() {
  // put your setup code here, to run once:
  //pinMode(7,OUTPUT);
  //digitalWrite(7,LOW);

  Serial.begin(9600);
  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);
  memset(recv, 0, BUFF_MAX);
  Serial.println("Get time");
  parse_cmd("T352715108102018",16);//T352312108102018 -> ssmmhh(dayofweak)ddmmyyyy
}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(7,LOW);
  //delay(3000);
   //digitalWrite(7,HIGH);
  //delay(3000);
}
