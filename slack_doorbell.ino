
#define ONE_DAY_MILLIS (24 * 60 * 60 * 1000)
#define ONE_MINUTE_MILLIS (60 * 1000)


int intpin = D0; 
int led2 = D7; 
//int speakerpin = A0;

boolean buttonUp = TRUE;
long lastInterrupt = millis();
const int debounceTime = 5000;
unsigned long lastHeartbeat = millis();



time_t lastSyncTimestamp;
unsigned long lastSync;
unsigned long cur;


void setup() {

  
  Serial.begin(19200);


  pinMode(led2, OUTPUT);
  pinMode(intpin,INPUT_PULLDOWN);
//  pinMode(speakerpin, INPUT);


  attachInterrupt(intpin,ISR,CHANGE);

  
}


void ISR(void)  {
  if (millis() - lastInterrupt > debounceTime) {
    lastInterrupt = millis();
    Serial.println("DOORBELL RUNG");
    Particle.publish("ppnyDoorbell_to_ppnyslack");
  }

}


void maybeSyncTime(void) {

  lastSync = Particle.timeSyncedLast(lastSyncTimestamp);
  if (millis() - lastSync > ONE_DAY_MILLIS) {
    cur = millis();
    Serial.printlnf("Time was last synchronized %lu milliseconds ago", millis() - lastSync);
    if (lastSyncTimestamp > 0)
    {
      Serial.print("Time received from Particle Device Cloud was: ");
      Serial.println(Time.timeStr(lastSyncTimestamp));
    }
    // Request time synchronization from Particle Device Cloud
    Particle.syncTime();
    // Wait until Photon receives time from Particle Device Cloud (or connection to Particle Device Cloud is lost)
    waitUntil(Particle.syncTimeDone);
    // Check if synchronized successfully
    if (Particle.timeSyncedLast() >= cur)
    {
      // Print current time
      Serial.println(Time.timeStr());
    }
  }

}

void maybeCallHome(void) {
      // Call home each minute
  if (millis() - lastHeartbeat > ONE_MINUTE_MILLIS) {
    Spark.publish("ppnyDoorbell_heartbeat");
    lastHeartbeat = millis();
  }

}




void loop() {
    
  digitalWrite(led2, HIGH);

  delay(1000);

  digitalWrite(led2, LOW);

  delay(1000);

  maybeSyncTime();
  
  maybeCallHome();
  
  
  //Serial.println( analogRead(speakerpin));
  

}





