#ifndef LINE_OVERLAY_H
#define LINE_OVERLAY_H

#include "ImpBrush.h"

class ImpressionistDoc;

class LineOverlay {
public:
	LineOverlay();
	void setStart(const IPoint target);
	void draw(const IPoint target);
	void release(const IPoint target);

	ImpressionistDoc* m_pDoc;
private:
	bool m_validBrush;
	IPoint m_start;
	int m_size, m_angle;
};

#endif