#include "ScatteredPointBrush.h"

#include "impressionistDoc.h"
#include "impressionistUI.h"

ScatteredPointBrush::ScatteredPointBrush(ImpressionistDoc* pDoc, char* name)
	: PointBrush(pDoc, name), m_regionSize(1), m_density(1.0f) {
}

void ScatteredPointBrush::BrushBegin(const IPoint source, const IPoint target)
{
	ImpressionistDoc* pDoc = GetDocument();

	m_size = 1;
	m_regionSize = pDoc->getSize();
	m_density = 0.2f;

	BrushMove(source, target);
}

void ScatteredPointBrush::BrushMove(const IPoint source, const IPoint target) {
	ImpressionistDoc* pDoc = GetDocument();
	
	if (pDoc == NULL) {
		printf("ScatteredPointBrush::BrushMove  document is NULL\n");
		return;
	}
	
	int dx, dy;
	for (int i = 0; i < (int)(m_regionSize * m_regionSize * m_density); ++i) {
		dx = rand() % m_regionSize - (int)(m_regionSize / 2);
		dy = rand() % m_regionSize - (int)(m_regionSize / 2);
		drawPoint(IPoint(source.x + dx, source.y + dy), IPoint(target.x + dx, target.y + dy));
	}
}