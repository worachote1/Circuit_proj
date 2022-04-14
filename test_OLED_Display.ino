#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // pixel ความกว้าง
#define SCREEN_HEIGHT 64 // pixel ความสูง 


#define OLED_RESET -1 
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

long var = 0; 

void setup()
{
    pinMode(A5, INPUT);
    pinMode(8,OUTPUT);
    Serial.begin(9600);

    if (!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // สั่งให้จอ OLED เริ่มทำงานที่ Address 0x3C
    Serial.println("SSD1306 allocation failed");
  } else {
    Serial.println("ArdinoAll OLED Start Work !!!");
  }
}

long t = 44;
void loop()
{
    OLED.clearDisplay(); // ลบภาพในหน้าจอทั้งหมด
    OLED.setTextColor(WHITE, BLACK);  //กำหนดข้อความสีขาว ฉากหลังสีดำ
    OLED.setCursor(0, 0); // กำหนดตำแหน่ง x,y ที่จะแสดงผล
    OLED.setTextSize(2); // กำหนดขนาดตัวอักษร
    OLED.println("OLED"); // แสดงผลข้อความ ALL
  
    OLED.setTextSize(1);
    OLED.println("welcome to All");
  //  OLED.println(" All");
    OLED.setTextColor(BLACK, WHITE); //กลับสีข้อความกับพื้นหลัง
    OLED.print("www.arduinoall.com");
    OLED.setTextColor(WHITE, BLACK);
    OLED.setCursor(60, 0);
    OLED.setTextSize(2);
    OLED.println((var++));
  
    OLED.setCursor(0, 40);
    OLED.setTextSize(2);
    OLED.println("ArduinoALL"); // แสดงผลข้อความ ALL
  
  
    OLED.display(); // สั่งให้จอแสดงผล
    var++;
    delay(500);
}
