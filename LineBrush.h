#ifndef LINE_BRUSH_H
#define LINE_BRUSH_H

#include "ImpBrush.h"
#include "Convolution.h"

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
	const int MOUSE_HISTORY_SIZE = 5;
	std::vector<Point> m_prevTargets;
	const int GAUSSIAN_3[9] = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };
	const int GAUSSIAN_5[25] = { 1, 4, 7, 4, 1,
								  4, 16, 26, 16, 4,
		                          7, 26, 41, 26, 7,
								  4, 16, 26, 16, 4,
								  1, 4, 7, 4, 1 };
	const int SOBEL_X[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
	const int SOBEL_Y[9] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 };
	Convolution* m_gaussian, *m_sobelX, *m_sobelY;
	unsigned char* m_currentImage;
};

#endif