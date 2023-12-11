//
//  main.cpp
//
//
//  Original OpenGLUT/OpenGL Template file created by Jean-Yves Hervé on 10/04/23.
//  Migrated into the Othello main project file by Michael Felix on 11/13/23.
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

#include "Board.hpp"
#include "Tile.hpp"
#include "Disc.hpp"
#include "GameState.hpp"
#include "Player.hpp"
#include "AiMind.hpp"

using namespace std;
using namespace othello;

bool showingWhiteMoves = false;
bool showingBlackMoves = false;
bool turnStarted = false;

const unsigned int MINIMAX_DEPTH = 4;

/// Was the last turn ended because the player had no valid moves?
/// In othello, the game can end early (before the board is filled) if neither player has a valid move.
bool lastMoveInvalid = false;

float cur_ai_turn_wait = 0;
const float SECS_BETWEEN_AI_MOVES = 1.0;

// weights for each factor based on their importance
// used in computing Gamestate Advantage Score (measures how "good" a player's current gamestate is)
const unsigned int NUM_DISC_WEIGHT = 3;
const unsigned int MOBILITY_WEIGHT = 1;
const unsigned int STABILITY_WEIGHT = 2;
const unsigned int CORNER_WEIGHT = 4;
const int NUM_FRONTIER_WEIGHT = -1;
const int CORNER_ADJ_WEIGHT = -2;


vector<shared_ptr<GraphicObject>> allObjects; // objects to be rendered

shared_ptr<Board> gameBoard;

shared_ptr<Player> playerNull;
shared_ptr<Player> playerWhite;
shared_ptr<Player> playerBlack;
shared_ptr<AiMind> AI_MIND;

shared_ptr<GameState> gameState;

bool currentTurn = 0; // 1 = white, 0 = black
bool gameOver = 0;

// during each player's turn, these vectors will store the tiles they can place pieces on
vector<shared_ptr<Tile>> whitePlayableTiles;
vector<shared_ptr<Tile>> blackPlayableTiles;

const RGBColor DEFAULT_TILE_COLOR = RGBColor{0.2f, 1.f, 0.4f};

const char* WIN_TITLE = "Othello";


/// GLUT/OpenGL functions
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


void endGame();

/// Passes the turn to the given player - startTurn() must be called separately.
///@param toWho Which player to pass the turn to.
void passTurn(shared_ptr<Player>& toWho);

/// Starts the given player's turn. Computes & highlights their possible moves.
/// @param whoseTurn The player whose turn should begin.
void startTurn(shared_ptr<Player>& whoseTurn);


const int INIT_WIN_X = 10, INIT_WIN_Y = 32;

time_t startTime = -1;
int winWidth = 800,
    winHeight = 800;

float Board::pixelToWorldRatio;
float Board::worldToPixelRatio;
float Board::drawInPixelScale;

RGBColor WHITE = RGBColor{1, 1, 1};
RGBColor BLACK = RGBColor{0, 0, 0};


void endGame() {
    unsigned int numBlackTiles, numWhiteTiles;
    vector<vector<shared_ptr<Tile>>> blackTiles, whiteTiles;
    gameOver = 1; // game over
    
    // how many tiles does each player control?
    numBlackTiles = gameState->getPlayerTiles(playerBlack, blackTiles);
    numWhiteTiles = gameState->getPlayerTiles(playerWhite, whiteTiles);
    
    if (numBlackTiles > numWhiteTiles) {
        cout << "\nBLACK WINS\n\n";
        for (unsigned int r = 0; r < whiteTiles.size(); r++) {
            for (unsigned int c = 0; c < whiteTiles[r].size(); c++) {
                whiteTiles[r][c]->setColor(1,0,0);
            }
        }
    } else if (numBlackTiles < numWhiteTiles) {
        cout << "\nWHITE WINS\n\n";
        for (unsigned int r = 0; r < blackTiles.size(); r++) {
            for (unsigned int c = 0; c < blackTiles[r].size(); c++) {
                blackTiles[r][c]->setColor(1,0,0);
            }
        }
    } else {
        cout << "\nTIE\n\n";
    }
}


void passTurn(shared_ptr<Player>& toWho) {
    turnStarted = false;
    if (toWho->getMyColor().isEqualTo(BLACK)) {
        currentTurn = 0; // signifies black's turn
    } else {
        currentTurn = 1; // signifies white's turn
    }
}

void startTurn(shared_ptr<Player>& whoseTurn) {
    turnStarted = true;
    if (whoseTurn->getMyColor().isEqualTo(WHITE)) {
        /// White's turn
        
        // it's not black's turn, so clear their moves
        if (blackPlayableTiles.size() > 0) {
            for (auto tile: blackPlayableTiles) {
                tile->setColor(DEFAULT_TILE_COLOR);
            }
            blackPlayableTiles.clear();
        }
        
        // populate white's moves if not already done
        if (whitePlayableTiles.size() == 0) {
            gameState->getPlayableTiles(playerWhite, whitePlayableTiles);
            if (whitePlayableTiles.size() == 0) { // if white has no valid moves
                if (lastMoveInvalid) // if black also had no valid moves
                    endGame();
                passTurn(playerBlack);
                lastMoveInvalid = true;
            }
        }
        
        // display current possible moves
        for (auto tile: whitePlayableTiles) {
            tile->setColor(0.8, 1, 1);
        }
    } else if (whoseTurn->getMyColor().isEqualTo(BLACK)) {
        /// Black's turn
        
        // it's not white's turn, so clear their moves
        if (whitePlayableTiles.size() > 0) {
            for (auto tile: whitePlayableTiles) {
                tile->setColor(DEFAULT_TILE_COLOR);
            }
            whitePlayableTiles.clear();
        }
        
        // populate black's moves if not already done
        if (blackPlayableTiles.size() == 0) {
            gameState->getPlayableTiles(playerBlack, blackPlayableTiles);
            if (blackPlayableTiles.size() == 0) { // if black has no valid moves
                if (lastMoveInvalid) // if white also had no valid moves
                    endGame();
                passTurn(playerWhite);
                cur_ai_turn_wait = 0;
                lastMoveInvalid = true;
            }
        }
        
         // display current possible moves
         for (auto tile: blackPlayableTiles) {
         tile->setColor(1, 0.8, 1);
         }
    }
}

