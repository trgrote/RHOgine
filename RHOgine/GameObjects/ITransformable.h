#ifndef ITRANSFORMABLE_H
#define ITRANSFORMABLE_H

#include "../HashString.h"

namespace rho
{
class ITransformable
{
public:
	virtual void setPosition( int const & x, int const & y ) = 0;
	virtual int getPositionX() const = 0;
	virtual int getPositionY() const = 0;
	virtual int getCenterX() const = 0;
	virtual int getCenterY() const = 0;
	virtual int getWidth() const = 0;
	virtual int getHeight() const = 0;
	virtual float getRotation() const = 0;

	virtual ~ITransformable(){}
};

}

#endif
