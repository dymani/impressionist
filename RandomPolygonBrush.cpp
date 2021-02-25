//
// RandomPolygonBrush.cpp
//
// The implementation of Random Polygon Brush.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "RandomPolygonBrush.h"
#include <ctime>

extern float frand();

RandomPolygonBrush::RandomPolygonBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void RandomPolygonBrush::BrushBegin(const IPoint source, const IPoint target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;


	// Generate random number of vertices of the polygon
	srand(time(NULL));
	numOfVertices = rand() % 14 + 3;

	BrushMove(source, target);
}

void RandomPolygonBrush::BrushMove(const IPoint source, const IPoint target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredCircleBrush::BrushMove  document is NULL\n");
		return;
	}

	// Get the radius
	float r = (double)pDoc->getSize() / 2.0f;

	float theta = 0;

	glBegin(GL_POLYGON);
	SetColor(source, pDoc->getAlpha());

	for (int i = 0; i < numOfVertices; i++) {
		theta += (rand() % (360 / (numOfVertices - 1))) / (2.0f * M_PI);
		bool half = (rand() % 2 == 0) ? false : true;
		if (half) {
			glVertex2f(target.x + (r / 2 * cosf(theta)), target.y - (r / 2 * sinf(theta)));
		}
		else {
			glVertex2f(target.x + (r * cosf(theta)), target.y - (r * sinf(theta)));
		}
	}

	glEnd();
}


void RandomPolygonBrush::BrushEnd(const IPoint source, const IPoint target)
{
	// do nothing so far
}
