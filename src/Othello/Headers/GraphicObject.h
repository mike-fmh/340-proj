//
//  GraphicObject.h
//
//  Created by Jean-Yves Hervé on 2023-10-05.
//

#ifndef GRAPHIC_OBJECT_H
#define GRAPHIC_OBJECT_H

#include "Board.hpp"
#include "Object.h"

namespace othello
{
	class GraphicObject	: public virtual Object	// abstract class
	{
		private:

		public:
		
			GraphicObject(const TilePoint& tile, float angle);
			
			GraphicObject(float x, float y, float angle);

			//	The rule of thumb is:  If your class contains at least one virtual
			//	method (which indicates that it may be used polymorphically), then
			//	its destructor should be virtual
			virtual ~GraphicObject() = default;
			
			virtual void draw() const = 0;  // pure virtual function

			//disabled constructors & operators
			GraphicObject() = delete;
			GraphicObject(const GraphicObject& obj) = delete;	// copy
			GraphicObject(GraphicObject&& obj) = delete;		// move
			GraphicObject& operator = (const GraphicObject& obj) = delete;	// copy operator
			GraphicObject& operator = (GraphicObject&& obj) = delete;		// move operator
	};
}

#endif // GRAPHIC_OBJECT_H 
