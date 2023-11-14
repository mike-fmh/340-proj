//
//  common.h
//  Week 08 - Earshooter
//
//  Template file created by Jean-Yves Hervé on 11/9/22.
//  Project file created by Michael Felix on 11/13/23.
//

#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <string>

namespace othello
{
    /**	A set of predefined color labels
	 */
	enum class ColorIndex
    {
        WHITE = 0,
        RED,
        GREEN,
        BLUE,
        YELLOW,
        ORANGE,
        PURPLE,
        BROWN,
        PINK,
        GREY,
        NO_COLOR,
        //
        NB_COLORS
    };


    /**	The RGBA values corresponding to the above labels
	 */
    static	float	COLOR[static_cast<int>(ColorIndex::NB_COLORS)][4] =
		{	{1.f, 1.f, 1.f, 1.f},	//	white
			{1.f, 0.f, 0.f, 1.f},	//	red
			{0.f, 1.f, 0.f, 1.f},	//	green
			{0.f, 0.f, 1.f, 1.f},	//	blue
			{1.f, 1.f, 0.f, 1.f},	//	yellow
			{1.f, 0.6f, 0.2f, 1.f},  // orange
			{0.4f, 0.f, 0.7f, 1.f},  // purple
			{0.2f, 0.1f, 0.f, 1.f},  // brown
			{1.f, 0.f, 0.5f, 1.f},   // pink
			{0.5f, 0.5f,0.5f, 1.f}, //  grey
			{0.f, 0.f, 0.f, 0.f},    // no color
		};

//===============================================================
//	Disabled interface constants
//	I would use these if I wanted to choose colors or sizes
//	through the GUI
//===============================================================

//    static const std::string colorMenuStr[static_cast<int>(ColorIndex::NB_COLORS)] =
//		{	"White",
//			"Red",
//			"Green",
//			"Blue",
//			"Yellow",
//			"Orange",
//			"Purple",
//			"Brown",
//			"Pink",
//			"Grey",
//			"No Color",
//		};
//
//    enum class SizeIndex
//    {
//        TINY = 0,
//        VERY_SMALL,
//        SMALL,
//        MEDIUM,
//        LARGE,
//        VERY_LARGE,
//        REALLY_HUGE,	//	For wahtever reason, HUGE is not a valid identifier
//        //
//        NB_SIZES
//    };

//    static	float	SCALE[static_cast<int>(SizeIndex::NB_SIZES)] =
//		{	0.05f,  //  TINY
//			0.10f,  //  VERY_SMALL
//			0.15f,  //  SMALL
//			0.20f,  //  MEDIUM
//			0.25f,  //  LARGE
//			0.30f,  //  VERY_LARGE
//			0.35f	//  REALLY_HUGE
//		};

//    static const std::string segSizeMenuStr[static_cast<int>(SizeIndex::NB_SIZES)] =
//		{	"Tiny",
//			"Very small",
//			"Small",
//			"Medium",
//			"Large",
//			"Very large",
//			"Huge"
//		};
	
	
	/** Struct used to store the location of a tile on the board
	 */
	struct TilePoint{
		
		/** The tile's horizontal location.
		 */
		int x;

		/** The tile's vertical location.
		 */
		int y;
        
        int getCol() {
            return x;
        }
        int getRow() {
            return y;
        }

	};

	/** Struct used to store the coordinates of a point in pixel space
	 */
	struct PixelPoint{
		
		/** The point's horizontal coordinate.
		 */
		float x;

		/** The point's vertical coordinate.
		 */
		float y;

	};

    struct Velocity{
        
        /** The point's horizontal coordinate.
         */
        float vx;

        /** The point's vertical coordinate.
         */
        float vy;
        
        static Velocity NULL_VELOCITY;

    };

    struct RGBColor {
        float red;
        float blue;
        float green;
    };

}


#endif //	COMMON_TYPES_H
