//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "pointbrush.h"

extern float frand();

PointBrush::PointBrush( ImpressionistDoc* pDoc, char* name ) :
	ImpBrush(pDoc,name)
{
}

void PointBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	m_size = pDoc->getSize();
	
	BrushMove( source, target );
}

void PointBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();

	if ( pDoc == NULL ) {
		printf( "PointBrush::BrushMove  document is NULL\n" );
		return;
	}

	drawPoint(source, target);	
}

void PointBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

void PointBrush::drawPoint(const Point source, const Point target) {
	ImpressionistDoc* pDoc = GetDocument();

	glPointSize((float)m_size);
	glBegin(GL_POINTS);
		SetColor(source, pDoc->getAlpha());
		glVertex2d(target.x, target.y);
	glEnd();
}

