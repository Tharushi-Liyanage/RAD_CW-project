#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// Define valves
#define VALVE_JUICE1 10
#define VALVE_JUICE2 11
#define VALVE_SUGAR  12
#define VALVE_WATER  13

// Define timing for ingredients
#define TIME_JUICE1  5000
#define TIME_JUICE2  5000
#define TIME_SUGAR   1500
#define TIME_WATER   8000

// LCD and Keypad setup
LiquidCrystal_I2C lcd(0x27, 16, 2);
const byte ROWS = 4;
const byte COLS = 4;
char keyLayout[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};
Keypad keypad = Keypad(makeKeymap(keyLayout), rowPins, colPins, ROWS, COLS);

// --- Setup hardware ---
void initializeValves() {
  int pins[] = {VALVE_JUICE1, VALVE_JUICE2, VALVE_SUGAR, VALVE_WATER};
  for (int i = 0; i < 4; i++) {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], HIGH); // inactive (relay off)
  }
}

// --- Open a valve for a given time ---
void activateValve(int pin, int duration) {
  digitalWrite(pin, LOW); // open
  delay(duration);
  digitalWrite(pin, HIGH); // close
}

// --- Create a drink based on ID ---
void makeDrink(char choice) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Preparing...");

  if (choice == '1') {
    activateValve(VALVE_JUICE1, TIME_JUICE1);
    lcd.setCursor(0, 1);
    lcd.print("Juice 1 Mode");
  } 
  else if (choice == '2') {
    activateValve(VALVE_JUICE2, TIME_JUICE2);
    lcd.setCursor(0, 1);
    lcd.print("Juice 2 Mode");
  } 
  else if (choice == '3') {
    activateValve(VALVE_JUICE1, TIME_JUICE1 / 2);
    activateValve(VALVE_JUICE2, TIME_JUICE2 / 2);
    lcd.setCursor(0, 1);
    lcd.print("Mixed Juice");
  } 
  else {
    lcd.clear();
    lcd.print("Invalid Input");
    delay(1000);
    showMenu();
    return;
  }

  // Add sugar and water for all drinks
  activateValve(VALVE_SUGAR, TIME_SUGAR);
  activateValve(VALVE_WATER, TIME_WATER);

  // Final message
  lcd.clear();
  lcd.print("Mix to finish");
  delay(3000);
  lcd.clear();
  lcd.print("Drink is Ready!");
  delay(2000);
  showMenu();
}

// --- Display menu ---
void showMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Choose Your Drink");
  lcd.setCursor(0, 1);
  lcd.print("1=J1 2=J2 3=Mix");
}

void setup() {
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Slushies Ready!");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  initializeValves();
  showMenu();
}

void loop() {
  char input = keypad.getKey();
  if (input) {
    lcd.clear();
    lcd.print("Option: ");
    lcd.print(input);
    delay(1000);
    makeDrink(input);  // Call based on char input
  }
}

