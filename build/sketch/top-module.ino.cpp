#include <Arduino.h>
#line 1 "c:\\Users\\Madalin\\Documents\\GitHub\\Arduino-VGA-snake\\top-module\\top-module.ino"
#include <VGAX.h>

// uncomment vgax.h mga2560 define
// VGA DISPLAY
#define REFRESH_RATE 10
#define WINDOW_BOUNDRY_SIZE_X 5
#define WINDOW_BOUNDRY_SIZE_Y 5
#define WINDOW_BOUNDRY_COLOR 2  // 0-black, 1-blue, 2-red, 3-white

// RANDOM SEED / FOOD

#define IMG_FOOD_WIDTH 5
#define IMG_FOOD_BWIDTH 2
#define IMG_FOOD_HEIGHT 5
#define IMG_FOOD_SPRITES_CNT 4

// MOVEMENT

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

// SNAKE PROPERTIES
#define SNAKE_WIDTH 5
#define SNAKE_HEIGHT 5
#define SNAKE_MAX_POS_UP WINDOW_BOUNDRY_SIZE_X
#define SNAKE_MAX_POS_DOWN (VGAX_HEIGHT - SNAKE_HEIGHT - WINDOW_BOUNDRY_SIZE_Y)
#define SNAKE_MAX_POS_LEFT WINDOW_BOUNDRY_SIZE_Y
#define SNAKE_MAX_POS_RIGHT (VGAX_WIDTH - SNAKE_WIDTH - WINDOW_BOUNDRY_SIZE_X)
#define SNAKE_ARRAY_SIZE ((VGAX_WIDTH / SNAKE_WIDTH) * (VGAX_HEIGHT / SNAKE_HEIGHT))
#define SNAKE_INITIAL_SIZE 5  // less than 15
#define SNAKE_INITIAL_X 10
#define SNAKE_INITIAL_Y 10
#define SNAKE_INITIAL_HEAD_COLOR 0

enum COLORS_ENUM {
    BLACK = 0,
    BLUE,
    RED,
    WHITE
};

enum SNAKE_DIRECTION {
    UP,
    DOWN,
    LEFT,
    RIGHT
} dir;

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

// display
VGAX vga;
unsigned int rhCounter = 0;

// movement
unsigned int UP_READ, DOWN_READ, LEFT_READ, RIGHT_READ;
char key;

// snake
SnakeArray snake = SnakeArray();

// food
//image generated from 2BITIMAGE - by Sandro Maffiodo
//data size=40 bytes
const unsigned char img_food_data[IMG_FOOD_SPRITES_CNT][IMG_FOOD_HEIGHT][IMG_FOOD_BWIDTH] PROGMEM={
{ { 0xff, 0xc0, }, { 0xff, 0xc0, }, { 0xf7, 0xc0, }, { 0xea, 0xc0, }, { 0xea, 0xc0, }, },
{ { 0xff, 0xc0, }, { 0xf7, 0xc0, }, { 0xea, 0xc0, }, { 0xea, 0xc0, }, { 0xff, 0xc0, }, },
{ { 0xf7, 0xc0, }, { 0xea, 0xc0, }, { 0xea, 0xc0, }, { 0xff, 0xc0, }, { 0xff, 0xc0, }, },
{ { 0xff, 0xc0, }, { 0xf7, 0xc0, }, { 0xea, 0xc0, }, { 0xea, 0xc0, }, { 0xff, 0xc0, }, }
};
static byte foodSidx = 0;

#line 100 "c:\\Users\\Madalin\\Documents\\GitHub\\Arduino-VGA-snake\\top-module\\top-module.ino"
void setup();
#line 137 "c:\\Users\\Madalin\\Documents\\GitHub\\Arduino-VGA-snake\\top-module\\top-module.ino"
void loop();
#line 187 "c:\\Users\\Madalin\\Documents\\GitHub\\Arduino-VGA-snake\\top-module\\top-module.ino"
void draw();
#line 193 "c:\\Users\\Madalin\\Documents\\GitHub\\Arduino-VGA-snake\\top-module\\top-module.ino"
void drawWindowBoundries();
#line 200 "c:\\Users\\Madalin\\Documents\\GitHub\\Arduino-VGA-snake\\top-module\\top-module.ino"
void drawSnake();
#line 207 "c:\\Users\\Madalin\\Documents\\GitHub\\Arduino-VGA-snake\\top-module\\top-module.ino"
void drawFood();
#line 213 "c:\\Users\\Madalin\\Documents\\GitHub\\Arduino-VGA-snake\\top-module\\top-module.ino"
char readKeys(byte C1, byte C2, byte C3, byte C4);
#line 232 "c:\\Users\\Madalin\\Documents\\GitHub\\Arduino-VGA-snake\\top-module\\top-module.ino"
void moveLogic(char key);
#line 267 "c:\\Users\\Madalin\\Documents\\GitHub\\Arduino-VGA-snake\\top-module\\top-module.ino"
void moveSnake(byte dx, byte dy);
#line 100 "c:\\Users\\Madalin\\Documents\\GitHub\\Arduino-VGA-snake\\top-module\\top-module.ino"
void setup() {

    // movement pins
    pinMode(R1_MOVEMENT_PIN, OUTPUT);
    pinMode(R1_MOVEMENT_PIN, HIGH);

    pinMode(C1_MOVEMENT_PIN, INPUT_PULLUP);
    pinMode(C2_MOVEMENT_PIN, INPUT_PULLUP);
    pinMode(C3_MOVEMENT_PIN, INPUT_PULLUP);
    pinMode(C4_MOVEMENT_PIN, INPUT_PULLUP);

    // mock up snake:
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

    // movement
    dir = DOWN;

    // display
    vga.begin();
    vga.clear(11);
}

