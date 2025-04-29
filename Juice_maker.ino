#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

#define JUICE1 10
#define JUICE2 11
#define SUGAR 12
#define WATER 13

#define JUICE1_TIME_100ML 4500  // 4.5 seconds for 100ml
#define JUICE2_TIME_100ML 4800  // 4.8 seconds for 100ml
#define SUGAR_TIME_30ML   1300  // 1.3 seconds for 30ml
#define WATER_TIME_200ML  7500  // 7.5 seconds for 200ml

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hello Slushies!");
  lcd.setCursor(0, 1);
  lcd.print("Juice Machine");
  delay(2500);
  lcd.clear();
  lcd.print("Select a Drink:");

  pinMode(JUICE1, OUTPUT);
  pinMode(JUICE2, OUTPUT);
  pinMode(SUGAR, OUTPUT);
  pinMode(WATER, OUTPUT);

  digitalWrite(JUICE1, HIGH);
  digitalWrite(JUICE2, HIGH);
  digitalWrite(WATER, HIGH);
  digitalWrite(SUGAR, HIGH);
}

void dispense(int juice1_time, int juice2_time, int sugar_time, int water_time) {
  lcd.clear();
  lcd.print("Dispensing...");

  if (juice1_time > 0) {
    digitalWrite(JUICE1, LOW);
    delay(juice1_time);
    digitalWrite(JUICE1, HIGH);
  }

  if (juice2_time > 0) {
    digitalWrite(JUICE2, LOW);
    delay(juice2_time);
    digitalWrite(JUICE2, HIGH);
  }

  if (sugar_time > 0) {
    digitalWrite(SUGAR, LOW);
    delay(sugar_time);
    digitalWrite(SUGAR, HIGH);
  }

  if (water_time > 0) {
    digitalWrite(WATER, LOW);
    delay(water_time);
    digitalWrite(WATER, HIGH);
  }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Press Switch");
  lcd.setCursor(0,1);
  lcd.print("to Mix");
  delay(4000);
  lcd.clear();
  lcd.print("Drink is Ready!");
  delay(2000);
  lcd.clear();
  lcd.print("Select a Drink:");
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    lcd.clear();
    delay(200); 
    if (key == '1') {
      lcd.print("Juice 1 Selected");
      delay(1000);
      dispense(JUICE1_TIME_100ML, 0, SUGAR_TIME_30ML, WATER_TIME_200ML);
    } 
    else if (key == '2') {
      lcd.print("Juice 2 Selected");
      delay(1000);
      dispense(0, JUICE2_TIME_100ML, SUGAR_TIME_30ML, WATER_TIME_200ML);
    } 
    else if (key == '3') {
      lcd.print("Mix Selected");
      delay(1000);
      dispense(JUICE1_TIME_100ML / 2, JUICE2_TIME_100ML / 2, SUGAR_TIME_30ML, WATER_TIME_200ML);
    } 
    else {
      lcd.print("Invalid Choice");
      delay(1000);
      lcd.clear();
      lcd.print("Select a Drink:");
    }
  }
}
