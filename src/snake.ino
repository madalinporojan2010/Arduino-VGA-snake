#include <VGAX.h>

// uncomment vgax.h mga2560 define
// VGA DISPLAY
#define INITIAL_SPEED 7
#define WINDOW_BOUNDRY_SIZE_X 5
#define WINDOW_BOUNDRY_SIZE_Y 5
#define WINDOW_BOUNDRY_COLOR 2  // 0-black, 1-blue, 2-red, 3-white

// FONT
#define FNT_UFONT_HEIGHT 6
#define FNT_UFONT_SYMBOLS_COUNT 95

#define SCORE_DIGIT_NUMBER 4

// RANDOM SEED / FOOD SPRITES
#define MIN_TIME_FOOD_RESPAWN 10000
#define MAX_TIME_FOOD_RESPAWN 50000

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
#define SNAKE_MAX_POS_UP 0
#define SNAKE_MAX_POS_DOWN (VGAX_HEIGHT - SNAKE_HEIGHT)
#define SNAKE_MAX_POS_LEFT 0
#define SNAKE_MAX_POS_RIGHT (VGAX_WIDTH - SNAKE_WIDTH)
#define SNAKE_ARRAY_SIZE ((VGAX_WIDTH / SNAKE_WIDTH) * (VGAX_HEIGHT / SNAKE_HEIGHT))
#define SNAKE_INITIAL_SIZE 5  // less than 15
#define SNAKE_INITIAL_X 10
#define SNAKE_INITIAL_Y 10
#define SNAKE_INITIAL_HEAD_COLOR 0

    // SNAKE SPRITES
#define IMG_SNAKE_WIDTH 5
#define IMG_SNAKE_BWIDTH 2
#define IMG_SNAKE_HEIGHT 5
#define IMG_SNAKE_SPRITES_CNT 14

#define SNAKE_SPRITE_HEAD_UP 0
#define SNAKE_SPRITE_HEAD_DOWN 1
#define SNAKE_SPRITE_HEAD_LEFT 2
#define SNAKE_SPRITE_HEAD_RIGHT 3
#define SNAKE_SPRITE_BODY_VERTICAL 4
#define SNAKE_SPRITE_BODY_HORIZONTAL 5
#define SNAKE_SPRITE_CORNER_RIGHT_UP 6
#define SNAKE_SPRITE_CORNER_RIGHT_DOWN 7
#define SNAKE_SPRITE_CORNER_DOWN_RIGHT 8
#define SNAKE_SPRITE_CORNER_UP_RIGHT 9
#define SNAKE_SPRITE_TAIL_UP 10
#define SNAKE_SPRITE_TAIL_DOWN 11
#define SNAKE_SPRITE_TAIL_LEFT_ 12
#define SNAKE_SPRITE_TAIL_RIGHT 13


// FOOD PROPS
#define FOOD_INITIAL_X (VGAX_WIDTH - 3 * IMG_FOOD_WIDTH)
#define FOOD_INITIAL_Y (VGAX_HEIGHT - 3 * IMG_FOOD_HEIGHT)

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
} lastDir, dir;

