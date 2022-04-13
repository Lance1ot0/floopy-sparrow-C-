// Inclusion des fonctionnalités
#include <SFML/Graphics.hpp>
#include <iostream>
#include<time.h>
#include "input.h"



// Constantes du programme main
const int WINDOW_WIDTH = 864;
const int WINDOW_HEIGHT = 640;

const int REC_SIZE_WIDTH = 64;
const int REC_SIZE_HEIGHT = 64;
const float REC_JUMP_HEIGHT = 7.f;

const int PIPE_SIZE_WIDTH = 96;
const int PIPE_SIZE_HEIGHT = WINDOW_HEIGHT / 1.6 + 16;

const int GROUND_POS_Y = WINDOW_HEIGHT - 32;

const float GRAVITY_FORCE_COEF = 0.35f;


// Namespaces
using namespace sf;
using namespace std;

// Prototypes
void CheckBtn();
int RandomPipePosY();
void fillColorPipes(RectangleShape& groundPipe, RectangleShape& cellingPipe);
void setInitialPipePos(RectangleShape& groundPipe, RectangleShape& cellingPipe, int nbGap);
void movePipes(RectangleShape &groundPipe, RectangleShape &cellingPipe);
void getHitboxPipes(FloatRect& hitboxGroundPipe, FloatRect& hitboxCellingPipe, RectangleShape& groundPipe, RectangleShape& cellingPipe);
void pipeOutOfScreen(Vector2f& pipePos, RectangleShape& groundPipe, RectangleShape& cellingPipe);