#include <Wire.h>
#include "DHT.h"
#define DHTPIN 11
#include <Adafruit_INA219.h>
#define BLYNK_TEMPLATE_ID "TMPLpxc0DHTL"
#define BLYNK_DEVICE_NAME "Uno"
#define BLYNK_AUTH_TOKEN "TN9z9UfrRILFx2yaVFa84flq2RUOML06"

Adafruit_INA219 ina219;

#define BLYNK_PRINT Serial
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

//definisi variabel
double Vbat, Vout, Vpanel;
double busvoltage = 0;
double current_mA = 0;
double power_mW = 0;
// int valPWM = 0;
// int PWMpin=3;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "David surya";
char pass[] = "davidsurya123";


// definisi pin koneksi ke ESP8266
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(7, 8); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 38400

//Inisiasi esp digunakan sebagai wifi shield
ESP8266 wifi(&EspSerial);

void setup()
{
  //membuat pin vcc dan gnd untuk dht11
  pinMode(10, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(10, HIGH);
  digitalWrite(12, LOW);
  
  // Debug console
  Serial.begin(115200);

  //Membuat koneksi dengan ESP8266
  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  
  //memulai sensor suhu dht11
  dht.begin();

  //memulai framework blynk
  Blynk.begin(auth, wifi, ssid, pass, "blynk.cloud", 80);
  
  //memulai sensor ina219
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) {
      delay(10);
    }
  }
  
}

void loop()
{
  //Menjalankan framework blynk
  Blynk.run();
  
  //mengambil nilai tegangan pv dari sensor ina219
  busvoltage = ina219.getBusVoltage_V();
  
  //mengambil nilai arus pv dari sensor ina219
  current_mA = ina219.getCurrent_mA() / 1000;
  
  //mengambil nilai daya pv dari sensor ina219
  power_mW = ina219.getPower_mW();
  
  //mengambil nilai tegangan baterai dari A1
  Vbat = analogRead(A1) / 40.12688;
  
  //mengambil nilai tegangan output buckboost dari A2
  Vout = 0.0409*analogRead(A2) + 0.9762;
  
  
  //mengatur nilai output buck boost dengan arduino//
  //=====================gagal=====================//
//   if (Vout > 15) {
//     if (valPWM > 0) {
//       valPWM++;
//       //      Serial.println("val++");
//     }
//   }
//   else {
//     if (valPWM < 255) {
//       valPWM--;
//       //     Serial.println("val--");
//     }
//   }
//   if (valPWM > 50) {
//     valPWM = 0;
//   }
//   analogWrite(PWMpin, valPWM);
  //===============================================//
  
  //mengambil nilai suhu dari sensor dht11
  float t = dht.readTemperature();
  
 //Memasukkan nilai ke aplikasi blynk
  Blynk.virtualWrite(V2, busvoltage);
  Blynk.virtualWrite(V3, current_mA);
  Blynk.virtualWrite(V4, power_mW/1000);
  Blynk.virtualWrite(V5, Vbat);
  Blynk.virtualWrite(V6, t);
  Blynk.virtualWrite(V7, Vout);
  Blynk.virtualWrite(V8, valPWM);

}
