byte buf[26];  //serial read buffer
byte readchar;
int recieveSum = 0;
int checkSum = 0;
int counter = 0;
int counterbuf = 0;
int PM01Value = 0;        //define PM1.0 value
int PM2_5Value = 0;       //define PM2.5 value
int PM10Value = 0;       //define PM10 value
char mode = 'a';
bool toggle = 0;
char rx_trigger;
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
}

// Standby mode. For low power consumption and prolong the life of the sensor.
void sleep()
{
  uint8_t command[] = { 0x42, 0x4D, 0xE4, 0x00, 0x00, 0x01, 0x73 };
  Serial1.write(command, sizeof(command));
  mode = 's';
}

// Operating mode. Stable data should be got at least 30 seconds after the sensor wakeup from the sleep mode because of the fan's performance.
void wakeUp()
{
  if(mode =='s'){
    uint8_t command[] = { 0x42, 0x4D, 0xE4, 0x00, 0x01, 0x01, 0x74 };
    Serial1.write(command, sizeof(command));
    mode = 'a';
  }
}

// Active mode. Default mode after power up. In this mode sensor would send serial data to the host automatically.
void activeMode()
{
  uint8_t command[] = { 0x42, 0x4D, 0xE1, 0x00, 0x01, 0x01, 0x71 };
  Serial1.write(command, sizeof(command));
  mode = 'a';
}

// Passive mode. In this mode sensor would send serial data to the host only for request.
void passiveMode()
{
  uint8_t command[] = { 0x42, 0x4D, 0xE1, 0x00, 0x00, 0x01, 0x70 };
  Serial1.write(command, sizeof(command));
  mode = 'p';
}

// Request read in Passive Mode.
void requestRead()
{
  if (mode == 'p')
  {
    uint8_t command[] = { 0x42, 0x4D, 0xE2, 0x00, 0x00, 0x01, 0x71 };
    Serial1.write(command, sizeof(command));
    toggle = 1;
  }
}

void loop()
{
  if( Serial.available() > 0){
    rx_trigger = Serial.read();
    Serial.println(rx_trigger);
  }

  if(rx_trigger=='a'){
     activeMode();
  }
  else if(rx_trigger=='s'){
    sleep();
  }
  else if(rx_trigger=='w'){
    wakeUp();
  }
  else if(rx_trigger=='p'){
    passiveMode();
  }
  else if(rx_trigger == 'r'){
    requestRead();
  }
    
    while(mode =='a' || toggle == 1){
      while (!Serial1.available()) {
 
      }
      readchar = Serial1.read();
      if (readchar == 0x42) { //start reading char if first frame
        Serial.println();
        recieveSum = 0;
        counter = 0;
        counterbuf = 0;
      }
      Serial.print(readchar, HEX);
      Serial.print(" ");
      if (counter < 30) {
        recieveSum = recieveSum + readchar;
        if (counter > 3) { //first two bytes are for init, 3rd and 4th are for frame size, not needed for data
          buf[counterbuf] = readchar;
          counterbuf++;
        }
      }
      if (counter == 30) 
        checkSum = readchar; //last two bytes of frame are for checksum
      if (counter == 31) {
        checkSum = (checkSum << 8) + readchar;
        Serial.print(" CheckSum: "); Serial.print(recieveSum); Serial.print(" "); Serial.println(checkSum);
        if (checkSum == recieveSum) 
          break;
      }
      counter++;
      //delay(2000);
    }
  
    Serial.println();
    //  Serial.println(buf[6]<<8);
    //  Serial.println(buf[7]);
  
    PM01Value = (buf[6] << 8) + buf[7];
    PM2_5Value = (buf[8] << 8) + buf[9];
    PM10Value = (buf[10] << 8) + buf[11];
  
    /*pm standard particle
     * pm1  buf[0] << 8) + buf[1]
     * pm2.5   buf[2] << 8) + buf[3]
     * pm10   buf[4] << 8) + buf[5]
     * 
     * can be printed.
     * 
     */
  
     /*number of particles in 0.1L of air
      * 0.3  buf[12] << 8) + buf[13]
      * 0.5   buf[14] << 8) + buf[15]
      * 1   buf[16] << 8) + buf[17]
      * 2.5   buf[18] << 8) + buf[19]
      * 5.0   buf[20] << 8) + buf[21]
      * 10   buf[22] << 8) + buf[23]
      * 
      *  can be printed.
      */
    if(mode == 'a' || toggle == 1){
      Serial.println("PM concentration unit under atmospheric environment (ug/m3)");
      Serial.print("PM   1: ");
      Serial.print(PM01Value);
      Serial.print("  ug/m3");
      Serial.print("\tPM 2.5: ");
      Serial.print(PM2_5Value);
      Serial.print("  ug/m3");
      Serial.print("\tPM  10: ");
      Serial.print(PM10Value);
      Serial.print("  ug/m3");
      Serial.println();
    }
    else{
      Serial.print("sensor is in mode ");
      Serial.println(mode);
    }
    toggle = 0;
    delay (3000);
    
}
