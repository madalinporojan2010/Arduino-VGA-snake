#include <Keypad.h>


#define ledpin 13


const byte rows = 3;
const byte cols = 2;
char keys[rows][cols] = {
  {'*','>'},
  {'^','_'},
  {'#','<'} 
};

byte rowPins[rows] = {48, 47, 46};
byte colPins[cols] = {44, 45};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

void setup() { 
  pinMode(ledpin,OUTPUT);
  digitalWrite(ledpin, HIGH);
  Serial.begin(9600); 
}

void loop() {
  // put your main code here, to run repeatedly:
  char key = keypad.getKey();
    if(key)  // Check for a valid key.
    {
      switch (key)
      {
        case '*':
          digitalWrite(ledpin, LOW);
          Serial.println(key);
          break;
        case '#':
          digitalWrite(ledpin, HIGH);
          Serial.println(key);
          break;
        default:
          Serial.println(key);
      }
    }
}
