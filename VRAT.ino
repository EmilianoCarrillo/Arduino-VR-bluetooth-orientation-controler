#include <Servo.h>
#include <MeetAndroid.h>

//Values we are gonna get from Amarino App
int   compassValue = 0;
int   lastCompassValue = 0; 
float accelerometerValues[3] = {0};
int   intAccalerometerValues[3] = {0};
//My Servo names
Servo azimuth;
Servo pitch;
Servo roll;
//Final position of servos
int posAzimuth = 0; //Compass Sensor
int posPitch = 0;   //Accelerometer
int posRoll = 0;    //Accelerometer


// ERROR FUNCTION: Displays an error exception if occured.
void error(uint8_t flag, uint8_t values){
  Serial.print("ERROR: ");
  Serial.print(flag);
}
MeetAndroid meetAndroid(error);


 void setup() {
  Serial.begin(9600); //Begin serial link @ 9600bps
  //Stablish each event for each sensor
  meetAndroid.registerFunction(compassSensor, 'A'); //X
  meetAndroid.registerFunction(accelerometerSensor, 'B'); //Y & Z  
  
  //Attach Servos to Pins
  azimuth.attach(9); 
  pitch.attach(10);
  roll.attach(11);

  Serial.println("REMEMBER TO CONNECT RX AND TX PINS.");
}


void loop(){
  meetAndroid.receive(); // Receive events 
}


/*AZIMUTH (X) O: [0.359] -> [90, 270] : [0, 180] */
void compassSensor(byte flag, byte numOfValues){
  compassValue = meetAndroid.getInt();

  //Standard Deviation of +-15
  // Stay put if there is a sudden peak or low
  if(compassValue>lastCompassValue){
    if(compassValue-lastCompassValue > 15){
      posAzimuth = map(lastCompassValue, 90, 270, 180, 0);
    }else{
      posAzimuth = map(compassValue, 90, 270, 180, 0);
      lastCompassValue = compassValue;
    }
  } else{
    if(lastCompassValue-compassValue > 15){
      posAzimuth = map(lastCompassValue, 90, 270, 180, 0);
    } else{
      posAzimuth = map(compassValue, 90, 270, 180, 0);
      lastCompassValue = compassValue;
    }
  }

  //If values are out of bounds, correct them [0, 180]
  if(posAzimuth < 0  ){posAzimuth = 0;  }
  if(posAzimuth > 180){posAzimuth = 180;}
  azimuth.write(posAzimuth);

  //Put leading Zeros on Console
  char azimuthLeadingZeros[4];
  sprintf(azimuthLeadingZeros, "%03d", posAzimuth);
  char compassLeadingZeros[4];
  sprintf(compassLeadingZeros, "%03d", compassValue);

  Serial.print("A: "); //Debugging purposes
  Serial.print(azimuthLeadingZeros); Serial.print(" ");
  Serial.println(compassLeadingZeros);
}







/*PITCH (Y) [-10, 10] -> [0,180] */
/*ROLL (Z)  [-10, 10] -> [0,180] */
void accelerometerSensor(byte flag, byte numOfValues){
  
  meetAndroid.getFloatValues(accelerometerValues);
  
  posPitch  = map(int(accelerometerValues[1]), -10, 10, 0, 180);
  posRoll   = map(int(accelerometerValues[2]), -10, 10, 180, 0);

  pitch.write(posPitch);
  roll.write(posRoll);
}