struct snake_type {
    byte posX, posY, width, height;
    byte color;
    SNAKE_DIRECTION bodyDir;
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

struct food_type {
    byte posX, posY, width, height;
    food_type()
        : posX(FOOD_INITIAL_X), posY(FOOD_INITIAL_Y), width(IMG_FOOD_WIDTH), height(IMG_FOOD_HEIGHT) {}
};

// display
VGAX vga;
volatile unsigned int gameSpeed = INITIAL_SPEED;
volatile unsigned int rhCounter = 0;
volatile bool gameOver = false;
static const char gameOver_Message0[] PROGMEM="GAME OVER";
static const char gameOver_Message1[] PROGMEM="RESTART GAME";
static const char score_Message[] PROGMEM="SCORE:";

static const char char_0[] PROGMEM="0";
static const char char_1[] PROGMEM="1";
static const char char_2[] PROGMEM="2";
static const char char_3[] PROGMEM="3";
static const char char_4[] PROGMEM="4";
static const char char_5[] PROGMEM="5";
static const char char_6[] PROGMEM="6";
static const char char_7[] PROGMEM="7";
static const char char_8[] PROGMEM="8";
static const char char_9[] PROGMEM="9";

volatile unsigned short int score = 0;

// font
//font generated from 2BITFONT - by Sandro Maffiodo
//data size=570 bytes
const unsigned char fnt_ufont_data[FNT_UFONT_SYMBOLS_COUNT][FNT_UFONT_HEIGHT + 1] PROGMEM={
{ 1, 128, 128, 128,   0, 128,   0, }, //glyph '!' code=0
{ 3, 160, 160,   0,   0,   0,   0, }, //glyph '"' code=1
{ 5,  80, 248,  80, 248,  80,   0, }, //glyph '#' code=2
{ 5, 120, 160, 112,  40, 240,   0, }, //glyph '$' code=3
{ 5, 136,  16,  32,  64, 136,   0, }, //glyph '%' code=4
{ 5,  96, 144, 104, 144, 104,   0, }, //glyph '&' code=5
{ 2, 128,  64,   0,   0,   0,   0, }, //glyph ''' code=6
{ 2,  64, 128, 128, 128,  64,   0, }, //glyph '(' code=7
{ 2, 128,  64,  64,  64, 128,   0, }, //glyph ')' code=8
{ 3,   0, 160,  64, 160,   0,   0, }, //glyph '*' code=9
{ 3,   0,  64, 224,  64,   0,   0, }, //glyph '+' code=10
{ 2,   0,   0,   0,   0, 128,  64, }, //glyph ',' code=11
{ 3,   0,   0, 224,   0,   0,   0, }, //glyph '-' code=12
{ 1,   0,   0,   0,   0, 128,   0, }, //glyph '.' code=13
{ 5,   8,  16,  32,  64, 128,   0, }, //glyph '/' code=14
{ 4,  96, 144, 144, 144,  96,   0, }, //glyph '0' code=15
{ 3,  64, 192,  64,  64, 224,   0, }, //glyph '1' code=16
{ 4, 224,  16,  96, 128, 240,   0, }, //glyph '2' code=17
{ 4, 224,  16,  96,  16, 224,   0, }, //glyph '3' code=18
{ 4, 144, 144, 240,  16,  16,   0, }, //glyph '4' code=19
{ 4, 240, 128, 224,  16, 224,   0, }, //glyph '5' code=20
{ 4,  96, 128, 224, 144,  96,   0, }, //glyph '6' code=21
{ 4, 240,  16,  32,  64,  64,   0, }, //glyph '7' code=22
{ 4,  96, 144,  96, 144,  96,   0, }, //glyph '8' code=23
{ 4,  96, 144, 112,  16,  96,   0, }, //glyph '9' code=24
{ 1,   0, 128,   0, 128,   0,   0, }, //glyph ':' code=25
{ 2,   0, 128,   0,   0, 128,  64, }, //glyph ';' code=26
{ 3,  32,  64, 128,  64,  32,   0, }, //glyph '<' code=27
{ 3,   0, 224,   0, 224,   0,   0, }, //glyph '=' code=28
{ 3, 128,  64,  32,  64, 128,   0, }, //glyph '>' code=29
{ 4, 224,  16,  96,   0,  64,   0, }, //glyph '?' code=30
{ 4,  96, 144, 176, 128, 112,   0, }, //glyph '@' code=31
{ 4,  96, 144, 240, 144, 144,   0, }, //glyph 'A' code=32
{ 4, 224, 144, 224, 144, 224,   0, }, //glyph 'B' code=33
{ 4, 112, 128, 128, 128, 112,   0, }, //glyph 'C' code=34
{ 4, 224, 144, 144, 144, 224,   0, }, //glyph 'D' code=35
{ 4, 240, 128, 224, 128, 240,   0, }, //glyph 'E' code=36
{ 4, 240, 128, 224, 128, 128,   0, }, //glyph 'F' code=37
{ 4, 112, 128, 176, 144, 112,   0, }, //glyph 'G' code=38
{ 4, 144, 144, 240, 144, 144,   0, }, //glyph 'H' code=39
{ 3, 224,  64,  64,  64, 224,   0, }, //glyph 'I' code=40
{ 4, 240,  16,  16, 144,  96,   0, }, //glyph 'J' code=41
{ 4, 144, 160, 192, 160, 144,   0, }, //glyph 'K' code=42
{ 4, 128, 128, 128, 128, 240,   0, }, //glyph 'L' code=43
{ 5, 136, 216, 168, 136, 136,   0, }, //glyph 'M' code=44
{ 4, 144, 208, 176, 144, 144,   0, }, //glyph 'N' code=45
{ 4,  96, 144, 144, 144,  96,   0, }, //glyph 'O' code=46
{ 4, 224, 144, 224, 128, 128,   0, }, //glyph 'P' code=47
{ 4,  96, 144, 144, 144,  96,  16, }, //glyph 'Q' code=48
{ 4, 224, 144, 224, 160, 144,   0, }, //glyph 'R' code=49
{ 4, 112, 128,  96,  16, 224,   0, }, //glyph 'S' code=50
{ 3, 224,  64,  64,  64,  64,   0, }, //glyph 'T' code=51
{ 4, 144, 144, 144, 144,  96,   0, }, //glyph 'U' code=52
{ 3, 160, 160, 160, 160,  64,   0, }, //glyph 'V' code=53
{ 5, 136, 168, 168, 168,  80,   0, }, //glyph 'W' code=54
{ 4, 144, 144,  96, 144, 144,   0, }, //glyph 'X' code=55
{ 3, 160, 160,  64,  64,  64,   0, }, //glyph 'Y' code=56
{ 4, 240,  16,  96, 128, 240,   0, }, //glyph 'Z' code=57
{ 2, 192, 128, 128, 128, 192,   0, }, //glyph '[' code=58
{ 5, 128,  64,  32,  16,   8,   0, }, //glyph '\' code=59
{ 2, 192,  64,  64,  64, 192,   0, }, //glyph ']' code=60
{ 5,  32,  80, 136,   0,   0,   0, }, //glyph '^' code=61
{ 4,   0,   0,   0,   0, 240,   0, }, //glyph '_' code=62
{ 2, 128,  64,   0,   0,   0,   0, }, //glyph '`' code=63
{ 3,   0, 224,  32, 224, 224,   0, }, //glyph 'a' code=64
{ 3, 128, 224, 160, 160, 224,   0, }, //glyph 'b' code=65
{ 3,   0, 224, 128, 128, 224,   0, }, //glyph 'c' code=66
{ 3,  32, 224, 160, 160, 224,   0, }, //glyph 'd' code=67
{ 3,   0, 224, 224, 128, 224,   0, }, //glyph 'e' code=68
{ 2,  64, 128, 192, 128, 128,   0, }, //glyph 'f' code=69
{ 3,   0, 224, 160, 224,  32, 224, }, //glyph 'g' code=70
{ 3, 128, 224, 160, 160, 160,   0, }, //glyph 'h' code=71
{ 1, 128,   0,   128, 128, 128, 0, }, //glyph 'i' code=72
{ 2,   0, 192,  64,  64,  64, 128, }, //glyph 'j' code=73
{ 3, 128, 160, 192, 160, 160,   0, }, //glyph 'k' code=74
{ 1, 128, 128, 128, 128, 128,   0, }, //glyph 'l' code=75
{ 5,   0, 248, 168, 168, 168,   0, }, //glyph 'm' code=76
{ 3,   0, 224, 160, 160, 160,   0, }, //glyph 'n' code=77
{ 3,   0, 224, 160, 160, 224,   0, }, //glyph 'o' code=78
{ 3,   0, 224, 160, 160, 224, 128, }, //glyph 'p' code=79
{ 3,   0, 224, 160, 160, 224,  32, }, //glyph 'q' code=80
{ 3,   0, 224, 128, 128, 128,   0, }, //glyph 'r' code=81
{ 2,   0, 192, 128,  64, 192,   0, }, //glyph 's' code=82
{ 3,  64, 224,  64,  64,  64,   0, }, //glyph 't' code=83
{ 3,   0, 160, 160, 160, 224,   0, }, //glyph 'u' code=84
{ 3,   0, 160, 160, 160,  64,   0, }, //glyph 'v' code=85
{ 5,   0, 168, 168, 168,  80,   0, }, //glyph 'w' code=86
{ 3,   0, 160,  64, 160, 160,   0, }, //glyph 'x' code=87
{ 3,   0, 160, 160, 224,  32, 224, }, //glyph 'y' code=88
{ 2,   0, 192,  64, 128, 192,   0, }, //glyph 'z' code=89
{ 3,  96,  64, 192,  64,  96,   0, }, //glyph '{' code=90
{ 1, 128, 128, 128, 128, 128,   0, }, //glyph '|' code=91
{ 3, 192,  64,  96,  64, 192,   0, }, //glyph '}' code=92
{ 3,  96, 192,   0,   0,   0,   0, }, //glyph '~' code=93
{ 4,  48,  64, 224,  64, 240,   0, }, //glyph '£' code=94
};

// movement
unsigned int UP_READ, DOWN_READ, LEFT_READ, RIGHT_READ;
char key;

// snake
SnakeArray snake = SnakeArray();

//image generated from 2BITIMAGE - by Sandro Maffiodo
//data size=140 bytes
const unsigned char img_snake_data[IMG_SNAKE_SPRITES_CNT][IMG_SNAKE_HEIGHT][IMG_SNAKE_BWIDTH] PROGMEM={
{ { 255, 192, }, { 209, 192, }, { 200, 192, }, { 200, 192, }, { 200, 192, }, },
{ { 200, 192, }, { 200, 192, }, { 200, 192, }, { 209, 192, }, { 255, 192, }, },
{ { 255, 192, }, { 208,   0, }, { 202, 128, }, { 208,   0, }, { 255, 192, }, },
{ { 255, 192, }, {   1, 192, }, { 168, 192, }, {   1, 192, }, { 255, 192, }, },
{ { 200, 192, }, { 200, 192, }, { 200, 192, }, { 200, 192, }, { 200, 192, }, },
{ { 255, 192, }, {   0,   0, }, { 170, 128, }, {   0,   0, }, { 255, 192, }, },
{ { 200, 192, }, {   8, 192, }, { 168, 192, }, {   0, 192, }, { 255, 192, }, },
{ { 255, 192, }, {   0, 192, }, { 168, 192, }, {   8, 192, }, { 200, 192, }, },
{ { 200, 192, }, { 200,   0, }, { 202, 128, }, { 192,   0, }, { 255, 192, }, },
{ { 255, 192, }, { 192,   0, }, { 202, 128, }, { 200,   0, }, { 200, 192, }, },
{ { 200, 192, }, { 200, 192, }, { 192, 192, }, { 243, 192, }, { 255, 192, }, },
{ { 255, 192, }, { 243, 192, }, { 192, 192, }, { 200, 192, }, { 200, 192, }, },
{ { 255, 192, }, {   3, 192, }, { 160, 192, }, {   3, 192, }, { 255, 192, }, },
{ { 255, 192, }, { 240,   0, }, { 194, 128, }, { 240,   0, }, { 255, 192, }, }
};

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

food_type food = food_type();

volatile bool inCollison_Snake;
volatile byte rand_X;
volatile byte rand_Y;

volatile static unsigned lastTime0, currentTime0;  
volatile static unsigned randomTimeout0;



void setup() {

    // movement pins
    pinMode(R1_MOVEMENT_PIN, OUTPUT);
    pinMode(R1_MOVEMENT_PIN, HIGH);

    pinMode(C1_MOVEMENT_PIN, INPUT_PULLUP);
    pinMode(C2_MOVEMENT_PIN, INPUT_PULLUP);
    pinMode(C3_MOVEMENT_PIN, INPUT_PULLUP);
    pinMode(C4_MOVEMENT_PIN, INPUT_PULLUP);

    // mock up snake:
    dir = DOWN;

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
        initial_snake_body[i].bodyDir = LEFT; // initial direction used for sprites

        snake.addLast(initial_snake_body[i]);
    }

