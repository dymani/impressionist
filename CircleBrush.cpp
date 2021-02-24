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

void CircleBrush::BrushBegin(const IPoint source, const IPoint target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	BrushMove(source, target);
}

void CircleBrush::BrushMove(const IPoint source, const IPoint target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("CircleBrush::BrushMove  document is NULL\n");
		return;
	}

	// Get the radius
	float r = (double)pDoc->getSize() / 2.0f;
	// Decide the generation method, as odd size and even size are a bit different from each other
	if (pDoc->getSize() % 2 == 0) {
		MakeCircle(pDoc, source, target, r, true);
	}
	else {
		MakeCircle(pDoc, source, target, r, false);
	}

}

void CircleBrush::MakeCircle(ImpressionistDoc* doc, const IPoint source, const IPoint target, double r, bool even) {
	// Get the theta of each segment
	float dtheta = 2.0f * M_PI / 360.0f;

	glBegin(GL_POLYGON);
		SetColor(source, doc->getAlpha());

		float theta = 0.0;

		if (even) {
			for (; theta < 2.0f * M_PI; theta += dtheta) {
				glVertex2f(target.x + (r * cosf(theta)), target.y - (r * sinf(theta)));
			}
		}
		else {
			for (; theta < 2.0f * M_PI; theta += dtheta) {
				glVertex2i(target.x + (r * cosf(theta)), target.y + (r * sinf(theta)));
			}
		}

	glEnd();
}

void CircleBrush::BrushEnd(const IPoint source, const IPoint target)
{
	// do nothing so far
}

