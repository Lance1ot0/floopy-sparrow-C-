// Inclusion des fonctionnalités
#include "main.h"

// Variables

// La fenêtre de jeu
RenderWindow window;
// Pour la gestion des inputs
Input input;
bool upIsPressed = false;
bool lKeyIsPressed = false;
bool SKeyIsPressed = false;
bool HKeyIsPressed = false;

// Lignes du cadriage
VertexArray horizontalLines(sf::Lines, 40);
VertexArray verticalLines(sf::Lines, 54);

bool gridDisplay = false;
bool stopMovement = false;

// Déclaration des variables liée au joueur rec
Texture spriteTexture;
Sprite playerSprite;

RectangleShape rec(Vector2f(REC_SIZE_WIDTH, REC_SIZE_HEIGHT));
Vector2f recPosition;
float recPosY = 30;
float recSpeed = 0;
float jumpCoulDown = 0.20f;
float rotationAngle = 0.f;
bool recIsAlive = true;
bool recJumped = false;
bool recTouchPipe = false;
Time jumpCoulDownStart;
FloatRect hitboxRec;
bool playedHitboxVisible = false;
bool pipepassed = false;


// Déclaration des variables liée aux pipes
RectangleShape groundPipe1(Vector2f(PIPE_SIZE_WIDTH, PIPE_SIZE_HEIGHT));
RectangleShape cellingPipe1(Vector2f(PIPE_SIZE_WIDTH, PIPE_SIZE_HEIGHT));
FloatRect hitboxGroundPipe1;
FloatRect hitboxCellingPipe1;

RectangleShape groundPipe2(Vector2f(PIPE_SIZE_WIDTH, PIPE_SIZE_HEIGHT));
RectangleShape cellingPipe2(Vector2f(PIPE_SIZE_WIDTH, PIPE_SIZE_HEIGHT));
FloatRect hitboxGroundPipe2;
FloatRect hitboxCellingPipe2;

RectangleShape groundPipe3(Vector2f(PIPE_SIZE_WIDTH, PIPE_SIZE_HEIGHT));
RectangleShape cellingPipe3(Vector2f(PIPE_SIZE_WIDTH, PIPE_SIZE_HEIGHT));
FloatRect hitboxGroundPipe3;
FloatRect hitboxCellingPipe3;


int pipeMaxHeightY = (WINDOW_HEIGHT - PIPE_SIZE_HEIGHT);
int pipeMinHeightY = (WINDOW_HEIGHT - 96);
int pipeSpeedX = -4;
int pipesGap = 192;
int gapSizeBetweenPipes = 384;

int setActualRandomPipePosY;

Vector2f pipe1Pos;
Vector2f pipe2Pos;
Vector2f pipe3Pos;

// Déclaration des varaibles liée au sol
Texture groudTexture;
RectangleShape ground(Vector2f(WINDOW_WIDTH, 32));

// ScreenText Score
Font font;
Text Score;
int intScore = 0;



