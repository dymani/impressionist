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

	GLubyte color[3];
	memcpy(color, pDoc->getEdgePixel(source.x, source.y), 3);
	double sinT = sin(m_angle * PI / 180);
	double cosT = cos(m_angle * PI / 180);
	double y = m_width / 2.0;

	int sx, sy;
	for (int i = 0; i < m_numLines; ++i) {
		sx = rand() % m_size - (int)(m_size / 2);
		sy = rand() % m_size - (int)(m_size / 2);


		if (pDoc->m_isEdgeClippingOn) {
			double lim1 = 0, lim2 = 0;
			for (double j = 0; j <= m_size / 2; ++j) {
				if (!isEqual(color, pDoc->getEdgePixel(source.x - j * cosT, source.y - j * sinT)))
					break;
				lim1 = j;
			}
			for (double j = 0; j <= m_size / 2; ++j) {
				if (!isEqual(color, pDoc->getEdgePixel(source.x + j * cosT, source.y + j * sinT)))
					break;
				lim2 = j;
			}
			double px = sx * cosT + sy * sinT;
			if (px < -lim1 || px > lim2) {
				--i;
				continue;
			}
		}

		updateAttributes(Point(source.x + sx, source.y + sy), Point(target.x + sx, target.y + sy));

		if (m_mode == Mode::MOVEMENT) // correct angle for movement brush
			m_angle = correctAngle;

		drawLine(Point(source.x + sx, source.y + sy), Point(target.x + sx, target.y + sy));
	}

	while (m_prevTargets.size() > MOUSE_HISTORY_SIZE)
		m_prevTargets.erase(m_prevTargets.begin());
}