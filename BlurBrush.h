#ifndef BLUR_BRUSH_H
#define BLUR_BRUSH_H

#include "ImpBrush.h"

class BlurBrush: public ImpBrush{
public:
	BlurBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
protected:
	int m_size;
};


#endif