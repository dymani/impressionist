#ifndef WARP_BRUSH_H
#define WARP_BRUSH_H

#include "ImpBrush.h"
#include <FL/Fl.H>

class WarpBrush : public ImpBrush {
public:
	WarpBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
protected:
	void initKernel();
	int m_size;
	Point m_prev;
	double* m_kernel;
};

#endif