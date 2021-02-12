#include "ScatteredPointBrush.h"

#include "impressionistDoc.h"
#include "impressionistUI.h"

ScatteredPointBrush::ScatteredPointBrush(ImpressionistDoc* pDoc, char* name)
	: ImpBrush(pDoc, name), m_size(1), m_density(1.0f) {
}

void ScatteredPointBrush::BrushBegin(const Point source, const Point target) {
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	m_size = pDoc->getSize();
	m_density = 0.2f;

	glPointSize((float)1);

	BrushMove(source, target);
}

void ScatteredPointBrush::BrushMove(const Point source, const Point target) {
	ImpressionistDoc* pDoc = GetDocument();
	
	if (pDoc == NULL) {
		printf("ScatteredPointBrush::BrushMove  document is NULL\n");
		return;
	}
		
	glBegin(GL_POINTS);
		int dx, dy;
		for (int i = 0; i < (int)(m_size * m_size * m_density); ++i) {
			dx = rand() % m_size - (int)(m_size / 2);
			dy = rand() % m_size - (int)(m_size / 2);

			SetColor(Point(source.x + dx, source.y + dy), pDoc->getAlpha());

			glVertex2d((double)target.x + dx, (double)target.y + dy);
		}	
	glEnd();
}

void ScatteredPointBrush::BrushEnd(const Point source, const Point target) {

}