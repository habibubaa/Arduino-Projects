                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

const int place[]={10,9,8}; //inisialisasi pin digital untuk ldr, konstanta membuat nilai tidak dapat diubah pada program
int val[3]; //jumlah sensor ldr
int ldr1,ldr2,ldr3; //inisialisasi variabel untuk ldr 
int in=11; //letak digital sensor ldr ketika mobil masuk pada pin 11
int out=12; //letak digital sensor ldr ketika mobil keluar pada pin 12
int count; //variabel sementara untuk menghitung jumlah mobil
int valin=0; //variabel untuk membaca sensor ketika mobil masuk
int valout=0; //variabel untuk membaca sensor ketika mobil keluar
int y; //variabel sementara
int red1 = 2; //inisialisasi pin untuk LED
int red2 = 4;
int red3 = 6;
int green1 = 3;
int green2 = 5;
int green3 = 7;

void setup() {

  Serial.begin(9600); //set baud rate (kecepatan mengolah data)

  lcd.begin(16, 2); //nentuin lcd yg digunakan
  lcd.setCursor(0, 0); //posisi awal nulis
  lcd.print("Smart Parking"); //memunculkan string di lcd
  lcd.setCursor(0, 1);
  lcd.print("Hello!");
  delay(1000); //waktu delay
  lcd.clear(); //hapus semua tulisan di lcd
  count=0; //set count menjadi 0

  pinMode(red1, OUTPUT); //menginisialisasi bahwa pin led sebagai output
  pinMode(red2, OUTPUT);
  pinMode(red3, OUTPUT);
  pinMode(green1, OUTPUT);
  pinMode(green2, OUTPUT);
  pinMode(green3, OUTPUT);
}

void loop() {
    Serial.begin(9600);
    Serial.println(ldr1); //test di serial monitor
    delay(500);
    
    valin=digitalRead(in); //deklarasi variabel untuk pin sensor ldr hitung kendaraan
    valout=digitalRead(out);

    //ketika sensor pada pintu masuk mendeteksi ada kendaraan
    if (valin==0)
    {
      count++; //count akan bertambah
      if(count>=4){count=4;} //ketika count lebih dari sama dgn 4 maka count reset ke 3/full
    }
     
    //ketika sensor pada pintu masuk mendeteksi ada kendaraan
    if (valout==0)
    {
      count--; //count akan berkurang
      if(count<=0){count=0;} //ketika count kurang dari 0 maka count akan reset ke 0
    }

    ldr1 = digitalRead(8); //deklarasi variabel membaca sensor pada pin 8
    if(ldr1==0) //ketika nilai sensor ldr = 0
    {
      led1(0,HIGH); //menyalakan kaki warna hijau pada LED
    }
    else
    {
      led1(HIGH, 0); //menyalakan kaki warna merah pada LED
    } 


    ldr2 = digitalRead(9);
    if(ldr2==0)
    {
      led2(0,HIGH);
    }
    else
    {
      led2(HIGH, 0);
    } 


    ldr3 = digitalRead(10);
    if(ldr3==0)
    {
      led3(0,HIGH); //green
    }
      else
    {
      led3(HIGH, 0); //red
    } 


    lcd.setCursor(0, 0);
    lcd.print("P-L:");
    lcd.setCursor(0, 1);
  
    for(y=0;y<3;y++) //program mengulang hingga 
    {      
      val[y]=digitalRead(place[y]);
      if(val[y]==0)
      {
        lcd.print(y+1);
      }            
    }
    lcd.print("       ");     

//********************************************************       
    lcd.setCursor(10, 1);
    lcd.print(" cnt:");
    lcd.print(count);  
    if(count>=3)
    {
      lcd.setCursor(5, 1);lcd.print("       full   ");
    }
 }

void led1(int r1, int g1) //inisialisasi isi dari led1 sebagai r1 (kaki merah 1) dan g1 (kaki hijau 1)
{
  digitalWrite(red1,r1);  //mengisi red1 dengan nilai r1
  digitalWrite(green1,g1);  //mengisi green1 dengan nilai g1
}

void led2(int r2, int g2)
{
  digitalWrite(red2,r2);
  digitalWrite(green2,g2);}

void led3(int r3, int g3)
{
  digitalWrite(red3,r3);
  digitalWrite(green3,g3);
}

