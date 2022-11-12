# 1 "c:\\Users\\Madalin\\Documents\\GitHub\\Arduino-VGA-snake\\top-module\\top-module.ino"
# 2 "c:\\Users\\Madalin\\Documents\\GitHub\\Arduino-VGA-snake\\top-module\\top-module.ino" 2

// uncomment vgax.h mga2560 define
// VGA DISPLAY





// MOVEMENT
# 26 "c:\\Users\\Madalin\\Documents\\GitHub\\Arduino-VGA-snake\\top-module\\top-module.ino"
// SNAKE PROPERTIES
# 39 "c:\\Users\\Madalin\\Documents\\GitHub\\Arduino-VGA-snake\\top-module\\top-module.ino"
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
    snake_type snakePart[(((30 /*number of bytes in a row*/*4) /*number of pixels in a row*/ / 5) * (80 /*number of lines*/ / 5))]; // snake head at index 0
    SnakeArray()
        : size(0) {}
    void addLast(snake_type part) {
        if (size < (((30 /*number of bytes in a row*/*4) /*number of pixels in a row*/ / 5) * (80 /*number of lines*/ / 5)) - 1) {
            snakePart[size] = part;
            size++;
        }
    }
};

const byte rows = 3;
const byte cols = 2;
char keys[rows][cols] = {
    {'*', '>'},
    {'^', '_'},
    {'#', '<'}};

// display
VGAX vga;
unsigned int rhCounter = 0;

// movement
unsigned int UP_READ, DOWN_READ, LEFT_READ, RIGHT_READ;
char key;

// snake
SnakeArray snake = SnakeArray();

void setup() {
    // movement pins
    pinMode(48, 0x1);
    pinMode(48, 0x1);

    pinMode(44, 0x2);
    pinMode(45, 0x2);
    pinMode(46, 0x2);
    pinMode(47, 0x2);

    // mock up snake:
    snake_type initial_snake_body[5];

    for (byte i = 0; i < 5 /* less than 15*/; i++) {
        if (i == 0) {
            initial_snake_body[i].color = 0; // snake head
        } else {
            initial_snake_body[i].color = BLUE;
        }

        initial_snake_body[i].posX = i * 5 + 10;
        initial_snake_body[i].posY = 10;
        initial_snake_body[i].height = 5;
        initial_snake_body[i].width = 5;

        snake.addLast(initial_snake_body[i]);
    }

    // movement
    dir = DOWN;

    // display
    vga.begin();
}

void loop() {
    key = readKeys(44, 45, 46, 47);
    if (key != '\0') {
        switch (key) {
            case '#': // play music
                break;
            case '*': // stop music
                break;
            default:
                moveLogic(key);
                break;
        }
    }

    draw();

    rhCounter++;
    if (rhCounter >= 2) {
        rhCounter = 0;

        // snake movement
        switch (dir) {
            case UP:
                if (snake.snakePart[0].posY >= 5 + 5) {
                    moveSnake(0, -1);
                }
                break;
            case DOWN:
                if (snake.snakePart[0].posY <= (80 /*number of lines*/ - 5 - 5) - 5) {
                    moveSnake(0, 1);
                }
                break;
            case LEFT:
                if (snake.snakePart[0].posX >= 5 + 5) {
                    moveSnake(-1, 0);
                }
                break;
            case RIGHT:
                if (snake.snakePart[0].posX <= ((30 /*number of bytes in a row*/*4) /*number of pixels in a row*/ - 5 - 5) - 5) {
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
}

void drawWindowBoundries() {
    vga.fillrect(0, 0, (30 /*number of bytes in a row*/*4) /*number of pixels in a row*/, 5, 2 /* 0-black, 1-blue, 2-red, 3-white*/); // up
    vga.fillrect(0, 80 /*number of lines*/ - 5, (30 /*number of bytes in a row*/*4) /*number of pixels in a row*/, 5, 2 /* 0-black, 1-blue, 2-red, 3-white*/); // down
    vga.fillrect(0, 5, 5, 80 /*number of lines*/ - 5, 2 /* 0-black, 1-blue, 2-red, 3-white*/); // left
    vga.fillrect((30 /*number of bytes in a row*/*4) /*number of pixels in a row*/ - 5, 5, 5, 80 /*number of lines*/ - 5, RED); // right
}

void drawSnake() {
    for (byte i = 0; i < snake.size; i++) {
        vga.fillrect(snake.snakePart[i].posX, snake.snakePart[i].posY, snake.snakePart[i].width, snake.snakePart[i].height, snake.snakePart[i].color);
    }
}

char readKeys(byte C1, byte C2, byte C3, byte C4) {
    UP_READ = digitalRead(C1);
    DOWN_READ = digitalRead(C2);
    LEFT_READ = digitalRead(C3);
    RIGHT_READ = digitalRead(C4);

    if (UP_READ == 0) {
        return '^'; // read up
    } else if (DOWN_READ == 0) {
        return '_'; // read down
    } else if (LEFT_READ == 0) {
        return '<'; // read left
    } else if (RIGHT_READ == 0) {
        return '>'; // read right
    } else {
        return '\0';
    }
}

void moveLogic(char key) {
    switch (key) {
        case '^': // up
            if (dir != DOWN && dir != UP) {
                if (snake.snakePart[0].posY >= 5 + 5) {
                    dir = UP;
                }
            }
            break;
        case '_': // down
            if (dir != DOWN && dir != UP) {
                if (snake.snakePart[0].posY <= (80 /*number of lines*/ - 5 - 5) + 5) {
                    dir = DOWN;
                }
            }
            break;
        case '<': // left
            if (dir != LEFT && dir != RIGHT) {
                if (snake.snakePart[0].posX >= 5 + 5) {
                    dir = LEFT;
                }
            }
            break;
        case '>': // right
            if (dir != LEFT && dir != RIGHT) {
                if (snake.snakePart[0].posX <= ((30 /*number of bytes in a row*/*4) /*number of pixels in a row*/ - 5 - 5) + 5) {
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
    snake.snakePart[0].posX = snake.snakePart[0].posX + 5 * dx;
    snake.snakePart[0].posY = snake.snakePart[0].posY + 5 * dy;
}