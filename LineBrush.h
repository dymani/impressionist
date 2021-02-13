#ifndef LINE_BRUSH_H
#define LINE_BRUSH_H

#include "ImpBrush.h"

class LineBrush : public ImpBrush {
public:
	enum class Mode { SLIDER, GRADIENT, MOVEMENT };

	LineBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	void setMode(int mode) {
		if (mode >= 0 && mode < 3)
			m_mode = static_cast<Mode>(mode);
	}
	Mode getMode() {
		return m_mode;
	}
protected:
	void updateAttributes(const Point source, const Point target);
	void drawLine(const Point source, const Point target);
	int m_size, m_width, m_angle;	
	Mode m_mode;
	Point m_prevTarget;
	const int m_gaussian[9] = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };
	const int m_gaussian5[25] = { 1, 4, 7, 4, 1,
								  4, 16, 26, 16, 4,
		                          7, 26, 41, 26, 7,
								  4, 16, 26, 16, 4,
								  1, 4, 7, 4, 1 };
	const int m_sobelX[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
	const int m_sobelY[9] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 };
};

#endif