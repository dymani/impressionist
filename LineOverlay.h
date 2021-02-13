#ifndef LINE_OVERLAY_H
#define LINE_OVERLAY_H

#include "ImpBrush.h"

class ImpressionistDoc;

class LineOverlay {
public:
	LineOverlay(ImpressionistDoc* pDoc);
	void setStart(const Point target);
	void draw(const Point target);
	void release(const Point target);
private:
	ImpressionistDoc* m_pDoc;
	bool m_validBrush;
	Point m_start;
	int m_size, m_angle;
};

#endif