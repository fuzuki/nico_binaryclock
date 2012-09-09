/* 
 * Binary LED clock 
 *
 * TimeSerialDateStrings.pde
 * example code illustrating Time library date strings
 *
 * This sketch adds date string functionality to TimeSerial.pde
 * 
 */ 
 
#include <Time.h>  

#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by unix time_t as ten ascii digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

#define H1 12
#define H2 11
#define H3 10
#define H4 9
#define H5 8

#define M1 13
#define M2 3
#define M3 2
#define M4 4
#define M5 6
#define M6 7

int min_pin[6];
int hour_pin[5];
int temp;

void setup()  {
  int i;
  Serial.begin(9600);
  setSyncProvider( requestSync);  //set function to call when sync required
  Serial.println("Waiting for sync message");

  min_pin[0] = M1;
  min_pin[1] = M2;
  min_pin[2] = M3;
  min_pin[3] = M4;
  min_pin[4] = M5;
  min_pin[5] = M6;

  hour_pin[0] = H1;
  hour_pin[1] = H2;
  hour_pin[2] = H3;
  hour_pin[3] = H4;
  hour_pin[4] = H5;
  
  for(i=0;i<5;i++){
    pinMode(hour_pin[i], OUTPUT);
    //digitalWrite(hour_pin[i],HIGH);
  }
  for(i=0;i<6;i++){
    pinMode(min_pin[i], OUTPUT);
    //digitalWrite(min_pin[i],HIGH);    
  }
  temp = HIGH;
}

void loop(){

  if(Serial.available() ) 
  {
    processSyncMessage();
  }
  if(timeStatus()!= timeNotSet) 
  {
    //digitalClockDisplay();
    setLED();
  }else{
    //
    digitalWrite(13,temp);
    temp = !temp;
  }
  delay(1000);
}

void processSyncMessage() {
  // if time sync available from serial port, update time and return true
  while(Serial.available() >=  TIME_MSG_LEN ){  // time message consists of a header and ten ascii digits
    char c = Serial.read() ; 
    Serial.print(c);  
    if( c == TIME_HEADER ) {       
      time_t pctime = 0;
      for(int i=0; i < TIME_MSG_LEN -1; i++){   
        c = Serial.read();          
        if( c >= '0' && c <= '9'){   
          pctime = (10 * pctime) + (c - '0') ; // convert digits to a number    
        }
      }   
      setTime(pctime);   // Sync Arduino clock to the time received on the serial port
    }else{
      //setTime(current);
    }  
  }
}

time_t requestSync()
{
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}
/**
 * Set LED 
 */
void setLED(){
  int i;
  int hour_v,min_v;

  hour_v = hour();
  min_v = minute();

  // Set Hour
  for(i=0;i<5;i++){
    if((hour_v>>i)&0x01){
      digitalWrite(hour_pin[i],HIGH);
    }else{
      digitalWrite(hour_pin[i],LOW);
    }
  }

  // Set Minute
  for(i=0;i<6;i++){
    if((min_v>>i)&0x01){
      digitalWrite(min_pin[i],HIGH);
    }else{
      digitalWrite(min_pin[i],LOW);
    }
  }
}

