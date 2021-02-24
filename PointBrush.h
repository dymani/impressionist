//
// PointBrush.h
//
// The header file for Point Brush. 
//

#ifndef POINTBRUSH_H
#define POINTBRUSH_H

#include "ImpBrush.h"

class PointBrush : public ImpBrush
{
public:
	PointBrush( ImpressionistDoc* pDoc = NULL, char* name = NULL );

	void BrushBegin( const IPoint source, const IPoint target );
	void BrushMove( const IPoint source, const IPoint target );
	void BrushEnd( const IPoint source, const IPoint target );
	char* BrushName( void );
protected:
	void drawPoint(const IPoint source, const IPoint target);
	int m_size;
};

#endif