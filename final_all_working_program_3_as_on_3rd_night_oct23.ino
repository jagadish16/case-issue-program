#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define LED_PIN 8          // Define the data pin for your WS2812B LED strip
#define NUM_LEDS 30        // Define the number of LEDs in your strip
#define BRIGHTNESS 255     // Set the brightness (0-255)
#define BUZZER_PIN 9       // Define the buzzer pin

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800); // NeoPixel object

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C // Replace with your OLED's I2C address

#define OLED_RESET -1 // Reset pin # (or -1 if not used)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // Initialize with the I2C address
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  // Initialize the Serial communication
  Serial.begin(9600);

  // Initialize the NeoPixel strip
  strip.begin();
  strip.show();  // Initialize all pixels to 'off'
  strip.setBrightness(BRIGHTNESS); // Set brightness

  // Initialize the buzzer pin
  pinMode(BUZZER_PIN, OUTPUT);

  // Clear the display buffer
  display.clearDisplay();
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    if (command.startsWith("#~10,")) {
      String message = command.substring(5); // Extract the message (excluding the sub-command)
      handleCustomCommand(message);
    } else {
      displayDefaultMessage(); // Display a default message
      
    }
  }
}
void handleCustomCommand(String message) {
  message.trim();
  display.clearDisplay();
  display.setTextSize(2);      // Larger text size
  display.setTextColor(SSD1306_WHITE); // White text
  display.setCursor(10, 20);

  if (message.equalsIgnoreCase("DRUNK")) {
    display.println("FAIL");
    display.display(); // Display immediately
    delay(100); // Delay to allow the display to update
    redFlash();
    playAlarmSound(); // Play an alarm buzzer sound
    delay(2000); // Display for 2 seconds
    displayDefaultMessage(); // Clear to default message
    display.display(); // Display immediately
  } else if (message.equalsIgnoreCase("DRINKING")) {
    display.println("MODERATE");
    display.display(); // Display immediately
    delay(100); // Delay to allow the display to update
    orangeFlash();
    
    // Play a buzzer sound at 1500 Hz for 500 milliseconds
    playBuzzer(1500, 500);
    
    delay(2000); // Display for 2 seconds
    displayDefaultMessage(); // Clear to default message
    display.display(); // Display immediately
  
  } else if (message.equalsIgnoreCase("NO ALCOHOL CONTENT")) {
    display.println("PASS");
    display.display(); // Display immediately
    delay(100); // Delay to allow the display to update
    greenFlash();
    playGoodSound(); // Play a good buzzer sound
    delay(2000); // Display for 2 seconds
    displayDefaultMessage(); // Clear to default message
    display.display(); // Display immediately
  } else if (message.equalsIgnoreCase("BLOW STATE")) {
    display.println("BLOWING");
    display.display(); // Display immediately
    delay(100); // Delay to allow the display to update
    redFlash();
    playAlarmSound(); // Play an alarm buzzer sound
    delay(2000); // Display for 2 seconds
    displayDefaultMessage(); // Clear to default message
    display.display(); // Display immediately
  } else if (message.equalsIgnoreCase("WARMING STATE")) {
    display.println("System Busy");
    display.display(); // Display immediately
    delay(100); // Delay to allow the display to update
    blueLights();
    playWarningSound(); // Play a warning buzzer sound
    delay(2000); // Display for 2 seconds
    displayDefaultMessage(); // Clear to default message
    display.display(); // Display immediately
  } else {
    displayDefaultMessage(); // Display a default message for unknown cases
    display.display(); // Display immediately
    delay(100); // Delay to allow the display to update
    purpleLights(); // Use a purple LED pattern for unknown cases
    delay(2000); // Display for 2 seconds
    displayDefaultMessage(); // Clear to default message
    display.display(); // Display immediately
  }
}

void playBuzzer(int frequency, int duration) {
  // Calculate the delay between high and low states for the desired frequency
  int halfPeriodMicroseconds = 1000000 / (2 * frequency);
  
  // Calculate the number of cycles for the desired duration
  int numCycles = (duration * 1000) / (2 * halfPeriodMicroseconds);
  
  // Generate the buzzer sound
  for (int i = 0; i < numCycles; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(halfPeriodMicroseconds);
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(halfPeriodMicroseconds);
  }
}

void displayDefaultMessage() {
  display.clearDisplay();
  display.setTextSize(2);      // Larger text size
  display.setTextColor(SSD1306_WHITE); // White text
  display.setCursor(10, 20);
  display.println("Default Message");
}



void redFlash() {
  for (int i = 0; i < 3; i++) {
    redLights();
    playAlarmSound(); // Play an alarm buzzer sound
    delay(500);
    turnOffLights();
    delay(500);
  }
}

void orangeFlash() {
  for (int i = 0; i < 3; i++) {
    orangeLights();
    playWarningSound(); // Play a warning buzzer sound
    delay(500);
    turnOffLights();
    delay(500);
  }
}

void greenFlash() {
  for (int i = 0; i < 3; i++) {
    greenLights();
    playGoodSound(); // Play a good buzzer sound
    delay(500);
    turnOffLights();
    delay(500);
  }
}

void blueLights() {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 255)); // Blue
  }
  strip.show();
}

void playGoodSound() {
  // Play a "good" buzzer sound
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);
}

void playWarningSound() {
  // Play a "warning" buzzer sound
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1000);
  digitalWrite(BUZZER_PIN, LOW);
}

void playAlarmSound() {
  // Play an "alarm" buzzer sound
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1000);
  digitalWrite(BUZZER_PIN, LOW);
}

void drawBlinkingLines(int x, int y, uint8_t color) {
  for (int i = 0; i < 5; i++) {
    display.drawLine(x, y + i, x + 40, y + i, color);
  }
}

void drawBouncingBall(int x, int y, uint8_t color) {
  static int direction = 1;
  static int position = 0;
  display.fillCircle(x + position, y, 4, color);
  position += direction * 4;
  if (position >= 36 || position <= 0) {
    direction *= -1;
  }
}

void drawRotatingStar(int x, int y, int size, uint8_t color) {
  static int angle = 0;
  for (int i = 0; i < 5; i++) {
    int x1 = x + size * cos(angle + i * 0.4);
    int y1 = y + size * sin(angle + i * 0.4);
    int x2 = x + size * cos(angle + i * 0.8);
    int y2 = y + size * sin(angle + i * 0.8);
    display.drawLine(x, y, x1, y1, color);
    display.drawLine(x, y, x2, y2, color);
  }
  angle += 5;
}

void drawBouncingText(int x, int y, const char* text, uint8_t color) {
  static int direction = 1;
  static int position = 0;
  display.setTextSize(1);
  display.setTextColor(color);
  display.setCursor(x + position, y);
  display.println(text);
  position += direction * 4;
  if (position >= 20 || position <= 0) {
    direction *= -1;
  }
}

void redLights() {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0));
  }
  strip.show();
}

void greenLights() {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(0, 255, 0));
  }
  strip.show();
}

void orangeLights() {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(255, 165, 0)); // Orange
  }
  strip.show();
}



void purpleLights() {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(128, 0, 128)); // Purple
  }
  strip.show();
}

void turnOffLights() {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0)); // Turn off all LEDs
  }
  strip.show();
}