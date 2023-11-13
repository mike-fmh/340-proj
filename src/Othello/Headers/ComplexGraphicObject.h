//
//  ComplexGraphicObject.h
//
//  Created by Jean-Yves Hervé on 2023-10-05.
//

#ifndef COMPLEX_GRAPHIC_OBJECT_H
#define COMPLEX_GRAPHIC_OBJECT_H

#include <memory>
#include <vector>
//
#include "GraphicObject.h"

namespace othello
{
	class ComplexGraphicObject : public GraphicObject
	{
		private:
		
			std::vector< std::shared_ptr<GraphicObject> > partList_;

		public:
		
			ComplexGraphicObject(float centerX, float centerY, float angle);

			ComplexGraphicObject(const TilePoint& tile, float angle);

			virtual ~ComplexGraphicObject() = default;
			
			//	Here, this class implements default behavior (draw all parts),
			//	but there may be a subclass that needs to do something different
			virtual void draw() const;

			void addPart(std::shared_ptr<GraphicObject> part);

			//disabled constructors & operators
			ComplexGraphicObject() = delete;
			ComplexGraphicObject(const ComplexGraphicObject& obj) = delete;	// copy
			ComplexGraphicObject(ComplexGraphicObject&& obj) = delete;		// move
			ComplexGraphicObject& operator = (const ComplexGraphicObject& obj) = delete;	// copy operator
			ComplexGraphicObject& operator = (ComplexGraphicObject&& obj) = delete;		// move operator

	};
}

#endif //	COMPLEX_GRAPHIC_OBJECT_H
