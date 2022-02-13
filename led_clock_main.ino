#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <FastLED.h>
#include <avr/pgmspace.h>
#include <SimpleTimer.h>
#include <SoftwareSerial.h>


SoftwareSerial BTserial(5, 6); // RX | TX
SimpleTimer timer;

// How many leds in your strip?
#define NUM_LEDS 300
#define DATA_PIN 2

//ThreeWire myWire(6, 7, 8); // IO, SCLK, CE
//RtcDS1302<ThreeWire> Rtc(myWire);

CRGB leds[NUM_LEDS];
int currentLED = 0;

float LedR = random(0, 255);
float LedG = random(0, 255);
float LedB = random(0, 255);

unsigned long ms_from_start =0;
unsigned long ms_previous_read_LED1 = 0;

bool inProgress = false;

int currentRun = 0;

String command = "";
String tmpRead;

int startLED = 0;
int endLED = 100;



void setup() {


  Serial.begin(115200);
  BTserial.begin(9600);  
  randomSeed(analogRead(0));

  //LedR = random(0, 255);
  //LedG = random(0, 255);
  //LedB = random(0, 255);

  LedR = 255;
  LedG = 255;
  LedB = 255;

  
  delay(100);
  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.println(__TIME__);

  FastLED.addLeds<WS2811, DATA_PIN, BRG>(leds, NUM_LEDS);
  //FastLED.setBrightness(1000);

  #define BRIGHTNESS 255   /* Control the brightness of your leds */
#define SATURATION 255   /* Control the saturation of your leds */


}

void bloop() {

for (int j = startLED; j < endLED; j++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(i - (j * 2), SATURATION, BRIGHTNESS); /* The higher the value 4 the less fade there is and vice versa */ 
    }
    FastLED.show();
    delay(25); /* Change this to your hearts desire, the lower the value the faster your colors move (and vice versa) */
  }
  
}

void loop() {


    if (BTserial.available())
    {  
        tmpRead = BTserial.readString();
        Serial.println(tmpRead);

        if (tmpRead == "0") AllOff();
        if (tmpRead == "1") Trail1();
        if (tmpRead == "2") AllOn();

        if (tmpRead == "crveno") {
          LedR = 255;
          LedG = 0;
          LedB = 0;
          BTserial.println("Postavljam crvenu");
        }
        if (tmpRead == "plavo") {
          LedR = 0;
          LedG = 0;
          LedB = 255;
          BTserial.println("Postavljam plavu");
        }
        if (tmpRead == "zeleno") {
          LedR = 0;
          LedG = 255;
          LedB = 0;
          BTserial.println("Postavljam zelenu");
        }
        
        if (tmpRead.startsWith("b") && tmpRead.endsWith("e")) {
          BTserial.println("Postavljam plavu na " + tmpRead.substring(1,4));
          LedB = tmpRead.substring(1,4).toInt();
          BTserial.println("Boja je sada - " + String(LedR) + ", " + String(LedG) + ", " + String(LedB));
        }
        if (tmpRead.startsWith("r") && tmpRead.endsWith("e")) {
          BTserial.println("Postavljam crvenu na " + tmpRead.substring(1,4));
          LedR = tmpRead.substring(1,4).toInt();
          BTserial.println("Boja je sada - " + String(LedR) + ", " + String(LedG) + ", " + String(LedB));
        }
        if (tmpRead.startsWith("g") && tmpRead.endsWith("e")) {
          BTserial.println("Postavljam zelenu na " + tmpRead.substring(1,4));
          LedG = tmpRead.substring(1,4).toInt();
          BTserial.println("Boja je sada - " + String(LedR) + ", " + String(LedG) + ", " + String(LedB));
        }
        if (tmpRead.startsWith("s") && tmpRead.endsWith("e")) {
          BTserial.println("Postavljam pocetnu na " + tmpRead.substring(1,4));
          startLED = tmpRead.substring(1,4).toInt();
        }
        if (tmpRead.startsWith("e") && tmpRead.endsWith("e")) {
          BTserial.println("Postavljam zavrsnu na " + tmpRead.substring(1,4));
          endLED = tmpRead.substring(1,4).toInt();
        }

    }

    if (Serial.available())
    {
        BTserial.write(Serial.read());
    }




  
}

void AllOn() {

    for (int i = startLED; i <= endLED; i++) {
      leds[i] = CRGB(LedR, LedG, LedB);
  }
FastLED.show();
}

void AllOff() {
    for (int i = startLED; i <= endLED; i++) {
      leds[i] = CRGB(0, 0, 0);
  }
FastLED.show();
  
}

void Trail1() {
  
    
    for (int i = startLED; i <= endLED; i++) {
      currentLED = i;
      SetLEDsTrail();
    }

}

void ChangeColor() {
    LedR = random(0, 255);
    LedG = random(0, 255);
    LedB = random(0, 255);
}


void SetLEDsTrail() {

  for (int i = startLED; i <= endLED; i++) {
      
      leds[i] = CRGB(0, 0, 0);
      if (i == currentLED) leds[i] = CRGB(LedR, LedG, LedB);
      if (i == currentLED-1) leds[i] = CRGB(LedR*0.75, LedG*0.75, LedB*0.75);
      if (i == currentLED-2) leds[i] = CRGB(LedR*0.5, LedG * 0.5, LedB * 0.5);
      if (i == currentLED-3) leds[i] = CRGB(LedR*0.25, LedG * 0.25, LedB * 0.25);

  }

      FastLED.show();

  
}
