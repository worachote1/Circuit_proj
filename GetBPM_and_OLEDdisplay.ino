//For OLED libary
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // pixel ความกว้าง
#define SCREEN_HEIGHT 64 // pixel ความสูง 


#define OLED_RESET -1
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//for calculate BPM
int UpperThreshold = 500;
int LowerThreshold = 490;
int reading = 0;
float BPM = 0.0;
bool IgnoreReading = false;

const unsigned long delayTime = 10;

const unsigned long delayTime2 = 1000;

const unsigned long delayTime3[4] = { 500,800,1100,2000 }; // for swap heart logo
int dealyTime3_length = 4;

const unsigned long baudRate = 9600;

unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0; // for swap heart logot

long rand_index; // for swap heart logo

int count_index = 0;
double heart_rates[4], BPM_avg;
int heart_rate_length = 4;

// draw bitmap picture section
static const unsigned char PROGMEM logo_Heart_24X21[] =
{ 0x03, 0xC0, 0xF0, 0x06, 0x71, 0x8C, 0x0C, 0x1B, 0x06, 0x18, 0x0E, 0x02, 0x10, 0x0C, 0x03, 0x10,
  0x04, 0x01, 0x10, 0x04, 0x01, 0x10, 0x40, 0x01, 0x10, 0x40, 0x01, 0x10, 0xC0, 0x03, 0x08, 0x88,
  0x02, 0x08, 0xB8, 0x04, 0xFF, 0x37, 0x08, 0x01, 0x30, 0x18, 0x01, 0x90, 0x30, 0x00, 0xC0, 0x60,
  0x00, 0x60, 0xC0, 0x00, 0x31, 0x80, 0x00, 0x1B, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x04, 0x00,
};

static const unsigned char PROGMEM logo_Heart_32X32[] =
{ 0x01, 0xF0, 0x0F, 0x80, 0x06, 0x1C, 0x38, 0x60, 0x18, 0x06, 0x60, 0x18, 0x10, 0x01, 0x80, 0x08,
  0x20, 0x01, 0x80, 0x04, 0x40, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x02, 0xC0, 0x00, 0x08, 0x03,
  0x80, 0x00, 0x08, 0x01, 0x80, 0x00, 0x18, 0x01, 0x80, 0x00, 0x1C, 0x01, 0x80, 0x00, 0x14, 0x00,
  0x80, 0x00, 0x14, 0x00, 0x80, 0x00, 0x14, 0x00, 0x40, 0x10, 0x12, 0x00, 0x40, 0x10, 0x12, 0x00,
  0x7E, 0x1F, 0x23, 0xFE, 0x03, 0x31, 0xA0, 0x04, 0x01, 0xA0, 0xA0, 0x0C, 0x00, 0xA0, 0xA0, 0x08,
  0x00, 0x60, 0xE0, 0x10, 0x00, 0x20, 0x60, 0x20, 0x06, 0x00, 0x40, 0x60, 0x03, 0x00, 0x40, 0xC0,
  0x01, 0x80, 0x01, 0x80, 0x00, 0xC0, 0x03, 0x00, 0x00, 0x60, 0x06, 0x00, 0x00, 0x30, 0x0C, 0x00,
  0x00, 0x08, 0x10, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x01, 0x80, 0x00
};
boolean active_smallerHeart = true, active_largerHeart = false;


//timer function
// First event timer
int myTimer1(long delayTime, long currentMillis)
{
  if (currentMillis - previousMillis >= delayTime)
  {
    previousMillis = currentMillis;
    return 1;
  }
  else {
    return 0;
  }
}

// Second event timer
int myTimer2(long delayTime2, long currentMillis)
{
  if (currentMillis - previousMillis2 >= delayTime2)
  {
    previousMillis2 = currentMillis;
    return 1;
  }
  else
  {
    return 0;
  }
}
// Second event timer
int myTimer3(long delayTime3, long currentMillis)
{
  if (currentMillis - previousMillis3 >= delayTime3)
  {
    previousMillis3 = currentMillis;
    return 1;
  }
  else
  {
    return 0;
  }
}
//Display OLED Function

