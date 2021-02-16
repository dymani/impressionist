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

	m_prevTargets.push_back(target);
	while (m_prevTargets.size() > MOUSE_HISTORY_SIZE)
		m_prevTargets.erase(m_prevTargets.begin());

	// return if it is the first draw of the movement
	if (m_mode == Mode::MOVEMENT && m_prevTargets.size() < 3) {
		m_prevTargets.push_back(target);
		return;
	}

	m_numLines = rand() % 2 + 3;

	int correctAngle = angleRegression();

	int sx, sy;
	for (int i = 0; i < m_numLines; ++i) {
		sx = rand() % m_size - (int)(m_size / 2);
		sy = rand() % m_size - (int)(m_size / 2);
		
		updateAttributes(Point(source.x + sx, source.y + sy), Point(target.x + sx, target.y + sy));

		if (m_mode == Mode::MOVEMENT) // correct angle for movement brush
			m_angle = correctAngle;

		drawLine(Point(source.x + sx, source.y + sy), Point(target.x + sx, target.y + sy));
	}

	while (m_prevTargets.size() > MOUSE_HISTORY_SIZE)
		m_prevTargets.erase(m_prevTargets.begin());
}