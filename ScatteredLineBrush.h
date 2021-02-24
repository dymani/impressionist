#ifndef SCATTERED_LINE_BRUSH_H
#define SCATTERED_LINE_BRUSH_H

#include "LineBrush.h"

class ScatteredLineBrush : public LineBrush {
public:
	ScatteredLineBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	void BrushMove(const IPoint source, const IPoint target);
private:
	int m_numLines;
};

#endif