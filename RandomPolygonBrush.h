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

	void BrushBegin(const IPoint source, const IPoint target);
	void BrushMove(const IPoint source, const IPoint target);
	void BrushEnd(const IPoint source, const IPoint target);
	char* BrushName(void);
private:
	int numOfVertices;
};

#endif