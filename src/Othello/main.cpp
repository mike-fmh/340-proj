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

#include "Board.hpp"
#include "Tile.hpp"
#include "Disc.hpp"
#include "GameState.hpp"
#include "Player.hpp"
//

using namespace std;
using namespace othello;

bool showingWhiteMoves = false;
bool showingBlackMoves = false;

float cur_ai_turn_wait = 0;
const float SECS_BETWEEN_AI_MOVES = 1.0;

// weights for each factor based on their importance
const int MOBILITY_WEIGHT = 1;
const int STABILITY_WEIGHT = 2;
const int CORNER_WEIGHT = 4;

/// Power refers to how many pieces a move flips
const int POWER_WEIGHT = 3;

shared_ptr<Board> gameBoard;
vector<shared_ptr<GraphicObject>> allObjects;

shared_ptr<Player> playerNull;
shared_ptr<Player> playerWhite;
shared_ptr<Player> playerBlack;

shared_ptr<GameState> gameState;

bool currentTurn = 1; // 1 = white, 0 = black

// during each player's turn, these vectors will store the tiles they can place pieces on
vector<shared_ptr<Tile>> whitePlayableTiles;
vector<shared_ptr<Tile>> blackPlayableTiles;

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


const RGBColor DEFAULT_TILE_COLOR = RGBColor{0.2f, 1.f, 0.4f};

const char* WIN_TITLE = "Othello";


struct GamestateScore {
    
    /// Score based on mobility, which represents the amount of possible moves the player has.
    int mobilityScore;
    
    /// Score based on stability, which represents how many of the player's tiles aren't currently flanked (can't be flipped) by their opponent.
    int pseudostabilityScore;
    
    /// Score based on stability, which represents how many of the player's tiles couldn't possibly be flanked by their opponent for the remainder of the game.
    int stabilityScore;
    
    /// Score based on how many corner pieces the player has.
    int cornerControlScore;
    
    /// Score based on how many opposing pieces were flipped in the move which resulted in this gamestate
    int powerScore;
    
    /// The full gamestate score, computed by multiplying each score value by its corresponding weight and summing them together.
    int totalScore;
    
    int sum() {
        return cornerControlScore + stabilityScore + pseudostabilityScore + mobilityScore + powerScore;
    }
};

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

void startTurn(shared_ptr<Player>& whoseTurn);
void addGamePiece(TilePoint location, shared_ptr<Player> whose, shared_ptr<Board> theBoard, bool addObj);
unsigned int evalGamestateScore(shared_ptr<Player>& AIplayer, shared_ptr<GameState>& hypotheticalGamestate, int movePower);
unsigned int bestMoveHeuristic(shared_ptr<Player>& AIplayer, vector<shared_ptr<Tile>> possibleMoves);

//--------------------------------------
#if 0
#pragma mark Constants
#endif
//--------------------------------------
const int INIT_WIN_X = 10, INIT_WIN_Y = 32;

// traditional othello board is 8x8 tiles
const int BOARD_ROWS_MIN = 1;
const int BOARD_ROWS_MAX = 8;
const int BOARD_COLS_MIN = 1;
const int BOARD_COLS_MAX = 8;

// the actual "world" to render should larger than the game board
const int BOARD_PADDING = 1;
//const int BOARD_X_MIN = Board::ROWS_MIN - Board::PADDING;
//const int BOARD_X_MAX = Board::ROWS_MAX + Board::PADDING;
//const int BOARD_Y_MIN = Board::COLS_MIN - Board::PADDING;
//const int BOARD_Y_MAX = Board::COLS_MAX + Board::PADDING;

//const float Board::WIDTH = Board::X_MAX - Board::X_MIN;
//const float Board::HEIGHT = Board::Y_MAX - Board::Y_MIN;

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

RGBColor WHITE = RGBColor{1, 1, 1};
RGBColor BLACK = RGBColor{0, 0, 0};

#if 0
#pragma mark -
#pragma mark Callback functions
#endif


void startTurn(shared_ptr<Player>& whoseTurn) {
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
                currentTurn = 0; // this equates to passing the turn to black (handled in the timer function)
                gameState->passTurn(playerBlack);
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
                currentTurn = 1; // pass the turn to white (handled in the timer function)
                gameState->passTurn(playerWhite);
                cur_ai_turn_wait = 0;
            }
        }
        
         // display current possible moves
         for (auto tile: blackPlayableTiles) {
         tile->setColor(1, 0.8, 1);
         }
    }
}

