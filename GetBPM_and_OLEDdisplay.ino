//For OLED libary
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//for buzzer
#define buzzer 2

//for note Nokia Tune
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0

// change this to make the song slower or faster
int tempo = 180;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {

  // Nokia Ringtone
  // Score available at https://musescore.com/user/29944637/scores/5266155

  NOTE_E5, 8, NOTE_D5, 8, NOTE_FS4, 4, NOTE_GS4, 4,
  NOTE_CS5, 8, NOTE_B4, 8, NOTE_D4, 4, NOTE_E4, 4,
  NOTE_B4, 8, NOTE_A4, 8, NOTE_CS4, 4, NOTE_E4, 4,
  NOTE_A4, 2,
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;


//-----------------------------------------------------------------------------------------------
//for OLED
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

const unsigned long delayTime3[4] = { 500, 800, 1100, 2000 }; // for swap heart logo
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

//play sound once heart logo is swapped
void sound_swapBitmapPicture()
{
  tone(buzzer, 440, 50);
  Serial.println("play sound alarm");
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

  //---------------------------------------------------------------------------------
  //play Nokia tone once program start
      // iterate over the notes of the melody.
      // Remember, the array is twice the number of notes (notes + durations)
      for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

        // calculates the duration of each note
        divider = melody[thisNote + 1];
        if (divider > 0) {
          // regular note, just proceed
          noteDuration = (wholenote) / divider;
        } else if (divider < 0) {
          // dotted notes are represented with negative durations!!
          noteDuration = (wholenote) / abs(divider);
          noteDuration *= 1.5; // increases the duration in half for dotted notes
        }

        // we only play the note for 90% of the duration, leaving 10% as a pause
        tone(buzzer, melody[thisNote], noteDuration * 0.9);

        // Wait for the specief duration before playing the next note.
        delay(noteDuration);

        // stop the waveform generation before the next note.
        noTone(buzzer);
      }

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

          //play sound 
          sound_swapBitmapPicture();
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
