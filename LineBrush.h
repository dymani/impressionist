#ifndef LINE_BRUSH_H
#define LINE_BRUSH_H

#include "ImpBrush.h"
#include <FL/Fl.H>

#include <vector>

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
	int angleRegression();
	void drawLine(const Point source, const Point target);
	int m_size, m_width, m_angle;	
	Mode m_mode;
	const unsigned int MOUSE_HISTORY_SIZE = 5;
	std::vector<Point> m_prevTargets;
	inline bool isEqual(unsigned char* left, unsigned char* right) {
		return left[0] == right[0] && left[1] == right[1] && left[2] == right[2];
	}
};

#endif