void getBPM_OLED(double BPM_avg)
{
  // Display BPM value section
  OLED.setCursor(80, 20); // กำหนดตำแหน่ง x,y ที่จะแสดงผล
  OLED.setTextSize(2); // กำหนดขนาดตัวอักษร

  OLED.setTextColor(WHITE , BLACK); //กำหนดข้อความสีดํา ฉากหลังสีขาว
  OLED.print("BPM:");

  OLED.setTextSize(1);
  OLED.setCursor(80, 40);
  OLED.print(BPM_avg);
  //OLED.drawLine(StartX, StartY, EndX, EndY, WHITE);
  OLED.drawLine(60, 0, 60, 64, WHITE);
  //  // Draw BPM Heart-bitmap picture section
  //
  //  //display.drawBitmap(Starting x pos, Starting y pos, Bitmap name, Width, Height, Color);
  //
  //  //draw smaller heart
  //   OLED.drawBitmap(15, 25, logo_Heart_24X21, 24, 21, WHITE);
  //
  //  //draw larger heart
  //  OLED.drawBitmap(12, 22, logo_Heart_32X32, 32, 32, WHITE);
  //
  //  //OLED.drawLine(StartX, StartY, EndX, EndY, WHITE);
  //  OLED.drawLine(60, 0, 60, 64, WHITE);

}
void getWarning_OLED()
{

  OLED.setCursor(30, 24); // กำหนดตำแหน่ง x,y ที่จะแสดงผล
  OLED.setTextSize(1); // กำหนดขนาดตัวอักษร

  OLED.setTextColor(WHITE , BLACK); //กำหนดข้อความสีดํา ฉากหลังสีขาว
  OLED.println("A finger is");
  OLED.setCursor(30, 44);
  OLED.println("not detected");

}

void setup() {
  pinMode(A5, INPUT);
  Serial.begin(baudRate);

  if (!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // สั่งให้จอ OLED เริ่มทำงานที่ Address 0x3C
    Serial.println("SSD1306 allocation failed");
  } else {
    Serial.println("ArdinoAll OLED Start Work !!!");
  }

  Serial.print("H");
  delay(500);
  Serial.print("F");
  delay(500);

  // for swap heart logo
  randomSeed(analogRead(A1));
  rand_index = random(dealyTime3_length);
  Serial.print("test first_randomNumber : "); 
  Serial.println(rand_index);
}

void loop() {

  OLED.clearDisplay(); // ลบภาพในหน้าจอทั้งหมด
  
  rand_index = random(dealyTime3_length); //random number for index , in order to swap bitmap picture

  // Get current time
  unsigned long currentMillis = millis();

  // First event calculate BPM
  if (myTimer1(delayTime, currentMillis) == 1 ) {

    reading = analogRead(0);

    // not attach finger
    if (reading < LowerThreshold )
    {
      IgnoreReading = true;
      getWarning_OLED();
    }

    //
    else if (reading > UpperThreshold )
    {
      IgnoreReading = false;

      // Calculate Beats Per Minute.
      // BPM = (1.0/reading) * 60.0 * 1000;
      heart_rates[count_index++] = (1.0 / reading) * 60.0 * 1000;

      if (count_index == heart_rate_length)
      {
        // find avg BPM
        int sum = 0;
        for (double heart_rate : heart_rates)
        {
          sum += heart_rate;
        }
        BPM_avg = (double)sum / heart_rate_length;
        count_index %= heart_rate_length;
      }

      // Display BPM in OLED
      getBPM_OLED(BPM_avg);

       // Draw BPM Heart-bitmap picture section
        //display.drawBitmap(Starting x pos, Starting y pos, Bitmap name, Width, Height, Color);
        if (active_smallerHeart)
        {
          active_smallerHeart = !(active_smallerHeart);
          active_largerHeart = !(active_largerHeart);

          //draw smaller heart
          OLED.drawBitmap(15, 25, logo_Heart_24X21, 24, 21, WHITE);
        }
      //use timer3 to swap bitmap picture (once time pass 1500 ms and a finger is detected)
      if (myTimer3(delayTime3[rand_index], currentMillis) == 1) {

        if (active_largerHeart)
        {
          active_smallerHeart = !(active_smallerHeart);
          active_largerHeart = !(active_largerHeart);
          //draw larger heart
          OLED.drawBitmap(12, 22, logo_Heart_32X32, 32, 32, WHITE);
        }

        rand_index = random(dealyTime3_length); //random number for index , in order to swap bitmap picture        
      }
    }
  }


  // Second event display section (in serial monitor)
  if (myTimer2(delayTime2, currentMillis) == 1 ) {

    if (!(IgnoreReading))
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
    else if (IgnoreReading)
    {
      Serial.println("a finger is not detected");
    }
  }
  OLED.display();// สั่งให้จอแสดงผล
}
