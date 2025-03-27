#include "Arduino.h"
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

const byte PIN_LENGTH = 4;
char password[PIN_LENGTH] = { '0', '0', '0', '0'};


const byte ROW_PINS[ROWS] = { 5, 4, 3, 2};
const byte COL_PINS[COLS] = { 6, 7, 8, 13};

const char BUTTONS[ROWS][COLS] = {
  { '1', '2', '3', 'A'},
  { '4', '5', '6', 'B'},
  { '7', '8', '9', 'C'},
  { '*', '0', '#', 'D'}
};

Keypad heroKeypad = Keypad(makeKeymap(BUTTONS), ROW_PINS, COL_PINS, ROWS, COLS);

const byte BUZZER_PIN = 12;

const byte RED_PIN = 11;
const byte GREEN_PIN = 10;
const byte BLUE_PIN = 9;


void setup(){
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  displayColor(128, 0, 0); //red color

  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Press * to make a new password or press # to access the system.");

}

void loop(){
  char button_character = heroKeypad.getKey();
  
  if (button_character == '#') {
    giveInputFeedback();
    bool access_allowed = validatePIN();
    if (access_allowed) {
        Serial.println("Welcome, authorized user. You may now begin using the system.");
    }
    else {
        Serial.println("Access Denied.");
        Serial.println("\nPress * to enter new PIN or # to access the system.");
    }
  }

  else if (button_character == '*') {
    giveInputFeedback();
    bool access_allowed = validatePIN();
    
    if (access_allowed) {
        displayColor(128, 80, 0);
        Serial.println("Welcome, authorized user. Please enter a new PIN: ");

        for (int i = 0; i < PIN_LENGTH; i++){
            password[i] = heroKeypad.waitForKey();

            if( i < (PIN_LENGTH - 1)){
              giveInputFeedback();
              displayColor(128, 80, 0); //amber color
            }

            Serial.print("*");
        }
        Serial.println();
        Serial.println("PIN successfully Changed!");  
        giveSuccessFeedback();      
    } 
    else {
        Serial.println("Access Denied! Cannot change PIN without the old or default.");
        Serial.println("\nPress * to enter new PIN or # to access the system.");
    }
  }
  else{
    Serial.print(button_character);
    Serial.println("This does nothing.");
    Serial.println("Push # or * to log in or change your password.");
  }

}

void displayColor(byte red_intensity, byte green_intensity, byte blue_intensity){
  analogWrite(RED_PIN, red_intensity);
  analogWrite(GREEN_PIN, green_intensity);
  analogWrite(BLUE_PIN, blue_intensity);
}

bool validatePIN(){
    Serial.println("Enter PIN to continue.");

    for (int i =0; i < PIN_LENGTH; i++){
        char button_character = heroKeypad.waitForKey();
        
        if (password[i] != button_character){
            giveErrorFeedback();
            Serial.println();
            Serial.print("WRONG PIN DIGIT: ");
            Serial.println(button_character);
            return false;
        }
        if (i < (PIN_LENGTH - 1)){
          giveInputFeedback();
        }
        Serial.print("*");
    }

    giveSuccessFeedback();
    Serial.println();
    Serial.println("Device Successfully unlocked!");
    return true;
}

void giveInputFeedback(){
  displayColor(0,0,0);
  tone(BUZZER_PIN, 880, 100);
  delay(200);
  displayColor(0,0,128); //blue color
}

void giveSuccessFeedback(){
  displayColor(0,0,0);
  play_song(); //add mario song to tab
  delay(200);
  displayColor(0, 128, 0); // green color

}

void giveErrorFeedback(){
  displayColor(0, 0, 0);
  tone(BUZZER_PIN, 300, 200);
  delay(200);

  tone(BUZZER_PIN, 200, 500);
  delay(500);
  displayColor(128, 0, 0); //red color
}
