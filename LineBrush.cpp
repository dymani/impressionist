#include "LineBrush.h"

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include <cmath>

const float PI = 3.14159;

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name)
	: ImpBrush(pDoc, name), m_size(1), m_width(1), m_angle(0), m_mode(Mode::SLIDER), m_prevTarget(-1, 0) {
}

void LineBrush::BrushBegin(const Point source, const Point target) {
	ImpressionistDoc* pDoc = GetDocument();

	updateAttributes(source, target);
		
	// do not draw when using mouse direction
	if (m_mode != Mode::MOVEMENT)
		BrushMove(source, target);

	// save mouse location
	m_prevTarget = target;
}

void LineBrush::BrushMove(const Point source, const Point target) {
	ImpressionistDoc* pDoc = GetDocument();

	if (pDoc == NULL) {
		printf("LineBrush::BrushMove  document is NULL\n");
		return;
	}

	// return if it is the first draw
	if (m_prevTarget.x == -1) {
		m_prevTarget = target;
		return;
	}		

	updateAttributes(source, target);

	glLineWidth((float)m_width);

	double dx = m_size / 2.0 * cos(m_angle * PI / 180);
	double dy = m_size / 2.0 * sin(m_angle * PI / 180);
	glBegin(GL_LINE_STRIP);
		SetColor(source);
		glVertex2d(target.x + dx, target.y + dy);
		glVertex2d(target.x - dx, target.y - dy);
	glEnd();

	m_prevTarget = target;
}

void LineBrush::BrushEnd(const Point source, const Point target) {
	m_prevTarget.x = -1;
}

void LineBrush::updateAttributes(const Point source, const Point target) {
	ImpressionistDoc* pDoc = GetDocument();

	// 0 - slider/mouse, 1 - gradient, 2 - movement
	m_mode = Mode::MOVEMENT;

	m_size = pDoc->getSize();

	switch (m_mode) {
		case Mode::SLIDER:
			// m_angle = pDoc->getAngle();
			m_angle = 30;
			break;
		case Mode::GRADIENT:
			m_angle = 30;
			break;
		case Mode::MOVEMENT:
			m_angle = atan2(m_prevTarget.y - target.y, m_prevTarget.x - target.x) / PI * 180;
			break;
	}

	// m_width = pDoc->getWdith();
	m_width = 1;
	
}
