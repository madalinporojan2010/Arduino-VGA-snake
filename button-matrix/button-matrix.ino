#include <Keypad.h>


#define ledpin 13


const byte rows = 2;
const byte cols = 3;
char keys[rows][cols] = {
  {'>','_','<'}, //INVERTED LINES
  {'*','^','#'}  //AND INVERTED COLLUMS
};

byte rowPins[rows] = {46, 47};
byte colPins[cols] = {42, 43, 44};

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
          break;
        case '#':
          digitalWrite(ledpin, HIGH);
          break;
        default:
          Serial.println(key);
      }
    }
}
