#include "ScatteredLineBrush.h"

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include <cmath>

const double PI = 3.14159;

ScatteredLineBrush::ScatteredLineBrush(ImpressionistDoc* pDoc, char* name)
	: ImpBrush(pDoc, name), m_size(1), m_width(40), m_angle(0), m_numLines(4) {
}

void ScatteredLineBrush::BrushBegin(const Point source, const Point target) {
	ImpressionistDoc* pDoc = GetDocument();
		
	m_size = pDoc->getSize();
	m_width = 10;
	m_angle = 45;
	m_numLines = 4;

	BrushMove(source, target);
}

void ScatteredLineBrush::BrushMove(const Point source, const Point target) {
	ImpressionistDoc* pDoc = GetDocument();

	if (pDoc == NULL) {
		printf("LineBrush::BrushMove  document is NULL\n");
		return;
	}

	int sx, sy;
	double dx, dy;
	for (int i = 0; i < m_numLines; ++i) {
		sx = rand() % m_size - (int)(m_size / 2);
		sy = rand() % m_size - (int)(m_size / 2);
		glBegin(GL_TRIANGLE_STRIP);
			SetColor(Point(source.x + sx, source.y + sy), pDoc->getAlpha());
			dx = m_size / 2.0 * cos(m_angle * PI / 180);
			dy = m_size / 2.0 * sin(m_angle * PI / 180);
			glVertex2d((double)target.x + sx + dx, (double)target.y + sy + dy - m_width / 2.0);
			glVertex2d((double)target.x + sx - dx, (double)target.y + sy - dy - m_width / 2.0);
			glVertex2d((double)target.x + sx + dx, (double)target.y + sy + dy + m_width / 2.0);
			glVertex2d((double)target.x + sx - dx, (double)target.y + sy - dy + m_width / 2.0);
		glEnd();
	}
}

void ScatteredLineBrush::BrushEnd(const Point source, const Point target) {
}