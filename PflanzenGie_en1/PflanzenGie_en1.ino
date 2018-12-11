
// during an alarm the INT pin of the RTC is pulled low
//
// this is handy for minimizing power consumption for sensor-like devices, 
// since they can be started up by this pin on given time intervals.


#include <Wire.h>
#include "ds3231.h"
#include "rtc.h"

#define BUFF_MAX 256

uint8_t sleep_period_1 = 1;       // the sleep interval in minutes between 2 consecutivealarms

//----wie lange relays angesteuert werden
int t_rel_1 = 5*1000;
int t_rel_2 = 5*1000;
int t_rel_3 = 5*1000;
int t_rel_4 = 5*1000;

// how often to refresh the info on stdout (ms)
unsigned long prev = 5000, interval = 5000;

//---reley inputs-----:
#define REL_IN1 2
#define REL_IN2 3
#define REL_IN3 4
#define REL_IN4 5


//                      mo  di  mi  do  fr  sa  so
bool wateringDays[] = { 0 , 1 , 1 , 0 , 0 , 0 , 0 };
short h = 21;
short m = 51;
int waterduration = 5;
short* waterDays=NULL;
short amound_dayToWater=0;

void setup()
{
    
    for(int i =0;i<7;i++){
      if(wateringDays[i])
        amound_dayToWater++;
    }
    waterDays = new short[amound_dayToWater];
    int j=0;
    for(int i =0;i<7;i++){
      if(wateringDays[i]){
          waterDays[j]=i+1;
          j++;
      }
    }

  
    Serial.begin(9600);
    Wire.begin();
    DS3231_init(DS3231_CONTROL_INTCN);
    DS3231_clear_a2f();
    //set_next_alarm(sleep_period_1);

    //-------InitRelayPins:------------
    pinMode(REL_IN1,OUTPUT);
    digitalWrite(REL_IN1,HIGH);

    pinMode(REL_IN2,OUTPUT);
    digitalWrite(REL_IN2,HIGH);

    pinMode(REL_IN3,OUTPUT);
    digitalWrite(REL_IN3,HIGH);

    pinMode(REL_IN4,OUTPUT);
    digitalWrite(REL_IN4,HIGH);
}

void loop()
{
    char buff[BUFF_MAX];
    //unsigned long now = millis();
    struct ts t;
    delay(1000);

    DS3231_get(&t);

    // display current time
    snprintf(buff, BUFF_MAX, "date: %d.%02d.%02d time: %02d:%02d:%02d  day: %02d", t.year, t.mon, t.mday, t.hour, t.min, t.sec, t.wday);
    Serial.println(buff);

    bool today_is_water_day = false;
    for(int i=0;i<amound_dayToWater;i++){
        if(waterDays[i] == t.wday){
            today_is_water_day=true;
            Serial.println("heute is gieß tag");
            break;
        }
    }

    if(today_is_water_day && (t.hour == h ) && (t.min == m )){
      
      digitalWrite(REL_IN1,LOW);
      delay(waterduration*1000);
      digitalWrite(REL_IN1,HIGH);
      Serial.println("Hab gegossen, lege mich 61 sekunden schlafen");
      for(int i=0;i<11;i++)
          delay(1000*10);
    }
}
