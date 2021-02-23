#include "LineOverlay.h"
#include "ImpressionistDoc.h"
#include "ImpressionistUI.h"

#include "LineBrush.h"
#include "ScatteredLineBrush.h"

#include <cmath>

const double PI = 3.14159;

LineOverlay::LineOverlay()
	: m_size(1), m_angle(0), m_validBrush(false) {
}

void LineOverlay::setStart(const Point target) {
	if (!m_pDoc) return;
	m_validBrush = false;

	if (dynamic_cast<LineBrush*>(m_pDoc->m_pCurrentBrush)
		&& dynamic_cast<LineBrush*>(m_pDoc->m_pCurrentBrush)->getMode() == LineBrush::Mode::SLIDER) {
		m_validBrush = true;
		m_start = target;
		m_size = m_pDoc->getSize();
		m_angle = m_pDoc->getAngle();
	}
}

void LineOverlay::draw(const Point target) {
	if (!m_pDoc) return;
	if (!m_validBrush) return;
	
	int dx = target.x - m_start.x;
	int dy = target.y - m_start.y;
	m_size = int(sqrt(dx * dx + dy * dy) + 0.5);
	if (m_size > 40) m_size = 40;
	m_angle = int(atan2(dy, dx) / PI * 180);

	glBegin(GL_LINE_STRIP);
		glColor3f(1.0, 0.0, 0.0);
		glVertex2d(m_start.x, m_start.y);
		glVertex2d(int(m_start.x + m_size * cos(m_angle * PI / 180)), int(m_start.y + m_size * sin(m_angle * PI / 180)));
	glEnd();
}

void LineOverlay::release(const Point target) {
	if (!m_pDoc) return;
	if (m_validBrush) {
		m_pDoc->setSize(m_size);
		m_pDoc->m_pUI->m_BrushSizeSlider->value(m_size);
		m_pDoc->setAngle(m_angle);
	}
	m_validBrush = false;
}