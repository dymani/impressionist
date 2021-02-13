#include "ScatteredLineBrush.h"

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include <cmath>

const double PI = 3.14159;

ScatteredLineBrush::ScatteredLineBrush(ImpressionistDoc* pDoc, char* name)
	: LineBrush(pDoc, name), m_numLines(4) {
}

void ScatteredLineBrush::BrushMove(const Point source, const Point target) {
	ImpressionistDoc* pDoc = GetDocument();

	if (pDoc == NULL) {
		printf("ScatteredLineBrush::BrushMove  document is NULL\n");
		return;
	}

	// return if it is the first draw of the movement
	if (m_mode == Mode::MOVEMENT && m_prevTarget.x == -1) {
		m_prevTarget = target;
		return;
	}
	
	m_numLines = rand() % 2 + 3;

	int sx, sy;
	for (int i = 0; i < m_numLines; ++i) {
		sx = rand() % m_size - (int)(m_size / 2);
		sy = rand() % m_size - (int)(m_size / 2);
		
		updateAttributes(Point(source.x + sx, source.y + sy), Point(target.x + sx, target.y + sy));

		if (m_mode == Mode::MOVEMENT) // correct angle for movement brush
			m_angle = int(atan2(m_prevTarget.y - target.y, m_prevTarget.x - target.x) / PI * 180);

		drawLine(Point(source.x + sx, source.y + sy), Point(target.x + sx, target.y + sy));
	}

	m_prevTarget = target;
}