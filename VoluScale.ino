// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 5, en = 7, d4 = 4, d5 = 6, d6 = 8, d7 = 9;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#include "HX711.h"

HX711 scale;

uint8_t dataPin = 3;
uint8_t clockPin = 2;

float dichten[] = {
  1.0f,
  1.040f,
  1.042f,
  1.054f,
  1.02353f,	
  1.01968f,
  1.00813f,
  1.00043f,
  1.064f,
  1.036f,
  1.023f,
  0.919f,
  1.029f,
  1.067f,
  1.077f,
  1.008f,
  1.157f,
  
};
enum {
  Produkt_Wasser,
  Produkt_Milch_35,
  Produkt_Milch_15,
  Produkt_Buttermilch,
  Produkt_Sahne_10,
  Produkt_Sahne_15,
  Produkt_Sahne_30,
  Produkt_Sahne_40,
  Produkt_Hafermilch,
  Produkt_Sojamilch,
  Produkt_Mandelmilch,
  Produkt_Sonnenblumenoel, 
  Produkt_Delikatessbruehe,
  Produkt_Zitronensaft,
  Produkt_Multifitaminsaft,
  Produkt_Wein,
  Produkt_Eierlikoer,
  Produktanzahl,
};

String produkte[] = {
  "Wasser",
  "Milch 35%",
  "Milch 15%",
  "Buttermilch",
  "Sahne 10%",
  "Sahne 15%",
  "Sahne 30%",
  "Sahne 40%",
  "Hafermilch",
  "Sojamilch",
  "Mandelmilch",
  "Sonnenblumenoel",
  "Delikatessbruehe",
  "Zitronensaft",
  "Multifitaminsaft",
  "Wein",
  "Eierlikoer",
  
};

 int produkt = Produkt_Milch_35;

unsigned long tsz = 0;

float pt = 0.0f;

byte Steffen[8] = {
	0b00100,
	0b01110,
	0b01110,
	0b00100,
	0b01110,
	0b00100,
	0b00100,
	0b01010
};

 byte Uhr[8] = {
  0b00000,
  0b01110,
  0b11001,
  0b10111,
  0b10001,
  0b01110,
  0b00000,
  0b00000
};

byte Gewicht[8] = {
  0b00000,
  0b01110,
  0b00100,
  0b01110,
  0b01110,
  0b01110,
  0b00000,
  0b00000
  };

byte Volumen[8] = {
  0b00000,
  0b10001,
  0b11101,
  0b10001,
  0b11101,
  0b10001,
  0b01110,
  0b00000
};

byte Logo_R[8] = {
  0b01110,
  0b10001,
  0b10001,
  0b10001,
  0b01010,
  0b11011,
  0b00000,
  0b11111
};

byte Summenzeichen[8] = {
	0b11111,
	0b10001,
	0b01000,
	0b00100,
	0b01000,
	0b10001,
	0b11111,
	0b00000
};

  byte Pfeil[8] = {
	0b00000,
	0b00100,
	0b00110,
	0b11111,
	0b00110,
	0b00100,
	0b00000,
	0b00000
};

enum {
 Symbol_Steffen,
 Symbol_Uhr,
 Symbol_Gewicht,
 Symbol_Volumen,
 Symbol_Logo_R,
 Symbol_Summenzeichen,
 Symbol_Pfeil,
};

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  lcd.createChar(Symbol_Steffen, Steffen);
  lcd.createChar(Symbol_Uhr, Uhr);
  lcd.createChar(Symbol_Gewicht, Gewicht);
  lcd.createChar(Symbol_Volumen, Volumen);
  lcd.createChar(Symbol_Logo_R, Logo_R);
  lcd.createChar(Symbol_Summenzeichen, Summenzeichen);
  lcd.createChar(Symbol_Pfeil, Pfeil);

 // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  // Print a message to the LCD.
  lcd.setCursor(5, 1);
  lcd.print("VoluScale");
  lcd.setCursor(0, 2);
  lcd.print("by Madeleine Muessig");
  lcd.setCursor(9, 3);
  lcd.write(Symbol_Logo_R);

  

  scale.begin(dataPin, clockPin);

  scale.set_offset(4294855874);
  scale.set_scale(406.865478);
  scale.tare();

}

// the loop routine runs over and over again forever:
void loop() {

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):



  unsigned long SekSeitStart = millis()/1000;

  float AktGew = scale.get_units(4);

  float ProGew = AktGew - pt;

  // read the input on analog pin 0:
  int x = analogRead(A5);
  // print out the value you read:

  lcd.clear();

  if (x>640&&x<700){
    lcd.setCursor(12, 1);
    lcd.print("Produkt-");
    produkt=produkt-1;
    if (produkt<0){
      produkt=Produktanzahl-1;
    }
  }

  if (x>140&&x<200) {
    lcd.setCursor(12, 1);
    lcd.print("Produkt+");
     produkt=produkt+1;
    produkt=produkt%Produktanzahl;
  }

  if (x>60&&x<110) {
    lcd.setCursor(10, 1);
    lcd.print("Behaelter-");
    lcd.setCursor(16, 2);
    lcd.print("Tara");
    scale.tare();
  }

  if (x>470&&x<530) {
    lcd.setCursor(12, 1);
    lcd.print("Produkt-");
    lcd.setCursor(16, 2);
    lcd.print("Tara");
    pt = AktGew;
  }

  if (x>300&&x<340) {
    lcd.setCursor(14, 1);
    lcd.print("Timer-");
    lcd.setCursor(12, 2);
    lcd.print("neustart");
    tsz = SekSeitStart;
  }

  lcd.setCursor(19, 3);
  lcd.write(Symbol_Logo_R);

if (ProGew>41.9f&&ProGew<43.0f){
  lcd.setCursor(18, 3);
  lcd.write(Symbol_Steffen);
}
  
  lcd.setCursor(0, 0);
  lcd.write(Symbol_Gewicht);
  lcd.setCursor(2, 0);
  lcd.print(ProGew);
  lcd.print("g");
  
  lcd.setCursor(0, 1);
  lcd.write(Symbol_Volumen);
  lcd.setCursor(2, 1);
  lcd.print(ProGew/dichten[produkt]);
  lcd.print("ml");

  unsigned long tz = SekSeitStart - tsz;

  lcd.setCursor(0, 2);
  lcd.write(Symbol_Uhr);
  lcd.setCursor(2, 2);
  lcd.print(tz/60);
  lcd.print("min ");
  lcd.print(tz%60);
  lcd.print("sek");

  lcd.setCursor(0, 3);
  lcd.write(Symbol_Pfeil);
  lcd.setCursor(2, 3);
  lcd.print(produkte[produkt]);

  lcd.setCursor(12, 0);
  lcd.write(Symbol_Summenzeichen);
  lcd.setCursor(14, 0);
  lcd.print(round(AktGew));
  lcd.print("g");

  delay(200);  // delay in between reads for stability
}
