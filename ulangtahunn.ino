#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

#define buzzer 11

char waktu[9];
char angka[15];

// Ubah di sini untuk mengubah waktu ultah
const int ultahMenit   = 10;     // ubah untuk jam
const int ultahJam     = 17;     // ubah untuk menit
const int ultahTanggal = 8;     // ubah untuk tanggal
const int ultahBulan   = 2;     // ubah untuk bulan

/*NOTED
TAHUN DAN DETIK NGGA USAH DIUBAH KARENA 
NANTI NGGA BISA DIPUTAR SAAT TAHUN DEPAN
*/

uint8_t play = false;
int akhir = -1;

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(buzzer, OUTPUT);
  if (! rtc.begin()) {
    while (1) delay(10);
  }

  //rtc.adjust(DateTime(tahun, bulan, tanggal, jam, menit, detik)); // -> manual
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // -> Setup Awal
  if (rtc.lostPower()) {
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // -> otomatis
}

}

void loop() {
  DateTime now = rtc.now();

  int tanggal  = now.day();
  int bulan    = now.month();
  int tahun    = now.year();
  int detik    = now.second();
  int menit    = now.minute();
  int jam      = now.hour();
  
  // Reset tiap tahun
  if(now.day() != akhir){
    play = false;
    akhir = now.day();
  }

  // TAMPILAN JAM
  lcd.setCursor(0, 0);
  lcd.print("JAM -> ");
  sprintf(waktu, "%02d:%02d:%02d",
          jam,
          menit,
          detik
          );
  lcd.print(waktu);

  // TAMPILAN TANGGAL
  lcd.setCursor(0, 1);
  lcd.print("TGL -> ");
  sprintf(angka, "%d-%d-%d", 
          tanggal, 
          bulan, 
          tahun
          );
  lcd.print(angka);

  if(jam        == ultahJam      
     && menit   == ultahMenit
     && tanggal == ultahTanggal
     && bulan   == ultahBulan
     && !play){

    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("HEYY!! SELAMAT"); 
    lcd.setCursor(2, 1);
    lcd.print("ULANG TAHUN!");
    mainkan();
    delay(300);
    mainkan();
    delay(1000);
    lcd.clear();

    play = true;
  }
}

void mainkan() {
  // Nada
  int melody[] = {
    262, 262, 294, 262, 349, 330, 0,
    262, 262, 294, 262, 392, 349, 0,
    262, 262, 523, 440, 349, 330, 294, 0,
    466, 466, 440, 349, 392, 349
  };

  // Durasi
   int duration[] = {
    200, 200, 400, 400, 400, 800, 200,
    200, 200, 400, 400, 400, 800, 200,
    200, 200, 400, 400, 400, 400, 800, 200,
    200, 200, 400, 400, 400, 400
  };


  int totalNotes = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < totalNotes; i++) {
    if (melody[i] == 0) {
      noTone(buzzer);
    } else {
      tone(buzzer, melody[i]);
    }
    delay(duration[i]);
    noTone(buzzer);
    delay(50);
    
  }
}