    // display
    vga.begin();
    vga.clear(WHITE);

    // TIME
    lastTime0 = vga.millis();
    randomTimeout0 = getRandomInRange(MIN_TIME_FOOD_RESPAWN, MAX_TIME_FOOD_RESPAWN);
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
    checkGameOver();


    rhCounter++;
    if (rhCounter >= gameSpeed) {
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
        redirectHead(); // comment for boundry kill
        snakeHeadCollisionWithFood();
        regenFoodAfterTimeElapsed();

        if(!gameOver) {
            vga.clear(WHITE);
        }
    }
}

void draw() {
    drawFood();
    drawSnake();
    drawWindowBoundries();
    drawGameOverText();
    drawScoreText();
}

void drawWindowBoundries() {
    vga.fillrect(0, 0, VGAX_WIDTH, WINDOW_BOUNDRY_SIZE_Y, WINDOW_BOUNDRY_COLOR);                                                               // up
    vga.fillrect(0, VGAX_HEIGHT - WINDOW_BOUNDRY_SIZE_Y, VGAX_WIDTH, WINDOW_BOUNDRY_SIZE_X, WINDOW_BOUNDRY_COLOR);                             // down
    vga.fillrect(0, WINDOW_BOUNDRY_SIZE_Y, WINDOW_BOUNDRY_SIZE_X, VGAX_HEIGHT - WINDOW_BOUNDRY_SIZE_Y, WINDOW_BOUNDRY_COLOR);                  // left
    vga.fillrect(VGAX_WIDTH - WINDOW_BOUNDRY_SIZE_X, WINDOW_BOUNDRY_SIZE_Y, WINDOW_BOUNDRY_SIZE_X, VGAX_HEIGHT - WINDOW_BOUNDRY_SIZE_Y, WINDOW_BOUNDRY_COLOR);  // right
}

