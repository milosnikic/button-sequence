#include <Arduino_FreeRTOS.h>
#include <FastLED.h>
#define BUTTON1 4
#define BUTTON2 5
#define BUTTON3 6
#define BUTTON4 7
#define BUTTON5 8
#define DATA_PIN     3
#define NUM_LEDS    11
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

void checkIfButtonIsPressedWithDebounce( void *pvParameters );
TaskHandle_t Handle_pushButtons;

bool button1Pressed = false;
bool button1FirstTimePressed = true;
bool button2Pressed = false;
bool button2FirstTimePressed = true;
bool button3Pressed = false;
bool button3FirstTimePressed = true;
bool button4Pressed = false;
bool button4FirstTimePressed = true;
bool button5Pressed = false;
bool button5FirstTimePressed = true;

void setup() {
  buttonSetup();
  resetAll();
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  shineOneColor(255, 0, 0);
  xTaskCreate(checkIfButtonIsPressedWithDebounce, "task1", 110, (void *) BUTTON1, 1, &Handle_pushButtons);
  xTaskCreate(checkIfButtonIsPressedWithDebounce, "task2", 110, (void *) BUTTON2, 1, &Handle_pushButtons);
  xTaskCreate(checkIfButtonIsPressedWithDebounce, "task3", 110, (void *) BUTTON3, 1, &Handle_pushButtons);
  xTaskCreate(checkIfButtonIsPressedWithDebounce, "task4", 110, (void *) BUTTON4, 1, &Handle_pushButtons);
  xTaskCreate(checkIfButtonIsPressedWithDebounce, "task5", 110, (void *) BUTTON5, 1, &Handle_pushButtons);
  vTaskStartScheduler();
}

void loop() {}

void checkIfButtonIsPressedWithDebounce(void * pvParameters) {
  unsigned long lastDebounceTime = 0;
  unsigned long debounceDelay = 100;
  Serial.println("button entered");
  byte buttonState;
  byte lastButtonState = LOW;
  while (1) {
    byte reading = digitalRead(( unsigned char ) pvParameters);
    if (reading != lastButtonState) {
      lastDebounceTime = millis();
    }
    if ((millis() - lastDebounceTime) > debounceDelay) {
      if (reading != buttonState) {
        buttonState = reading;
        if (reading == LOW) {
          switch (( uint32_t ) pvParameters) {
            case 4:
              // First button
              if (button3Pressed && button1FirstTimePressed) {
                // Button 1 is pressed and proceed
                button1FirstTimePressed = false;
                button1Pressed = true;
                shineSecondButtonPressed();
              } else {
                resetAll();
              }
              break;
            case 5:
              // Second button
              if (button3Pressed && button1Pressed && button2FirstTimePressed) {
                // Button 2 is pressed and proceed
                button2Pressed = true;
                button2FirstTimePressed = false;
                shineThirdButtonPressed();
              } else {
                resetAll();
              }
              break;
            case 6:
              // Third button
              if (button3FirstTimePressed) {
                // Button 3 is pressed and proceed
                button3Pressed = true;
                button3FirstTimePressed = false;
                shineFirstButtonPressed();
              } else {
                resetAll();
              }
              break;
            case 7:
              // Fourth button
              if (button3Pressed && button1Pressed && button2Pressed && button4FirstTimePressed) {
                // Button 4 is pressed and proceed
                button4Pressed = true;
                button4FirstTimePressed = false;
                shineFourthButtonPressed();
              } else {
                resetAll();
              }
              break;
            case 8:
              // Fifth button
              if (button3Pressed && button1Pressed && button2Pressed && button4Pressed && button5FirstTimePressed) {
                // Button 5 is pressed and proceed
                button5Pressed = true;
                button5FirstTimePressed = false;
                // This is condition that all buttons are pressed in specified sequence, so the task is completed
                shineLedTapeCompleted();
                while (1) {}
              } else {
                resetAll();
              }
              break;
          }
        }
      }
    }
    lastButtonState = reading;
  }
}

void buttonSetup() {
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(BUTTON3, INPUT);
  pinMode(BUTTON4, INPUT);
  pinMode(BUTTON5, INPUT);
  digitalWrite(BUTTON1, HIGH);
  digitalWrite(BUTTON2, HIGH);
  digitalWrite(BUTTON3, HIGH);
  digitalWrite(BUTTON4, HIGH);
  digitalWrite(BUTTON5, HIGH);
}

void shineFirstButtonPressed() {
  //Simulate first button pressed shine
  Serial.println("First sequence button pressed");
  shineOneColor(0, 50, 0);
}

void shineSecondButtonPressed() {
  //Simulate second button pressed shine
  Serial.println("Second sequence button pressed");
  shineOneColor(0, 100, 0);
}

void shineThirdButtonPressed() {
  //Simulate third button pressed shine
  Serial.println("Third sequence button pressed");
  shineOneColor(0, 150, 0);
}

void shineFourthButtonPressed() {
  //Simulate fourth button pressed shine
  Serial.println("Fourth sequence button pressed");
  shineOneColor(0, 200, 0);
}

void shineLedTapeCompleted() {
  // Simulate task completed
  Serial.println("Sequence completed");
  shineOneColor(0, 255, 0);

}
void resetAll() {
  Serial.println("Meeeh, try again!");
  button1Pressed = false;
  button2Pressed = false;
  button3Pressed = false;
  button4Pressed = false;
  button5Pressed = false;
  button1FirstTimePressed = true;
  button2FirstTimePressed = true;
  button3FirstTimePressed = true;
  button4FirstTimePressed = true;
  button5FirstTimePressed = true;
  turnOffLedTape();
}

void turnOffLedTape() {
  shineOneColor(255, 0, 0);
}

void shineOneColor(unsigned char red, unsigned char green, unsigned char blue) {
  for (unsigned char i = 0; i < NUM_LEDS; i++) {
    leds[i] =  CRGB(red, green, blue);
  }
  FastLED.show();
}
