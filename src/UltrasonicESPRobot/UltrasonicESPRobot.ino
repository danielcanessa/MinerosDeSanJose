#include <Ultrasonic.h>
#include <SoftwareSerial.h>

/* 
 * Module Declaration
 */
SoftwareSerial esp8266(3,4); 
Ultrasonic ultrafront(13,2);  // (Trig PIN,Echo PIN)
Ultrasonic ultraright(11,12);  // (Trig PIN,Echo PIN)
Ultrasonic ultraleft(5,6);   // (Trig PIN,Echo PIN)


const int AOUTpin=0;//the AOUT pin of the alcohol sensor goes into analog pin A0 of the arduino

/*
 * SSID Connection
 */
String ssid = "LosMalekus";
String password = "morza200";


//Declares input of the motors
int input1 = 10; //Forward pin of the right wheel
int input2 = 9; //Backward pin of the right wheel
int input3 = 7; //Forward pin of the left wheel
int input4 = 8; //Backward pint of the left wheel

int value; //Have the gas sensor value


void initializeESP8266()
{
 esp8266.begin(115200);                                       // Module Baud rate
 sendData("AT+RST\r\n",2000);                                 // Module Reset
 sendData("AT+CWMODE=3\r\n",1000);                            // Module must act as client
 sendData("AT+CWJAP=\""+ssid+"\",\""+password+"\"\r\n",8000); // SSID & Password to connect 
 sendData("AT+CIFSR\r\n",1000);                               // Current IP
 sendData("AT+CIPMUX=1\r\n",1000);                            // Enable multiple connections
}

//This method makes the post request to the API
void post(String carMovement){

 Serial.println(carMovement);
 String gasValue = String(value);
 String data = "{\"gas_level\":" + gasValue + ", \"direction\":\"forward\"}";

 /*
  * Specifies the lenght of the post headers
  */
  
 sendData("AT+CIPSTART=1,\"TCP\",\"104.198.237.45\",8081\r\n",50); 
 sendData("AT+CIPSEND=1,300\r\n",50); 
 
 /*
  * POST REQUEST HEADERS
  */
 sendData("POST /step HTTP/1.1\r\n",50); 
 sendData("HOST: 104.198.237.45:8081\r\n",50);   
 sendData("content-type: application/json\r\n",50); 
 sendData("accept: application/json\r\n",50); 
 sendData("accept-encoding: gzip, deflate\r\n",50); 
 sendData("accept-language: en-US,en;q=0.8\r\n",50); 
 sendData("content-type: application/json\r\n",50); 
 sendData("content-length: "+String(data.length())+"\r\n",50); 
 sendData("\r\n",50); //Required Enter as part of the protocol
 sendData(data + "\r\n",50); 

}


void setup() {
  pinMode(input1, OUTPUT);  
  pinMode(input2, OUTPUT);  
  pinMode(input3, OUTPUT);  
  pinMode(input4, OUTPUT);
  /* 
   * Initialization for motors 
   */
  digitalWrite(input1, LOW); //This pin is the forward pin of the left wheel
  digitalWrite(input2, LOW); //This pin is the forward pin of the left wheel
  digitalWrite(input3, LOW); //This pin is the forward pin of the left wheel
  digitalWrite(input4, LOW); //This pin is the forward pin of the left wheel
  Serial.begin(115200); 
  initializeESP8266();
  
}

/*
Enviar comando al esp8266 y verificar la respuesta del módulo, todo esto dentro del tiempo timeout
*/
void sendData(String comando, const int timeout)
{
 long int time = millis(); // Get current time to timeout verification
 
 esp8266.print(comando); // Send command to ESP
 
 while( (time+timeout) > millis()) //Meanwhile timeout not reach
 {

   while(esp8266.available()) //Meanwhile there is data to read
   { 
     char c = esp8266.read(); // Read Data
     //Serial.print(c);
   }
 } 
 return;
}

String movement = "forward";
int dir=0;
void loop()
{
   
  int frontValue=ultrafront.Ranging(CM);
  int rightValue = ultraright.Ranging(CM);
  int leftValue = ultraleft.Ranging(CM);

  value= analogRead(AOUTpin);//reads the analaog value from the alcohol sensor's AOUT pin 
  
  movement = "forward";
  if(frontValue<51 && dir==0)
  {
         
    if(rightValue>leftValue) //right
    {
      Serial.println("Right Direction");
      dir=1;
      /////Logica de doblar a la derecha
      goRight();
      movement = "right";
    }
    else //left
    {
      Serial.println("Left Direction"); 
      dir=2;
      /////Logica de doblar a la izquierda
      goLeft();
      movement = "left";    
    }  
  }
  else
  {
    if(dir==1)
    {
      int newfront = ultraleft.Ranging(CM);
      if(newfront>45)
      {
        dir=0;
        //Serial.print("Sensor left indica girar al front");
        ////////////////////////Logica girar a la izquierda
        goLeft();
        movement = "left";        
      } 
    }
    else if(dir==2)
    {
      int newfront = ultraright.Ranging(CM);
      if(newfront>45)
      {
        dir=0;        
    //    Serial.print("Sensor right indica girar al front");
        ////////////////////////Logica girar a la derecha
        goRight();
        movement = "right";        
      } 
      
    }
    post(movement);
    goUp();    
  }
}


//This function makes the robot go FORWARD
void goUp(){
  digitalWrite(input1, HIGH);  //This pin is the forward pin of the right wheel
  digitalWrite(input2, LOW);     //This pin is the backward pin of the right wheel
  digitalWrite(input3, HIGH); //This pin is the forward pin of the left wheel
  digitalWrite(input4, LOW);  //This pin is the backward pin of the left wheel
}

//This function makes the robot go BACKWARD
void goDown(){
  digitalWrite(input1, LOW);  //This pin is the forward pin of the right wheel
  digitalWrite(input2, HIGH);     //This pin is the backward pin of the right wheel
  digitalWrite(input3, LOW); //This pin is the forward pin of the left wheel
  digitalWrite(input4, HIGH);  //This pin is the backward pin of the left wheel
}

//This function makes the robot turn RIGHT
void goRight(){
  digitalWrite(input1, LOW);  //This pin is the forward pin of the right wheel
  digitalWrite(input2, LOW);     //This pin is the backward pin of the right wheel
  digitalWrite(input3, LOW); //This pin is the forward pin of the left wheel
  digitalWrite(input4, HIGH);  //This pin is the backward pin of the left wheel
  delay(300);
  digitalWrite(input1, LOW);  //This pin is the forward pin of the right wheel
  digitalWrite(input2, LOW);     //This pin is the backward pin of the right wheel
  digitalWrite(input3, LOW); //This pin is the forward pin of the left wheel
  digitalWrite(input4, LOW);  //This pin is the backward pin of the left wheel
}

//This function makes the robot turn LEFT
void goLeft(){
  digitalWrite(input1, LOW);  //This pin is the forward pin of the right wheel
  digitalWrite(input2, HIGH);     //This pin is the backward pin of the right wheel
  digitalWrite(input3, LOW); //This pin is the forward pin of the left wheel
  digitalWrite(input4, LOW);  //This pin is the backward pin of the left wheel
  delay(300);
  digitalWrite(input1, LOW);  //This pin is the forward pin of the right wheel
  digitalWrite(input2, LOW);     //This pin is the backward pin of the right wheel
  digitalWrite(input3, LOW); //This pin is the forward pin of the left wheel
  digitalWrite(input4, LOW);  //This pin is the backward pin of the left wheel
}