void addGamePiece(TilePoint location, shared_ptr<Player> whose, shared_ptr<Board> theBoard, bool addObj) {
    shared_ptr<Disc> thisDisc = make_shared<Disc>(location, whose->getMyColor());
    theBoard->addPiece(whose, thisDisc);
    if (addObj) {
        allObjects.push_back(thisDisc);
    }
}


unsigned int evalGamestateScore(shared_ptr<Player>& AIplayer, shared_ptr<GameState>& hypotheticalGamestate, int movePower) {
    int mobility, pseudostability, stability, cornerPieces;
    GamestateScore curScore;
    
    /// Find mobility (number of possible moves)
    std::vector<std::shared_ptr<Tile>> possibleMoves;
    hypotheticalGamestate->getPlayableTiles(AIplayer, possibleMoves);
    mobility = (int)possibleMoves.size();
    
    /// Calculate stability and count corner pieces
    std::vector<std::vector<std::shared_ptr<Tile>>> allMyPieces;  // tiles where I currently have pieces placed
    hypotheticalGamestate->getPlayerTiles(AIplayer, allMyPieces); // populate my tiles
    cornerPieces = 0;
    pseudostability = 0;
    stability = 0;
    for (unsigned int r = 0; r < allMyPieces.size(); r++) {
        for (unsigned int c = 0; c < allMyPieces[r].size(); c++) {
            std::shared_ptr<Tile> thisTile = allMyPieces[r][c];
            if (hypotheticalGamestate->isCornerTile(thisTile)) // if the tile is a corner piece
                cornerPieces++;
            if (hypotheticalGamestate->discIsPseudostable(thisTile, AIplayer)) // if the tile isn't flankable by the opponent
                pseudostability++;
        }
    }
    
    /// Multiply by weights and sum products together
    curScore.mobilityScore = mobility * MOBILITY_WEIGHT;
    curScore.cornerControlScore = cornerPieces * CORNER_WEIGHT;
    curScore.pseudostabilityScore = pseudostability * STABILITY_WEIGHT;
    curScore.stabilityScore = stability * STABILITY_WEIGHT;
    curScore.powerScore = movePower * POWER_WEIGHT;
    curScore.totalScore = curScore.sum();
    
    return curScore.totalScore; // totalScore represents the overall positional score for the AI for currentGamestate
}


