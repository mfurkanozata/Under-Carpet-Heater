#include <Arduino.h>

const byte dugme1 = 2,
           isitici1 = 4,
           sensor1 = A1,
           dugme2 = 8,
           isitici2 = 9,
           sensor2 = A2,
           kirmizi1 = 5,
           sari1 = 6,
           yesil1 = 7,
           kirmizi2 = 5,
           sari2 = 6,
           yesil2 = 7;
unsigned long lastTime_ilik = 0;
unsigned long lastTime = 0;
volatile boolean dugme1acik = false;
const int min_sicaklik = 38;
const int orta_sicaklik = 50;
const int maks_sicaklik = 55;
const int tepkiyoksa = 1;
const int dinlenme = 15;
const int tepkiyoksa_ms = 900000;
const int dinlenme_ms = dinlenme * 1000 * 60;

void setup()
{
  Serial.begin(9600);
  pinMode(dugme1, INPUT);
  pinMode(kirmizi1, OUTPUT);
  pinMode(sari1, OUTPUT);
  pinMode(yesil1, OUTPUT);
  pinMode(isitici1, OUTPUT);
  pinMode(dugme2, INPUT);
  pinMode(kirmizi2, OUTPUT);
  pinMode(sari2, OUTPUT);
  pinMode(yesil2, OUTPUT);
  pinMode(isitici2, OUTPUT);
  //    attachInterrupt(digitalPinToInterrupt(2), YuksekSicaklik1, RISING);
}

void loop()
{

  double sicaklik = Termistor();
  Serial.print(sicaklik);
  Serial.print("\n");
  if (digitalRead(dugme1) == 1 or dugme1acik == true)

  {
    dugme1acik = false;
    lastTime = 0;

    while (int(sicaklik) > 0 && int(sicaklik) < maks_sicaklik)
    {

      sicaklik = Termistor();
      Serial.print("sistem çalışıyor");
      Serial.print("\n");
      Serial.print(sicaklik);
      Serial.print("\n");
      isitici1Ac();
      kirmiziYak();
      delay(360000);
    }

    while (int(sicaklik) > orta_sicaklik or int(sicaklik) < 0)
    {

      sariYak();
      sicaklik = Termistor();
      Serial.print("dinlenme modu");
      Serial.print("\n");
      isitici1Kapat();
      while (int(sicaklik) < 0)
      {
        sariYak();
        delay(1000);
        sariSondur();
        delay(1000);
        sicaklik = Termistor();
      }
    }
    lastTime = millis();
    sariSondur();
  }
  else
  {
    yesilYak();
    if (millis() - lastTime < 900000)
    {

      if (int(sicaklik) > 0 && int(sicaklik) < min_sicaklik)
      {

        Serial.print("ılık tutma açık");
        Serial.print("\t");
        Serial.print(sicaklik);
        Serial.print("\t");
        Serial.print((sicaklik - int(sicaklik)));
        Serial.print("\n");

        isitici1Ac();
        if (digitalRead(dugme1) == 1)
        {
          dugme1acik = true;
          kirmiziYak();
        }
      }
      if (int(sicaklik) > orta_sicaklik or int(sicaklik) < 0)
      {

        Serial.print("ılık tutma kapalı");
        Serial.print("\n");
        Serial.print(sicaklik);
        Serial.print("\t");
        Serial.print((sicaklik - int(sicaklik)));
        Serial.print("\n");
        isitici1Kapat();
        while (int(sicaklik) < 0)
        {
          sariYak();
          delay(1000);
          sariSondur();
          delay(1000);
          sicaklik = Termistor();
        }
      }
    }
    else
    {
      isitici1Kapat();
      Serial.print("sistem kapalı | çalışmaya hazır");
      Serial.print("\n");
    }
  }
}

void kirmiziYak()
{
  digitalWrite(yesil1, 0);
  digitalWrite(sari1, 0);
  digitalWrite(kirmizi1, 1);
}
void kirmiziSondur()
{
  digitalWrite(kirmizi1, 0);
}
void sariYak()
{
  digitalWrite(yesil1, 0);
  digitalWrite(sari1, 1);
  digitalWrite(kirmizi1, 0);
}
void sariSondur()
{
  digitalWrite(sari1, 0);
}

void yesilYak()
{
  digitalWrite(yesil1, 1);
  digitalWrite(sari1, 0);
  digitalWrite(kirmizi1, 0);
}
void yesilSondur()
{
  digitalWrite(yesil1, 0);
}
void isitici1Ac()
{
  digitalWrite(isitici1, 1);
  digitalWrite(isitici2, 1);
}
void isitici1Kapat()
{
  digitalWrite(isitici1, 0);
  digitalWrite(isitici2, 0);
}

double Termistor()
{
  double geneltoplam_sicaklik;
  double sicaklik;
  double analogOkuma;
  for (int i = 0; i <= 10; i++)
  {
    analogOkuma = analogRead(sensor1);
    sicaklik = log(((10240000 / analogOkuma) - 10000));
    sicaklik = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * sicaklik * sicaklik)) * sicaklik);
    sicaklik = sicaklik - 273.15;
    geneltoplam_sicaklik+=sicaklik;
  }
  return geneltoplam_sicaklik/11;
}