void drawSnake() {
    if (!gameOver) {
        for (byte i = 0; i < snake.size; i++) {
            vga.blit((byte*)(img_snake_data[getSnakeSpriteIndex(i)]), IMG_SNAKE_WIDTH, IMG_SNAKE_HEIGHT, snake.snakePart[i].posX, snake.snakePart[i].posY);
        }
    }
}

void drawScoreText() {
    if(gameOver) {
        vga.printPROGMEM((byte*)fnt_ufont_data, FNT_UFONT_SYMBOLS_COUNT, FNT_UFONT_HEIGHT, 3, 1, score_Message, VGAX_WIDTH / 2 - 25, 44, RED);
        unsigned short int power_10 = 1;
        for(signed char i = SCORE_DIGIT_NUMBER - 1; i >= 0; i--) {
            switch((score / power_10) % 10) {
                case 0:
                    vga.printPROGMEM((byte*)fnt_ufont_data, FNT_UFONT_SYMBOLS_COUNT, FNT_UFONT_HEIGHT, 3, 1, char_0, VGAX_WIDTH / 2 + 5 + i * 5, 44, RED);
                    break;
                case 1:
                    vga.printPROGMEM((byte*)fnt_ufont_data, FNT_UFONT_SYMBOLS_COUNT, FNT_UFONT_HEIGHT, 3, 1, char_1, VGAX_WIDTH / 2 + 5 + i * 5, 44, RED);
                    break;
                case 2:
                    vga.printPROGMEM((byte*)fnt_ufont_data, FNT_UFONT_SYMBOLS_COUNT, FNT_UFONT_HEIGHT, 3, 1, char_2, VGAX_WIDTH / 2 + 5 + i * 5, 44, RED);
                    break;
                case 3:
                    vga.printPROGMEM((byte*)fnt_ufont_data, FNT_UFONT_SYMBOLS_COUNT, FNT_UFONT_HEIGHT, 3, 1, char_3, VGAX_WIDTH / 2 + 5 + i * 5, 44, RED);
                    break;
                case 4:
                    vga.printPROGMEM((byte*)fnt_ufont_data, FNT_UFONT_SYMBOLS_COUNT, FNT_UFONT_HEIGHT, 3, 1, char_4, VGAX_WIDTH / 2 + 5 + i * 5, 44, RED);
                    break;
                case 5:
                    vga.printPROGMEM((byte*)fnt_ufont_data, FNT_UFONT_SYMBOLS_COUNT, FNT_UFONT_HEIGHT, 3, 1, char_5, VGAX_WIDTH / 2 + 5 + i * 5, 44, RED);
                    break;
                case 6:
                    vga.printPROGMEM((byte*)fnt_ufont_data, FNT_UFONT_SYMBOLS_COUNT, FNT_UFONT_HEIGHT, 3, 1, char_6, VGAX_WIDTH / 2 + 5 + i * 5, 44, RED);
                    break;
                case 7:
                    vga.printPROGMEM((byte*)fnt_ufont_data, FNT_UFONT_SYMBOLS_COUNT, FNT_UFONT_HEIGHT, 3, 1, char_7, VGAX_WIDTH / 2 + 5 + i * 5, 44, RED);
                    break;
                case 8:
                    vga.printPROGMEM((byte*)fnt_ufont_data, FNT_UFONT_SYMBOLS_COUNT, FNT_UFONT_HEIGHT, 3, 1, char_8, VGAX_WIDTH / 2 + 5 + i * 5, 44, RED);
                    break;
                case 9:
                    vga.printPROGMEM((byte*)fnt_ufont_data, FNT_UFONT_SYMBOLS_COUNT, FNT_UFONT_HEIGHT, 3, 1, char_9, VGAX_WIDTH / 2 + 5 + i * 5, 44, RED);
                    break;
                default:
                    break;
            }
            power_10 *= 10;
        }
    }
}

