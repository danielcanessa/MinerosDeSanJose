#include <Ultrasonic.h>

Ultrasonic ultrafront(5,7);  // (Trig PIN,Echo PIN)
Ultrasonic ultraright(3,4);  // (Trig PIN,Echo PIN)
Ultrasonic ultraleft(10,8);   // (Trig PIN,Echo PIN)


void setup() {
  Serial.begin(9600); 
  pinMode(13, OUTPUT); // VCC pin
  pinMode(12, OUTPUT); // VCC pin
  pinMode(11, OUTPUT); // VCC pin
  
  digitalWrite(12, HIGH); // VCC +5V mode  
  digitalWrite(13, LOW); // VCC +5V mode    
  digitalWrite(11, LOW); // VCC +5V mode  
  
}

int dir=0;
void loop()
{
  int frontValue=ultrafront.Ranging(CM);
  if(frontValue<51 && dir==0)
  {
    digitalWrite(13, HIGH); // VCC +5V mode  
    digitalWrite(11, HIGH); // VCC +5V mode  
   
    delay(100);
    int rightValue = ultraright.Ranging(CM);
    int leftValue = ultraleft.Ranging(CM);
    if(ultraright.Ranging(CM)>ultraleft.Ranging(CM)) //right
    {
      Serial.println("Right Direction");
      dir=1;
      digitalWrite(11, LOW); //apaga el right
      /////Logica de doblar a la derecha
      
      
    }
    else //left
    {
      Serial.println("Left Direction"); 
      dir=2;
      digitalWrite(13, LOW); // apaga el left 
      /////Logica de doblar a la izquierda
     
     
    
    }  

    //digitalWrite(13, LOW); // VCC +5V mode  
    //digitalWrite(11, LOW); // VCC +5V mode  
  }
  else
  {
    if(dir==1)
    {
      int newfront = ultraleft.Ranging(CM);
      if(newfront>45)
      {
        dir=0;
        digitalWrite(13, LOW); // apaga el left 
        Serial.print("Sensor left indica girar al front");
        ////////////////////////Logica girar a la izquierda
      } 
      
    }
    else if(dir==2)
    {
      int newfront = ultraright.Ranging(CM);
      if(newfront>45)
      {
        dir=0;
        digitalWrite(11, LOW); // apaga el left 
        Serial.print("Sensor right indica girar al front");
        ////////////////////////Logica girar a la derecha
      } 
      
    }
    Serial.print("Front: ");
    Serial.print(frontValue); // CM or INC
    Serial.print(" cm" );
    
  }

  
 
  

  delay(500);
  
  Serial.println("");
}
