#include <VGAX.h>
#include <Keypad.h>

// uncomment vgax.h mga2560 define
#define REFRESH_RATE 6
#define WINDOW_BOUNDRY_SIZE_X 5
#define WINDOW_BOUNDRY_SIZE_Y 5
#define WINDOW_BOUNDRY_COLOR 2  // 0-black, 1-blue, 2-red, 3-white

#define SNAKE_WIDTH 5
#define SNAKE_HEIGHT 5
#define SNAKE_MAX_POSX (VGAX_WIDTH - SNAKE_WIDTH - WINDOW_BOUNDRY_SIZE_X)
#define SNAKE_MAX_POSY (VGAX_HEIGHT - SNAKE_HEIGHT - WINDOW_BOUNDRY_SIZE_Y)
#define SNAKE_ARRAY_SIZE ((VGAX_WIDTH / SNAKE_WIDTH) * (VGAX_HEIGHT / SNAKE_HEIGHT))
#define SNAKE_INITIAL_SIZE 5  // less than 15
#define SNAKE_INITIAL_X 10
#define SNAKE_INITIAL_Y 10
#define SNAKE_INITIAL_HEAD_COLOR 0

enum colors_enum {
  BLACK = 0,
  BLUE,
  RED,
  WHITE
};

struct snake_type {
  byte posX, posY, width, height;
  byte color;
};

struct SnakeArray {
  byte size;
  snake_type snakePart[SNAKE_ARRAY_SIZE];  // snake head at index 0
  SnakeArray()
    : size(0) {}
  void addLast(snake_type part) {
    if (size < SNAKE_ARRAY_SIZE - 1) {
      snakePart[size] = part;
      size++;
    }
  }
};

const byte rows = 3;
const byte cols = 2;
char keys[rows][cols] = {
  { '*', '>' },
  { '^', '_' },
  { '#', '<' }
};

VGAX vga;

byte rowPins[rows] = { 48, 47, 46 };  // btn matrix pins
byte colPins[cols] = { 44, 45 };
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

unsigned int rhCounter = 0;

SnakeArray snake = SnakeArray();


void setup() {
  //mock up snake:
  snake_type initial_snake_body[5];

  for (byte i = 0; i < SNAKE_INITIAL_SIZE; i++) {
    if (i == 0) {
      initial_snake_body[i].color = SNAKE_INITIAL_HEAD_COLOR;  // snake head
    } else {
      initial_snake_body[i].color = BLUE;
    }

    initial_snake_body[i].posX = i * SNAKE_WIDTH + SNAKE_INITIAL_X;
    initial_snake_body[i].posY = SNAKE_INITIAL_Y;
    initial_snake_body[i].height = SNAKE_HEIGHT;
    initial_snake_body[i].width = SNAKE_WIDTH;

    snake.addLast(initial_snake_body[i]);
  }

  //display
  vga.begin();
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    switch (key) {
      case '#':  // play music
        break;
      case '*':  // stop music
        break;
      case '^':  // SNAKE_UP
        break;
      case '_':  // SNAKE_DOWN
        break;
      case '<':  // SNAKE_LEFT
        break;
      case '>':  // SNAKE_RIGHT
        break;
      default:
        break;
    }
  }

  draw();


  rhCounter++;
  if (rhCounter >= REFRESH_RATE) {
    rhCounter = 0;
    vga.clear(3);
  }
}

void draw() {
  drawWindowBoundry();
  drawSnake();
}

void drawWindowBoundry() {
  vga.fillrect(0, 0, VGAX_WIDTH, WINDOW_BOUNDRY_SIZE_X, WINDOW_BOUNDRY_COLOR);                                                               // up
  vga.fillrect(0, VGAX_HEIGHT - WINDOW_BOUNDRY_SIZE_Y, VGAX_WIDTH, WINDOW_BOUNDRY_SIZE_X, WINDOW_BOUNDRY_COLOR);                             // down
  vga.fillrect(0, WINDOW_BOUNDRY_SIZE_Y, WINDOW_BOUNDRY_SIZE_X, VGAX_HEIGHT - WINDOW_BOUNDRY_SIZE_Y, WINDOW_BOUNDRY_COLOR);                  // left
  vga.fillrect(VGAX_WIDTH - WINDOW_BOUNDRY_SIZE_X, WINDOW_BOUNDRY_SIZE_Y, WINDOW_BOUNDRY_SIZE_X, VGAX_HEIGHT - WINDOW_BOUNDRY_SIZE_Y, RED);  // right
}

void drawSnake() {
  for (byte i = 0; i < snake.size; i++) {
    vga.fillrect(snake.snakePart[i].posX, snake.snakePart[i].posY, snake.snakePart[i].width, snake.snakePart[i].height, snake.snakePart[i].color);
  }
}