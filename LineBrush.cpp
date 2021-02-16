#include "LineBrush.h"

#include "impressionistDoc.h"
#include "impressionistUI.h"

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

	GLubyte color[3];
	int sumX = 0, sumY = 0;
	switch (m_mode) {
		case Mode::SLIDER:
			m_angle = pDoc->getAngle();
			break;
		case Mode::GRADIENT:
			for (int i = 0; i < 3; ++i) {
				for (int j = 0, luma; j < 3; ++j) {					
					luma = 0;
					// gaussian 3x3 kernel
					/*for (int y = 0; y < 3; ++y) {
						for (int x = 0; x < 3; ++x) {
							memcpy(color, pDoc->GetOriginalPixel(source.x - 2 + j + x, source.y - 2 + i + y), 3);
							luma += getLuma(color) * m_gaussian[y * 3 + x];
						}
					}
					sumX += luma * m_sobelX[i * 3 + j] / 16;
					sumY += luma * m_sobelY[i * 3 + j] / 16;*/	

					// gaussian 5x5 kernel
					for (int y = 0; y < 5; ++y) {
						for (int x = 0; x < 5; ++x) {
							memcpy(color, pDoc->GetOriginalPixel(source.x - 3 + j + x, source.y - 3 + i + y), 3);
							luma += getLuma(color) * m_gaussian5[y * 5 + x];
						}
					}
					sumX += luma * m_sobelX[i * 3 + j] / 273;
					sumY += luma * m_sobelY[i * 3 + j] / 273;
				}
			}			

			m_angle = int(atan2(sumX, sumY) / PI * 180);
			break;
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