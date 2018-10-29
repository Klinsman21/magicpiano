#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
#include <Arduino.h>
#include <Wire.h>
#include <MicroLCD.h>

LCD_SH1106 lcd;

SoftwareSerial DF(A1, A0); // RX, TX


const PROGMEM uint8_t nota[48 * 48 / 8] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xC0, 0xC0, 0x80, 0x80,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x3F,
  0x3F, 0x7E, 0xFC, 0xF0, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x03, 0x0F, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0xF8, 0xFC, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFC, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x07, 0x07,
  0x07, 0x07, 0x07, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#define teclaA 2
#define teclaB 3
#define teclaC 4
#define teclaD 5
#define teclaE 6
#define teclaF 7
#define teclaG 8
#define teclaH 9
#define teclaI 10
#define teclaJ 11
#define teclaK 12


int TONE[6][9] = {
  {30, 31, 32, 33, 34, 35, 36, 37, 38},//tone1
  {39, 40, 41, 42, 43, 44, 45, 46, 47},//tone2
  {48, 49, 50, 51, 52, 53, 54, 55, 56},//tone3
  {57, 58, 59, 60, 61, 62, 63, 64, 65},//tone4
  {66, 67, 68, 69, 70, 71, 72, 73, 74},//tone5
  {75, 76, 77, 78, 79, 80, 81, 82, 83},//tone6
};
int atualTone = 1;

void setup () {
  pinMode(teclaA, INPUT);
  pinMode(teclaB, INPUT);
  pinMode(teclaC, INPUT);
  pinMode(teclaD, INPUT);
  pinMode(teclaE, INPUT);
  pinMode(teclaF, INPUT);
  pinMode(teclaG, INPUT);
  pinMode(teclaH, INPUT);
  pinMode(teclaI, INPUT);
  pinMode(teclaJ, INPUT);
  pinMode(teclaK, INPUT);
  pinMode(13, OUTPUT);
  lcd.begin();
  Serial.begin (9600);
  DF.begin (9600);
  mp3_set_serial (DF);
  delay(1);
  mp3_set_volume (30);
  //lcd.setCursor(40, 1);

  //delay(2000);
  lcd.clear();

  lcd.setFontSize(FONT_SIZE_LARGE);
  lcd.setCursor(40, 2);
  lcd.draw(nota, 48, 48);
  lcd.setCursor(12, 0);
  lcd.print("Magic Piano");
  delay(1000);
  lcd.clear();
  lcd.setFontSize(FONT_SIZE_LARGE);
  lcd.setCursor(40, 3);
  String text = "TONE ";
  text += atualTone;
  lcd.print(text);

}


