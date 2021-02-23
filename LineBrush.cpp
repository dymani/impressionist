#include "LineBrush.h"

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "ConvolutionManager.h"

#include <cmath>

const double PI = 3.14159265358;

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name)
	: ImpBrush(pDoc, name), m_size(1), m_width(1), m_angle(0), m_mode(Mode::SLIDER) {
}

void LineBrush::BrushBegin(const Point source, const Point target) {
	ImpressionistDoc* pDoc = GetDocument();

	// save mouse location
	m_prevTargets.clear();
	m_prevTargets.push_back(target);
	
	updateAttributes(source, target);
		
	// do not draw when using mouse direction
	if (m_mode != Mode::MOVEMENT)
		BrushMove(source, target);
}

void LineBrush::BrushMove(const Point source, const Point target) {
	ImpressionistDoc* pDoc = GetDocument();

	if (pDoc == NULL) {
		printf("LineBrush::BrushMove  document is NULL\n");
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
	
	updateAttributes(source, target);

	drawLine(source, target);
}

void LineBrush::BrushEnd(const Point source, const Point target) {
	m_prevTargets.clear();
}

inline GLubyte getLuma(const GLubyte color[]) {
	return GLubyte(0.299 * color[0] + 0.587 * color[1] + 0.114 * color[2]);
}

void LineBrush::updateAttributes(const Point source, const Point target) {
	ImpressionistDoc* pDoc = GetDocument();

	m_size = pDoc->getSize();

	switch (m_mode) {
		case Mode::SLIDER:
			m_angle = pDoc->getAngle();
			break;
		case Mode::GRADIENT:
			{
				double x = pDoc->m_convolutionManager->getPresetResultPixel(ConvolutionManager::GRADIENT_SOBEL_X, target.x, target.y);
				double y = pDoc->m_convolutionManager->getPresetResultPixel(ConvolutionManager::GRADIENT_SOBEL_Y, target.x, target.y);
				m_angle = int(atan2(x, y) / PI * 180);
				break;
			}
		case Mode::MOVEMENT:
			if (m_prevTargets.size() <= 1)
				break;
			m_angle = angleRegression();
			break;
	}

	m_width = pDoc->getWidth();	
}

int LineBrush::angleRegression() {
	double xBar = 0, yBar = 0;
	for (Point& p : m_prevTargets) {
		xBar += p.x;
		yBar += p.y;
	}
	xBar = xBar / m_prevTargets.size();
	yBar = yBar / m_prevTargets.size();
	double sXY = 0, sX2 = 0, sY2 = 0;
	for (Point& p : m_prevTargets) {
		sXY += (p.x - xBar) * (p.y - yBar);
		sX2 += (p.x - xBar) * (p.x - xBar);
		sY2 += (p.y - yBar) * (p.y - yBar);
	}
	if (sXY == 0 && sX2 == 0)
		return 90;
	else if (sXY == 0 && sY2 == 0)
		return 0;
	else if (sX2 < sY2)
		return int(atan2(sY2, sXY) * 180 / PI);
	else
		return int(atan2(sXY, sX2) * 180 / PI);
}

void LineBrush::drawLine(const Point source, const Point target) {
	ImpressionistDoc* pDoc = GetDocument();
	if (pDoc->m_isEdgeClippingOn) {
		GLubyte color[3];
		memcpy(color, pDoc->getEdgePixel(source.x, source.y), 3);
		double sinT = sin(m_angle * PI / 180);
		double cosT = cos(m_angle * PI / 180);
		double y = m_width / 2.0;
		glBegin(GL_QUAD_STRIP);
			SetColor(source, pDoc->getAlpha());
			for (double i = 0; i <= m_size / 2; ++i) {
				if (!isEqual(color, pDoc->getEdgePixel(int(source.x - i * cosT), int(source.y - i * sinT))))
					break;
				glVertex2d(target.x - i * cosT + y * sinT, target.y - i * sinT - y * cosT);
				glVertex2d(target.x - i * cosT - y * sinT, target.y - i * sinT + y * cosT);
			}
		glEnd();
		glBegin(GL_QUAD_STRIP);
			SetColor(source, pDoc->getAlpha());
			for (double i = 0; i <= m_size / 2; ++i) {
				if (!isEqual(color, pDoc->getEdgePixel(int(source.x + i * cosT), int(source.y + i * sinT))))
					break;
				glVertex2d(target.x + i * cosT + y * sinT, target.y + i * sinT - y * cosT);
				glVertex2d(target.x + i * cosT - y * sinT, target.y + i * sinT + y * cosT);
			}
		glEnd();
	}
	else {
		glBegin(GL_TRIANGLE_STRIP);
			SetColor(source, pDoc->getAlpha());
			double sinT = sin(m_angle * PI / 180);
			double cosT = cos(m_angle * PI / 180);
			double x = m_size / 2.0;
			double y = m_width / 2.0;
			glVertex2d(target.x + x * cosT - y * sinT, target.y + x * sinT + y * cosT);
			glVertex2d(target.x - x * cosT - y * sinT, target.y - x * sinT + y * cosT);
			glVertex2d(target.x + x * cosT + y * sinT, target.y + x * sinT - y * cosT);
			glVertex2d(target.x - x * cosT + y * sinT, target.y - x * sinT - y * cosT);
		glEnd();
	}
}