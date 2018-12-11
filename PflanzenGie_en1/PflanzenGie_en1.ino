
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

//Wann wie relay angesteuert werden soll wird mit struct-instanz festgelegt, vom diesm typ:
struct relControlStruct{
  //an welchen tagen soll relay angeschltet werden?
  bool montag,dienstag,mittwoch,donnerstag,freitag,samstag,sonntag;

  //zu welchem Zeitpunkt soll eingeschaltet werden?
  short stunde,minute;

  //Wie lange soll Relai an sein in Sekunden?
  short relOnTime;
  
  };

relControlStruct giessZeiten;


void set_next_alarm(uint8_t sleep_period)
{
    struct ts t;
    unsigned char wakeup_min;

    DS3231_get(&t);

    // calculate the minute when the next alarm will be triggered
    wakeup_min = (t.min / sleep_period + 1) * sleep_period;
    if (wakeup_min > 59) {
        wakeup_min -= 60;
    }

    // flags define what calendar component to be checked against the current time in order
    // to trigger the alarm
    // A2M2 (minutes) (0 to enable, 1 to disable)
    // A2M3 (hour)    (0 to enable, 1 to disable) 
    // A2M4 (day)     (0 to enable, 1 to disable)
    // DY/DT          (dayofweek == 1/dayofmonth == 0)
    uint8_t flags[4] = { 0, 1, 1, 1 };

    // set Alarm2. only the minute is set since we ignore the hour and day component
    DS3231_set_a2(wakeup_min, 0, 0, flags);

    // activate Alarm2
    DS3231_set_creg(DS3231_CONTROL_INTCN | DS3231_CONTROL_A2IE);
}

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    DS3231_init(DS3231_CONTROL_INTCN);
    DS3231_clear_a2f();
    set_next_alarm(sleep_period_1);

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
    unsigned long now = millis();
    struct ts t;

    // once a while show what is going on
    if ((now - prev > interval) && (Serial.available() <= 0)) {
        DS3231_get(&t);

        // display current time
        snprintf(buff, BUFF_MAX, "%d.%02d.%02d %02d:%02d:%02d", t.year,
             t.mon, t.mday, t.hour, t.min, t.sec);
        Serial.println(buff);

        // display a2 debug info
        DS3231_get_a2(&buff[0], 59);
        Serial.println(buff);

        if (DS3231_triggered_a2()) {
            Serial.println(" -> alarm2 has been triggered");
            set_next_alarm(sleep_period_1);
            // clear a2 alarm flag and let INT go into hi-z
            DS3231_clear_a2f();  

           // -------max:-------
           digitalWrite(REL_IN1,LOW);
           delay(t_rel_1);
           digitalWrite(REL_IN1,HIGH);
           
        }
        prev = now;
    }
}
