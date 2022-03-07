//Code for Arduino Mega1
//This code 
//1. Take commands from PC and execute (liquid transfer pumps). 
//2. Send voltage to gas sparger. 
//3. Send voltage to pumps for liquid transfer.
//The following time stamps are all in seconds
//Start is earlier for inlet than outlet to compensate for evaporation
//For this, we just need food to colon and colon to waste
//Feed every 4 hours. Transfers for all reactors will happen at exact same time
//Inputting ~4.5ml of content and outputing ~4ml of content
//This ensures an overall ~30 hours retention time
#define TimeStamp1 14000//Food to start inputting liquid into reactors
#define TimeStamp2 14040 //Food to start removing liquid from reactors
#define TimeStamp3 14280 //Time to stop both transfers
#define TimeStamp4 14400 //Time to reset the system

#define initialization_time 1000 //Time to run pump priming

char instring[4]; //This stores the input from the Serial port
int instring_int[4]; //This stores the input from the Serial port converted to integer  
int i; 
int k;
int j;//Placeholder counter

int inputpin = 3; //pin for multichannel pump of adding liquid into reactors
int outputpin = 2;//pin for multichannel pump of removing liquid from reactors
//only 1 pin used because of using 6-channel pump
int spargerpin = 10;

int device_number;//To store the numbering of device
int device_type;//To store the type of device
int device_intensity;//To store intensity of device

int timer = 0;//Timer for liquid transfer
int tspa = 0; // Timer for sparging gas
int prime_flag = 0;//Flag for pumps having been primed

void setup() {
    Serial.begin(9600);
    pinMode (inputpin,OUTPUT);
    pinMode (outputpin,OUTPUT);
    pinMode (spargerpin,OUTPUT); 
  //Serial.setTimeout(50);
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
    device_number = instring_int[2]*10+ instring_int[3];
    device_type = instring_int[1];
    device_intensity = instring_int[0];
  } 
  if (device_type >0) {//Non-zero device types means pumps
      for (k=1;k<5;k++){
        if (k == device_number){//digital control
            if (device_intensity == 1){
              digitalWrite(outputpin,HIGH);
              digitalWrite(inputpin,HIGH);
            }
            else {
              digitalWrite(outputpin,LOW);
              digitalWrite(inputpin,HIGH);
            }
        }
      }
  }
  else {//Initialization process (pump priming)
  if(device_intensity >0){
//we dont need to actually "prime" anything
//for manual "priming", serial input 1001
  }
  device_intensity = 0;
  prime_flag = 1;
 }
}
if (prime_flag > 0){
//Now, check if we need liquid transfer
  if ((timer > TimeStamp1) && (timer < TimeStamp3)){
    digitalWrite(inputpin,HIGH);
  }
  else {
    digitalWrite(inputpin,LOW);
  }
  
  if ((timer > TimeStamp2) && (timer < TimeStamp3)){
    digitalWrite(outputpin,HIGH);
  }
  else {
    digitalWrite(outputpin,LOW);
  }
  
  if (timer > TimeStamp4){
    timer = 0;
  }
  timer += 1;

//Now, handles the gas sparging
  if (tspa > 5){//reset timer if it exceeds 5 secs
    tspa = 0;
  }
  tspa += 1;
  if (tspa > 3 && tspa < 5){//At the fifth second, sparge
    digitalWrite(spargerpin,HIGH);
  }
  else {
    digitalWrite(spargerpin,LOW);
  }

 //After everything, wait for 1 sec
  delay(1000); 
  Serial.println(timer);
}
}
