//==============KHUSUS TES DISPLAY ALAT================//
#include <Wire.h>
#include "RTClib.h"
#include "U8glib.h"

//inisialisasi pin arah angin
#define utara 0
#define tl 1
#define timur 4
#define tenggara 5
#define selatan 6
#define bd 7
#define barat 8
#define bl 9

//inisialisasi pin SPI pada LCD ke pin arduino
U8GLIB_ST7920_128X64_4X u8g(13, 11, 10);  // SPI Com: SCK = en = 13, MOSI = rw = 11, CS = di/rs = 10

long durasiSerial = 0;
long durasiLed = 0;
long jedaText = 3000;
long jedaLed = 20000;
long jedaSerial = 21000;

//Real Time Clock
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};

//Optocoupler
int sensor_angin = A0;
int sensor_air = 12;
unsigned long start_time = 0;
unsigned long end_time = 0;

//angin init
int steps_angin=0;
float steps_old_angin=0;
float temp_angin=0;
float rps_angin=0;
float putar_angin=0;
float knot;

//air init
int steps_air=0;
float steps_old_air=0;
float temp_air=0;
float rps_air=0;
float putar_air=0;

//+++++++++++++=====================  D R A W  E N T R A N C E  L O G O  ===================================++++++++++//

void drawColorBox(void)
{
  u8g_uint_t w,h;
  u8g_uint_t r, g, b;
  
  w = u8g.getWidth()/32;
  h = u8g.getHeight()/8;
  for( b = 0; b < 4; b++ )
    for( g = 0; g < 8; g++ )
      for( r = 0; r < 8; r++ )
      {
        u8g.setColorIndex((r<<5) |  (g<<2) | b );
        u8g.drawBox(g*w + b*w*8, r*h, w, h);
      }
}

void drawLogo(uint8_t d)
{
#ifdef MINI_LOGO
    u8g.setFont(u8g_font_gdr17r);
    u8g.drawStr(0+d, 22+d, "Y");
    u8g.setFont(u8g_font_gdr20n);
    u8g.drawStr90(17+d,8+d,"8");
    u8g.setFont(u8g_font_gdr17r);
    u8g.drawStr(39+d,22+d,"l");
    
    u8g.drawHLine(2+d, 25+d, 34);
    u8g.drawVLine(32+d, 22+d, 12);
#else
    u8g.setFont(u8g_font_gdr12r);
    u8g.drawStr(0+d, 14+d, "Siparus & ");
    u8g.setFont(u8g_font_gdr12r);
    u8g.drawStr(32+d, 30+d, "Sipadangin");
    
    u8g.drawHLine(2+d, 35+d, 47);
    u8g.drawVLine(45+d, 32+d, 12);
#endif
}

void drawURL(void)
{
#ifndef MINI_LOGO
  u8g.setFont(u8g_font_4x6);
  if ( u8g.getHeight() < 59 )
  {
    u8g.drawStr(53,9,"code.google.com");
    u8g.drawStr(77,18,"/p/u8glib");
  }
  else
  {
    u8g.drawStr(1,52,"Sistem Pemantau Arus dan Sistem");
    u8g.drawStr(1,61,"Pemantau Angin.");
  }
#endif
}


void draw(void) {
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    drawColorBox();
  }
  u8g.setColorIndex(1);
  if ( U8G_MODE_GET_BITS_PER_PIXEL(u8g.getMode()) > 1 ) {
    drawLogo(2);
    u8g.setColorIndex(2);
    drawLogo(1);
    u8g.setColorIndex(3);
  }
  drawLogo(0);
  drawURL();
  
}

void tulis(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, 12, "Hello World!");
  u8g.drawStr( 0, 25, "Habib");
}

