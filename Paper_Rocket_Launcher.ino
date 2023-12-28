
/*Code for paper rocket launcher
 * 5/26/23
 * By Ronan Valadez
 */

//pin numbers representing rgb button colors
int red = 19;
int green = 18;
int blue = 17; 


bool isYellowOn = false;  //bool variable to see if yellow light was on 
int ultrasonicSensor_value =0; // represents the value the ultra sonic sensor reads
int ultrasonicSensor = A6; // represenst the pin number for the ultrasonic sensor
float voltage = 0; // represents voltage from ultra sonic sensor
float feet = 0;  // represents the number of feet the ultrasonic sensor sees
float distanceLimit = 5;  // minimum distance in order to fire
int distanceSensorOverride = 6;  //Sensor pin number that is used to overrride ultrasonic sensor
int distanceSensorOverrideState = 0;  // represents the value of the distanceSensorOverride
int buttonPin = 3;    // represents button pin number
int buttonState = 0;  //represents value of fire button
int driver = 10;      // represents driver pin number
int limit_switch = 4;  //represents the switch that lets the rocket fire(acts as a safety switch) aka arm switch
int switch_state = 0; // represents the state of the arm switch
int redInterval = 10000; // represents cooldown time
int greenInterval = 1000; // represents green delay time
int yellowInterval = 1000; //  represents yellow delay time
int blueInterval = 1500; // represents blue delay time
int driverInterval = 250; // represents driver that fires the rocket 

void setup() {
  // put your setup code here, to run once:
  //set color pins to OUTPUT
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  //set distanceSensorOverride to input
  pinMode(distanceSensorOverride, INPUT_PULLUP);

  // set driver pin to OUTPUT
  pinMode(driver, OUTPUT);

  //set switch pin and button pin to input
  pinMode(limit_switch, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);

  // turn rgb pins to HIGH
  digitalWrite(blue, HIGH);
  digitalWrite(green, HIGH);
  digitalWrite(red, HIGH);
  
  //set driver to HIGH and switch to LOW
  digitalWrite(driver, LOW);
  digitalWrite(limit_switch, HIGH);
  
  Serial.begin(9600);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(buttonPin);  // read button state
  switch_state = digitalRead(limit_switch);  // read switch state
 // Serial.println( digitalRead(buttonPin));
  //Serial.println( digitalRead(limit_switch));

  //read distanceSensorOverride
  distanceSensorOverrideState = digitalRead(distanceSensorOverride);

  // find how far the ultrasonic senor is sensing something in feet
  ultrasonicSensor_value = analogRead(ultrasonicSensor); // read analog value
  voltage = ultrasonicSensor_value * (5.0/1023); // convert analog value to voltage
  feet = (voltage/.0098)/12; //convert voltage to feet


  /*if distanceSensorOverrideState is connected, check ultrasonic sensor, 
  if distance is less than feet, you can fire
  otherwise, turn button to yellow which signifies something is in the path of the rocket
  After ther path is clear, the button turns to blue which is a safety check. The path of the rocket must be clear for 1.5 seconds
  Once clear, the button turns to green, and it is ready to fire*/
  
  if(distanceSensorOverrideState == LOW){
    
    if(feet > distanceLimit){

      /*Checks to see if button is yellow, if it is,
       * turn button to blue for 1.5 seconds, then to green when you exit if statement
       */
      if(isYellowOn== true){ 
        turnOffYellow();
        turnOnBlue();
        delay(blueInterval);
        turnOffBlue();
        isYellowOn = false;
        
      }
      
      turnOnGreen();
        if (buttonState == LOW && switch_state == LOW) {
          turnOffGreen();
          turnOnDriver();
          turnOnRed();
          turnOnGreen();
          }
    }
    else{
      turnOnYellow();
      
    }
    
  }

  //otherwise if distanceSensorOverride is not connected, ignore ultrasonic sensor, and proceed to fire
  else{
    turnOffYellow();
      turnOnGreen();
      // if button is pressed and switch is on, execute code
      if (buttonState == LOW && switch_state == LOW) {
        turnOffGreen();
        turnOnDriver();
        turnOnRed();
        turnOnGreen();
      }
  }
  

}


void turnOnRed() { //function to turn on red light for 10 seconds, symbolizes cooldown period
  digitalWrite(red, LOW);
  delay(redInterval);
  digitalWrite(red, HIGH);
  //delay(1000);
}

void turnOnGreen() { //turn on green light. Symbolizes ready to launch
  digitalWrite(green, LOW);
  //delay(greenInterval);

}

void turnOffGreen() { //turns off green light
  digitalWrite(green, HIGH);
  //delay(1000);
}

bool turnOnYellow(){ //turns color to yellow, signifies something is in the path of the rocket
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  isYellowOn = true;
  return isYellowOn;
  //delay(yellowInterval);
}

void turnOffYellow(){ //turns off yellow light
  digitalWrite(red, HIGH);
  digitalWrite(green, HIGH);
}

void turnOnBlue(){  // turns on blue light, symbolizes second safety check to make sure path of the rocket is clear
  digitalWrite(blue, LOW);
  
}

void turnOffBlue(){  // turns off blue light
  digitalWrite(blue, HIGH);
}


void turnOnDriver() { //activates driver
  digitalWrite(driver, HIGH);
  delay(driverInterval);
  digitalWrite(driver, LOW);

}
