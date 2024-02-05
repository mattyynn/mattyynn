#include <Arduino.h>
#include <MsTimer2.h>

static uint16_t pressureSensor = 0;  // 0-1023
const char Set = 'S';
const char Reset = 'R';

const char highJump = 13;
const char lowJump = 12;
const char switchingSR = 11;
uint8_t highJumpBorder = 240;
uint8_t lowJumpBorder = 200;

int readInterval = 100;

const char identification = 'A';
static uint8_t clampedPressureValue = 0;  // 0-255
static char resetFlag = Set;

void getPressureValue(void) { pressureSensor = analogRead(0); }

void setup() {
    Serial.begin(9600);

    pinMode(highJump, OUTPUT);
    pinMode(lowJump, OUTPUT);
    pinMode(switchingSR, INPUT_PULLUP);

    MsTimer2::set(readInterval, getPressureValue);
    MsTimer2::start();
}

void loop() {
    clampedPressureValue = 255 - (uint8_t)(pressureSensor / 4);  // 0~255
    resetFlag = Set;
    if (clampedPressureValue >= highJumpBorder) {
        digitalWrite(highJump, HIGH);
        digitalWrite(lowJump, LOW);
    } else if (clampedPressureValue >= lowJumpBorder) {
        digitalWrite(highJump, LOW);
        digitalWrite(lowJump, HIGH);
    } else {
        digitalWrite(highJump, LOW);
        digitalWrite(lowJump, LOW);
    }

    if (digitalRead(switchingSR) == 0) {
        resetFlag = (resetFlag == Set) ? Reset : Set;
    }

    Serial.print(identification);
    Serial.print(' ');
    Serial.print(clampedPressureValue);
    Serial.print(' ');
    Serial.print(resetFlag);
    Serial.print('\n');
}
