#define TRIGPIN             10
#define ECHOPIN              2
#define BUTTON1             A2  //Red
#define BUTTON2             A3  //Green
#define RIGHTDIRECTION      12
#define RIGHTSPEED           3
#define RIGHTBRAKE           9
#define RIGHTCURRENT        A0
#define LEFTDIRECTION       13
#define LEFTSPEED           11
#define LEFTBRAKE            8
#define LEFTCURRENT         A1

// DC-motor (A = høyre, B = venstre)
const int maxSpeed = 255; 

// Stepermotor
int aPin = 4; //IN1: coil a one end
int bPin = 5; //IN2: coil b one end
int aPrimePin = 6; //IN3: coil aPrime other end of coil a
int bPrimePin = 7; //IN4: coil bPrime other end of coil b

int degree = 0;
int delay1 = 2; // The delay between each step in milliseconds
int count = 0; // The number of steps

// Ultrasonic sensor
//long duration, distance;

void setup() {
  Serial.begin(9600);
  //define digital pins
  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  pinMode(RIGHTDIRECTION, OUTPUT);
  pinMode(RIGHTBRAKE, OUTPUT);
  pinMode(LEFTDIRECTION, OUTPUT);
  pinMode(LEFTBRAKE, OUTPUT);
  pinMode(aPin, OUTPUT);
  pinMode(bPin, OUTPUT);
  pinMode(aPrimePin, OUTPUT);
  pinMode(bPrimePin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  //start with all coils off
  digitalWrite(aPin, LOW);
  digitalWrite(bPin, LOW);
  digitalWrite(aPrimePin, LOW);
  digitalWrite(bPrimePin, LOW);
}

void loop() {
  int distance = getDistance();
  Serial.println(distance);
  if (distance < 0){
  }
  else if (distance >= 0 && distance <=10){
    fullStop();
  }
  else {
    goForward(70);
  }
  
  mainStepper();  

}

void calibration(){
  int button1 = analogRead(BUTTON1);
  int button2 = analogRead(BUTTON2);

  if(button1 > 1000){
    Serial.println("Calibrating Clockwise..");
    initCW();
    rotateES();
  }
  else if(button2 > 1000){
    Serial.println("Calibrating Anti Clockwise..");
    initACW();
    rotateES();
  }
}

void mainStepper(){
  if (analogRead(BUTTON2) > 1000 && analogRead(BUTTON1) > 1000){
    int button2 = 0;
    int button1 = 0;
    Serial.println("Calibration..");
    delay(2000);
    while(button2 % 2 == 0 && button1 % 2 == 0) {
      if (analogRead(BUTTON2) > 1000 && analogRead(BUTTON1) > 1000){
        button1++;
        button2++;
      } else {
        calibration();
      }
    }
    Serial.println("Exiting Calibration..");
    digitalWrite(LED_BUILTIN, HIGH);
    degree = 0;
    count = 0;
    Serial.println("Current position is set to HOME..");
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
  }
  else if (buttonPressed()){
    returnHome();
  }
  
  if (degree == 90) { // Check if number of rotations is even
  // if so reverse direction
    initACW();
  }
  else if (degree == 0){ // If number of rotations is an odd number
    initCW();
  }

  rotateES();
}

boolean buttonPressed(){
  if(analogRead(BUTTON1) > 1000){
    Serial.print("Reading BUTTON. Value is: ");
    Serial.println(analogRead(BUTTON1));
    return true;
  }
  else{
    return false;
  }
}

void returnHome(){
  fullStop();   //stop DC-motors
  Serial.println("Returning home..");
  int i = 0;
  while(degree != 0){   //while not home at 0 degrees
    if(i == 0){
      initACW();    //initialize rotation anti clockwise
      i++;
    } else rotateES();   //rotate eight steps
  }
  Serial.println("HOME..");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(10000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(2000);
}

void initACW(){   //initialize rotation anti clockwise
  Serial.println("Initialize ACW..");
  aPin = 7;
  bPin = 6;
  aPrimePin = 5;
  bPrimePin = 4;
}

void initCW(){    //initialize rotation clockwise
  Serial.println("Initialize CW..");
  aPin = 4;
  bPin = 5;
  aPrimePin = 6;
  bPrimePin = 7;
}

void rotateES(){  //rotate eight steps in chosen direction
  // 1. Set the aPin High
  digitalWrite(aPin, HIGH);
  digitalWrite(bPin, LOW);
  digitalWrite(aPrimePin, LOW);
  digitalWrite(bPrimePin, LOW);
  // Allow some delay between energizing the coils to allow the stepper rotor time to respond.
  delay(delay1); // So, delay1 milliseconds

  // 2. Energize aPin and bPin to HIGH
  digitalWrite(aPin, HIGH);
  digitalWrite(bPin, HIGH);
  digitalWrite(aPrimePin, LOW);
  digitalWrite(bPrimePin, LOW);
  // Allow some delay between energizing the coils to allow the stepper rotor time to respond.
  delay(delay1); // So, delay1 milliseconds

  // 3. Set the bPin to High
  digitalWrite(aPin, LOW);
  digitalWrite(bPin, HIGH);
  digitalWrite(aPrimePin, LOW);
  digitalWrite(bPrimePin, LOW);
  // Allow some delay between energizing the coils to allow the stepper rotor time to respond.
  delay(delay1); // So, delay1 milliseconds

  // 4. Set the bPin and the aPrimePin to HIGH
  digitalWrite(aPin, LOW);
  digitalWrite(bPin, HIGH);
  digitalWrite(aPrimePin, HIGH);
  digitalWrite(bPrimePin, LOW);
  // Allow some delay between energizing the coils to allow the stepper rotor time to respond.
  delay(delay1); // So, delay1 milliseconds

  // 5. Set the aPrime Pin to high
  digitalWrite(aPin, LOW);
  digitalWrite(bPin, LOW);
  digitalWrite(aPrimePin, HIGH);
  digitalWrite(bPrimePin, LOW);
  // Allow some delay between energizing the coils to allow the stepper rotor time to respond.
  delay(delay1); // So, delay1 milliseconds

  // 6. Set the aPrimePin and the bPrime Pin to HIGH
  digitalWrite(aPin, LOW);
  digitalWrite(bPin, LOW);
  digitalWrite(aPrimePin, HIGH);
  digitalWrite(bPrimePin, HIGH);
  // Allow some delay between energizing the coils to allow the stepper rotor time to respond.
  delay(delay1); // So, delay1 milliseconds

  // 7. Set the bPrimePin to HIGH
  digitalWrite(aPin, LOW);
  digitalWrite(bPin, LOW);
  digitalWrite(aPrimePin, LOW);
  digitalWrite(bPrimePin, HIGH);
  // Allow some delay between energizing the coils to allow the stepper rotor time to respond.
  delay(delay1); // So, delay1 milliseconds

  // 8. Set the bPrimePin and the aPin to HIGH
  digitalWrite(aPin, HIGH);
  digitalWrite(bPin, LOW);
  digitalWrite(aPrimePin, LOW);
  digitalWrite(bPrimePin, HIGH);
  // Allow some delay between energizing the coils to allow
  // the stepper rotor time to respond.
  delay(delay1); // So, delay1 milliseconds

  if (aPin == 7){
    count = count - 8;
  }
  else if (aPin == 4){
    count = count + 8;
  }
  degree = (360.0 * (count / 4096.0));
}

int getDistance(){
  int duration, distance;
  digitalWrite(TRIGPIN, LOW);  //
  delayMicroseconds(2); //
  
  digitalWrite(TRIGPIN, HIGH); //Send pulse
  delayMicroseconds(10); // Wait 10ms
  digitalWrite(TRIGPIN, LOW); //turn off
  
  duration = pulseIn(ECHOPIN, HIGH); //listen for pulse
  return distance = (duration/2) * 0.0343; //calculate distance
}

void goForward(int goSpeed){
  rightForward(goSpeed);
  leftForward(goSpeed);
}

void goBackward(int goSpeed){
  rightBackward(goSpeed);
  leftBackward(goSpeed);
}

void turnLeftOnSpot(int turnSpeed){
  rightForward(turnSpeed);
  leftBackward(turnSpeed);
}

void turnRightOnSpot(int turnSpeed){
  rightBackward(turnSpeed);
  leftForward(turnSpeed);
}

void fullStop(){
  brakeRight();
  brakeLeft();
}
void rightForward(int goSpeed){
  digitalWrite(RIGHTDIRECTION, LOW);
  digitalWrite(RIGHTBRAKE, LOW);
  analogWrite(RIGHTSPEED, goSpeed); //max goSpeed = 255
}

void rightBackward(int goSpeed){
  brakeRight();
  digitalWrite(RIGHTDIRECTION, HIGH);
  digitalWrite(RIGHTBRAKE, LOW);
  analogWrite(RIGHTSPEED, goSpeed);
}

void leftForward(int goSpeed){
  digitalWrite(LEFTDIRECTION, HIGH);
  digitalWrite(LEFTBRAKE, LOW);
  analogWrite(LEFTSPEED, goSpeed);
}

void leftBackward(int goSpeed){
  brakeLeft();
  digitalWrite(LEFTDIRECTION, LOW);
  digitalWrite(LEFTBRAKE, LOW);
  analogWrite(LEFTSPEED, goSpeed);
}

void brakeRight(){
  digitalWrite(RIGHTBRAKE, HIGH);
}
void brakeLeft(){
  digitalWrite(LEFTBRAKE, HIGH);
}


