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
  char year[] =     "2018";
  char month[]=     "12";
  char* mday  =     "12";
  char* wday  =     "3";
  char* hour  =     "14";
  char* mins  =     "17";
  char* sec   =     "11";
  
  //parse_cmd("T003620211122018",16);//T352312108102018 -> ssmmhh(dayofweak)ddmmyyyy
  
  
  char parseStr[16];
  parseStr[0]='T';
  parseStr[1]=sec[0];
  parseStr[2]=sec[1];
  parseStr[3]=mins[0];
  parseStr[4]=mins[1];
  parseStr[5]=hour[0];
  parseStr[6]=hour[1];
  parseStr[7]=wday[0];
  parseStr[8]=mday[0];
  parseStr[9]=mday[1];
  parseStr[10]=month[0];
  parseStr[11]=month[1];
  parseStr[12]=year[0];
  parseStr[13]=year[1];
  parseStr[14]=year[2];
  parseStr[15]=year[3];

  Serial.println(parseStr);
  parse_cmd(parseStr,16);
  
  
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(7,LOW);
  //delay(3000);
   //digitalWrite(7,HIGH);
  //delay(3000);
}
