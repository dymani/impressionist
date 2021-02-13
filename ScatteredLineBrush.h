#ifndef SCATTERED_LINE_BRUSH_H
#define SCATTERED_LINE_BRUSH_H

#include "LineBrush.h"

class ScatteredLineBrush : public LineBrush {
public:
	ScatteredLineBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	void BrushMove(const Point source, const Point target);
private:
	int m_numLines;
};

#endif