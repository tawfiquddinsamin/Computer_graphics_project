#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++WINDOW++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define WIDTH 800
#define HEIGHT 600

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++ROOFTOP++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define ROOF_TOP 180.0f

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++PLAYER++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
float playerX = 400.0f;
float playerY = ROOF_TOP;
float playerSpeed = 7.0f;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++BULLET++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
float bulletX = 0.0f;
float bulletY = 0.0f;
float bulletSpeed = 10.0f;
int bulletActive = 0;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++ALIENS++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define ROWS 3
#define COLS 8
float alienX[ROWS][COLS];
float alienY[ROWS][COLS];
int alienAlive[ROWS][COLS];
float alienSpeed = 1.0f;
int alienDirection = 1;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++GAME++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
int score = 0;
/*0 = playing      1 = game over       2 = win*/
int gameOver = 0;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++KEYBOARD++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
int leftPressed = 0;
int rightPressed = 0;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++DRAW RECTANGLE++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void drawRect(float x, float y, float width, float height)
{
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++TEXT++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void drawText(float x, float y, const char* text)
{
    int i;
    glRasterPos2f(x, y);
    for (i = 0; text[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, text[i]);
    }
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++SKY++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void drawSky()
{
    glBegin(GL_QUADS);
    glColor3f(0.88f, 0.90f, 0.92f);   /*horizon*/
    glVertex2f(0.0f, ROOF_TOP);
    glVertex2f((float)WIDTH, ROOF_TOP);
    glColor3f(0.72f, 0.78f, 0.85f);         /*top*/
    glVertex2f((float)WIDTH, (float)HEIGHT);
    glVertex2f(0.0f, (float)HEIGHT);
    glEnd();
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++RoofTop++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void drawRooftop()
{
    int i;
    int f;
    float floorH = 45.0f;   /*floors*/

    /*main facade*/
    glColor3f(0.92f, 0.91f, 0.88f);
    drawRect(0.0f, 0.0f, (float)WIDTH, ROOF_TOP);

    /*windows*/
    for (f = 0; f < 4; f++)
    {
        float baseY = f * floorH;

        /*left wing glass ribbon*/
        if (f == 3) glColor3f(0.55f, 0.72f, 0.66f);
        else        glColor3f(0.28f, 0.44f, 0.52f);
        drawRect(0.0f, baseY + 10.0f, 430.0f, 26.0f);

        /*right wing glass ribbon*/
        glColor3f(0.22f, 0.38f, 0.50f);
        drawRect(445.0f, baseY + 10.0f, (float)WIDTH - 445.0f, 26.0f);

        /*dividing the glass ribbons */
        glColor3f(0.92f, 0.91f, 0.88f);
        for (i = 20; i < 410; i += 40)
            drawRect((float)i, baseY + 10.0f, 6.0f, 26.0f);
        for (i = 465; i < WIDTH - 10; i += 40)
            drawRect((float)i, baseY + 10.0f, 6.0f, 26.0f);
    }

    /* white vertical joint separating the two wings */
    glColor3f(0.95f, 0.95f, 0.95f);
    drawRect(430.0f, 0.0f, 15.0f, ROOF_TOP);

    /* maroon strip on the far right*/
    glColor3f(0.45f, 0.26f, 0.22f);
    drawRect(792.0f, 0.0f, 8.0f, ROOF_TOP);

    /*roof ledge the shooter stands*/
    glColor3f(0.80f, 0.80f, 0.82f);
    drawRect(0.0f, ROOF_TOP - 6.0f, (float)WIDTH, 6.0f);

    /*side view of a background building*/
    glColor3f(0.85f, 0.86f, 0.88f);
    drawRect(150.0f, ROOF_TOP, 64.0f, 78.0f);
    glColor3f(0.25f, 0.40f, 0.50f);              /*glass stripes*/
    drawRect(158.0f, ROOF_TOP, 10.0f, 78.0f);
    drawRect(176.0f, ROOF_TOP, 12.0f, 78.0f);
    drawRect(196.0f, ROOF_TOP, 10.0f, 78.0f);
    glColor3f(0.70f, 0.72f, 0.75f);           /*cap*/
    drawRect(146.0f, ROOF_TOP + 78.0f, 72.0f, 8.0f);

    /*dark name sign*/
    glColor3f(0.13f, 0.13f, 0.15f);
    drawRect(440.0f, ROOF_TOP, 352.0f, 28.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(484.0f, ROOF_TOP + 9.0f, "Daffodil International University");
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++PLAYER++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void drawPlayer()
{
    glColor3f(0.0f, 1.0f, 0.0f);
    drawRect(playerX - 25, playerY, 50, 20);
    drawRect(playerX - 5, playerY + 20, 10, 15);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++BULLET++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void drawBullet()
{
    if (!bulletActive)
        return;
    glColor3f(1.0f, 0.45f, 0.0f);
    drawRect(bulletX - 2, bulletY, 4, 12);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++ALIEN++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void drawAlien(float x, float y)
{
    glColor3f(1.0f, 0.0f, 0.0f);
    drawRect(x, y, 35, 20);                   /* Body */
    drawRect(x + 5, y + 20, 25, 10);           /* Head */
    drawRect(x + 5, y - 5, 5, 5);                /* Legs */
    drawRect(x + 25, y - 5, 5, 5);           /* Legs */
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++ALL ALIENS++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void drawAliens()
{
    int i;
    int j;
    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            if (alienAlive[i][j])
            {
                drawAlien(alienX[i][j], alienY[i][j]);
            }
        }
    }
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++COLLISION++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
int collision(float x1, float y1, float w1, float h1,
    float x2, float y2, float w2, float h2)
{
    if (x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2)
    {
        return 1;
    }
    return 0;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++BULLET COLLISION++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void checkBulletCollision()
{
    int i;
    int j;
    if (!bulletActive)
        return;
    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            if (alienAlive[i][j])
            {
                if (collision(bulletX - 2, bulletY, 4, 12,
                    alienX[i][j], alienY[i][j], 35, 30))
                {
                    alienAlive[i][j] = 0;
                    bulletActive = 0;
                    score += 10;
                    return;
                }
            }
        }
    }
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++WIN CHECK++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void checkWin()
{
    int i;
    int j;
    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            if (alienAlive[i][j])
                return;
        }
    }
    gameOver = 2;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++UPDATE++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void update(int value)
{
    int i;
    int j;
    int hitEdge = 0;
    if (!gameOver)
    {
        /* Player Movement */
        if (leftPressed)  playerX -= playerSpeed;
        if (rightPressed) playerX += playerSpeed;
        if (playerX < 30)         playerX = 30;
        if (playerX > WIDTH - 30) playerX = WIDTH - 30;

        /* Bullet Movement */
        if (bulletActive)
        {
            bulletY += bulletSpeed;
            if (bulletY > HEIGHT)
                bulletActive = 0;
        }

        /* Aliens Movement */
        for (i = 0; i < ROWS; i++)
        {
            for (j = 0; j < COLS; j++)
            {
                if (alienAlive[i][j])
                {
                    alienX[i][j] += alienSpeed * alienDirection;
                    if (alienX[i][j] < 20 ||
                        alienX[i][j] > WIDTH - 60)
                    {
                        hitEdge = 1;
                    }
                }
            }
        }

        /* Alien Move down */
        if (hitEdge)
        {
            alienDirection *= -1;
            for (i = 0; i < ROWS; i++)
                for (j = 0; j < COLS; j++)
                    alienY[i][j] -= 20;
        }

        /* Game over */
        for (i = 0; i < ROWS; i++)
        {
            for (j = 0; j < COLS; j++)
            {
                if (alienAlive[i][j])
                {
                    if (alienY[i][j] < playerY + 30)
                        gameOver = 1;
                }
            }
        }
        checkBulletCollision();
        checkWin();
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++KEY DOWN++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void keyboardDown(unsigned char key, int x, int y)
{
    if (key == 'a' || key == 'A') leftPressed = 1;
    if (key == 'd' || key == 'D') rightPressed = 1;

    /*Shoot Bullet Interaction*/
    if (key == ' ' && !bulletActive && !gameOver)
    {
        bulletX = playerX;
        bulletY = playerY + 35;
        bulletActive = 1;
    }

    /*Restart*/
    if ((key == 'r' || key == 'R') && gameOver)
    {
        int i;
        int j;
        score = 0;
        gameOver = 0;
        bulletActive = 0;
        alienDirection = 1;
        for (i = 0; i < ROWS; i++)
        {
            for (j = 0; j < COLS; j++)
            {
                alienAlive[i][j] = 1;
                alienX[i][j] = 100 + j * 70;
                alienY[i][j] = 560 - i * 55;
            }
        }
    }

    /*ESC*/
    if (key == 27) exit(0);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++KEY UP++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void keyboardUp(unsigned char key, int x, int y)
{
    if (key == 'a' || key == 'A') leftPressed = 0;
    if (key == 'd' || key == 'D') rightPressed = 0;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++DISPLAY++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void display()
{
    char scoreText[50];
    glClear(GL_COLOR_BUFFER_BIT);

    /* Sky and building are drawn first, so game objects render on top of them*/
    drawSky();
    drawRooftop();

    /*Game objects render*/
    drawAliens();
    drawPlayer();
    drawBullet();

    /*Score View */
    glColor3f(0.15f, 0.15f, 0.20f);
    sprintf(scoreText, "Score: %d", score);
    drawText(20, 575, scoreText);

    /*Game Instructions*/
    drawText(300, 38, "A/D = Move    SPACE = Shoot");

    /*Game over Sign*/
    if (gameOver == 1)
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        drawText(350, 300, "GAME OVER");
        drawText(320, 270, "Press R to restart");
    }

    /*Win Sign*/
    if (gameOver == 2)
    {
        glColor3f(0.0f, 0.55f, 0.0f);
        drawText(360, 300, "YOU WIN!");
        drawText(320, 270, "Press R to restart");
    }
    glutSwapBuffers();
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++INITIALIZE ALIENS++++++++++++++++++++++++++++++++++++++++++++++*/
void initializeAliens()
{
    int i;
    int j;
    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            alienAlive[i][j] = 1;
            alienX[i][j] = 100 + j * 70;
            alienY[i][j] = 560 - i * 55;  /*higher start at the top of the window*/
        }
    }
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++OPENGL INITIALIZATION+++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    printf("DIU building backdrop (photo style) ready.\n");
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++MAIN+++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Simple Space Invaders");
    init();
    initializeAliens();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}