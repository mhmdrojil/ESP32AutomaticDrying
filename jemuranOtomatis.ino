#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6yb0x_qcl"
#define BLYNK_TEMPLATE_NAME "Jemuran Otomatis"
#define BLYNK_AUTH_TOKEN "0_2VMdPs-7YPmwaXv9VYwkDLgIc8BUcg"

#define dhtpin 5 //D5
#define dhttype DHT11

#define limitSpeed 200 //kecepatan
#define limitMotor 2000 //waktu
#define motor A5 //D33
#define maju 18 //D19
#define mundur 19 //D18

#define maxCerah 5
#define maxHujan 20

#define maxGelap 20
#define maxTerang 50

#define maxDingin 20
#define maxPanas 30

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

char ssid[] = "Vivov20se";
char pass[] = "akudewee";
const int ldrPin = A0; //VP
const int rainPin = A6; //D34
float outputTerakhir;
bool stop = false;
String hasil = "";

WidgetLCD lcd(V0);
DHT dht(dhtpin, dhttype);

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(maju, OUTPUT); digitalWrite(maju, LOW);
  pinMode(mundur, OUTPUT); digitalWrite(mundur, LOW);
  dht.begin(); analogReadResolution(12);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop(){
  Blynk.run(); lcd.clear();
  int rain = map(analogRead(rainPin), 4096, 0, 0, 100);
  int cahaya = map(analogRead(ldrPin), 4096, 0, 0, 100);
  float suhu = dht.readTemperature();
  float output = defuzzify(rain, cahaya, suhu);
  
  tampilkanKeBlynk(rain, cahaya, suhu, output);
  Serial.print("Rain : "); Serial.print(rain); 
  Serial.print("\tCahaya : "); Serial.print(cahaya);
  Serial.print("\tSuhu : "); Serial.print(suhu);
  Serial.print("\tOutput : "); Serial.print(output);
  Serial.print("\t");

  if(output == 1){
    hasil = "Jemur";
    Serial.print(hasil); 
    lcd.print(9, 1, hasil);
  } else if (output == 0){
    hasil = "Angkat";
    Serial.print(hasil); 
    lcd.print(9, 1, hasil);
  }

  if(outputTerakhir != output){
    stop = false;
    if(output == 1){
      aksiJemur();
    } else if (output == 0){
      aksiAngkat(); 
    }
  }
  outputTerakhir = output;
  Serial.println();
  delay(1000);
}


void aksiJemur(){
  analogWrite(motor, limitSpeed);
  digitalWrite(maju, HIGH);
  digitalWrite(mundur, LOW);
  delay(limitMotor);
  berhenti();
}

void aksiAngkat(){
  analogWrite(motor, limitSpeed);
  digitalWrite(maju, LOW);
  digitalWrite(mundur, HIGH);
  delay(limitMotor);
  berhenti();
}

void berhenti(){
  analogWrite(motor, 0);
  digitalWrite(maju, LOW);
  digitalWrite(mundur, LOW);
  stop = true;
}

void tampilkanKeBlynk(int rain, int cahaya, float suhu, float output){
  String statusRain = "Cerah";
  String statusCahaya ="Gelap";
  String statusSuhu = "Dingin";
  Blynk.virtualWrite(V1, rain);
  Blynk.virtualWrite(V2, cahaya);
  Blynk.virtualWrite(V3, suhu);
  Blynk.virtualWrite(V4, output);
  if(hujan(rain)){
    statusRain = "hujan";
  }
  
  if(normal(cahaya)){
    statusCahaya = "Normal";
  } else if(terang(cahaya)){
    statusCahaya = "Terang";
  }

  if(hangat(suhu)){
    statusSuhu = "Hangat";
  } else if(panas(suhu)){
    statusSuhu = "Panas";
  }

  lcd.print(0, 0, statusRain);
  lcd.print(9, 0, statusCahaya);
  lcd.print(0, 1, statusSuhu);
}

float defuzzify(int rain, int cahaya, float suhu){
  if(cerah(rain) && terang(cahaya)){ // rule 1
    return 1;
  } else if(cerah(rain) && terang(cahaya) && hangat(suhu)){ // rule 2
    return 1;
  } else if(hujan(rain) && gelap(cahaya) && dingin(suhu)){ //rule 3
    return 0;
  } else if(cerah(rain) && normal(cahaya) && hangat(suhu)){ //rule 4
    return 1;
  } else if(hujan(rain) && terang(cahaya) && hangat(suhu)){ //rule 5
    return 0;
  } else if(cerah(rain) && gelap(cahaya)){ //rule 6
    return 0;
  }
  return 0;
}

bool cerah(float x){
  return x < maxHujan;
}

bool hujan(float x){
  return x >= maxHujan;
}

bool gelap(float x){
  return x <= maxGelap;
}

bool normal(float x){
  return x > maxGelap && x < maxTerang;
}

bool terang(float x){
  return x >= maxTerang;
}

bool dingin(float x){
  return x <= maxDingin;
}

bool hangat(float x){
  return x > maxDingin && x < maxPanas;
}

bool panas(float x){
  return x >= maxPanas;
}