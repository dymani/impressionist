#ifndef LINE_BRUSH_H
#define LINE_BRUSH_H

#include "ImpBrush.h"

class LineBrush : public ImpBrush {
public:
	LineBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
private:
	void updateAttributes(const Point source, const Point target);
	int m_size, m_width, m_angle;
	enum class Mode {SLIDER, GRADIENT, MOVEMENT} m_mode;
	Point m_prevTarget;
};

#endif