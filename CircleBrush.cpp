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

	// Get the radius
	double r = (double)pDoc->getSize() / 2.0;
	if (pDoc->getSize() % 2 == 0) {
		Circle(source, target, r, true);
	}
	else {
		Circle(source, target, r, false);
	}

}

void CircleBrush::Circle(const Point source, const Point target, double r, bool even) {
	// Get the theta of each segment
	double dtheta = 2.0 * (double)M_PI / 360.0;

	glBegin(GL_POLYGON);
		SetColor(source);

		double theta = 0.0;

		if (even) {
			for (int i = 0; i < 360; i++) {
				glVertex2d((double)target.x + (r * (double)cosf(theta)), (double)target.y + (r * (double)sinf(theta)));
				theta += dtheta;
			}
		}
		else {
			for (int i = 0; i < 360; i++) {
				glVertex2i(target.x + (r * (double)cosf(theta)), target.y + (r * (double)sinf(theta)));
				theta += dtheta;
			}
		}

	glEnd();
}

void CircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