void drawGameOverText() {
    if(gameOver) {
        vga.printPROGMEM((byte*)fnt_ufont_data, FNT_UFONT_SYMBOLS_COUNT, FNT_UFONT_HEIGHT, 3, 1, gameOver_Message0, VGAX_WIDTH / 2 - 23, 30, BLACK);
        vga.printPROGMEM((byte*)fnt_ufont_data, FNT_UFONT_SYMBOLS_COUNT, FNT_UFONT_HEIGHT, 3, 1, gameOver_Message1, VGAX_WIDTH / 2 - 30, 37, BLACK);
    }
}

byte getSnakeSpriteIndex(byte partIndex) {
    if (partIndex == 0) { // HEAD
        return SNAKE_SPRITE_HEAD_UP + snake.snakePart[partIndex].bodyDir;
    } else if (partIndex == snake.size - 1) { // TAIL
        return SNAKE_SPRITE_TAIL_UP + snake.snakePart[partIndex - 1].bodyDir;
    } else {
        if (snake.snakePart[partIndex - 1].bodyDir == snake.snakePart[partIndex].bodyDir) {
            return SNAKE_SPRITE_BODY_VERTICAL + ((snake.snakePart[partIndex].bodyDir == UP || snake.snakePart[partIndex].bodyDir == DOWN) ? 0 : 1);
        } else {
            if ((snake.snakePart[partIndex].bodyDir == RIGHT && snake.snakePart[partIndex - 1].bodyDir == UP) || (snake.snakePart[partIndex].bodyDir == DOWN && snake.snakePart[partIndex - 1].bodyDir == LEFT)) {
                return SNAKE_SPRITE_CORNER_RIGHT_UP;
            } else if ((snake.snakePart[partIndex].bodyDir == RIGHT && snake.snakePart[partIndex - 1].bodyDir == DOWN) || (snake.snakePart[partIndex].bodyDir == UP && snake.snakePart[partIndex - 1].bodyDir == LEFT)) {
                return SNAKE_SPRITE_CORNER_RIGHT_DOWN;
            } else if ((snake.snakePart[partIndex].bodyDir == DOWN && snake.snakePart[partIndex - 1].bodyDir == RIGHT) || (snake.snakePart[partIndex].bodyDir == LEFT && snake.snakePart[partIndex - 1].bodyDir == UP)) {
                return SNAKE_SPRITE_CORNER_DOWN_RIGHT;
            } else {
                return SNAKE_SPRITE_CORNER_UP_RIGHT;
            }
        }
    }
}


