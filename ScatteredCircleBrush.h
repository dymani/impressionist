//
// ScatteredCircleBrush.h
//
// The header file for Scattered Circle Brush. 
//

#ifndef SCATTEREDCIRCLEBRUSH_H
#define SCATTEREDCIRCLEBRUSH_H

#include "CircleBrush.h"

class ScatteredCircleBrush : public CircleBrush
{
public:
	ScatteredCircleBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const IPoint source, const IPoint target);
	void BrushMove(const IPoint source, const IPoint target);
	void BrushEnd(const IPoint source, const IPoint target);
	char* BrushName(void);
private:
	int numOfCircles;
};

#endif