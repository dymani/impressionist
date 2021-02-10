//
// CircleBrush.cpp
//
// The implementation of Circle Brush.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "circlebrush.h"

extern float frand();

CircleBrush::CircleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void CircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize((float)size);

	BrushMove(source, target);
}

void CircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("CircleBrush::BrushMove  document is NULL\n");
		return;
	}

	// Use GL_POLYGON to create a "circle"

	// Get the radius
	double r = pDoc->getSize() / 2.0f;
	// Decide the number of vertices
	double precision = 5.0 * r;
	// Get the theta of each segment
	double dtheta = 2.0 * M_PI / precision;

	glBegin(GL_POLYGON);
		SetColor(source);

		// Initial theta set to 0 i.e. vertex at (0,r)
		double theta = 0;
		
		for (int i = 0; i < precision; i++) {
			glVertex2d(target.x + (double)(r * sinf(theta)), target.y + (double)(r * cosf(theta)));
			theta += dtheta;
		}
		
		// Yes. I know I mix up the sine and cosine for x and y value.

	glEnd();
}

void CircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

