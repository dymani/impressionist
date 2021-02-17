#include "Marker.h"
#include "ImpressionistDoc.h"

Marker::Marker() : m_source(100, 100) {
}

void Marker::update(const Point source) {
	m_source = source;
}

void Marker::draw(int yOffset) {
	glBegin(GL_QUADS);
		glColor3f(1.0, 0.0, 0.0);
		glVertex2d(m_source.x - SIZE / 2, m_source.y - SIZE / 2 + yOffset);
		glVertex2d(m_source.x + SIZE / 2, m_source.y - SIZE / 2 + yOffset);
		glVertex2d(m_source.x + SIZE / 2, m_source.y + SIZE / 2 + yOffset);
		glVertex2d(m_source.x - SIZE / 2, m_source.y + SIZE / 2 + yOffset);
	glEnd();
}