void drawFood() {
    if (!gameOver) {
        vga.blit((byte*)(img_food_data[foodSidx]), IMG_FOOD_WIDTH, IMG_FOOD_HEIGHT, food.posX, food.posY);
        foodSidx = (foodSidx + 1) % 4;
    }
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
                    lastDir = dir;
                    dir = UP;
                }
            }
            break;
        case '_':  // down
            if (dir != DOWN && dir != UP) {
                if (snake.snakePart[0].posY <= SNAKE_MAX_POS_DOWN + WINDOW_BOUNDRY_SIZE_Y) {
                    lastDir = dir;
                    dir = DOWN;
                }
            }
            break;
        case '<':  // left
            if (dir != LEFT && dir != RIGHT) {
                if (snake.snakePart[0].posX >= SNAKE_MAX_POS_LEFT + WINDOW_BOUNDRY_SIZE_X) {
                    lastDir = dir;
                    dir = LEFT;
                }
            }
            break;
        case '>':  // right
            if (dir != LEFT && dir != RIGHT) {
                if (snake.snakePart[0].posX <= SNAKE_MAX_POS_RIGHT + WINDOW_BOUNDRY_SIZE_X) {
                    lastDir = dir;
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
        snake.snakePart[i] = snake.snakePart[i - 1];
    }
    snake.snakePart[0].posX = snake.snakePart[0].posX + SNAKE_WIDTH * dx;
    snake.snakePart[0].posY = snake.snakePart[0].posY + SNAKE_HEIGHT * dy;
    snake.snakePart[0].bodyDir = dir;
}

