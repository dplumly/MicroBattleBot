#include <Adafruit_TiCoServo.h>
#include <known_16bit_timers.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

/*
 Motor    = D5 
 Neopixel = D6
 Servo    = D9
 RC       = D10
 */

// Servo
Adafruit_TiCoServo myservo;
int pos = 0;


// Neopixel
#define PIN 6
#define Pixels 16
#define BG 1
Adafruit_NeoPixel strip = Adafruit_NeoPixel(Pixels, PIN, NEO_GRB + NEO_KHZ800);

// DC MOTOR
  int motor = 5;


void setup() {
  Serial.begin(9600);
  myservo.attach(9);
  strip.begin();
  strip.show();

  // PINMODES
  pinMode(10, INPUT);
  pinMode(pos, OUTPUT);
  pinMode(motor, OUTPUT);
}

void loop() {
  // BUTTON
  int chan5; // channel from RC receiver

  //controlState = digitalRead(controlPin);

  chan5 = pulseIn(10, HIGH, 20000);
  Serial.print("Channel 5:");
  Serial.println(chan5);
  delay(100);


  if (chan5 < 1300 ) {
    battleMode();
  }
  else {
    attackMode();
  }
}



// Battlebot functions
/////////////////////////////////////////////////
void battleMode() {
  digitalWrite(motor, LOW);
  myservo.write(60);  // set servo to mid-poin
  battleLights();
}

void attackMode() {
  digitalWrite(motor, HIGH);
  myservo.write(0);  // set servo to mid-point
  attackLights(strip.Color(255, 0, 0), 50);
}


// Neopixel Animations
/////////////////////////////////////////////////

void attackLights(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }

  for (int j = 0; j < 30; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}


void battleLights() {

  // BattleLights
  int color;
  int center = 0;
  int step = -1;
  int maxSteps = 16;
  float fadeRate = 0.6;
  int diff;
  uint32_t currentBg = random(256);
  uint32_t nextBg = currentBg;

  if (BG) {
    if (currentBg == nextBg) {
      nextBg = random(256);
    }
    else if (nextBg > currentBg) {
      currentBg++;
    } else {
      currentBg--;
    }
    for (uint16_t l = 0; l < Pixels; l++) {
      strip.setPixelColor(l, Wheel(currentBg, 0.1));
    }
  } else {
    for (uint16_t l = 0; l < Pixels; l++) {
      strip.setPixelColor(l, 0, 0, 0);
    }
  }


  if (step == -1) {
    center = random(Pixels);
    color = random(256);
    step = 0;
  }



  if (step == 0) {
    strip.setPixelColor(center, Wheel(color, 1));
    step ++;
  }
  else {
    if (step < maxSteps) {
      strip.setPixelColor(wrap(center + step), Wheel(color, pow(fadeRate, step)));
      strip.setPixelColor(wrap(center - step), Wheel(color, pow(fadeRate, step)));
      if (step > 3) {
        strip.setPixelColor(wrap(center + step - 3), Wheel(color, pow(fadeRate, step - 2)));
        strip.setPixelColor(wrap(center - step + 3), Wheel(color, pow(fadeRate, step - 2)));
      }
      step ++;
    }
    else {
      step = -1;
    }
  }

  strip.show();
  delay(50);
}


int wrap(int step) {
  if (step < 0) return Pixels + step;
  if (step > Pixels - 1) return step - Pixels;
  return step;
}


uint32_t Wheel(byte WheelPos, float opacity) {

  if (WheelPos < 85) {
    return strip.Color((WheelPos * 3) * opacity, (255 - WheelPos * 3) * opacity, 0);
  }
  else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color((255 - WheelPos * 3) * opacity, 0, (WheelPos * 3) * opacity);
  }
  else {
    WheelPos -= 170;
    return strip.Color(0, (WheelPos * 3) * opacity, (255 - WheelPos * 3) * opacity);
  }
}