// Fonction main, point de départ du programme
int main()
{
    font.loadFromFile("res/font.ttf");
    Score.setPosition(WINDOW_WIDTH / 2 - (Score.getLocalBounds().width/ 2), WINDOW_HEIGHT / 14);
    Score.setCharacterSize(100);
    Score.setOutlineThickness(3);
    Score.setOutlineColor(Color::Black);
    Score.setFont(font);

    //On définie une seed différente pour rand() avec srand()
    srand(time(0));

    // Dessine le cadriage
    int nbHL = 1;
    int nbVL = 1;

    for (int i = 0; i < 38; i += 2)
    {
        horizontalLines[i].position = Vector2f(0, 32 * nbHL);
        horizontalLines[i + 1].position = Vector2f(WINDOW_WIDTH, 32 * nbHL);
        nbHL++;
    }

    for (int i = 0; i < 52; i += 2)
    {

        verticalLines[i].position = Vector2f(32 * nbVL, 0);
        verticalLines[i + 1].position = Vector2f(32 * nbVL, WINDOW_HEIGHT);
        nbVL++;
    }


    // Création de la fenêtre type RenderWindow avec comme argument : taille, titre
    window.create(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32), "Floopy Sparrow", Style::Default);
    // Activation de la vsync
    window.setVerticalSyncEnabled(true);
   

    // Couleur des pipes
    fillColorPipes(groundPipe1, cellingPipe1);
    fillColorPipes(groundPipe2, cellingPipe2);
    fillColorPipes(groundPipe3, cellingPipe3);

    

    // Set Pos
    // Initialisation de la pose 1
    setActualRandomPipePosY = RandomPipePosY();
    setInitialPipePos(groundPipe1, cellingPipe1, 0);
    // Initialisation de la pose 2
    setActualRandomPipePosY = RandomPipePosY();
    setInitialPipePos(groundPipe2, cellingPipe2, 1);
    // Initialisation de la pose 3
    setActualRandomPipePosY = RandomPipePosY();
    setInitialPipePos(groundPipe3, cellingPipe3, 2);

    // Couleur du rec + sprite
    spriteTexture.loadFromFile("res/floppy_sparrow.png");
    playerSprite.setTexture(spriteTexture);
    playerSprite.setScale(Vector2f(0.40f, 0.42f));

    // Couleur du sol + Pos
    groudTexture.loadFromFile("res/ground_texture.png");
    groudTexture.setRepeated(true);
    groudTexture.setSmooth(true);
    ground.setTextureRect({ 0, 0, WINDOW_WIDTH, 32 });
    ground.setTexture(&groudTexture);
    ground.setPosition(0, GROUND_POS_Y);
    ground.setOutlineColor(Color::Black);
    ground.setOutlineThickness(4);

    Clock clock;


    // Boucle qui tourne tant que la fenêtre est ouverte
    while (window.isOpen()) 
    {

        // Déplace de le rec
        if (!stopMovement)
        {
            rec.setPosition((WINDOW_WIDTH / 3) + (rotationAngle/2), recPosY);
            playerSprite.setPosition((WINDOW_WIDTH / 3) - 10 + (rotationAngle/2), recPosY - 3);
        } 
        else
        {
            recSpeed = 0;
            rotationAngle = rotationAngle - 0.05f;
        }
       
        // Récupère la hit box du rec
        hitboxRec = rec.getGlobalBounds();

        Event event; // Variable pour gérer tout type d'évenement (Touche de clavier, clique de souris)
        // Boucle sur les évenement de la fenêtre
        while (window.pollEvent(event))
        {
            // Gestion des inputs / events
            input.InputHandler(event, window);
            CheckBtn();
        }


        // Gestion des inputs
        if (upIsPressed)
        {
            if (!recJumped && recIsAlive)
            {
                recSpeed = -REC_JUMP_HEIGHT;
                recJumped = true;
                jumpCoulDownStart = clock.getElapsedTime();
            }
        }
        else
        {
            Time jumpCoulDownElapsed = clock.getElapsedTime();
            if (jumpCoulDownStart.asSeconds() <= jumpCoulDownElapsed.asSeconds() - jumpCoulDown)
                recJumped = false;
        }

        // Couleur de la fenêtre en noir
        window.clear(Color(119, 222, 238));

        // Déssine les lignes
        if (gridDisplay)
        {
            window.draw(horizontalLines);
            window.draw(verticalLines);
        }
        

        // Déssine les pipes
        window.draw(groundPipe1);
        window.draw(cellingPipe1);
        window.draw(groundPipe2);
        window.draw(cellingPipe2);
        window.draw(groundPipe3);
        window.draw(cellingPipe3);

        // Déssine le joueur
        window.draw(playerSprite);
        if(playedHitboxVisible)
            rec.setFillColor(Color(255, 0, 0, 125));
        else
            rec.setFillColor(Color(255, 0, 0, 0));
        window.draw(rec);

        // Mise a jour de la position et de la vitesse du rec tant qu'il est en vie
        if (recIsAlive)
        {
            recPosY += recSpeed;
            recSpeed += GRAVITY_FORCE_COEF;

            // Sprite and hitbox rotation
            if (recSpeed > 0)
                rotationAngle += recSpeed/6;
            else
                if(rotationAngle > -15)
                    rotationAngle += recSpeed;
            rec.setRotation(rotationAngle);
            playerSprite.setRotation(rotationAngle);
        }  

        // Si le rec touche le haut de l'écran
        if (recPosY <= 0)
        {
            recPosY = 0;
        }

        // Si il touche le sol == Mort ou touche les pipes
        if (recPosY >= (GROUND_POS_Y - REC_SIZE_HEIGHT) && recIsAlive || recTouchPipe)
        {
            // Set l'état Alive a false et stop le mouvement des pipes
            recIsAlive = false;
            pipeSpeedX = 0;

            // Si il n'a pas touché les pipes (donc uniquement touché le sol
            if (!recTouchPipe)
            {
                recPosY = (GROUND_POS_Y) - REC_SIZE_HEIGHT;
                recSpeed = 0;
            }   
            else
            {
                if (recPosY <= GROUND_POS_Y - REC_SIZE_HEIGHT)
                {
                    recPosY += recSpeed;
                    recSpeed += GRAVITY_FORCE_COEF;
                }
                else
                {
                    recSpeed = 0;
                    recPosY = (GROUND_POS_Y + 1) - REC_SIZE_HEIGHT;
                    rec.setRotation(0);
                    playerSprite.setRotation(0);
                }    

            }
            
        }

        // Déplace les pipes ground et celling
        if (!stopMovement)
        {
            movePipes(groundPipe1, cellingPipe1);
            movePipes(groundPipe2, cellingPipe2);
            movePipes(groundPipe3, cellingPipe3);
        }
        

        // On récupère la position du pipe du bas
        pipe1Pos = groundPipe1.getPosition();
        pipe2Pos = groundPipe2.getPosition();
        pipe3Pos = groundPipe3.getPosition();

        // Récupère la hit box d'un couple de pipe
        getHitboxPipes(hitboxGroundPipe1, hitboxCellingPipe1, groundPipe1, cellingPipe1);
        getHitboxPipes(hitboxGroundPipe2, hitboxCellingPipe2, groundPipe2, cellingPipe2);
        getHitboxPipes(hitboxGroundPipe3, hitboxCellingPipe3, groundPipe3, cellingPipe3);

        // Si le joueur touche le pipe1 du bas ou du haut
        if (hitboxRec.intersects(hitboxGroundPipe1) || hitboxRec.intersects(hitboxCellingPipe1))
            recTouchPipe = true;
        else if (hitboxRec.intersects(hitboxGroundPipe2) || hitboxRec.intersects(hitboxCellingPipe2))
            recTouchPipe = true;
        else if (hitboxRec.intersects(hitboxGroundPipe3) || hitboxRec.intersects(hitboxCellingPipe3))
            recTouchPipe = true;


        // Compteur du score
        if (pipe1Pos.x + PIPE_SIZE_WIDTH <= (WINDOW_WIDTH / 3) and !pipepassed)
        {
            intScore += 1;
            pipepassed = true;
        }
        else if (pipe2Pos.x + PIPE_SIZE_WIDTH <= (WINDOW_WIDTH / 3) and !pipepassed)
        {
            intScore += 1;
            pipepassed = true;
        }
        else if (pipe3Pos.x + PIPE_SIZE_WIDTH <= (WINDOW_WIDTH / 3) and !pipepassed)
        {
            intScore += 1;
            pipepassed = true;
        }
            

        // Si le pipe sort de l'écran on le remet au bord et on recalcule sa position en hauteur
        pipeOutOfScreen(pipe1Pos, groundPipe1, cellingPipe1);
        pipeOutOfScreen(pipe2Pos, groundPipe2, cellingPipe2);
        pipeOutOfScreen(pipe3Pos, groundPipe3, cellingPipe3);
       
       
        // Déssine le sol
        window.draw(ground);

        Score.setString(to_string(intScore));
        window.draw(Score);

        // Dessiner à l'écran tous les éléments
        window.display();

    }
    
    // Fin du programme
    return 0;
}



