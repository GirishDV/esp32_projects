
byte sensorPin = 27; //water flow meter connected to pin 27 of esp32
// The hall-effect flow sensor outputs approximately 4.5 pulses per second per
// litre/minute of flow.
float calibrationFactor = 4.5;

volatile byte pulseCount;
float flowRate;
unsigned int flowMilliLitres;
float totalMilliLitres;
float in_L = 0.00;
int l;
unsigned long oldTime;

void setup() {
  
  Serial.begin(9600);  //Initialize serial
  
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);
  
  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;

  /*The Hall-effect sensor is connected to pin 27 which uses interrupt 0.
    Configured to trigger on a FALLING state change (transition from HIGH
    state to LOW state)*/
    
  attachInterrupt(sensorPin, pulseCounter, FALLING);
}

 void loop(){
  
   /* -----------------------------WATER FLOW SENSOR----------------------------*/
  if((millis() - oldTime) > 1000)    // Only process counters once per second
  { 
    /* Disable the interrupt while calculating flow rate and sending the value to
       the host
       detachInterrupt(sensorInterrupt);
        
       Because this loop may not complete in exactly 1 second intervals we calculate
       the number of milliseconds that have passed since the last execution and use
       that to scale the output. We also apply the calibrationFactor to scale the output
       based on the number of pulses per second per units of measure (litres/minute in
       this case) coming from the sensor.*/
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    
    /* Note the time this processing pass was executed. Note that because we've
       disabled interrupts the millis() function won't actually be incrementing right
       at this point, but it will still return the value it was set to just before
       interrupts went away.*/
       
    oldTime = millis();
    
    /* Divide the flow rate in litres/minute by 60 to determine how many litres have
       passed through the sensor in this 1 second interval, then multiply by 1000 to
       convert to millilitres.*/
       
    flowMilliLitres = (flowRate / 60) * 1000;
    
    /*Add the millilitres passed in this second to the cumulative total*/
    
    totalMilliLitres += flowMilliLitres; 
    unsigned int frac;
    
    /*Print the flow rate for this second in litres / minute*/
    
    Serial.print("Flow rate-----------------:");
    Serial.print(int(flowRate)); 
    Serial.println("L/min");

    /* Print the cumulative total of litres flowed since starting */
    
    Serial.print("Total Water used today----:");        
    in_L = (totalMilliLitres/1000);                //converting mililitres to litres
    Serial.print(in_L);
    Serial.println("L");
    if (in_L>0){
      l = (0+in_L);
    }
    Serial.print("total usage---------------:");
    Serial.print(l);
    Serial.println(" L");
    /*Reset the pulse counter so we can start incrementing again*/
    
    pulseCount = 0;
    
    /*Enable the interrupt again now that we've finished sending output */
    
    attachInterrupt(sensorPin, pulseCounter, FALLING);
  }
 }
  
void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}
