#ifndef LINE_BRUSH_H
#define LINE_BRUSH_H

#include "ImpBrush.h"

class LineBrush : public ImpBrush {
public:
	LineBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
};

#endif