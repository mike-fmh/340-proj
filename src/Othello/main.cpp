//
//  main.cpp
//
//
//  Template file created by Jean-Yves Hervé on 10/04/23.
//  Project file created by Michael Felix on 11/13/23.
//
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <memory>
#include <random>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "Board.h"
#include "Tile.hpp"
#include "Disc.hpp"
//

using namespace std;
using namespace othello;

vector<shared_ptr<Tile>> boardTiles;
vector<shared_ptr<GraphicObject>> allObjects;

//--------------------------------------
#if 0
#pragma mark Custom data types
#endif
//--------------------------------------

//    I like to setup my meny item indices as enmerated values, but really
//    regular int constants would do the job just fine.
//    Note that in modern C++, it's generally advised to use "scoped enums", which
//    are declared as "enum class NameOfType".  I agree and do so in general, but
//    here these are meant to be used with my glut interface, and it's really
//    bothersome to do the casting to int each each time.

const char* WIN_TITLE = "Othello";


enum TextColorSubmenuItemID {    FIRST_TEXT = 11,
                                RED_TEXT = 11,
                                GREEN_TEXT,
                                WHITE_TEXT,
                                MAGENTA_TEXT,
                                //
                                MAX_COUNT_TEXT
};
const int NUM_TEXT_COLORS = MAX_COUNT_TEXT - FIRST_TEXT;

enum BackgroundColorSubmenuItemID {    FIRST_BGND = 21,
                                    LIGHT_GREY_BGND = 21,
                                    DARK_GREY_BGND,
                                    GREEN_BGND,
                                    BLUE_BGND,
                                    BROWN_BGND,
                                    //
                                    MAX_COUNT_BGND
};
const int NUM_BGND_COLORS = MAX_COUNT_BGND - FIRST_BGND;

enum FontSize {
                    SMALL_FONT_SIZE = 0,
                    MEDIUM_FONT_SIZE,
                    LARGE_FONT_SIZE,
                    //
                    NUM_FONT_SIZES
};

//--------------------------------------
#if 0
#pragma mark Function prototypes
#endif
//--------------------------------------
void displayTextualInfo(const string& infoStr, int textRow);
void displayTextualInfo(const char* infoStr, int textRow);
void myDisplayFunc(void);
void myResizeFunc(int w, int h);
void myKeyHandler(unsigned char c, int x, int y);
void myKeyUpHandler(unsigned char c, int x, int y);
void mySpecialKeyHandler(int key, int x, int y);
void mySpecialKeyUpHandler(int key, int x, int y);
void myTimerFunc(int val);
void applicationInit();


//--------------------------------------
#if 0
#pragma mark Constants
#endif
//--------------------------------------
const int INIT_WIN_X = 10, INIT_WIN_Y = 32;

// traditional othello board is 8x8 tiles
const int Board::ROWS_MIN = 1;
const int Board::ROWS_MAX = 8;
const int Board::COLS_MIN = 1;
const int Board::COLS_MAX = 8;
const float Board::WIDTH = Board::ROWS_MAX - Board::ROWS_MIN;
const float Board::HEIGHT = Board::COLS_MAX - Board::COLS_MIN;

#define SMALL_DISPLAY_FONT    GLUT_BITMAP_HELVETICA_10
#define MEDIUM_DISPLAY_FONT   GLUT_BITMAP_HELVETICA_12
#define LARGE_DISPLAY_FONT    GLUT_BITMAP_HELVETICA_18
const int H_PAD = 10;
const int V_PAD = 5;

const string TEXT_COLOR_STR[NUM_TEXT_COLORS] = {"white",        //    WHITE_TEXT
                                                "red",            //    RED_TEXT
                                                "green",        //    GREEN_TEXT
                                                "magenta"};        //    MAGENTA_TEXT
                            
const string BGND_COLOR_STR[NUM_BGND_COLORS] = {"light gray",    //    LIGHT_GREY_BGND
                                                "dark gray",    //    DARK_GREY_BGND
                                                "green",        //    GREEN_BGND
                                                "blue",            //    BLUE_BGND
                                                "brown"};        //    BROWN_BGND

