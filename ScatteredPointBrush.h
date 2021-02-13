#ifndef SCATTERED_POINT_BRUSH_H
#define SCATTERED_POINT_BRUSH_H

#include "PointBrush.h"

class ScatteredPointBrush : public PointBrush {
public:
	ScatteredPointBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
private:
	int m_regionSize;
	float m_density;
};

#endif