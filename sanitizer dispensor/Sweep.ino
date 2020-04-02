#include <ESP32Servo.h>
Servo servo1;
Servo servo2;
int trigPin = 21;
int echoPin = 19;
long distance;
long duration;
 
void setup() 
{
servo1.attach(18);
servo2.attach(12); 
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);// put your setup code here, to run once:
}
 
void loop() {
  ultra();
  servo1.write(0);
  servo2.write(50);
  if(distance <= 10){
  servo1.write(180);
  servo2.write(180);
  }
}
void ultra(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration*0.034/2;
  }
