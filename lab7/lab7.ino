#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// animatiile omuletului
byte s0[8] = {B00100,B01110,B00100,B01110,B00100,B01010,B10001,B00000};
byte s1[8] = {B00100,B01110,B00100,B01100,B00110,B01010,B01001,B00000};
byte s2[8] = {B00100,B01110,B00100,B01110,B00100,B10001,B01010,B00000};
byte s3[8] = {B00100,B01110,B00100,B00110,B01100,B01010,B10010,B00000};
byte fum[8] = {B00000,B00000,B00100,B00000,B00000,B00100,B00000,B00000};

int coloana = -1;
int linie = 1;
int anim = 0;

unsigned long tAnim = 0;
unsigned long tMisc = 0;
unsigned long viteza = 120;
unsigned long vitezaAnim = 90;

bool pauza = false;
unsigned long ultimButon = 0;

int citireButon() {
  int v = analogRead(A0);
  if (v < 50) return 0;
  if (v < 200) return 1;
  if (v < 400) return 2;
  if (v < 600) return 3;
  if (v < 800) return 4;
  return -1;
}

void afisHeader() {
  lcd.setCursor(0, 0);
  lcd.print("v=");
  lcd.print(viteza);
  lcd.print(pauza ? " STOP" : " RUN ");
}

void setup() {
  lcd.begin(16, 2);
  lcd.clear();

  lcd.createChar(0, s0);
  lcd.createChar(1, s1);
  lcd.createChar(2, s2);
  lcd.createChar(3, s3);
  lcd.createChar(4, fum);

  afisHeader();
}

void butoane() {
  int b = citireButon();

  if (b == 1 && viteza > 30) {
    viteza -= 10;
    afisHeader();
  }

  if (b == 2 && viteza < 400) {
    viteza += 10;
    afisHeader();
  }

  if (b == 4) {
    unsigned long t = millis();
    if (t - ultimButon > 200) {
      pauza = !pauza;
      ultimButon = t;
      afisHeader();
    }
  }
}

void loop() {
  butoane();
  if (pauza) return;

  unsigned long t = millis();

  // schimbare frame animatie
  if (t - tAnim >= vitezaAnim) {
    tAnim = t;
    anim++;
    if (anim > 3) anim = 0;
  }

  // miscare pe ecran
  if (t - tMisc >= viteza) {
    tMisc = t;

    if (coloana >= 0 && coloana < 16) {
      lcd.setCursor(coloana, linie);
      lcd.write((uint8_t)4);
    }

    if (coloana - 2 >= 0 && coloana - 2 < 16) {
      lcd.setCursor(coloana - 2, linie);
      lcd.print(" ");
    }

    coloana++;

    if (coloana >= 16) {
      for (int i = 0; i < 16; i++) {
        lcd.setCursor(i, linie);
        lcd.print(" ");
      }
      coloana = -1;
      linie = (linie == 0 ? 1 : 0);
    }

    if (coloana >= 0 && coloana < 16) {
      lcd.setCursor(coloana, linie);
      lcd.write((uint8_t)anim);
    }
  }
}