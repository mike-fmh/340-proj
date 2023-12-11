//
//  Object.h
//  Template Project
//
//  Created by Jean-Yves Hervé on 2023-10-17.
//

#ifndef OBJECT_H
#define OBJECT_H

#include "commonTypes.h"

namespace othello
{
	class Object
	{
        /// This class was written by Professor Jean-Yves Hervé (provided in template code from CSC406).
		private:
		
			float x_ , y_, angle_;

		public:
		
			Object(float x, float y, float angle);
			Object(const TilePoint& tile, float angle);
			
			//	The rule of thumb is:  If your class contains at least one virtual
			//	method (which indicates that it may be used polymorphically), then
			//	its destructor should be virtual
			virtual ~Object() = default;
			
			//disabled constructors & operators
			Object() = delete;
			Object(const Object& obj) = delete;	// copy
			Object(Object&& obj) = delete;		// move
			Object& operator = (const Object& obj) = delete;	// copy operator
			Object& operator = (Object&& obj) = delete;		// move operator

			inline float getX() const
			{
				return x_;
			}
			
			inline void setX(float x)
			{
				x_ = x;
			}

			inline float getY() const
			{
				return y_;
			}

			inline void setY(float y)
			{
				y_ = y;
			}

			inline float getAngle() const
			{
				return angle_;
			}

			inline void setAngle(float angle)
			{
				angle_ = angle;
			}

	};
}

#endif //	OBJECT_H
