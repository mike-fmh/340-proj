// Board.h
//
// Created by Michael Felix 11/13/23 with help from Jean-Yves Hervé.

#ifndef BOARD_H
#define BOARD_H

#include <random>
#include <cmath>
#include "commonTypes.h"
#include "glPlatform.h"


namespace othello {


	struct Board {

		static const float X_MIN, X_MAX, Y_MIN, Y_MAX;

		static const float WIDTH, HEIGHT;

		static float pixelToWorldRatio;
		static float worldToPixelRatio;
		static float drawInPixelScale;


		/** Function called through the initialization of a global variable in the
		 *	main program.  Although the user specifies dimensions for the rendering pane,
		 *	the function may set different values that agree better with the world
		 *	aspect ratio.
		 * @param paneWidth		user-set width of the redering pane
		 * @param paneHeight	user-set height of the redering pane
		 */
		static void setScalingRatios(int& paneWidth, int& paneHeight);

	};

	WorldPoint pixelToWorld(float ix, float iy);
	PixelPoint worldToPixel(float wx, float wy);
	WorldPoint pixelToWorld(const PixelPoint& pt);
	PixelPoint worldToPixel(const WorldPoint& pt);

}

#endif  //  BOARD_H
