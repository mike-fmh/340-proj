// Board.hpp
//
// Created by Michael Felix 11/13/23 with help from Jean-Yves Hervé.

#ifndef BOARD_HPP
#define BOARD_HPP

#include "Player.hpp"
#include "commonTypes.h"
#include "glPlatform.h"
#include "Tile.hpp"
#include <algorithm>
#include <cmath>
#include <vector>
#include <memory>

namespace othello
{
    class Board: public GraphicObject
    {
        private:
            /// Vector of rows of Tiles, made up of references to each Tile in that row.
            std::vector<std::vector<std::shared_ptr<Tile>>> allBoardTiles_;
            
            /// Reference to the 'null' player, who controls blank tiles.
            const std::shared_ptr<Player> nullplayerRef_;
        
            /// The min/max size of the full screen in TilePoints
            static const int X_MIN_, X_MAX_, Y_MIN_, Y_MAX_;
        
            /// The TilePoint coord value for the minimum tile column/row.
            static const int COLS_MIN_, ROWS_MIN_;
        
            /// The TilePoint coord value for the maximum tile column/row.
            static const int COLS_MAX_, ROWS_MAX_;
        
            /// Number of TilePoint spaces to keep between the edge of the game board and edge of the screen.
            static const int PADDING_;
        
            /// Overall width & heigh of the game screen, in TilePoints.
            static const float WIDTH_, HEIGHT_;
            
            /// Color of the game board.
            RGBColor DEFAULT_TILE_COLOR_;
            
            /// These variables are used in calculations related to resizing the window and converting between TilePoints and screen (mouse) coords.
            static float pixelToWorldRatio;
            static float worldToPixelRatio;
            static float drawInPixelScale;

        public:
            /// Constructs a new game board.
            /// @param tileColor The default color of tiles (color of the game board).
            /// @param nullplayerRef Reference to the 'null' player (null player controls tiles that have no pieces on them).
            Board(RGBColor tileColor, std::shared_ptr<Player>& nullplayerRef);
        
            //disabled constructors & operators
            Board() = delete;
            Board(Board&& obj) = delete;        // move
            Board& operator = (const Board& obj) = delete;    // copy operator
            Board& operator = (Board&& obj) = delete;        // move operator

            /// The draw function is run every frame.
            void draw() const;
        
            /// Adds a new piece to a tile, and the given player gains control of that tile.
            /// @param forWho Reference to the player who should control this piece.
            /// @param piece Reference to the new piece to place.
            void addPiece(std::shared_ptr<Player>& forWho, std::shared_ptr<Disc>& piece);
            
            /// Returns ALL game pieces that exist on the board in a new vector.
            std::vector<std::shared_ptr<Disc>> getAllPieces() const;
            
            /// Returns the Tile object in worldTiles at the given TilePoint, if any exist
            /// @param at the location of the Tile to return
            std::shared_ptr<Tile> getBoardTile(TilePoint& at);
        
            /// If there's a disc placed at the given point, return its owner
            std::shared_ptr<Player> getTileOwner(TilePoint& at);
            
            /// Returns a tile's neighbors including diagonals
            /// @param tile the original tile to get the neighbors of
            /// @param neighbors the vector to populate with neighboring tiles
            void getNeighbors(TilePoint& tile, std::vector<std::shared_ptr<Tile>>& neighbors);
            
            inline bool isValidPosition(TilePoint& at) {
                return ((at.x >= ROWS_MIN_) & (at.x <= ROWS_MAX_) & (at.y >= COLS_MIN_) & (at.y <= COLS_MAX_));
            }
        
            /// Returns a reference to the board's tiles vector.
            inline std::vector<std::vector<std::shared_ptr<Tile>>>* getBoardTiles() {
                return &allBoardTiles_;
            }
        
            /// Returns a reference to the null player.
            inline std::shared_ptr<Player> getNullPlayer() const {
                return nullplayerRef_;
            }
        
            /// Getters for the board's max/min coordinate variables.
            inline int getXmin() const {
                return X_MIN_;
            }
            inline int getXmax() const {
                return X_MAX_;
            }
            inline int getYmin() const {
                return Y_MIN_;
            }
            inline int getYmax() const {
                return Y_MAX_;
            }
            inline int getColsMin() const {
                return COLS_MIN_;
            }
            inline int getColsMax() const {
                return COLS_MAX_;
            }
            inline int getRowsMin() const {
                return ROWS_MIN_;
            }
            inline int getRowsMax() const {
                return ROWS_MAX_;
            }
        
        /// The following functions were adapted from template code provided in CSC406, written by Professor Jean-Yves Hervé.
        /** Function called through the initialization of a global variable in the
         *    main program.  Although the user specifies dimensions for the rendering pane,
         *    the function may set different values that agree better with the world
         *    aspect ratio.
         *
         *
         * @param paneWidth      user-set width of the redering pane
         * @param paneHeight    user-set height of the redering pane
         */
        void setScalingRatios(int& paneWidth, int& paneHeight);
        TilePoint pixelToWorld(float ix, float iy);
        PixelPoint worldToPixel(float wx, float wy);
        TilePoint pixelToWorld(const PixelPoint& pt);
        PixelPoint worldToPixel(const TilePoint& pt);
    };
}


#endif  //  BOARD_HPP