void loop() {
    key = readKeys(C1_MOVEMENT_PIN, C2_MOVEMENT_PIN, C3_MOVEMENT_PIN, C4_MOVEMENT_PIN);
    if (key != '\0') {
        switch (key) {
            case '#':  // play music
                break;
            case '*':  // stop music
                break;
            default:
                moveLogic(key);
                break;
        }
    }

    draw();

    rhCounter++;
    if (rhCounter >= REFRESH_RATE) {
        rhCounter = 0;

        // snake movement
        switch (dir) {
            case UP:
                if (snake.snakePart[0].posY >= SNAKE_MAX_POS_UP + WINDOW_BOUNDRY_SIZE_Y) {
                    moveSnake(0, -1);
                }
                break;
            case DOWN:
                if (snake.snakePart[0].posY <= SNAKE_MAX_POS_DOWN - WINDOW_BOUNDRY_SIZE_Y) {
                    moveSnake(0, 1);
                }
                break;
            case LEFT:
                if (snake.snakePart[0].posX >= SNAKE_MAX_POS_LEFT + WINDOW_BOUNDRY_SIZE_X) {
                    moveSnake(-1, 0);
                }
                break;
            case RIGHT:
                if (snake.snakePart[0].posX <= SNAKE_MAX_POS_RIGHT - WINDOW_BOUNDRY_SIZE_X) {
                    moveSnake(1, 0);
                }
                break;
            default:
                break;
        }

        vga.clear(3);
    }
}

void draw() {
    drawWindowBoundries();
    drawSnake();
    drawFood();
}

void drawWindowBoundries() {
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


void drawFood() {
    vga.blit((byte*)(img_food_data[foodSidx]), IMG_FOOD_WIDTH, IMG_FOOD_HEIGHT, 40, 40);
    foodSidx = (foodSidx + 1) % 4;
}


char readKeys(byte C1, byte C2, byte C3, byte C4) {
    UP_READ = digitalRead(C1);
    DOWN_READ = digitalRead(C2);
    LEFT_READ = digitalRead(C3);
    RIGHT_READ = digitalRead(C4);

    if (UP_READ == 0) {
        return '^';  // read up
    } else if (DOWN_READ == 0) {
        return '_';  // read down
    } else if (LEFT_READ == 0) {
        return '<';  // read left
    } else if (RIGHT_READ == 0) {
        return '>';  // read right
    } else {
        return '\0';
    }
}

void moveLogic(char key) {
    switch (key) {
        case '^':  // up
            if (dir != DOWN && dir != UP) {
                if (snake.snakePart[0].posY >= SNAKE_MAX_POS_UP + WINDOW_BOUNDRY_SIZE_Y) {
                    dir = UP;
                }
            }
            break;
        case '_':  // down
            if (dir != DOWN && dir != UP) {
                if (snake.snakePart[0].posY <= SNAKE_MAX_POS_DOWN + WINDOW_BOUNDRY_SIZE_Y) {
                    dir = DOWN;
                }
            }
            break;
        case '<':  // left
            if (dir != LEFT && dir != RIGHT) {
                if (snake.snakePart[0].posX >= SNAKE_MAX_POS_LEFT + WINDOW_BOUNDRY_SIZE_X) {
                    dir = LEFT;
                }
            }
            break;
        case '>':  // right
            if (dir != LEFT && dir != RIGHT) {
                if (snake.snakePart[0].posX <= SNAKE_MAX_POS_RIGHT + WINDOW_BOUNDRY_SIZE_X) {
                    dir = RIGHT;
                }
            }
            break;
        default:
            return;
    }
}

void moveSnake(byte dx, byte dy) {
    for (byte i = snake.size - 1; i > 0; i--) {
        byte oldColor = snake.snakePart[i].color;
        snake.snakePart[i] = snake.snakePart[i - 1];
        snake.snakePart[i].color = oldColor;
    }
    snake.snakePart[0].posX = snake.snakePart[0].posX + SNAKE_WIDTH * dx;
    snake.snakePart[0].posY = snake.snakePart[0].posY + SNAKE_HEIGHT * dy;
}