const GLfloat TEXT_COLOR[NUM_TEXT_COLORS][3] = { {1.f, 1.f, 1.f},    //    WHITE_TEXT
                                                 {1.f, 0.f, 0.f},    //    RED_TEXT
                                                 {0.f, .8f, 0.f},    //    GREEN_TEXT
                                                 {1.f, 0.f, 1.f}};    //    MAGENTA_TEXT
                            
const GLfloat BGND_COLOR[NUM_BGND_COLORS][3] = { {.5f, .5f, .5f},    //    LIGHT_GREY_BGND
                                                 {.3f, .3f, .3f},    //    DARK_GREY_BGND
                                                 {0.f, .4f, 0.f},    //    GREEN_BGND
                                                 {0.f, 0.f, .4f},    //    BLUE_BGND
                                                 {.4f, .2f, 0.f}};    //    BROWN_BGND

const bool displayTextOnLeft = false;
const bool displayTextOnTop = true;
const FontSize fontSize = LARGE_FONT_SIZE;

//--------------------------------------
#if 0
#pragma mark Global variables
#endif
//--------------------------------------

time_t startTime = -1;
int winWidth = 800,
    winHeight = 800;


float Board::pixelToWorldRatio;
float Board::worldToPixelRatio;
float Board::drawInPixelScale;

const GLfloat* textColor = TEXT_COLOR[0];
const GLfloat* bgndColor = BGND_COLOR[0];


#if 0
#pragma mark -
#pragma mark Callback functions
#endif


void myDisplayFunc(void)
{
    //    This clears the buffer(s) we draw into.  We will see later this
    //    semester what this really means
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //    There are two "matrix modes" in OpenGL: "projection", which sets up the
    //    camera, and "model view" which we have to be in to do any drawing
    glMatrixMode(GL_MODELVIEW);
    
    //    This says that we start from the origin of the camera, whose coordinates
    //    (in the 2D case) coincide with that of the world's orogin.
    glLoadIdentity();
    glPushMatrix();

    for (auto obj : allObjects) {
        if (obj != nullptr) {
            obj->draw();
        }
    }


    glutSwapBuffers();
}

void myResizeFunc(int w, int h)
{
    winWidth = w;
    winHeight = h;
    
    Board::setScalingRatios(winWidth, winHeight);
    if (winWidth != w || winHeight != h)
    {
        glutReshapeWindow(winWidth, winHeight);
    }
    
    //    This calls says that I want to use the entire dimension of the window for my drawing.
    glViewport(0, 0, winWidth, winHeight);

    //    Here I create my virtual camera.  We are going to do 2D drawing for a while, so what this
    //    does is define the dimensions (origin and units) of the "virtual world that my viewport
    //    maps to.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //    Here I define the dimensions of the "virtual world" that my
    //    window maps to
    //  Display more in the window than space exists on the board
    gluOrtho2D(Board::ROWS_MIN - 1 , Board::ROWS_MAX + 1, Board::COLS_MIN - 1, Board::COLS_MAX + 1);

    //    When it's done, request a refresh of the display
    glutPostRedisplay();
 }

void myMenuHandler(int choice)
{
    switch (choice)
    {
        default:    //    this should not happen
            break;
    
    }

    glutPostRedisplay();
}

//  in this example my submenu selection indicates the keyboard handling
//  function to use.
void mySubmenuHandler(int choice)
{
    switch (choice)
    {
        default:
            break;
    }
}


void mySpecialKeyHandler(int key, int x, int y) {
    switch (key)
    {
        default:
            break;
    }
}

void mySpecialKeyUpHandler(int key, int x, int y) {
    switch (key)
    {
        default:
            break;
    }
}


