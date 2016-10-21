#include <Ultrasonic.h>

Ultrasonic ultrafront(5,7);  // (Trig PIN,Echo PIN)
Ultrasonic ultraright(11,12);  // (Trig PIN,Echo PIN)
Ultrasonic ultraleft(10,8);   // (Trig PIN,Echo PIN)

//Declares input of the motors
int input1 = 1; //Forward pin of the right wheel
int input2 = 9; //Backward pin of the right wheel
int input3 = 6; //Forward pin of the left wheel
int input4 = 2; //Backward pint of the left wheel


void setup() {
  Serial.begin(9600); 
  
  pinMode(input1, OUTPUT);  
  pinMode(input2, OUTPUT);  
  pinMode(input3, OUTPUT);  
  pinMode(input4, OUTPUT);
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
      goRight();
      
    }
    else //left
    {
      Serial.println("Left Direction"); 
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
        Serial.print("Sensor left indica girar al front");
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
        Serial.print("Sensor right indica girar al front");
        ////////////////////////Logica girar a la derecha
        goRight();
      } 
      
    }
    goUp();
    Serial.print("Front: ");
    Serial.print(frontValue); // CM or INC
    Serial.print(" cm" );
    
  }

  delay(500); 
  Serial.println("");
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
  delay(600);
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
  delay(600);
  digitalWrite(input1, LOW);  //This pin is the forward pin of the right wheel
  digitalWrite(input2, LOW);     //This pin is the backward pin of the right wheel
  digitalWrite(input3, LOW); //This pin is the forward pin of the left wheel
  digitalWrite(input4, LOW);  //This pin is the backward pin of the left wheel
}
