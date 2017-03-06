
#ifndef _RectF_H_
#define _RectF_H_

namespace touchcontrols
{

class RectF
{
public:
	float bottom,left,top,right;

	RectF();

	RectF(float left, float top, float right, float bottom);

	bool contains(float x, float y);
	void offset(float dx,float dy);
	void offsetTo(float newLeft, float newTop);

	float width();
	float height();
};

}

#endif