void redirectHead() {
    switch(dir) {
        case UP:
            if (snake.snakePart[0].posY < WINDOW_BOUNDRY_SIZE_Y) {
                snake.snakePart[0].posY = VGAX_HEIGHT - WINDOW_BOUNDRY_SIZE_Y - SNAKE_HEIGHT;
            }
            break;
        case DOWN:
            if (snake.snakePart[0].posY > VGAX_HEIGHT - WINDOW_BOUNDRY_SIZE_Y - SNAKE_HEIGHT) {
                snake.snakePart[0].posY = WINDOW_BOUNDRY_SIZE_Y;
            }
            break;
        case LEFT:
            if (snake.snakePart[0].posX < WINDOW_BOUNDRY_SIZE_X) {
                snake.snakePart[0].posX = VGAX_WIDTH - WINDOW_BOUNDRY_SIZE_X - SNAKE_WIDTH;
            }
            break;
        case RIGHT:
            if (snake.snakePart[0].posX > VGAX_WIDTH - WINDOW_BOUNDRY_SIZE_X - SNAKE_WIDTH) {
                snake.snakePart[0].posX = WINDOW_BOUNDRY_SIZE_X;
            }
            break;
        default:
            return;
    }
}

void growSnake() {
    snake_type newPart = {};
    newPart.posX = snake.snakePart[snake.size - 1].posX;
    newPart.posY = snake.snakePart[snake.size - 1].posY;
    newPart.width = SNAKE_WIDTH;
    newPart.height = SNAKE_HEIGHT;
    newPart.color = BLUE;
    newPart.bodyDir = snake.snakePart[snake.size - 1].bodyDir;
    snake.addLast(newPart);
}