void CheckBtn()
{ 
    // Controle le rec avec space
    if (input.GetButton().space == true) // Tant que Space est appuyé
    {
        if (!upIsPressed)
            upIsPressed = true;   
    }
    else if (input.GetButton().space == false) // Tant que Space est relaché
    {
        upIsPressed = false;     
    }

    // Button pour afficher ou enlever les lignes
    if (input.GetButton().keyL == true && !lKeyIsPressed)
    {
        lKeyIsPressed = true;
        if (gridDisplay)
        {
            gridDisplay = false;
            return;
        }    
        else
        {
            gridDisplay = true;
            return;
        }        
    }
    else if (input.GetButton().keyL == false)
    {
        lKeyIsPressed = false;
    }

    // Input pour stopper le temps
    if (input.GetButton().keyS == true && !SKeyIsPressed)
    {
        SKeyIsPressed = true;
        if (!stopMovement)
        {
            stopMovement = true;
            return;
        }
        else
        {
            stopMovement = false;
            return;
        }
    }
    else if (input.GetButton().keyS == false)
    {
        SKeyIsPressed = false;
    }

    // Input pour voir la hitbox du player
    if (input.GetButton().keyH == true && !HKeyIsPressed)
    {
        HKeyIsPressed = true;
        if (!playedHitboxVisible)
        {
            playedHitboxVisible = true;
            return;
        }
        else
        {
            playedHitboxVisible = false;
            return;
        }
    }
    else if (input.GetButton().keyH == false)
    {
        HKeyIsPressed = false;
    }

    // Si touche echap appuyé : quitt le jeu
    if (input.GetButton().escape == true)
    {
        window.close();
    }
}


