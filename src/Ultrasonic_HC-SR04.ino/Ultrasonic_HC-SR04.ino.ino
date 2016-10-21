#include <Ultrasonic.h>

Ultrasonic ultrafront(5,7);  // (Trig PIN,Echo PIN)
Ultrasonic ultraright(11,12);  // (Trig PIN,Echo PIN)
Ultrasonic ultraleft(10,8);   // (Trig PIN,Echo PIN)


void setup() {
  Serial.begin(9600); 
}

int dir=0;
void loop()
{
  int frontValue=ultrafront.Ranging(CM);
  int rightValue = ultraright.Ranging(CM);
  int leftValue = ultraleft.Ranging(CM);

  Serial.println("left sensor: "+String(leftValue)+", front sensor: "+String(frontValue)+", right value: "+String(rightValue));
 
  
  if(frontValue<15 && dir==0)
  {
       
    delay(100);
    
    if(ultraright.Ranging(CM)>ultraleft.Ranging(CM)) //right
    {
      Serial.println("Right Direction");
      dir=1;
      /////Logica de doblar a la derecha
      
      
    }
    else //left
    {
      Serial.println("Left Direction"); 
      dir=2;
      /////Logica de doblar a la izquierda
     
     
    
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
