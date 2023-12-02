#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const uint8_t buttonPin_1 = 2;
const uint8_t buttonPin_2 = 3;
const uint8_t buttonPin_3 = 4;
uint8_t buttonState_1;
uint8_t buttonState_2;
uint8_t buttonState_3;
uint8_t Apurva = 0;
uint8_t Ayushi = 0;
uint8_t Aditi = 0;

uint8_t currentState = 1;  // 1: Enter Password, 2: Password Correct, 3: Password Incorrect, 4: Check Identity, 5: Vote
uint8_t password = 1234;
String enteredPassword = "";
const uint8_t totalVoters = 5;
bool hasVoted[totalVoters] = { false };

void setup() {
  currentState = 1;

  pinMode(buttonPin_1, INPUT);
  pinMode(buttonPin_2, INPUT);
  pinMode(buttonPin_3, INPUT);

  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("GME12864 allocation failed"));
    for (;;) {
    }
  }

  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.display();
}

void loop() {
  display.clearDisplay();

  switch (currentState) {
    case 1:  // Enter Password
      display.setCursor(0, 10);
      display.println("Kindly input the login password:");
      display.display();
      if (Serial.available() > 0) {
        String inputString = Serial.readStringUntil('\n');
        Serial.println(inputString);
        uint8_t id = inputString.toInt();
        if (id == password) {
          currentState = 2;
          display.clearDisplay();
          display.setCursor(0, 10);
          display.println("Completed");
          display.display();
          delay(2000);
        } else {
          currentState = 3;
        }
      }
      break;

    case 2:  // Password Correct
      display.clearDisplay();
      display.setCursor(0, 10);
      display.println("1.Cast Vote");
      display.println("2.Check Result");
      display.display();
      buttonState_1 = 0;
      buttonState_2 = 0;
      buttonState_3 = 0;
      buttonState_1 = digitalRead(buttonPin_1);
      buttonState_2 = digitalRead(buttonPin_2);
      buttonState_3 = digitalRead(buttonPin_3);
      if (buttonState_1 == HIGH) {
        currentState = 4;
        display.clearDisplay();
        display.setCursor(0, 10);
        display.println("Voter User ID:");
        display.display();
      } else if (buttonState_2 == HIGH) {
        currentState = 7;
        // display.clearDisplay();
        // display.println("Enter result Password :");
        // display.display();
      }

      break;

    case 7:  // Check result password
      if (Serial.available() > 0) {
        String inputString = Serial.readStringUntil('\n');
        Serial.println(inputString);
        uint8_t id = inputString.toInt();
        if (id == password) {
          currentState = 8;
        } else {
          currentState = 7;
        }
      }
      break;


    case 8:
      display.clearDisplay();
      display.setCursor(0, 10);
      display.print("Apurva:");
      display.print(Apurva);
      display.print("\n");
      display.print("Ayushi:");
      display.print(Ayushi);
      display.print("\n");
      display.print("Aditi:");
      display.print(Aditi);
      display.print("\n Winner:");
      if (Apurva > Ayushi and Apurva > Aditi) {
        display.print("Apurva");
        display.println();
      } else if (Apurva < Ayushi and Ayushi > Aditi) {
        display.print("Ayushi");
        display.println();
      } else if (Apurva < Aditi and Ayushi < Aditi) {
        display.print("Aditi");
        display.println();
      } else {
        display.print("NULL");
        display.println();
      }
      display.display();
      delay(7000);
      currentState = 2;
      break;
    case 3:  // Password Incorrect
      display.setCursor(0, 10);
      display.println("Password isn't correct");
      display.println("Try Again");
      display.display();
      delay(2000);
      currentState = 1;
      break;

    case 6:  // Voter name Incorrect
      display.setCursor(0, 10);
      display.println("voter_ID isn't correct");
      display.println("Try Again");
      display.display();
      delay(5000);
      currentState = 2;
      break;

    case 4:  // Check Identity
      if (Serial.available() > 0) {
        String inputString = Serial.readStringUntil('\n');
        Serial.println(inputString);
        uint8_t id = inputString.toInt();
        if (id >= 0 && id < 5) {
          if (hasVoted[id] == false) {
            currentState = 5;
            hasVoted[id] = true;
            break;
          } else {
            display.clearDisplay();
            display.setCursor(0, 10);
            display.println("Casted");
            display.display();
            delay(3000);
            currentState = 2;
          }
        } else {
          display.clearDisplay();
          display.setCursor(0, 10);
          display.println("Invalid_Id");
          display.display();
          delay(3000);
          currentState = 2;
        }
      }
      break;

    case 5:  // Vote
      buttonState_1 = digitalRead(buttonPin_1);
      buttonState_2 = digitalRead(buttonPin_2);
      buttonState_3 = digitalRead(buttonPin_3);

      display.clearDisplay();
      display.setCursor(0, 10);
      display.println("Please vote:");
      display.println("1. Apurva");
      display.println("2. Ayushi");
      // display.println("");
      display.println("3. Aditi");
      display.display();
      if (buttonState_1 == HIGH) {
        Apurva++;
        display.clearDisplay();
        display.setCursor(0, 10);
        display.println("Voted to Apurva");
        display.display();
        delay(2000);
        currentState = 2;
      } else if (buttonState_2 == HIGH) {
        Ayushi++;
        display.clearDisplay();
        display.setCursor(0, 10);
        display.println("Voted to Ayushi");
        display.display();
        delay(2000);
        currentState = 2;
      } else if (buttonState_3 == HIGH) {
        Aditi++;
        display.clearDisplay();
        display.setCursor(0, 10);
        display.println("Voted to Aditi");
        display.display();
        delay(2000);
        currentState = 2;
      }
      break;

    default:
      break;
  }
}