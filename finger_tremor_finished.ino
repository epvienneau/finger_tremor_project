//#include <Statistic.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <math.h>

// Pins
uint8_t LED1_pin = 5; //lsb
uint8_t LED2_pin = 4; 
uint8_t LED3_pin = 3;
uint8_t LED4_pin = 2; //msb
uint8_t LEDG_pin = 6; //green one that flashes during calibration
uint8_t data_pin = A0; //data input pin from photoresistor

// Variables
uint8_t tremorCount = 0; //counter for number of tremors
int was_device_off=0;

  // For Sampling
int samplingDelay = 8; //(ms) 
int skip = 300; // (dont sample for 200 ms if you detect a tremor 
int newsample=0;
int analogSignal[30];
int baseline = 0; //baseline signal set during calibration 
int numSamples = 30; // for baseline signal


void setup() {
  //Serial.begin(9600);
  pinMode(LED1_pin, OUTPUT);
  pinMode(LED2_pin, OUTPUT);
  pinMode(LED3_pin, OUTPUT);
  pinMode(LED4_pin, OUTPUT);
  pinMode(LEDG_pin, OUTPUT);
  pinMode(data_pin, INPUT);
  calibrate();
}

void loop() {
  sampleSignal();
  if(newsample < 100){ // device is off dont do anything
    deviceisOFF();
  }else if(was_device_off==1){ // device was just off and is now on... calibrate 
    was_device_off=0;
    calibrate();
  }else{ // device was on and is still on
    was_device_off=0;
  }
}


void sampleSignal(){
  newsample = analogRead(data_pin); 
  if (newsample > (1.15*baseline)){ //arbitrarily saying value must be 15% greater than baseline to be tremor
    tremorCount++;
    if (tremorCount > 15){
      tremorCount = 0; //reset tremor count when it gets too high
      flashGreenLED(); //indicate to user that count was reset
    }
    updateLEDs(); //update LEDs to reflect new tremor count
    delay(skip); //skip ahead because clearly the next several points will be suprathreshold but want to find next tremor
  }
  delay(samplingDelay); //normal samplying delay
}


void calibrate(){
  digitalWrite(LEDG_pin, HIGH);
  tremorCount = 0;
  updateLEDs();
  for (int i=0; i<numSamples; i++){
    analogSignal[i] = analogRead(data_pin); 
    delay(samplingDelay);
  }   
  delay(skip);
  digitalWrite(LEDG_pin, LOW);
  setBaseline(); 
  delay(skip);
  flashGreenLED();
}

void setBaseline(){
  float sum = 0;
  for (int i=0; i<numSamples; i++){
    sum += analogSignal[i];//analogRead(data_pin);
  }
  baseline = sum/numSamples;
}

void deviceisOFF(){
  tremorCount = 0;
  updateLEDs();
  was_device_off=1;
  delay(skip);
}

void flashGreenLED(){
  digitalWrite(LEDG_pin, HIGH);
  delay(200);
  digitalWrite(LEDG_pin, LOW);
}


void updateLEDs(){
  if(tremorCount == 1){
    digitalWrite(LED1_pin, HIGH);
  }else if(tremorCount == 2){
    digitalWrite(LED1_pin, LOW);
    digitalWrite(LED2_pin, HIGH);
  }else if(tremorCount == 3){
    digitalWrite(LED1_pin, HIGH);
  }else if(tremorCount == 4){
    digitalWrite(LED1_pin, LOW);
    digitalWrite(LED2_pin, LOW);
    digitalWrite(LED3_pin, HIGH);
  }else if(tremorCount == 5){
    digitalWrite(LED1_pin, HIGH);
  }else if(tremorCount == 6){
    digitalWrite(LED1_pin, LOW);
    digitalWrite(LED2_pin, HIGH);
  }else if(tremorCount == 7){
    digitalWrite(LED1_pin, HIGH);
  }else if(tremorCount == 8){
    digitalWrite(LED1_pin, LOW);
    digitalWrite(LED2_pin, LOW);
    digitalWrite(LED3_pin, LOW);
    digitalWrite(LED4_pin, HIGH);
  }else if(tremorCount == 9){ //1001
    digitalWrite(LED1_pin, HIGH);
  }else if(tremorCount == 10){ //1010
    digitalWrite(LED1_pin, LOW);
    digitalWrite(LED2_pin, HIGH);
  }else if(tremorCount == 11){ //1011
    digitalWrite(LED1_pin, HIGH);
  }else if(tremorCount == 12){ //1100
    digitalWrite(LED1_pin, LOW);
    digitalWrite(LED2_pin, LOW);
    digitalWrite(LED3_pin, HIGH);
  }else if(tremorCount == 13){
    digitalWrite(LED1_pin, HIGH);
  }else if(tremorCount == 14){
    digitalWrite(LED1_pin, LOW);
    digitalWrite(LED2_pin, HIGH);
  }else if(tremorCount == 15){
    digitalWrite(LED1_pin, HIGH);
    digitalWrite(LED2_pin, HIGH);
  }else{
    digitalWrite(LED1_pin, LOW);
    digitalWrite(LED2_pin, LOW);
    digitalWrite(LED3_pin, LOW);
    digitalWrite(LED4_pin, LOW);
  }
}




