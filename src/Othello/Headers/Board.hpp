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
        
            static std::vector<std::vector<std::shared_ptr<Tile>>> allBoardTiles;
            
            const std::shared_ptr<Player> nullplayerRef_;
            const int X_MIN_, X_MAX_, Y_MIN_, Y_MAX_;
            const int COLS_MIN_, COLS_MAX_, ROWS_MIN_, ROWS_MAX_, PADDING_;
            const float WIDTH_, HEIGHT_;
            
            RGBColor DEFAULT_TILE_COLOR_;
            
            static float pixelToWorldRatio;
            static float worldToPixelRatio;
            static float drawInPixelScale;

        public:
            //disabled constructors & operators
            Board() = delete;
            Board(Board&& obj) = delete;        // move
            Board& operator = (const Board& obj) = delete;    // copy operator
            Board& operator = (Board&& obj) = delete;        // move operator

            Board(int boardMinWidth, int boardMaxWidth, int boardMinHeight, int boardMaxHeight, int boardPadding, RGBColor tileColor, std::shared_ptr<Player> nullplayerRef);
            Board(const Board& obj);   // copy
        
            void draw() const;
        
            void addPiece(std::shared_ptr<Player>& forWho, std::shared_ptr<Disc>& piece);
            void addPiece(Player& forWho, std::shared_ptr<Disc>& piece);
            
            /** Function called through the initialization of a global variable in the
             *    main program.  Although the user specifies dimensions for the rendering pane,
             *    the function may set different values that agree better with the world
             *    aspect ratio.
             * @param paneWidth        user-set width of the redering pane
             * @param paneHeight    user-set height of the redering pane
             */
            void setScalingRatios(int& paneWidth, int& paneHeight);
            
            /// Returns the Tile object in worldTiles at the given TilePoint, if any exist
            /// @param at the location of the Tile to return
            std::shared_ptr<Tile> getBoardTile(TilePoint& at) const;
        
            /// If there's a disc placed at the given point, return its owner
            std::shared_ptr<Player> getTileOwner(TilePoint& at);
            
            /// Returns a tile's neighbors including diagonals
            /// @param tile the original tile to get the neighbors of
            /// @param neighbors the vector to populate with neighboring tiles
            void getNeighbors(TilePoint& tile, std::vector<std::shared_ptr<Tile>>& neighbors);
        
            TilePoint pixelToWorld(float ix, float iy);
            PixelPoint worldToPixel(float wx, float wy);
            TilePoint pixelToWorld(const PixelPoint& pt);
            PixelPoint worldToPixel(const TilePoint& pt);
            
            inline bool isValidPosition(TilePoint& at) {
                return ((at.x >= ROWS_MIN_) & (at.x <= ROWS_MAX_) & (at.y >= COLS_MIN_) & (at.y <= COLS_MAX_));
            }
        
            inline std::vector<std::vector<std::shared_ptr<Tile>>> getBoardTiles() {
                return allBoardTiles;
            }
            inline int getXmin() {
                return X_MIN_;
            }
            inline int getXmax() {
                return X_MAX_;
            }
            inline int getYmin() {
                return Y_MIN_;
            }
            inline int getYmax() {
                return Y_MAX_;
            }
            inline int getColsMin() {
                return COLS_MIN_;
            }
            inline int getColsMax() {
                return COLS_MAX_;
            }
            inline int getRowsMin() {
                return ROWS_MIN_;
            }
            inline int getRowsMax() {
                return ROWS_MAX_;
            }
        
            inline std::shared_ptr<Player> getNullPlayer() {
                return nullplayerRef_;
            }

    };
}


#endif  //  BOARD_HPP
