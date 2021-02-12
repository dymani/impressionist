//
// ScatteredCircleBrush.cpp
//
// The implementation of Scattered Circle Brush.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredCircleBrush.h"
#include <ctime>

extern float frand();

ScatteredCircleBrush::ScatteredCircleBrush(ImpressionistDoc* pDoc, char* name) :
	CircleBrush(pDoc, name)
{
}

void ScatteredCircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	srand(time(NULL));
	numOfCircles = ((rand() % 2 + 1) % 2 == 0 ? 3 : 4);
	BrushMove(source, target);
}

void ScatteredCircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredCircleBrush::BrushMove  document is NULL\n");
		return;
	}

	// Get the radius
	float r = (double)pDoc->getSize() / 2.0f;
	for (int i = 0; i < numOfCircles; i++) {
		int dx = rand() % pDoc->getSize() - r;
		int dy = rand() % pDoc->getSize() - r;
		if (pDoc->getSize() % 2 == 0) {
			CircleBrush::MakeCircle(Point(source.x + dx, source.y + dy), Point(target.x + dx, target.y + dy), r, true);
		}
		else {
			CircleBrush::MakeCircle(Point(source.x + dx, source.y + dy), Point(target.x + dx, target.y + dy), r, false);
		}
	}
}


void ScatteredCircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
