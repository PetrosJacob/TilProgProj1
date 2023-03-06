//Includes nödvändiga biblotek för projektet
#include <RtcDS3231.h>
#include <Wire.h>


RtcDS3231<TwoWire> rtcModule(Wire);


// Create variables
int pirPin = 2;
int buzz = 6 ;
int pirStat = 0;
int closeAlarm = 0;
int procAlarm = 0;

byte hours;
byte minutes;
byte seconds;
String tid;


//Setup, starts the serial output, and the wire, 
void setup() {
  Wire.begin();
  Serial.begin(9600);

  // Update RTC module time to compilation time
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  rtcModule.SetDateTime(compiled);

  //Sets port 2/pirPin to a input, and port 6/buzz to a outpout
  pinMode(pirPin, INPUT);
  pinMode(buzz, OUTPUT);

}


void loop(void) {
  //Loops and cycles through the following functions: 
  time_now ();
  activateAlarm ();
  alarm ();

}
//Gathers the current, and writes it in the serial monitor.
//Input: Inget
//Output: Inget
void time_now () {

  RtcDateTime now = rtcModule.GetDateTime();
  hours = now.Hour();
  minutes = now.Minute();
  seconds = now.Second();
  tid = String(hours) + ": " + String(minutes) + ": " + String(seconds);
  Serial.println(tid);
}

//This function activates the alarm and writes if there has been movment to the serial console.

void activateAlarm () {
  
  //First it sees if there has been an input detected at the pirStat pin, 
  //which means it checks if there has been movement detected, if there has been movment, it will set the "closeAlarm" variable to 1, 

  RtcDateTime now = rtcModule.GetDateTime();
  pirStat = digitalRead(pirPin);
  if (pirStat == HIGH) {
    Serial.println("Rörelse detected");
    closeAlarm = 1;
  }
  else {
    Serial.println("Ingen rörelse");
    closeAlarm = 0;
  }
  delay(1000);


  //This if statement checks if the current time matches the time that the alarm is supposed to activate, if it matches, it will set the "procAlarm" variable to 1, while this if statement is untrue, the procalarm shall be atributed 0.
  if (now.Hour() == 10 && now.Minute() == 0 && now.Second() == 0) {
    procAlarm = 1;
    Serial.println("hej");
  }
  else if (closeAlarm == 1) {
    procAlarm = 0;
  }
  else {
    procAlarm = 0;
  }

}

//This function activates the alarm through a buzzer and sees if it should be turned off.
void alarm () {

  //While the "procAlarm" variable is set to one, it will sound the alarm
  while (procAlarm == 1) {
    //Sounds the alarm while the alarm is on.
    digitalWrite(buzz, HIGH);
    delay(100);
    digitalWrite(buzz, LOW);
    delay(100);

    //if the pir detects movement it will it will send out an HIGH, this will activate set the "procAlarm" variable to 0,
    if ((digitalRead(pirPin) == HIGH)) {
      Serial.println("Rörelse detected");
      procAlarm = 0;
      delay(1000);
    }
  }
}
