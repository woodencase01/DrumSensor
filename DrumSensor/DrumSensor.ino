/*

Drum Sensor - A serial drum trigger

-=-=-=- Flow chart -=-=-=-

1. Read and send buffer

2. Check inputs

3. Send new data

-=-=-=- General features -=-=-=-
*Prioritize I/O for low latency
*Parallel reading for sensors

-=-=-=- General features -=-=-=-

-=-=-=- Data protocol -=-=-=-
byte 0: Start byte (254)
byte 1: Sensor id
byte 2: [0 - 200]: Sensor strength
byte 3: End byte (255)
*/
const int databytes = 4;
byte databuffer[databytes];
byte datastroke[databytes];
bool senddata = false;

//#define DEBUG

const int nbsensors = 1;

const int sensorspin[] = {0, 1, 2, 3, 4};
int threshold[] = {2,2,2,2,2};

const int peakread = 1; // time in millis before a peak is read
const int mindecay = 2;   // Maximum decay time (ms)
const int maxdecay = 15;   // Maximum decay time (ms)

int maxsensorvalue[] = {0, 0, 0, 0, 0};
byte sensorstroke[] = {0, 0, 0, 0, 0};
byte sensorstate[] = {0, 0, 0, 0, 0};
/*
0: Waiting for decay
1: Not ready
2: First read
3: Successive readings
4: Final value
*/
unsigned long sensorsdecayend[] = {0, 0, 0, 0, 0};
unsigned long startread[] = {0, 0, 0, 0, 0};

#ifdef DEBUG
int numberreads[] = {0,0,0,0,0};
#endif

void setup() {
  Serial.begin(9600);
  
  #ifdef DEBUG
  Serial.println("Drum Sensor Ready");
  #endif
}

void loop() {

  readBuffer();
  
  if(senddata){
    sendBuffer();
  }
  
  manageSensors();

}

void readBuffer(){
  
  int buffersize = Serial.available();
  
  if(buffersize >= databytes){
    
    #ifdef DEBUG
      Serial.print("Current buffer: ");
      Serial.println(buffersize);
    #endif 
    
    bool goodbuffer = false;
    
    while(Serial.available() > databytes){
      byte bufferread = Serial.read();
      
      #ifdef DEBUG
        Serial.print("Current read: ");
        Serial.println(bufferread);
      #endif 
      
      if (bufferread == 254) goodbuffer = true;
    }
    
    if(goodbuffer){
      for(int i=0; i<databytes-2; i++){
      databuffer[i+1] = Serial.read();
      }
      senddata = true;
      
      #ifdef DEBUG
        Serial.read(); // Delete char 10 when using console
        Serial.print("Received: ");
          for(int i=0; i<databytes; i++){
          Serial.print(databuffer[i]);
          Serial.print(",");
          }
        Serial.println("");
      #endif
    }
  }
}

void sendBuffer(){
  databuffer[1]++;
  Serial.write(databuffer, databytes);
  senddata = false;
  
  #ifdef DEBUG
  Serial.print("Sent: ");
  for(int i=0; i<databytes-1; i++){
        Serial.print(databuffer[i]);
        Serial.print(",");
      }
    Serial.println(databuffer[databytes]);  
  #endif  
}

void sendStroke(byte padstroke){
  datastroke[0] = 0;
  datastroke[1] = padstroke;
  Serial.write(datastroke, databytes);
  
  #ifdef DEBUG
  Serial.print("Sent: ");
  for(int i=0; i<databytes; i++){
        Serial.print(datastroke[i]);
        Serial.print(",");
      }
  Serial.println("");
  #endif  
}

void manageSensors(){
/*
0: Waiting for decay
1: Not ready
2: First read
3: Successive readings
4: Final value
*/

  unsigned long now = millis(); 

// First check and adjust the sensors state
  for(int i=0; i<nbsensors; i++){
    
    if(sensorstate[i] == 0){ //0: Waiting for decay
      if(sensorsdecayend[i] < now) sensorstate[i] = 2;
    }
    
    if(sensorstate[i] == 1){ //1: Not ready
      
    }
    
    if(sensorstate[i] == 3){ //3: Successive readings
      if(startread[i] + peakread < now) sensorstate[i] = 4;
    }
    
    if(sensorstate[i] == 4){ //4: Final value
      
      sensorstroke[i] = map(maxsensorvalue[i], 0, 1023, 0, 200);
      sensorsdecayend[i] = map(maxsensorvalue[i], 0, 1023,mindecay,maxdecay) + now;
      
      if(sensorstroke[i] > 0){
        sendStroke(sensorstroke[i]);
      }
      
      maxsensorvalue[i] = 0;
      sensorstate[i] = 0;
      
      #ifdef DEBUG
      Serial.print("Mumber of reads: ");
      Serial.println(numberreads[i]);
      numberreads[i] = 0;
      #endif DEBUG
    }    
  }

// Read sensors  
  for(int i=0; i<nbsensors; i++){
    
    if(sensorstate[i] == 2  ){
      int val = analogRead(sensorspin[i]);
      if(val > threshold[i]){
        if (val>maxsensorvalue[i]) maxsensorvalue[i]=val;
        
        #ifdef DEBUG
        numberreads[i]++;
        #endif DEBUG
        
        sensorstate[i] = 3;
        startread[i] = now;
      }
    }
      
    if(sensorstate[i] == 3 ){
      int val = analogRead(sensorspin[i]);
      if (val>maxsensorvalue[i]) maxsensorvalue[i]=val;
      
      #ifdef DEBUG
      numberreads[i]++;
      #endif DEBUG
    }
  }
}
