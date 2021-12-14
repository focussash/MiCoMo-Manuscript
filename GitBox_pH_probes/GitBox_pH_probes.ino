//Code for Arduino UNO. This code
//1. Take readings from pH sensors 
//2. Send them to PC (independent of PC itself)
//3. Take commands from PC and execute (pH pumps)

//Tweak the following parameters to achieve better pH control
#define tol 14 //tolerance for pH
#define pumptime 4000//How long each acid/base addition last, in milliseconds
#define initialization_time 1 //Time to run pump priming
#define NumTransfer 4

//Set the setpoints
float setpoint[3] = {6.7,6.7,6.7};

char instring[4]; //This stores the input from the Serial port
int instring_int[4]; //This stores the input from the Serial port converted to integer 

int analogPins[5] = {A0,A1,A2,A3,A4};

int pumppins[6]= {5,6,7,8,9,10};//pins for pumps for pH control
//For these pump pins, #1 = acid & #2 = base are for sensor 1,
//#3=acid & #4=base are for sensor 2, etc

int device_number;//To store the numbering of device
int device_type;//To store the type of device
int device_intensity;//To store intensity of device

int readings[6];//stores the readings from each sensor
float readingsf[6];//Actual pH readings
float slope[6];//Slopes of sensors
float intercept[6];//intercepts of sensors

int i,j;
int k = 3;//k denotes the maximum sensors in use right now
int timer; //for tracking how oftern do we send a signal to PC
int prime_flag = 0;//Flag for pumps having been primed


void setup() { 
 for (i = 0;i<k;i++)
  {
    pinMode(analogPins[i],INPUT);
  }
   for (i = 0;i<6;i++)
  {
    pinMode(pumppins[i],OUTPUT);
  }
  Serial.begin(9600);  
  //Serial.setTimeout(50);

  //Sensor 1
  slope[0] = 0.0176642;
  intercept[0] = -2.2314673;
  //Sensor 2
  slope[1] = 0.0176642; 
  intercept[1] = -2.2314673;
  //Sensor 3
  slope[2] = 0.0176642;
  intercept[2] = -2.2314673
  ;
}

void loop() {

//This part remains the same; If the PC sends
  //a signal, parse it and execute the commands
  // clear all the input caches and variables
  for (i = 0;i<4;i++)
  {
    instring[i] = ""; 
  }
    device_number = 0;
    device_type = 0;
    device_intensity = 0;
 if (Serial.available()>0){
  
  Serial.readBytesUntil("\r",instring,4);
  if ((int)instring[3]>48){ //check if the signal is valid?
    for (i = 0;i<4;i++)
    {
      instring_int[i] = (int)instring[i]-48; //parse the input data into 5 integer; the -48 is to convert the ascii number into integer
    }
    device_number = instring_int[2]*10+ instring_int[3] - NumTransfer;
    device_type = instring_int[1];
    device_intensity = instring_int[0];
  } 
  if (device_type > 0){//Non-zero device types means pumps
      for (j=1;j<6;j++){
        if (j == device_number){//digital control
            if (device_intensity == 1){
              digitalWrite(pumppins[j-1],HIGH);
            }
            else {
              digitalWrite(pumppins[j-1],LOW);
            }
        }
      } 
  }
  else {//Initialization process (pump priming)
    //for manual priming, serial input 1001
  if(device_intensity >0){
  prime_flag = 1;
  for (j = 0;j<6;j++){
  digitalWrite(pumppins[j],HIGH);
  }
 delay(initialization_time);
 for (j = 0;j<6;j++){
  digitalWrite(pumppins[j],LOW);
   } 
  }
  device_intensity = 0;
 }
}

if (prime_flag > 0){
  //Reading signals from sensors
 for (j = 0;j<k;j++)
  {
    readings[j] = analogRead(analogPins[j]); 
    //calculate actual pH 
    readingsf[j] = readings[j]*slope[j] + intercept[j];  
    //Following 2 lines are used with the UI{
    //Serial.print(j+1);
    //Serial.println(int(readings[j]));
    //}
    Serial.print("Sensor ");
    Serial.print(j+1);
    Serial.print(" pH is ");
    Serial.println(readingsf[j]);
    delay(1000);
  }

//Controlling pumps
//Optional condition: timer > 3
  if (1) {
    for (j = 0;j<k;j++) {
      if (readingsf[j] - setpoint[j] > tol) {
        digitalWrite(pumppins[2*j],HIGH);
        Serial.print("Adding acid to vessel ");
        Serial.println(j+1);
        delay(pumptime);
        Serial.println("stop addition");
        digitalWrite(pumppins[2*j],LOW);
      }
      if (readingsf[j] - setpoint[j] < - tol){
        digitalWrite(pumppins[2*j+1],HIGH);
        Serial.print("Adding base to vessel ");
        Serial.println(j+1);
        delay(pumptime);
        Serial.println("stop addition");
        digitalWrite(pumppins[2*j+1],LOW);
      }
    }
    timer = 0;
  }
    timer += 1;
 }
  delay(5000);
}
