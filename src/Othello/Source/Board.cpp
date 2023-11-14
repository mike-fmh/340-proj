
#include <cmath>
#include "Board.h"

using namespace othello;


void Board::setScalingRatios(int& paneWidth, int& paneHeight){
	float widthRatio = WIDTH / paneWidth;
	float heightRatio = HEIGHT / paneHeight;
	float maxRatio = fmax(widthRatio,heightRatio);

	pixelToWorldRatio = maxRatio;
	worldToPixelRatio = 1.f / pixelToWorldRatio;
	drawInPixelScale = pixelToWorldRatio;
	
	paneWidth = static_cast<int>(round(WIDTH * worldToPixelRatio));
	paneHeight = static_cast<int>(round(HEIGHT * worldToPixelRatio));
}

TilePoint othello::pixelToWorld(float ix, float iy)
{
	return TilePoint{	(int)round(Board::ROWS_MIN + ix*Board::pixelToWorldRatio),
					(int)round(Board::COLS_MAX - iy*Board::pixelToWorldRatio)
				};
}

TilePoint othello::pixelToWorld(const PixelPoint& pt)
{
	return TilePoint{	(int)round(Board::ROWS_MIN + pt.x*Board::pixelToWorldRatio),
					(int)round(Board::COLS_MAX - pt.y*Board::pixelToWorldRatio)
				};
}

PixelPoint othello::worldToPixel(float wx, float wy)
{
	return PixelPoint{(wx - Board::ROWS_MIN)*Board::worldToPixelRatio,
				 (Board::COLS_MAX - wy)*Board::worldToPixelRatio};

}

PixelPoint othello::worldToPixel(const TilePoint& pt)
{
	return PixelPoint{(pt.x - Board::ROWS_MIN)*Board::worldToPixelRatio,
				 (Board::COLS_MAX - pt.y)*Board::worldToPixelRatio};

}
