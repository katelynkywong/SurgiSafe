//Libraries
#include <LiquidCrystal.h> // LCD Screen
#include <Wire.h>// Color Sensor
#include <SparkFun_APDS9960.h>// Color Sensor
// No libraries needed for Temperature sensor
// No libraries needed for Moisture

//Initalize pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12); //LCD Screen
#define temp A0 //Tempe8rature sensor
#define moisturePin A1 //Moisture Sensor
#define outputPin 6 //Moisture Sensor
SparkFun_APDS9960 apds = SparkFun_APDS9960(); //Color Sensor

String color;
//checker variables
float checkTemp = 0.0;
int checkMoist = 0;
String checkColor = "Pink";


void setup(void) {
  Serial.begin(9600); //Set data rate 9600 bits per second
  lcd.begin(16, 2); //Start LCD screen
  Wire.begin(); // Start Color Sensor

  // Color Sensor Connection
  if (apds.init()) {
    Serial.println("APDS-9960 initialized");
  } else {
    Serial.println("Error initializing APDS-9960");
  }

  // Enable color sensing mode
  if (apds.enableLightSensor(false)) {
    Serial.println("Ambient light sensor enabled");
  } else {
    Serial.println("Error enabling light sensor");
  }

  delay(100); // Give sensor time to initialize
  //No set up Temperature Sensor 
  //No Set up for moisture

}
 
 
 void loop() {
  
  //Temperature Check
  float voltage = analogRead(temp)*0.004882813;
  float degreesC = (voltage - 0.5)*100.0; //Measured in Celsius
  Serial.print("Temperature °C = ");
  Serial.println(degreesC);
  if (degreesC >=50 && checkTemp >50){
    lcd.setCursor(0,0);
    lcd.print("Bandage Alert!");
    lcd.setCursor(0,1);
    lcd.print("High Temperature");
    delay(100000000000);
  }
  checkTemp = degreesC;

  //Moisture Check
  int moistureValue = analogRead(moisturePin);  // Read the analog value from sensor
  int outputValue = map(moistureValue, 0, 1023, 255, 0); // map the 10-bit data to 8-bit data
  analogWrite(outputPin, outputValue); // generate PWM signal
  Serial.print("Moisture = "); 
  Serial.println(outputValue);
  if (outputValue >=140 && checkMoist >=140){
    lcd.setCursor(0,0);
    lcd.print("Bandage Alert!");
    lcd.setCursor(0,1);
    lcd.print("High Moisture");
    delay(100000000000);
  }
  checkMoist = outputValue;

  // Color Check
  uint16_t red, green, blue, ambient;
  if (!apds.readAmbientLight(ambient) || !apds.readRedLight(red) || !apds.readGreenLight(green) || !apds.readBlueLight(blue)) {
    Serial.println("Error reading light values");
  
  }
  // Print color values
  Serial.print("Ambient: "); Serial.print(ambient);
  Serial.print("\tRed: "); Serial.print(red);
  Serial.print("\tGreen: "); Serial.print(green);
  Serial.print("\tBlue: "); Serial.println(blue);

  // Color identification logic (simple threshold based example)
  if (red > green && red > blue) {
    color = "Red";
  } else if (green > red && green > blue) {
    color = "Green";
  } else if (blue > red && blue > green) {
    color = "Blue";
  } else {
    color = "Mixed";
  }
  Serial.println(color);
  if (color == "Blue" && checkColor == "Blue"){
    lcd.setCursor(0,0);
    lcd.print("Bandage Alert!");
    lcd.setCursor(0,1);
    lcd.print("High Alkaline");
    delay(100000000000);
  }
  checkColor = color;

  //time check
  int timeB = 0;
  timeB+=5;
  if (timeB >= 172800) {
    lcd.setCursor(0,0);
    lcd.print("Bandage Alert!");
    lcd.setCursor(0,1);
    lcd.print("Change Bandage");
    delay(100000000000);
  }
  
  lcd.setCursor(0,0);
  lcd.print("Bandage Normal");
  
  delay(5000);


  }
  
