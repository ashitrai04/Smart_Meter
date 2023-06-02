#include <WiFi.h>

const char* ssid = "iPhone";
const char* password = "ashit@123";
const char* serverIP = "172.20.10.3";

const int ledPin = 4; // Pin connected to the LED
const int pirPin = 27; // PIR sensor pin
const int switchPin = 2; // Switch pin

bool switchOn = false;
int switchStatus = 0; // Switch status variable
bool switchPressed = false;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT); // Set the LED pin as output
  pinMode(pirPin, INPUT); // Initialize the PIR sensor pin as an input
  pinMode(switchPin, INPUT_PULLUP); // Initialize the switch pin as an input with internal pull-up resistor enabled
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
updateSwitchStatus();
  serialEvent();
  pir();
}
// Function to update switch status based on switch button press
void updateSwitchStatus() {
  // Read the state of the switch
  int switchState = digitalRead(switchPin);

  if (switchState == HIGH && !switchPressed) {
    switchOn = !switchOn;
    switchPressed = true;

    if (switchOn) {
      switchStatus = 1; // Set switch status to 1
      Serial.println("Switch turned ON!");
    } else {
      switchStatus = 0; // Set switch status to 0
      Serial.println("Switch turned OFF!");
    }
  } else if (switchState == LOW) {
    switchPressed = false;
  }
}

// Function to handle serial input
void serialEvent() {
    WiFiClient client;
   if (client.connect(serverIP, 80)) {
    Serial.println("Connected to server");

    while (client.connected()) {
      
        String command = client.readStringUntil('\r');
        Serial.println(command);

        if (command == "LED_ON") {
       switchStatus = 1;
          Serial.println("LED turned ON");
        } else if (command == "LED_OFF") {
        switchStatus = 0;
          Serial.println("LED turned OFF");
      }
    }
  }
}
void pir(){
  
  int pirState = digitalRead(pirPin);

  // Update switch status based on switch button press
  
  // If the switch is on and motion is detected by the PIR sensor, turn on the LED
  if (switchStatus == 1 && pirState == HIGH) {
    digitalWrite(ledPin, HIGH);
    Serial.println("Motion detected! LED ON");
  } else {
    digitalWrite(ledPin, LOW);
  }

  delay(100);
}
