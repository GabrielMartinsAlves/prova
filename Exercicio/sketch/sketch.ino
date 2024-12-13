#include <WiFi.h>// library to acess wifi
#include <HTTPClient.h> // library to make http requisitions

#define blue_led 48 // Pin to control blue led
#define green_led 41 // Pin to control green led
#define red_led 40 // Pin to control red led
#define yellow_led 9 // Pin to control yellow led

const int buttonPin = 18;  // the number of the pushbutton pin
int buttonState = 0;  // variable for reading the pushbutton status

const int ldrPin = 4;  // Ldr pin

// ldr constants
const float GAMMA = 0.7;
const float RL10 = 50;
const int threshold=600;


void setup() {

  // Pin configurations (input, output)
  pinMode(blue_led,OUTPUT);
  pinMode(green_led,OUTPUT);
  pinMode(red_led,OUTPUT);
  pinMode(yellow_led,OUTPUT);

  pinMode(buttonPin, INPUT_PULLDOWN); // Initialize the pushbutton pin as an input_pulldown

  // Making sure that pins are in the low logical state
  digitalWrite(blue_led, LOW);
  digitalWrite(green_led, LOW);
  digitalWrite(red_led, LOW);
  digitalWrite(yellow_led, LOW);



  Serial.begin(9600); // Setting the serial to 9600 baud rate

  WiFi.begin("Wokwi-GUEST", "", 6); // Connecting with wokwi's hotspot
  Serial.print("Connecting to wifi");

  while (WiFi.status() == WL_CONNECT_FAILED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("Wi-Fi connected!"); // Exiting the loop, printing success message

  if(WiFi.status() == WL_CONNECTED){ // Verifing if it's connected
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // HTTP's endpoint

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET(); // Result of HTTP's requisiiton

    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      }
      http.end();
    }

  else {
    Serial.println("WiFi Disconnected");
  }
}

void loop() {
  int ldrstatus = analogRead(ldrPin);
  float voltage = ldrstatus / 1024. * 5;
  float resistance = 2000 * voltage / (1 - voltage / 5);
  float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));

    // Verifing button's state
  buttonState = digitalRead(buttonPin);
  // Debounce
  delay(100);

  Serial.print(buttonState);
  if (buttonState == 1) {
    delay(1000);
    digitalWrite(green_led, HIGH);
  } else {
    Serial.println("Button isn't pressed!");
    if(lux>=threshold){
      Serial.print("its dark turn on led");
      Serial.println(ldrstatus);
      digitalWrite(yellow_led, HIGH);
      delay(1000);
      digitalWrite(yellow_led, LOW);
    } else{
      digitalWrite(green_led, HIGH);
      delay(3000);
      digitalWrite(green_led, LOW);
      digitalWrite(yellow_led, HIGH);
      delay(2000);
      digitalWrite(yellow_led, LOW);
      digitalWrite(red_led, HIGH);
      delay(5000);
      digitalWrite(red_led, LOW);
    }
  }
}