//    This callback function is called when a keyboard event occurs
//
void myKeyHandler(unsigned char c, int x, int y)
{
    
   // cout << c << endl;
    // silence warning
    
    (void) x;
    (void) y;
        
    switch (c)
    {
        case 'q':
        case 'Q':
        case 27:
            exit(0);
            break;
        
        default:
            break;
    }
}

void myKeyUpHandler(unsigned char c, int x, int y)
{
    // silence warning
    (void) x;
    (void) y;

    switch (c) {
        default:
            break;
    }
}


void myTimerFunc(int value)
{
    static int frameIndex=0;
    static chrono::high_resolution_clock::time_point lastTime = chrono::high_resolution_clock::now();

    //    "re-prime the timer"
    glutTimerFunc(1, myTimerFunc, value);
    
    chrono::high_resolution_clock::time_point currentTime = chrono::high_resolution_clock::now();
    float dt = chrono::duration_cast<chrono::duration<float> >(currentTime - lastTime).count();

    // do stuff
    
    lastTime = currentTime;

    
    //    And finally I perform the rendering
    if (frameIndex++ % 10 == 0)
    {
        glutPostRedisplay();
    }
}

//--------------------------------------
#if 0
#pragma mark -
#pragma mark Utilities
#endif
//--------------------------------------

void displayTextualInfo(const string& infoStr, int textRow){
    displayTextualInfo(infoStr.c_str(), textRow);
}

void displayTextualInfo(const char* infoStr, int textRow)
{
    //-----------------------------------------------
    //  0.  Build the string to display <-- parameter
    //-----------------------------------------------
    int infoLn = static_cast<int> (strlen(infoStr));

    //-----------------------------------------------
    //  1.  Determine the string's length (in pixels)
    //-----------------------------------------------
    int textWidth = 0, fontHeight=-1;
    switch(fontSize)
    {
        case SMALL_FONT_SIZE:
            fontHeight = 10;
            for (int k=0; k<infoLn; k++)
            {
                textWidth += glutBitmapWidth(SMALL_DISPLAY_FONT, infoStr[k]);
            }
            break;
        
        case MEDIUM_FONT_SIZE:
            fontHeight = 12;
            for (int k=0; k<infoLn; k++)
            {
                textWidth += glutBitmapWidth(MEDIUM_DISPLAY_FONT, infoStr[k]);
            }
            break;
        
        case LARGE_FONT_SIZE:
            fontHeight = 16;
            for (int k=0; k<infoLn; k++)
            {
                textWidth += glutBitmapWidth(LARGE_DISPLAY_FONT, infoStr[k]);
            }
            break;
            
        default:
            break;
    }

    //-----------------------------------------------
    //  2.  get current material properties
    //-----------------------------------------------
    float oldAmb[4], oldDif[4], oldSpec[4], oldShiny;
    glGetMaterialfv(GL_FRONT, GL_AMBIENT, oldAmb);
    glGetMaterialfv(GL_FRONT, GL_DIFFUSE, oldDif);
    glGetMaterialfv(GL_FRONT, GL_SPECULAR, oldSpec);
    glGetMaterialfv(GL_FRONT, GL_SHININESS, &oldShiny);

    glPushMatrix();
    if (displayTextOnTop)
    {
        glTranslatef(0.f, textRow*(fontHeight+2*V_PAD), 0.f);
    }
    else
    {
        glTranslatef(0.f, winHeight - (textRow+1)*(fontHeight+2*V_PAD),0.f);
    }

    //-----------------------------------------------
    //  3.  Clear background rectangle if required
    //-----------------------------------------------
    
    //    Move "up" from current plane, to make sure that we overwrite
    glTranslatef(0.f, 0.f, 0.1f);

    //-----------------------------------------------
    //  4.  Draw the string
    //-----------------------------------------------
    //    Where do we start drawing from
    int xPos = displayTextOnLeft ? H_PAD : winWidth - textWidth - H_PAD;
    int yPos = fontHeight + V_PAD;

    glColor3fv(textColor);
    int x = xPos;
    switch(fontSize)
    {
        case SMALL_FONT_SIZE:
            for (int k=0; k<infoLn; k++)
            {
                glRasterPos2i(x, yPos);
                glutBitmapCharacter(SMALL_DISPLAY_FONT, infoStr[k]);
                x += glutBitmapWidth(SMALL_DISPLAY_FONT, infoStr[k]);
            }
            break;
        
        case MEDIUM_FONT_SIZE:
            for (int k=0; k<infoLn; k++)
            {
                glRasterPos2i(x, yPos);
                glutBitmapCharacter(MEDIUM_DISPLAY_FONT, infoStr[k]);
                x += glutBitmapWidth(MEDIUM_DISPLAY_FONT, infoStr[k]);
            }
            break;
        
        case LARGE_FONT_SIZE:
            for (int k=0; k<infoLn; k++)
            {
                glRasterPos2i(x, yPos);
                glutBitmapCharacter(LARGE_DISPLAY_FONT, infoStr[k]);
                x += glutBitmapWidth(LARGE_DISPLAY_FONT, infoStr[k]);
            }
            break;
            
        default:
            break;
    }

    //-----------------------------------------------
    //  5.  Restore old material properties
    //-----------------------------------------------
    glMaterialfv(GL_FRONT, GL_AMBIENT, oldAmb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, oldDif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, oldSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, oldShiny);
    
    //-----------------------------------------------
    //  6.  Restore reference frame
    //-----------------------------------------------
    glPopMatrix();
}

