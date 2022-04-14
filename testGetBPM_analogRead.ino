int UpperThreshold = 500;
int LowerThreshold = 490;
int reading = 0;
float BPM = 0.0;
bool IgnoreReading = false;
bool FirstPulseDetected = false;
unsigned long FirstPulseTime = 0;
unsigned long SecondPulseTime = 0;
unsigned long PulseInterval = 0;
const unsigned long delayTime = 10;
const unsigned long delayTime2 = 1000;
const unsigned long baudRate = 9600;
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;

//edit
int count_index = 0;
double heart_rates[4],BPM_avg;
int heart_rate_length = 4;

//timer function
// First event timer
int myTimer1(long delayTime, long currentMillis)
{
  if(currentMillis - previousMillis >= delayTime)
  {
    previousMillis = currentMillis;
    return 1;
   }
  else{return 0;}
}

// Second event timer
int myTimer2(long delayTime2, long currentMillis)
{
  if(currentMillis - previousMillis2 >= delayTime2)
  {
    previousMillis2 = currentMillis;
    return 1;
   }
  else
  {
    return 0;
   }
}


void setup(){
  Serial.begin(baudRate);

}


void loop(){

  // Get current time
  unsigned long currentMillis = millis();

  // First event calculate BPM
  if(myTimer1(delayTime, currentMillis) == 1){

    reading = analogRead(0); 

    // not attach finger
    if(reading < LowerThreshold && IgnoreReading == false)
    {
      IgnoreReading = true;
    }
    
    // 
    else if(reading > UpperThreshold )
    {
      IgnoreReading = false;
    
     // Calculate Beats Per Minute.
    // BPM = (1.0/reading) * 60.0 * 1000;
       heart_rates[count_index++] = (1.0/reading) * 60.0 * 1000;

      if(count_index == heart_rate_length)
      {
        // find avg BPM
        int sum = 0;
        for(double heart_rate : heart_rates)
        {
          sum += heart_rate; 
        }
        BPM_avg = (double)sum/heart_rate_length;
        count_index %= heart_rate_length;
      }
    }
  
  }

    // Second event display section
  if(myTimer2(delayTime2, currentMillis) == 1){
   
    if(!(IgnoreReading))
    {
      Serial.print(reading);
      Serial.print("\t");
   
      Serial.print(BPM_avg);
      Serial.print(" BPM");
      Serial.print("\t");
   
      Serial.print(" prn ");
      Serial.println(heart_rate_length);
    }
    
    //Not attach finger
    else if(IgnoreReading)
    {
      Serial.println("a finger is not detected");
    }
  }
}