unsigned int bestMoveHeuristic(shared_ptr<Player>& AIplayer, vector<shared_ptr<Tile>> possibleMoves) {
    unsigned int bestMoveInd = 0;
    unsigned int bestMoveScore = 0;
    unsigned int curMoveScore = 0;
    for (unsigned int i = 0; i < possibleMoves.size(); i++) {
        shared_ptr<Tile> thisMove = possibleMoves[i];
        shared_ptr<Player> tempWhite = make_shared<Player>(WHITE, "white");
        shared_ptr<Player> tempBlack = make_shared<Player>(BLACK, "black");
        shared_ptr<Player> tempNull = make_shared<Player>(RGBColor{-1, -1, -1}, "null");
        
        shared_ptr<Board> tempBoard = make_shared<Board>(BOARD_ROWS_MIN, BOARD_ROWS_MAX, BOARD_COLS_MIN, BOARD_COLS_MAX, BOARD_PADDING, DEFAULT_TILE_COLOR, tempNull);
        shared_ptr<GameState> tempGamestate = make_shared<GameState>(tempWhite, tempBlack, tempBoard);
        
        shared_ptr<Player> tempOwner;
        for (shared_ptr<Disc> piece : gameBoard->getAllPieces()) {
            
            TilePoint thisPiecePos = piece->getPos();
            if (piece->getColor().isEqualTo(WHITE)) {
                tempOwner = tempWhite;
            } else {
                tempOwner = tempBlack;
            }
            addGamePiece(thisPiecePos, tempOwner, tempBoard, false);
        }
        
        TilePoint thisMoveLoc = thisMove->getPos();
        shared_ptr<Tile> hypMove = tempBoard->getBoardTile(thisMoveLoc);
        
        // get AIplayer reference for the temp boardstate
        if (AIplayer->getMyColor().isEqualTo(WHITE)) {
            tempOwner = tempWhite;
        } else {
            tempOwner = tempBlack;
        }
        unsigned int numFlipped = tempGamestate->placePiece(tempOwner, hypMove, true);
        curMoveScore = evalGamestateScore(tempOwner, tempGamestate, numFlipped);
        
        if (curMoveScore > bestMoveScore) {
            bestMoveInd = i;
            bestMoveScore = curMoveScore;
        }
    
    }
    return bestMoveInd;
}

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
        /*
        case 'w': {
            vector<shared_ptr<Tile>> mov;
            gameState->getPlayableTiles(playerWhite, mov);
            for (auto tile : mov) {
                if (!showingWhiteMoves) {
                    tile->setColor(RGBColor{0.8, 1, 1});
                } else {
                    tile->setColor(DEFAULT_TILE_COLOR);
                }
            }
            showingWhiteMoves = !showingWhiteMoves;
            break;
        }
        case 'b': {
            vector<shared_ptr<Tile>> mov;
            gameState->getPlayableTiles(playerBlack, mov);
            for (auto tile : mov) {
                if (!showingBlackMoves) {
                    tile->setColor(RGBColor{0.8, 1, 1});
                } else {
                    tile->setColor(DEFAULT_TILE_COLOR);
                }
            }
            showingBlackMoves = !showingBlackMoves;
            break;
        }
        */
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
    
    /// TODO: update all Tiles
    
    
    
    // do stuff
    
    if (currentTurn) { // white's turn
        startTurn(playerWhite);
        
        // white's turn logic is handled in the mouse function
        
    } else {
        startTurn(playerBlack);
        
        // black's (AI) turn logic
        if (cur_ai_turn_wait >= SECS_BETWEEN_AI_MOVES) {
            // compute black's best move and play it
            unsigned int bestMoveIndex = bestMoveHeuristic(playerBlack, blackPlayableTiles);
            TilePoint bestMoveLoc = blackPlayableTiles[bestMoveIndex]->getPos();
            shared_ptr<Tile> bestMove = gameBoard->getBoardTile(bestMoveLoc);
            shared_ptr<Disc> newPiece = gameState->placePiece(playerBlack, bestMove);
            allObjects.push_back(newPiece);
            
            currentTurn = 1;
            gameState->passTurn(playerWhite);
            cur_ai_turn_wait = 0;
        } else {
            cur_ai_turn_wait += dt;
        }
        
    }
    
    
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
//    static int clickCount = 0;

    switch (button)
    {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
            {
                if (currentTurn) {
                    // white's turn
                    shared_ptr<Tile> clickedTile = gameState->computeTileClicked(ix, iy, whitePlayableTiles);
                    if (clickedTile != nullptr) {  // nullptr means an invalid tile was clicked on
                        shared_ptr<Disc> newPiece = gameState->placePiece(playerWhite, clickedTile);
                        //cout << clickedTile->getCol() << ", " << clickedTile->getRow() << endl;
                        allObjects.push_back(newPiece);
                        currentTurn = 0;
                        gameState->passTurn(playerBlack);
                    }
                    
                } else {
                    // black's turn
                    
                    /*
                    shared_ptr<Tile> clickedTile = gameState->computeTileClicked(ix, iy, blackPlayableTiles);
                    if (clickedTile != nullptr) {  // nullptr means an invalid tile was clicked on
                        shared_ptr<Disc> newPiece = gameState->placePiece(playerBlack, clickedTile);
                        //cout << clickedTile->getCol() << ", " << clickedTile->getRow() << endl;
                        allObjects.push_back(newPiece);
                        currentTurn = 1;
                        gameState->passTurn(playerWhite);
                    }
                    */
                }
                
            }
            else if (state == GLUT_UP)
            {
                
            }
            break;
            
        default:
            break;
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
    playerNull = make_shared<Player>(RGBColor{-1, -1, -1}); // owns tiles with nothing placed on them
    
    gameBoard = make_shared<Board>(BOARD_ROWS_MIN, BOARD_ROWS_MAX, BOARD_COLS_MIN, BOARD_COLS_MAX, BOARD_PADDING, DEFAULT_TILE_COLOR, playerNull);
    allObjects.push_back(gameBoard);
    
    
    playerWhite = make_shared<Player>(RGBColor{1, 1, 1}, "black");
    playerBlack = make_shared<Player>(RGBColor{0, 0, 0}, "white");
    
    // 4 starting pieces (discs)
    addGamePiece(TilePoint{4, 4}, playerBlack, gameBoard, true);
    addGamePiece(TilePoint{5, 5}, playerBlack, gameBoard, true);
    
    addGamePiece(TilePoint{5, 4}, playerWhite, gameBoard, true);
    addGamePiece(TilePoint{4, 5}, playerWhite, gameBoard, true);

    gameState = make_shared<GameState>(playerWhite, playerBlack, gameBoard);
  
/*
    thisPnt = TilePoint{5, 6};
    gameBoard->getBoardTile(thisPnt)->setColor(1, 1, 0.3);
    gameState->TileIsFlanked(gameBoard->getBoardTile(thisPnt), playerWhite);
*/
    
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
