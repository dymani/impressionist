#ifndef SHARPEN_BRUSH_H
#define SHARPEN_BRUSH_H

#include "ImpBrush.h"

class SharpenBrush : public ImpBrush {
public:
	SharpenBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	void BrushBegin(const IPoint source, const IPoint target);
	void BrushMove(const IPoint source, const IPoint target);
	void BrushEnd(const IPoint source, const IPoint target);
protected:
	int m_size;
};


#endif