unsigned getRandomInRange(unsigned lower, unsigned upper) {
    return  (rand() % (upper - lower + 1)) + lower;
}

byte getRandomMultipleInRange(byte lower, byte upper, byte multiple) {
    return  ((rand() % (((upper) / (multiple)) - ((lower) / (multiple)) + 1)) + ((lower) / (multiple))) * (multiple);
}

void generateFoodRandCoords() {
    // collision with snake:    
    do {
        inCollison_Snake = false;
        for (byte i = 0; i < snake.size; i++) {
            rand_X = getRandomMultipleInRange(WINDOW_BOUNDRY_SIZE_X, VGAX_WIDTH - WINDOW_BOUNDRY_SIZE_X - IMG_FOOD_WIDTH, IMG_FOOD_WIDTH);
            rand_Y = getRandomMultipleInRange(WINDOW_BOUNDRY_SIZE_Y, VGAX_HEIGHT - WINDOW_BOUNDRY_SIZE_Y - IMG_FOOD_HEIGHT, IMG_FOOD_HEIGHT);

            if ((rand_X == snake.snakePart[i].posX) && (rand_Y == snake.snakePart[i].posY)) {
                inCollison_Snake = true;
                break;
            }
        }
    } while(inCollison_Snake == true); // In some frames the generated food collides with snake. Maybe imposibile to solve?

    food.posX = rand_X;
    food.posY = rand_Y;

    randomTimeout0 = getRandomInRange(MIN_TIME_FOOD_RESPAWN, MAX_TIME_FOOD_RESPAWN);
}

void regenFoodAfterTimeElapsed() {
    if(snake.snakePart[0].posX != food.posX || snake.snakePart[0].posY != food.posY) {
        currentTime0 = vga.millis();
        if(currentTime0 - lastTime0 >= randomTimeout0) {
            generateFoodRandCoords();
            lastTime0 = currentTime0;
        }
    }
}

void checkGameOver() {
    snakeHeadCollisionWithTail();
    //snakeHeadCollisionWithBounds(); // used for snake kill by boundry
}

void snakeHeadCollisionWithFood() {
    if (snake.snakePart[0].posX == food.posX && snake.snakePart[0].posY == food.posY) {
        score = (score + 5) % 10000;
        if(score % 10 == 0) {
            gameSpeed = gameSpeed > 3 ? gameSpeed - 1 : gameSpeed;
        }
        growSnake();
        generateFoodRandCoords();
    }
}

void snakeHeadCollisionWithTail () {
    for (byte i = 1; i < snake.size; i++) {
        if(snake.snakePart[0].posX == snake.snakePart[i].posX && snake.snakePart[0].posY == snake.snakePart[i].posY) {
            gameOver = true;
        }
    }
}

void snakeHeadCollisionWithBounds () {
    switch(dir) {
        case UP:
            if (snake.snakePart[0].posY < WINDOW_BOUNDRY_SIZE_Y) {
                gameOver = true;
            }
            break;
        case DOWN:
            if (snake.snakePart[0].posY > VGAX_HEIGHT - WINDOW_BOUNDRY_SIZE_Y - SNAKE_HEIGHT) {
                gameOver = true;
            }
            break;
        case LEFT:
            if (snake.snakePart[0].posX < WINDOW_BOUNDRY_SIZE_X) {
                gameOver = true;
            }
            break;
        case RIGHT:
            if (snake.snakePart[0].posX > VGAX_WIDTH - WINDOW_BOUNDRY_SIZE_X - SNAKE_WIDTH) {
                gameOver = true;
            }
            break;
        default:
            return;
    }
}