//
void loop () {
  analogWrite(teclaJ, 3);

  if (digitalRead(teclaK) == 1) {
    mp3_stop();
    if (atualTone == 7) {
      atualTone = 0;
    }
    delay(200);
    if (atualTone <= 6) {
      if (atualTone != 7) {
        atualTone++;
      }

      if (atualTone == 7) {
        lcd.clear();
        lcd.setFontSize(FONT_SIZE_MEDIUM);
        lcd.setCursor(40, 0);
        lcd.print("Musicas");
        lcd.setCursor(25, 3);
        lcd.print("Classicas");
      }
      else {
        lcd.clear();
        lcd.setFontSize(FONT_SIZE_LARGE);
        lcd.setCursor(40, 3);
        String text = "TONE ";
        text += atualTone;
        lcd.print(text);
      }

    }
  }

  switch (atualTone) {
    case 1:

      if (digitalRead(teclaA) == 1) {

        mp3_play (TONE[0][0]);

      }
      if (digitalRead(teclaB) == 1) {

        mp3_play (TONE[0][1]);

      }
      if (digitalRead(teclaC) == 1) {

        mp3_play (TONE[0][2]);

      }
      if (digitalRead(teclaD) == 1) {

        mp3_play (TONE[0][3]);

      }
      if (digitalRead(teclaE) == 1) {

        mp3_play (TONE[0][4]);

      }
      if (digitalRead(teclaF) == 1) {

        mp3_play (TONE[0][5]);

      }
      if (digitalRead(teclaG) == 1) {

        mp3_play (TONE[0][6]);

      }
      if (digitalRead(teclaH) == 1) {

        mp3_play (TONE[0][7]);

      }
      if (digitalRead(teclaI) == 1) {

        mp3_play (TONE[0][8]);

      }

      break;
    case 2:
      if (digitalRead(teclaA) == 1) {

        mp3_play (TONE[1][0]);

      }
      if (digitalRead(teclaB) == 1) {

        mp3_play (TONE[1][1]);

      }
      if (digitalRead(teclaC) == 1) {

        mp3_play (TONE[1][2]);

      }
      if (digitalRead(teclaD) == 1) {

        mp3_play (TONE[1][3]);

      }
      if (digitalRead(teclaE) == 1) {

        mp3_play (TONE[1][4]);

      }
      if (digitalRead(teclaF) == 1) {

        mp3_play (TONE[1][5]);

      }
      if (digitalRead(teclaG) == 1) {

        mp3_play (TONE[1][6]);

      }
      if (digitalRead(teclaH) == 1) {

        mp3_play (TONE[1][7]);

      }
      if (digitalRead(teclaI) == 1) {

        mp3_play (TONE[1][8]);

      }

      break;
    case 3:
      if (digitalRead(teclaA) == 1) {

        mp3_play (TONE[2][0]);

      }
      if (digitalRead(teclaB) == 1) {

        mp3_play (TONE[2][1]);

      }
      if (digitalRead(teclaC) == 1) {

        mp3_play (TONE[2][2]);

      }
      if (digitalRead(teclaD) == 1) {

        mp3_play (TONE[2][3]);

      }
      if (digitalRead(teclaE) == 1) {

        mp3_play (TONE[2][4]);

      }
      if (digitalRead(teclaF) == 1) {

        mp3_play (TONE[2][5]);

      }
      if (digitalRead(teclaG) == 1) {

        mp3_play (TONE[2][6]);

      }
      if (digitalRead(teclaH) == 1) {

        mp3_play (TONE[2][7]);

      }
      if (digitalRead(teclaI) == 1) {

        mp3_play (TONE[2][8]);

      }

      break;
    case 4:
      if (digitalRead(teclaA) == 1) {

        mp3_play (TONE[3][0]);

      }
      if (digitalRead(teclaB) == 1) {

        mp3_play (TONE[3][1]);

      }
      if (digitalRead(teclaC) == 1) {

        mp3_play (TONE[3][2]);

      }
      if (digitalRead(teclaD) == 1) {

        mp3_play (TONE[3][3]);

      }
      if (digitalRead(teclaE) == 1) {

        mp3_play (TONE[3][4]);

      }
      if (digitalRead(teclaF) == 1) {

        mp3_play (TONE[3][5]);

      }
      if (digitalRead(teclaG) == 1) {

        mp3_play (TONE[3][6]);

      }
      if (digitalRead(teclaH) == 1) {

        mp3_play (TONE[3][7]);

      }
      if (digitalRead(teclaI) == 1) {

        mp3_play (TONE[3][8]);

      }

      break;
    case 5:
      if (digitalRead(teclaA) == 1) {

        mp3_play (TONE[4][0]);

      }
      if (digitalRead(teclaB) == 1) {

        mp3_play (TONE[4][1]);

      }
      if (digitalRead(teclaC) == 1) {

        mp3_play (TONE[4][2]);

      }
      if (digitalRead(teclaD) == 1) {

        mp3_play (TONE[4][3]);

      }
      if (digitalRead(teclaE) == 1) {

        mp3_play (TONE[4][4]);

      }
      if (digitalRead(teclaF) == 1) {

        mp3_play (TONE[4][5]);

      }
      if (digitalRead(teclaG) == 1) {

        mp3_play (TONE[4][6]);

      }
      if (digitalRead(teclaH) == 1) {

        mp3_play (TONE[4][7]);

      }
      if (digitalRead(teclaI) == 1) {

        mp3_play (TONE[4][8]);

      }

      break;
    case 6:
      if (digitalRead(teclaA) == 1) {

        mp3_play (TONE[5][0]);

      }
      if (digitalRead(teclaB) == 1) {

        mp3_play (TONE[5][1]);

      }
      if (digitalRead(teclaC) == 1) {

        mp3_play (TONE[5][2]);

      }
      if (digitalRead(teclaD) == 1) {

        mp3_play (TONE[5][3]);

      }
      if (digitalRead(teclaE) == 1) {
        mp3_play (TONE[5][4]);

      }
      if (digitalRead(teclaF) == 1) {

        mp3_play (TONE[5][5]);

      }
      if (digitalRead(teclaG) == 1) {

        mp3_play (TONE[5][6]);

      }
      if (digitalRead(teclaH) == 1) {

        mp3_play (TONE[5][7]);

      }
      if (digitalRead(teclaI) == 1) {
        mp3_play (TONE[5][8]);

      }

      break;
    default:
      if (digitalRead(teclaA) == 1) {

        mp3_play (91);

      }
      if (digitalRead(teclaB) == 1) {

        mp3_play (92);

      }
      if (digitalRead(teclaC) == 1) {

        mp3_play (93);

      }
      if (digitalRead(teclaD) == 1) {

        mp3_play (94);

      }
      if (digitalRead(teclaE) == 1) {
        mp3_play (95);

        break;
      }
  }




      //for (int pos = 30; pos <= 90; pos += 1){
      //  mp3_set_volume (30);
      //  mp3_play (pos);
      //  delay (400);
      //}
  }