void draw2(void) {
  DateTime now = rtc.now();
  u8g.firstPage(); 
   do {
  u8g.setFont(u8g_font_7x13B);
  char hari[9];
  sprintf (hari, "%s,", daysOfTheWeek[now.dayOfTheWeek()]);
  u8g.drawStr(0, 11, hari);

  char tgl[9];
  sprintf (tgl, "%d/", now.day());
  u8g.drawStr(50, 11, tgl);
  
  char bln[9];
  sprintf (bln, "%d/", now.month());
  u8g.drawStr(74, 11, bln);

  char thn[9];
  sprintf (thn, "%d", now.year());
  u8g.drawStr(97, 11, thn);

  u8g.setFont(u8g_font_courB10);

  if(now.hour()< 10)
  {
  char jam[9];
  sprintf (jam, "0%d", now.hour());
  u8g.drawStr(30, 24, jam);
  }
  else
  {
  char jam[9];
  sprintf (jam, "%d", now.hour());
  u8g.drawStr(30, 24, jam);    
  }

  u8g.setPrintPos(47, 24);
  u8g.print(":");

  if(now.minute()< 10)
  {
  char mnt[9];
  sprintf (mnt, "0%d", now.minute());
  u8g.drawStr(55, 24, mnt);
  }
  else
  {
  char mnt[9];
  sprintf (mnt, "%d", now.minute());
  u8g.drawStr(55, 24, mnt);
  }

  u8g.setPrintPos(72, 24);
  u8g.print(":");

  if(now.second()< 10)
  {
  char dtk[9];
  sprintf (dtk, "0%d", now.second());
  u8g.drawStr(80, 24, dtk);
  }
  else
  {
  char dtk[9];
  sprintf (dtk, "%d", now.second());
  u8g.drawStr(80, 24, dtk);
  }
  
  u8g.drawHLine(0, 27, 150);
    
  u8g.setFont(u8g_font_lucasfont_alternate);
    
  u8g.setPrintPos(0, 38);
//  u8g.print("Wind speed : "); u8g.print(knot); u8g.print(" knot");
  u8g.print("Wind speed : "); u8g.print("0.00"); u8g.print(" knot");

  u8g.setPrintPos(0, 50);
  u8g.print("Flow speed : "); u8g.print(putar_air); u8g.print(" m/s");

  //============begin wind vane sensors===========//         
  if(digitalRead(utara)==LOW)
  {u8g.drawStr( 0, 62, "Arah Angin : UTARA");}
  else if(digitalRead(tl)==LOW)
  {u8g.drawStr( 0, 62, "Arah Angin : TIMUR LAUT");}
  else if(digitalRead(timur)==LOW)
  {u8g.drawStr( 0, 62, "Arah Angin : TIMUR");}
  else if(digitalRead(tenggara)==LOW)
  {u8g.drawStr( 0, 62, "Arah Angin : TENGGARA");}
  else if(digitalRead(selatan)==LOW)
  {u8g.drawStr( 0, 62, "Arah Angin : SELATAN");}
  else if(digitalRead(bd)==LOW)
  {u8g.drawStr( 0, 62, "Arah Angin : BARAT DAYA");}
  else if(digitalRead(barat)==LOW)
  {u8g.drawStr( 0, 62, "Arah Angin : BARAT");}
  else if(digitalRead(bl)==LOW)
  {u8g.drawStr( 0, 62, "Arah Angin : BARAT LAUT");}
 //============end of wind vane sensors===========//
        
    } while( u8g.nextPage() );
}

//+++++++++++++=====================  E N D  D R A W  E N T R A N C E  L O G O  ===================================++++++++++//



void setup() 
{
  //========begin sim a6=======//
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);

  //==========pin mode Sensor Optocoupler============//
//  pinMode(sensor_angin,INPUT_PULLUP);
  pinMode(sensor_air,INPUT_PULLUP);
  //==========end pin mode Sensor Optocoupler=========//

  //==========pin mode wind vane============//
  pinMode(utara,INPUT_PULLUP);
  pinMode(tl,INPUT_PULLUP);
  pinMode(timur,INPUT_PULLUP);
  pinMode(tenggara,INPUT_PULLUP);
  pinMode(selatan,INPUT_PULLUP);
  pinMode(bd,INPUT_PULLUP);
  pinMode(barat,INPUT_PULLUP);
  pinMode(bl,INPUT_PULLUP);
  //==========end pin mode wind vane=========//

  //======start RTC initial====//
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));     // following line sets the RTC to the date & time this sketch was compiled
    
     Set the current date, and time in the following format:
     rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));     // This line sets the RTC with an explicit date & time, for example to set
  }
  //====end RTC initial========//
  
}
 
void loop()
{ 

   //==============Start Measuring===============//
 start_time=millis();
 end_time=start_time+1000;
 while(millis()<end_time)
 {
   if(digitalRead(sensor_angin))
   {
    steps_angin=steps_angin+1; 
    while(digitalRead(sensor_angin));
   }
   if(digitalRead(sensor_air))
   {
    steps_air=steps_air+1; 
    while(digitalRead(sensor_air));
   }
 }
 
    temp_angin=steps_angin-steps_old_angin;
    steps_old_angin=steps_angin;
    rps_angin=(temp_angin);
    putar_angin=(rps_angin*2*3.14*0.01); //convert rpm to m/s
    knot=(putar_angin*0.514); //convert m/s to knot

    temp_air=steps_air-steps_old_air;
    steps_old_air=steps_air;
    rps_air=(temp_air);
    putar_air=(rps_air*2*3.14*0.01); //convert rpm to m/s

    Serial.print("RPS Angin - ");
    Serial.print(rps_angin);
    Serial.print("   ");    
    Serial.print(putar_angin);
    Serial.print(" m/s");
    Serial.print("   ");        
    Serial.print(knot);
    Serial.println(" knot");

    Serial.print("RPS Air - ");
    Serial.print(rps_air);
    Serial.print("   ");    
    Serial.print(putar_air);
    Serial.print(" m/s");
    Serial.println("   ");
  
 //============begin Display LCD===========//
    unsigned long curr = millis();

     if(curr - durasiSerial < jedaText)
      {
          u8g.firstPage();  
          do {
            draw();
          u8g.setColorIndex(1);
          } while( u8g.nextPage() );
      }
      if(curr - durasiSerial > jedaText)
       {
          u8g.firstPage();
          do {  
            draw2();    
          } while( u8g.nextPage() ); 
       }
       
      if(curr - durasiSerial > jedaSerial)
            {
              durasiSerial = curr;
            }
 //============End Display LCD===========//
}
