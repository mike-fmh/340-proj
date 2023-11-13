
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

WorldPoint othello::pixelToWorld(float ix, float iy)
{
	return WorldPoint{	Board::X_MIN + ix*Board::pixelToWorldRatio,
					Board::Y_MAX - iy*Board::pixelToWorldRatio
				};
}

WorldPoint othello::pixelToWorld(const PixelPoint& pt)
{
	return WorldPoint{	Board::X_MIN + pt.x*Board::pixelToWorldRatio,
					Board::Y_MAX - pt.y*Board::pixelToWorldRatio
				};
}

PixelPoint othello::worldToPixel(float wx, float wy)
{
	return PixelPoint{(wx - Board::X_MIN)*Board::worldToPixelRatio,
				 (Board::Y_MAX - wy)*Board::worldToPixelRatio};

}

PixelPoint othello::worldToPixel(const WorldPoint& pt)
{
	return PixelPoint{(pt.x - Board::X_MIN)*Board::worldToPixelRatio,
				 (Board::Y_MAX - pt.y)*Board::worldToPixelRatio};

}
