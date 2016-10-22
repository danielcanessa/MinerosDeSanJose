#include <Ultrasonic.h>
#include <SoftwareSerial.h>
 // arduino Rx (pin 2) ---- ESP8266 Tx
 // arduino Tx (pin 3) ---- ESP8266 Rx
SoftwareSerial esp8266(3,4); 

String ssid = "LosMalekus";
String password = "morza200";

Ultrasonic ultrafront(5,7);  // (Trig PIN,Echo PIN)
Ultrasonic ultraright(11,12);  // (Trig PIN,Echo PIN)
Ultrasonic ultraleft(10,8);   // (Trig PIN,Echo PIN)

//Declares input of the motors
int input1 = 1  ; //Forward pin of the right wheel
int input2 = 9; //Backward pin of the right wheel
int input3 = 6; //Forward pin of the left wheel
int input4 = 2; //Backward pint of the left wheel



void initializeESP8266()
{
 esp8266.begin(115200); // baud rate del ESP8266
 sendData("AT+RST\r\n",2000);      // resetear módulo
 sendData("AT+CWMODE=1\r\n",1000); // configurar como cliente
 sendData("AT+CWJAP=\""+ssid+"\",\""+password+"\"\r\n",8000); //SSID y contraseña para unirse a red 
 sendData("AT+CIFSR\r\n",1000);    // obtener dirección IP
 sendData("AT+CIPMUX=1\r\n",1000); // configurar para multiples conexiones
 sendData("AT+CIPSERVER=1,80\r\n",1000);         // servidor en el puerto 80  
}

void handleESP8266()
{
  if(esp8266.available())   // revisar si hay mensaje del ESP8266
 {
   if(esp8266.find("+IPD,")) // revisar si el servidor recibio datos
   {
    // delay(1000); // esperar que lleguen los datos hacia el buffer
     int conexionID = esp8266.read()-48; // obtener el ID de la conexión para poder responder

     
          
     while(esp8266.available())
     {      
      char c = esp8266.read();
      Serial.print(c);
     }
   
     String webpage = "<h1>Mineros de San Jose</h1>";

        
     // comando para enviar página web
     String comandoWebpage = "AT+CIPSEND=";
     comandoWebpage+=conexionID;
     comandoWebpage+=",";
     comandoWebpage+=webpage.length();
     comandoWebpage+="\r\n";
     
     sendData(comandoWebpage,500);
   
     sendData(webpage,500);
     
     
     String comandoCerrar = "AT+CIPCLOSE=";
     comandoCerrar+=conexionID;
     comandoCerrar+="\r\n";   
     sendData(comandoCerrar,500);

    
   }
 }
}


void setup() {
  Serial.begin(115200); 
  initializeESP8266();
  
  pinMode(input1, OUTPUT);  
  pinMode(input2, OUTPUT);  
  pinMode(input3, OUTPUT);  
  pinMode(input4, OUTPUT);
//   digitalWrite(input1, HIGH);  //This pin is the forward pin of the right wheel
//  digitalWrite(input2, HIGH);     //This pin is the backward pin of the right wheel//
//  digitalWrite(input3, HIGH); //This pin is the forward pin of the left wheel
 // digitalWrite(input4, HIGH);  //This pin is the backward pin of the left wheel
}

/*
Enviar comando al esp8266 y verificar la respuesta del módulo, todo esto dentro del tiempo timeout
*/
void sendData(String comando, const int timeout)
{
 long int time = millis(); // medir el tiempo actual para verificar timeout
 
 esp8266.print(comando); // enviar el comando al ESP8266
 
 while( (time+timeout) > millis()) //mientras no haya timeout
 {

   while(esp8266.available()) //mientras haya datos por leer
   { 
     
     // Leer los datos disponibles
     char c = esp8266.read(); // leer el siguiente caracter
     Serial.print(c);
   }
 } 
 return;
}



int dir=0;
void loop()
{
  handleESP8266(); 
  delay(1000);
   
  int frontValue=ultrafront.Ranging(CM);
  int rightValue = ultraright.Ranging(CM);
  int leftValue = ultraleft.Ranging(CM);

   //delay(200);

// Serial.println("left sensor: "+String(leftValue)+", front sensor: "+String(frontValue)+", right value: "+String(rightValue));
 
  
  if(frontValue<51 && dir==0)
  {
         
    
    if(rightValue>leftValue) //right
    {
 //     Serial.println("Right Direction");
      dir=1;
      /////Logica de doblar a la derecha
      goRight();
      
    }
    else //left
    {
 //     Serial.println("Left Direction"); 
      dir=2;
      /////Logica de doblar a la izquierda
      goLeft();
     
    
    }  

    //digitalWrite(A0, LOW); // VCC +5V mode  
    //digitalWrite(A2, LOW); // VCC +5V mode  
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
      } 
      
    }
    goUp();
  //  Serial.print("Front: ");
  //  Serial.print(frontValue); // CM or INC
  //  Serial.print(" cm" );
    
  }

 

  //delay(200); 
  //Serial.println("");
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

//This function makes the robot turn LEFT
void goLeft(){
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

//This function makes the robot turn RIGHT
void goRight(){
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
