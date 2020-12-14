#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/* Change these values based on your calibration values */
#define soilWet 500   // Define max value we consider soil 'wet'
#define soilDry 750   // Define min value we consider soil 'dry'

// Sensor pins
#define sensorPower 7
#define sensorPin A0
#define led_light 3



void setup() {
  pinMode(sensorPower, OUTPUT);
  pinMode(led_light, OUTPUT);
  
  // Initially keep the sensor OFF
  digitalWrite(sensorPower, LOW);
  
  Serial.begin(9600);
  
  // testing the display  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...

//  testdrawstyles();    // Draw 'stylized' characters
}

void loop() {
  //get the reading from the function below and print it
  int moisture = readSensor();
  Serial.print("Analog Output: ");
  Serial.println(moisture);

  // Determine status of our soil
  if (moisture < soilWet) {
    Serial.println("Status: Soil is too wet");
    testdrawstyles("Status: Too Wet: ", (String)moisture);
    digitalWrite(led_light, HIGH);
    delay(900);
    digitalWrite(led_light, LOW);
  } else if (moisture >= soilWet && moisture < soilDry) {
    Serial.println("Status: Soil moisture is perfect");
    testdrawstyles("Status: Perfect", (String)moisture);
    digitalWrite(led_light, HIGH);
  } else {
    Serial.println("Status: Soil is too dry - time to water!");
    testdrawstyles("Status: Too Dry", (String)moisture);
    digitalWrite(led_light, LOW);
  }
  
  delay(36000);  // Take a reading every second for testing
          // Normally you should take reading perhaps once or twice a day
  Serial.println();
}

void testdrawstyles(String moisture_status, String output) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(moisture_status);

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.println(output);

  //inverting to prevent OLED burn in
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);

//  display.setTextSize(1.2);
//  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
//  display.println("Soil Moisture");
  
//  display.setTextSize(2);             // Draw 2X-scale text
//  display.setTextColor(SSD1306_WHITE);
//  display.print(F("0x")); display.println(0xDEADBEEF, HEX);

  display.display();
  delay(2000);
}

//  This function returns the analog soil moisture measurement
int readSensor() {
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(1000);              // Allow power to settle
  int val = analogRead(sensorPin);  // Read the analog value form sensor
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return val;             // Return analog moisture value
}
