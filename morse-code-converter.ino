#include <LiquidCrystal.h>
#define BUTTON 7
#define CLEARBUTTON 8
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
unsigned long currTime = 0;
unsigned long pressTime = 0;
unsigned long pressDuration = 0;
bool oldState = HIGH;
bool buttonState;
bool longPress = false;
int intervalButton = 50;
unsigned long startBreak;
int startPoint = 0;
unsigned long endBreak = 0;
unsigned long previousTime = 0;
////clear button
char start = 'f';
int position = 2;
bool clearState = LOW;
unsigned long pressedTime = 0;
unsigned long releasedTime = 0;
int lastState = HIGH;
String code = "";
byte customChar0[] = {
  B01100,
  B11110,
  B11111,
  B00011,
  B11111,
  B11111,
  B11110,
  B10010
};
byte customChar1[] = {
  B00110,
  B01111,
  B11111,
  B11000,
  B11111,
  B11111,
  B01111,
  B01001
};
void setup() {
  lcd.begin(16, 2);
  lcd.createChar(2, customChar0);
  lcd.createChar(1, customChar1);
  lcd.setCursor(0, 0);
  lcd.write(1);
  lcd.setCursor(15, 0);
  lcd.write(2);
  lcd.setCursor(2, 0);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(CLEARBUTTON, INPUT_PULLUP);
}
void loop() {
  currTime = millis();
  pressButton();
}
void morse() {
  static String morsecode[] = { "01", "1000", "1010", "100", "0", "0010", "110", "0000", "00", "0111", "101", "0100", "11", "10", "111", "0110", "1101", "010", "000", "1", "001", "0001", "011", "1001", "1011", "1100", "!" };
  int i = 0;
  while (morsecode[i] != "!") {
    if (morsecode[i] == code) {
      if (position < 14){
        lcd.setCursor(position,0);
      } else {
        lcd.setCursor(position - 12,1);
      }
      lcd.print(char('A' + i));
      position++;
      break;
    }
    i++;
  }
  if (morsecode[i] == "!"){
    lcd.setCursor(position, 0);
    lcd.print("ERROR");
    position += 5;
  }
  code = "";
}
void pressButton() {
  if (currTime - previousTime > intervalButton) {
    buttonState = digitalRead(BUTTON);
    clearState = digitalRead(CLEARBUTTON);
    if (lastState == HIGH && clearState == LOW){  // button is pressed
      pressedTime = millis();
      start = 't';
    }
    else if (lastState == LOW && clearState == HIGH) {  // button is released
      releasedTime = millis();
      
      unsigned long pressDuration = releasedTime - pressedTime;
      if (start == 't'){
      if (pressDuration < 800) {
        if (position > 2){
        lcd.setCursor(position - 1,0);
        lcd.write(" ");
        position--;
        }
      } else {
         lcd.clear();
         lcd.setCursor(0, 0);
         lcd.write(1);
         lcd.setCursor(15, 0);
         lcd.write(2);
         lcd.setCursor(2, 0);
         position = 2;
      }
      }
    }
      endBreak = millis();
      if (endBreak - startBreak > 1000 && startPoint == 2) {
        startPoint = 0;
        morse();
      }
      if (buttonState == LOW && oldState == HIGH && longPress == false) {
        pressTime = currTime;
        oldState = LOW;
      }
      pressDuration = currTime - pressTime;
      if (buttonState == LOW && longPress == false && pressDuration >= 200) {
        longPress = true;
        code += '1';
        startPoint = 1;
      }
      if (buttonState == HIGH && oldState == LOW) {
        oldState = HIGH;
        longPress = false;
        if (pressDuration >= 50 && pressDuration < 200) {
          code += '0';
          startPoint = 1;
        }
        if (startPoint == 1) {
          startBreak = currTime;
          startPoint = 2;
        }
      }
      previousTime = currTime;
  }
  lastState = clearState;
}