void myDisplayFunc(void)
{
    //    This clears the buffer(s) we draw into.
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
    
    gameBoard->setScalingRatios(winWidth, winHeight);
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
    gluOrtho2D(gameBoard->getXmin(), gameBoard->getXmax(), gameBoard->getYmin(), gameBoard->getYmax());

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
    // the timer function is run between every frame
    static int frameIndex=0;
    static chrono::high_resolution_clock::time_point lastTime = chrono::high_resolution_clock::now();

    //    re-prime the timer function
    glutTimerFunc(1, myTimerFunc, value);
    
    chrono::high_resolution_clock::time_point currentTime = chrono::high_resolution_clock::now();
    float dt = chrono::duration_cast<chrono::duration<float> >(currentTime - lastTime).count();
        
    if (!gameOver) { // turn logic
        if (currentTurn) { // white's turn
            if (!turnStarted)
                startTurn(playerWhite);
               // white's turn logic is handled in the mouse function
        } else {
            if (!turnStarted)
                startTurn(playerBlack);
            // black's (AI) turn logic
            if (cur_ai_turn_wait >= SECS_BETWEEN_AI_MOVES) {
                // compute black's best move and play it
                unsigned int bestMoveIndex = AI_MIND->bestMoveMinimax(playerBlack, gameBoard, gameState, blackPlayableTiles, MINIMAX_DEPTH);
                
                // for the tile flip animation to show, we need to reset currenttime after picking the move, because it can take a few seconds
                currentTime = chrono::high_resolution_clock::now();
                cout << "chose: " << bestMoveIndex << endl;
                TilePoint bestMoveLoc = blackPlayableTiles[bestMoveIndex]->getPos();
                shared_ptr<Tile> bestMove = gameBoard->getBoardTile(bestMoveLoc);
                shared_ptr<Disc> newPiece = gameState->placePiece(playerBlack, bestMove);
                allObjects.push_back(newPiece);
                cur_ai_turn_wait = 0;
                passTurn(playerWhite);
            } else {
                cur_ai_turn_wait += dt;
            }
            
        }
    }
    
    // update all discs (game pieces)
    for (auto disc : gameBoard->getAllPieces()) {
        disc->update(dt);
    }
    
    lastTime = currentTime;

    //    And finally I perform the rendering
    if (frameIndex++ % 10 == 0)
    {
        glutPostRedisplay();
    }
}

void myMouseHandler(int button, int state, int ix, int iy)
{
    switch (button)
    {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
            {
                if ((currentTurn) && (!gameOver)) { // white's turn
                    shared_ptr<Tile> clickedTile = gameState->computeTileClicked(ix, iy, whitePlayableTiles);
                    if (clickedTile != nullptr) {  // nullptr means an invalid tile was clicked on
                        shared_ptr<Disc> newPiece = gameState->placePiece(playerWhite, clickedTile);
                        allObjects.push_back(newPiece);
                        passTurn(playerBlack);
                    }
                    
                }
            }
            else if (state == GLUT_UP)
            {
                // mouse up
            }
            break;
            
        default:
            break;
    }
}

// INIT AND MAIN

void applicationInit()
{
    playerNull = make_shared<Player>(RGBColor{-1, -1, -1}); // owns tiles with nothing placed on them
    
    gameBoard = make_shared<Board>(DEFAULT_TILE_COLOR, playerNull);
    allObjects.push_back(gameBoard);
    
    playerWhite = make_shared<Player>(RGBColor{1, 1, 1});
    playerBlack = make_shared<Player>(RGBColor{0, 0, 0});
    
    gameState = make_shared<GameState>(playerWhite, playerBlack, gameBoard);
    
    AI_MIND = make_shared<AiMind>(NUM_DISC_WEIGHT, MOBILITY_WEIGHT, STABILITY_WEIGHT, CORNER_WEIGHT, CORNER_ADJ_WEIGHT, NUM_FRONTIER_WEIGHT, DEFAULT_TILE_COLOR);
    
    // 4 starting pieces (discs)
    gameState->addGamePiece(TilePoint{4, 4}, playerBlack, allObjects);
    gameState->addGamePiece(TilePoint{5, 5}, playerBlack, allObjects);
    
    gameState->addGamePiece(TilePoint{5, 4}, playerWhite, allObjects);
    gameState->addGamePiece(TilePoint{4, 5}, playerWhite, allObjects);
  

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
    glutMouseFunc(myMouseHandler);
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
