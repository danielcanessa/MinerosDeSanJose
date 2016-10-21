#include <SoftwareSerial.h>
 // arduino Rx (pin 2) ---- ESP8266 Tx
 // arduino Tx (pin 3) ---- ESP8266 Rx
SoftwareSerial esp8266(3,4); 

String ssid = "LosMalekus";
String password = "morza200";

int resetPin=3;

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
     delay(1000); // esperar que lleguen los datos hacia el buffer
     int conexionID = esp8266.read()-48; // obtener el ID de la conexión para poder responder

     int sendToServer=0;
     if(esp8266.find("send"))
     {
      sendToServer=1;
      
     }    
          
     while(esp8266.available())
     {      
      char c = esp8266.read();
      Serial.print(c);
     }
   
     String webpage = "<h1>Mineros de San Jose</h1>";

     if(sendToServer)
     {
       webpage+="<p>Enviando datos al server</p>";
       
     }
        
     // comando para enviar página web
     String comandoWebpage = "AT+CIPSEND=";
     comandoWebpage+=conexionID;
     comandoWebpage+=",";
     comandoWebpage+=webpage.length();
     comandoWebpage+="\r\n";
     
     sendData(comandoWebpage,1500);
   
     sendData(webpage,1500);
     
     
     String comandoCerrar = "AT+CIPCLOSE=";
     comandoCerrar+=conexionID;
     comandoCerrar+="\r\n";   
     sendData(comandoCerrar,1500);

    
   }
 }
}
void setup()
{
 Serial.begin(115200);  // monitor serial del arduino
 initializeESP8266();
}

void loop()
{
  handleESP8266(); 
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
