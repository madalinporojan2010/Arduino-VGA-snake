#define ledpin 13

#define R4_PIN 48

#define C1_PIN 47
#define C2_PIN 46
#define C3_PIN 45
#define C4_PIN 44

#define R1_MOVEMENT_PIN R4_PIN

#define C1_MOVEMENT_PIN C4_PIN
#define C2_MOVEMENT_PIN C3_PIN
#define C3_MOVEMENT_PIN C2_PIN
#define C4_MOVEMENT_PIN C1_PIN

unsigned int UP, DOWN, LEFT, RIGHT;
char key;

void setup() {
  pinMode(R1_MOVEMENT_PIN, OUTPUT);
  pinMode(R1_MOVEMENT_PIN, HIGH);

  pinMode(C1_MOVEMENT_PIN, INPUT_PULLUP);
  pinMode(C2_MOVEMENT_PIN, INPUT_PULLUP);
  pinMode(C3_MOVEMENT_PIN, INPUT_PULLUP);
  pinMode(C4_MOVEMENT_PIN, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  key = readKeys(C1_MOVEMENT_PIN, C2_MOVEMENT_PIN, C3_MOVEMENT_PIN, C4_MOVEMENT_PIN);
  if (key) {
    Serial.println(key);
  }
}

char readKeys(byte C1, byte C2, byte C3, byte C4) {
  UP = digitalRead(C1);
  DOWN = digitalRead(C2);
  LEFT = digitalRead(C3);
  RIGHT = digitalRead(C4);

  if (UP == 0) {
    return '^';
  } else if (DOWN == 0) {
    return '_';
  } else if (LEFT == 0) {
    return '<';
  } else if (RIGHT == 0) {
    return '>';
  } else {
    return NULL;
  }
}
