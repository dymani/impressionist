#ifndef SMUDGE_BRUSH_H
#define SMUDGE_BRUSH_H

#include "ImpBrush.h"
#include <FL/Fl.H>

class SmudgeBrush : public ImpBrush {
public:
	SmudgeBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	void BrushBegin(const IPoint source, const IPoint target);
	void BrushMove(const IPoint source, const IPoint target);
	void BrushEnd(const IPoint source, const IPoint target);
protected:
	void initKernel();
	int m_size;
	IPoint m_prev;
	double* m_kernel;
};

#endif