// Cacule la prochaine position Y du pipe
int RandomPipePosY()
{
    int posRandom = rand() % 6;
    cout << posRandom << endl;
    switch (posRandom)
    {
    case 0:
        return pipeMaxHeightY;
        break;
    case 1:
        return pipeMaxHeightY + 96;
        break;
    case 2:
        return pipeMaxHeightY + 64;
        break;
    case 3:
        return pipeMinHeightY - 64;
        break;
    case 4:
        return pipeMinHeightY - 96;
        break;
    case 5:
        return pipeMinHeightY;
        break;
    default:
        return 0;
        break;
    }
}

// Met la couleur aux pipes
void fillColorPipes(RectangleShape& groundPipe, RectangleShape& cellingPipe)
{
    groundPipe.setFillColor(Color(62, 170, 0));
    cellingPipe.setFillColor(Color(62, 170, 0));

    groundPipe.setOutlineThickness(3);
    groundPipe.setOutlineColor(Color::Black);

    cellingPipe.setOutlineThickness(3);
    cellingPipe.setOutlineColor(Color::Black);
}

// Initialise la position initiale des pipes
void setInitialPipePos(RectangleShape& groundPipe, RectangleShape& cellingPipe, int nbGap)
{
    groundPipe.setPosition(WINDOW_WIDTH + PIPE_SIZE_WIDTH + (gapSizeBetweenPipes* nbGap), setActualRandomPipePosY);
    cellingPipe.setPosition(WINDOW_WIDTH + PIPE_SIZE_WIDTH + (gapSizeBetweenPipes * nbGap), (setActualRandomPipePosY - PIPE_SIZE_HEIGHT) - pipesGap);
}

// Déplace les pipes
void movePipes(RectangleShape &groundPipe, RectangleShape &cellingPipe)
{
    groundPipe.move(pipeSpeedX, 0);
    cellingPipe.move(pipeSpeedX, 0);
}

// Récupère la Hitbox des pipes
void getHitboxPipes(FloatRect& hitboxGroundPipe, FloatRect& hitboxCellingPipe, RectangleShape& groundPipe, RectangleShape& cellingPipe)
{
    hitboxGroundPipe = groundPipe.getGlobalBounds();
    hitboxCellingPipe = cellingPipe.getGlobalBounds();
}

// Recalcule la hauteur des pipes et le remets aux bord de l'écran
void pipeOutOfScreen(Vector2f &pipePos, RectangleShape& groundPipe, RectangleShape& cellingPipe)
{
    if (pipePos.x + PIPE_SIZE_WIDTH + 96 <= 0)
    {
        setActualRandomPipePosY = RandomPipePosY();
        groundPipe.setPosition(WINDOW_WIDTH + PIPE_SIZE_WIDTH, setActualRandomPipePosY);
        cellingPipe.setPosition(WINDOW_WIDTH + PIPE_SIZE_WIDTH, (setActualRandomPipePosY - PIPE_SIZE_HEIGHT) - pipesGap);
        pipepassed = false;
    }
}