//--------------------------------------
#if 0
#pragma mark -
#pragma mark Application init and main
#endif
//--------------------------------------

void applicationInit()
{
    TilePoint thisPnt;
    bool blackOrWhite;
    for (int c = 1; c <= 8; c++) {
        // each row starts with alternating black/white tiles
        if (c % 2) {
            blackOrWhite = false;
        } else {
            blackOrWhite = true;
        }
        for (int r = 1; r <= 8; r++) {
            thisPnt = TilePoint{r, c};
            shared_ptr<Tile> thisTile = make_shared<Tile>(thisPnt, blackOrWhite);
            boardTiles.push_back(thisTile);
            allObjects.push_back(thisTile);
            blackOrWhite = !blackOrWhite;
        }
    }
    
    
    // 4 starting pieces (discs)
    shared_ptr<Disc> thisDisc;
    thisPnt = TilePoint{4, 4};
    thisDisc = make_shared<Disc>(thisPnt, true);
    allObjects.push_back(thisDisc);
    thisPnt = TilePoint{5, 4};
    thisDisc = make_shared<Disc>(thisPnt, false);
    allObjects.push_back(thisDisc);
    thisPnt = TilePoint{4, 5};
    thisDisc = make_shared<Disc>(thisPnt, false);
    allObjects.push_back(thisDisc);
    thisPnt = TilePoint{5, 5};
    thisDisc = make_shared<Disc>(thisPnt, true);
    allObjects.push_back(thisDisc);

    //    time really starts now
    startTime = time(nullptr);
}

int main(int argc, char * argv[])
{
    //    Initialize glut and create a new window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(INIT_WIN_X, INIT_WIN_Y);
    glutCreateWindow(WIN_TITLE);
    
    //    set up the callbacks
    glutDisplayFunc(myDisplayFunc);
    glutReshapeFunc(myResizeFunc);
    glutKeyboardFunc(myKeyHandler);
    glutKeyboardUpFunc(myKeyUpHandler);
    glutSpecialFunc(mySpecialKeyHandler);
    glutSpecialUpFunc(mySpecialKeyUpHandler);
    glutTimerFunc(1,    myTimerFunc,        0);
    //              time        name of        value to pass
    //              in ms        function    to the func
    
    //    Now we can do application-level
    applicationInit();

    //    Now we enter the main loop of the program and to a large extend
    //    "lose control" over its execution.  The callback functions that
    //    we set up earlier will be called when the corresponding event
    //    occurs
    glutMainLoop();
    
    //    This will never be executed (the exit point will be in one of the
    //    callback functions).
    return 0;
}
