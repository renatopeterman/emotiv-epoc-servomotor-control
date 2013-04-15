#include <Servo.h>

Servo servo1;
Servo servo2;

char buffer[4];
int received;

void setup(){
  
  Serial.begin(9600);
  
  servo1.attach(8);
  servo2.attach(9);
  
  servo1.write(90);
  servo2.write(90);
  
  received = 0;
  //buffer[received] = '\0';
}

void loop(){
  
  if(Serial.available()){
    
    char c = Serial.read();
    
    if(c == 97){
      Serial.println(buffer);
      int numero = atoi(buffer);
      servo1.write(numero);
      received = 0; 
    }
    
    if(c == 98){
      Serial.println(buffer);
      int numero = atoi(buffer);
      if(numero >= 60){
        servo2.write(numero);
        received = 0; 
      }
      received = 0; 
    }
    
    if(c != 97 && c != 98){
      buffer[received++] = c;
    }
    
    /*
    if(received >= (sizeof(buffer)-1)){
      
      Serial.println(buffer);
      int numero = atoi(buffer);
      servo1.write(numero);
      servo2.write(numero);
      received = 0; 
      
    }
    */
    Serial.flush();
  }

}
