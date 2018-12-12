
// during an alarm the INT pin of the RTC is pulled low
//
// this is handy for minimizing power consumption for sensor-like devices, 
// since they can be started up by this pin on given time intervals.


#include <Wire.h>
#include "ds3231.h"
//#include "WateringManager.h"

#define BUFF_MAX 256

//---reley inputs-----:
#define REL_IN1 2
#define REL_IN2 3
#define REL_IN3 4
#define REL_IN4 5

class WateringManager{
  private:

    short h;
    short m;
    short waterduration;
    short* waterDays=NULL;
    short amound_daysToWater;
    short outPin;
    
    
  public:
    WateringManager(bool mon,bool tue,bool wed,bool thu,bool fri,bool sat,bool sun,short hour, short mins,short water_duration,short rel_index)
    {
      // -------generat array "waterDays", that containes all weak days where we have to water:
      bool wateringDays[] = { mon , tue , wed , thu , fri , sat , sun };
      amound_daysToWater=0;
      for(int i =0;i<7;i++)
      {
        if(wateringDays[i])
          amound_daysToWater++;
      }
      
      waterDays = new short[amound_daysToWater];
      int j=0;
      for(int i =0;i<7;i++){
        if(wateringDays[i]){
           waterDays[j]=i+1;
           j++;
        }
      }

      waterduration = water_duration;
      this->h=hour;
      this->m=mins;

      switch(rel_index)
      {
        case 1:
          outPin=2;
          break;

        case 2:
          outPin=3;
          break;
        case 3:
          outPin=4;
          break;

        case 4:
          outPin=5;
          break;
          
        default:
          outPin=2;
      }
      digitalWrite(outPin,HIGH);
    };

    void watering_if_necessary(short t_wday, short t_hour, short t_min)
    {
      bool today_is_water_day = false;
      for(int i=0;i<amound_daysToWater;i++)
      {
        if(waterDays[i] == t_wday)
        {
            today_is_water_day=true;
            Serial.println("heute is gieß tag");
            break;
        }
      }

      if(today_is_water_day && (t_hour == h ) && (t_min == m ))
      {
        digitalWrite(outPin,LOW);
        delay(waterduration*1000);
        digitalWrite(outPin,HIGH);
        Serial.println("Hab gegossen, lege mich 70 sekunden schlafen");
        for(int i=0;i<11;i++)
            delay(1000*10);
      }
      else
        Serial.println("jetzt nicht gießen");
    }
};

//                                            mo  di  mi  do  fr  sa  so  h   m   dur   relNr
WateringManager* plant1 = new WateringManager(0,  0,  1,  0,  1,  0,  1,  20, 30,  1,    1);
// wasserdurchsatz: pro sekunde pumpen ca 50ml wasser. grob!(12V netzteil, starke pumpe)

void setup()
{
    
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
    struct ts t;
    delay(1000*5);

    DS3231_get(&t);

    // display current time
    snprintf(buff, BUFF_MAX, "date: %d.%02d.%02d time: %02d:%02d:%02d  day: %02d", t.mday, t.mon, t.year, t.hour, t.min, t.sec, t.wday);
    Serial.println(buff);

    plant1->watering_if_necessary(t.wday,t.hour,t.min);
    
}
