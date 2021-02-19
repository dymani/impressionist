//
// RandomPolygonBrush.h
//
// The header file for Random Polygon Brush. 
//

#ifndef RANDOMPOLYGONBRUSH_H
#define RANDOMPOLYGONBRUSH_H

#include <cmath>
#include "ImpBrush.h"

class RandomPolygonBrush : public ImpBrush
{
public:
	RandomPolygonBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
private:
	int numOfVertices;
};

#endif