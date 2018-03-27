boolean debug = true;

//Physical pins
int sensorPin      = A0; 
int sensorPinPower = A1;
int pumpPin        = A5; 

const long oneSecond = 1000;
const long oneMinute = oneSecond * 60;
const long oneHour   = oneMinute * 60;

int moistureLevelThreshold = 350;

int minutesToLoop  = 60;
int timesToSplit   = minutesToLoop / 10;
long delayTime;

int secondsToPump        = 5;
int secondsToPowerSensor = 10;

//The time to wait in the for loop.
long waitTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(sensorPinPower, OUTPUT);

  //Wait to check
  delayTime = (minutesToLoop / timesToSplit) * oneMinute;
  
  if(debug == true){
    delayTime = 1000;
    secondsToPump = 1;
  }
}

void loop() {
  int moistureLevel = getAverageMoistureLevel();

  if(moistureLevel < moistureLevelThreshold)
  {
    turnOnPump();
  }
}

int getAverageMoistureLevel(){
  int average = 0;
  //Check every `timesToSplit` / `minutesToLoop` for data.
  for (int i=1; i <= timesToSplit; i++){
    average += getMoistureLevel();

    delay(delayTime);
  }

  //Calculate average
  int tempAverage = (average / timesToSplit);
  
  sendDebugMessage(tempAverage, "Average");
  
  return tempAverage;
}

int getMoistureLevel(){
  //Power up pin
  turnOnSensorPin();

  //Get the data.
  int output_value = analogRead(sensorPin);

  sendDebugMessage(output_value, "Literal");
  
  turnOffSensorPin();
  
  return output_value;
}

void turnOnPump(){
  if(debug == true){
    Serial.println("PUMPINGG!!");
    return;
  }

  digitalWrite(pumpPin, HIGH);

  delay(secondsToPump * 1000);
  
  turnOffPump();
}

void turnOffPump(){
  if(debug == true) return;
  
  digitalWrite(pumpPin, LOW);
}

void turnOnSensorPin(){
  digitalWrite(sensorPinPower, HIGH);
  //The delay is because the program is faster than the sensor can handle.
  delay(secondsToPowerSensor * oneSecond);
}

void turnOffSensorPin(){
  digitalWrite(sensorPinPower, LOW);
}

void sendDebugMessage(int output_value, String kind){
  if(debug == true){
    int output_value_percentage = map(output_value,550,0,0,100);
    
    Serial.print(kind + " mositure : ");
    Serial.print(output_value);
    Serial.print(" raw, ");
    Serial.print(output_value_percentage);
    Serial.println("%.");
  }
}

//08 feb 16:10: Mositure : 259 raw, 52%.
//09 feb 11:31: Mositure : 341 raw, 38%.
//10 feb 16:08: Moisture : 357 raw, 35%.
//11 feb 19:41: Mositure : 371 raw, 32%.
//12 feb 01:21: Mositure : 392 raw, 28%.
//13 feb 00:47: Mositure : 407 raw, 26%.
//14 feb 00:38: Mositure : 424 raw, 22%. GAVE WATER
//15 feb 16:47: Mositure : 359 raw, 34%.
