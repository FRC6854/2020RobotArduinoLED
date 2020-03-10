#include <FastLED.h>
#define NUM_LEDS 44
#define DATA_PIN 2
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];
CRGB redAlliance = CRGB::Red;
CRGB blueAlliance = CRGB::Blue;
CRGB lowConveyor = CRGB::Yellow;
CRGB upConveyor = CRGB::Purple;
CRGB currentAlliance;

int lowConLedPos[(NUM_LEDS / 2) / 4];
int upConvLedPos[(NUM_LEDS / 2) / 4];
int visionLedPos[NUM_LEDS / 4];
int climbLedPos[NUM_LEDS / 4];

bool brownoutFlag = false;

char ledMode = '1';

void setup() {
  currentAlliance = blueAlliance;
  //FastLED.addLeds<NEOPIXEL, DATA_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(9600);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);

  FastLED.setBrightness(100);

  int num = 0;
  for (int i = 0; i < (NUM_LEDS / 2) / 4; i++) {
    lowConLedPos[i] = num;
    upConvLedPos[i] = num + (NUM_LEDS / 2);
    num += 4;
  }

  num = 0;
  for (int i = 0; i < NUM_LEDS / 4; i++) {
    visionLedPos[i] = num;
    climbLedPos[i] = num;
    num += 4;
  }
}

void loop() {
  if (Serial.available() > 0) {
    Serial.println("Serial Reading");
    ledMode = Serial.read();

    if (isUpperCase(ledMode)) {
      currentAlliance = blueAlliance;
    } else if(isLowerCase(ledMode)) {
      currentAlliance = redAlliance;
    }

    if (ledMode == 'k') {
      brownoutFlag = true;
    }
  }

  //Serial.println(ledMode);
  if (brownoutFlag == false) {
    if (ledMode == 'a' || ledMode == 'A') {
      lowConveyorRun(currentAlliance, 0);
    } else if (ledMode == 'b' || ledMode == 'B') {
      lowConveyorRun(currentAlliance, 1);
    } else if (ledMode == 'c' || ledMode == 'C') {
      upConveyorRun(currentAlliance, 0);
    } else if (ledMode == 'd' || ledMode == 'D') {
      upConveyorRun(currentAlliance, 1);
    } else if (ledMode == 'e' || ledMode == 'E') {
      runBothConveyors(currentAlliance, 0);
    } else if (ledMode == 'f' || ledMode == 'F') {
      runBothConveyors(currentAlliance, 1);
    } else if (ledMode == 'g' || ledMode == 'G') {
      aimVision(currentAlliance, false);
    } else if (ledMode == 'h' || ledMode == 'H') {
      aimVision(currentAlliance, true);
    } else if (ledMode == 'i' || ledMode == 'I') {
      climb(currentAlliance, 0);
    } else if (ledMode == 'j' || ledMode == 'J') {
      climb(currentAlliance, 1);
    } else if (ledMode == 'k' || ledMode == 'K') {

    } else {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = currentAlliance;
      }
      FastLED.show();
    }
  } else {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Red;
    }
    
    FastLED.show();
    delay(1000);

    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
    }
    
    FastLED.show();
    delay(1000);
  }
}

void lowConveyorRun(const CRGB& alliance, int direction) {

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = alliance;
  }

  if (direction == 0) {
    for (int i = 0; i < ((NUM_LEDS / 2) / 4); i++) {
      leds[lowConLedPos[i]] = lowConveyor;
      if (lowConLedPos[i] > NUM_LEDS / 2) {
        lowConLedPos[i] = 0;
      } else {
        lowConLedPos[i] ++;
      }
    }
  } else {
    for (int i = 0; i < ((NUM_LEDS / 2) / 4); i++) {
      leds[lowConLedPos[i]] = lowConveyor;
      if (lowConLedPos[i] <= 0) {
        lowConLedPos[i] = NUM_LEDS / 2;
      } else {
        lowConLedPos[i] --;
      }
    }
  }


  FastLED.show();

  delay(250);
}

void upConveyorRun(const CRGB& alliance, int direction) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = alliance;
  }

  if (direction == 0) {
    for (int i = 0; i < ((NUM_LEDS / 2) / 4); i++) {
      leds[upConvLedPos[i]] = upConveyor;
      if (upConvLedPos[i] > NUM_LEDS) {
        upConvLedPos[i] = NUM_LEDS / 2;
      } else {
        upConvLedPos[i] ++;
      }
    }
  } else {
    for (int i = 0; i < ((NUM_LEDS / 2) / 4); i++) {
      leds[upConvLedPos[i]] = upConveyor;
      if (upConvLedPos[i] <= NUM_LEDS / 2) {
        upConvLedPos[i] = NUM_LEDS;
      } else {
        upConvLedPos[i] --;
      }
    }
  }


  FastLED.show();

  delay(250);
}

void runBothConveyors(const CRGB& alliance, const int direction) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = alliance;
  }

  if (direction  == 0) {
    for (int i = 0; i < ((NUM_LEDS / 2) / 4); i++) {
      leds[lowConLedPos[i]] = lowConveyor;
      if (lowConLedPos[i] > NUM_LEDS / 2) {
        lowConLedPos[i] = 0;
      } else {
        lowConLedPos[i] ++;
      }
    }

    for (int i = 0; i < ((NUM_LEDS / 2) / 4); i++) {
      leds[upConvLedPos[i]] = upConveyor;
      if (upConvLedPos[i] > NUM_LEDS - 2) {
        upConvLedPos[i] = NUM_LEDS / 2;
      } else {
        upConvLedPos[i] ++;
      }
    }

  } else {
    for (int i = 0; i < ((NUM_LEDS / 2) / 4); i++) {
      leds[upConvLedPos[i]] = upConveyor;
      if ((upConvLedPos[i] <= NUM_LEDS / 2)) {
        upConvLedPos[i] = NUM_LEDS - 2;
      } else {
        upConvLedPos[i] --;
      }
    }

    for (int i = 0; i < ((NUM_LEDS / 2) / 4); i++) {
      leds[lowConLedPos[i]] = lowConveyor;
      if (lowConLedPos[i] <= 1) {
        lowConLedPos[i] = NUM_LEDS / 2;
      } else {
        lowConLedPos[i] --;
      }
    }
  }

  FastLED.show();
  delay(250 );
}

void aimVision(const CRGB& alliance, boolean foundTarget) {

  if (!foundTarget) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = alliance;
    }

    delay(120);
    FastLED.show();

    for (int i = 0; i < NUM_LEDS / 4; i++) {
      leds[visionLedPos[i]] = CRGB::Green;
    }
    delay(120);
    FastLED.show();

    for (int i = 0; i < NUM_LEDS / 4; i++) {
      leds[visionLedPos[i]] = CRGB::Black;
    }
  } else {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = alliance;
    }

    for (int i = 0; i < NUM_LEDS / 4; i++) {
      leds[visionLedPos[i]] = CRGB::Green;
    }
    FastLED.show();
  }
}

void climb(const CRGB& alliance, int climbMode) {
  if (climbMode == 0) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = alliance;
    }

    for (int i = 0; i < NUM_LEDS / 4; i++) {
      leds[climbLedPos[i]] = CRGB::White;
    }
    FastLED.show();

  } else {

    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = alliance;
    }

    delay(120);
    FastLED.show();

    for (int i = 0; i < NUM_LEDS / 4; i++) {
      leds[climbLedPos[i]] = CRGB::White;
    }
    delay(120);
    FastLED.show();

    for (int i = 0; i < NUM_LEDS / 4; i++) {
      leds[climbLedPos[i]] = CRGB::Black;
    }
  }
}
