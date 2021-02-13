#include "LineBrush.h"

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include <cmath>

const double PI = 3.14159265358;

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

	// return if it is the first draw of the movement
	if (m_mode == Mode::MOVEMENT && m_prevTarget.x == -1) {
		m_prevTarget = target;
		return;
	}		

	updateAttributes(source, target);;

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

	m_prevTarget = target;
}

void LineBrush::BrushEnd(const Point source, const Point target) {
	m_prevTarget.x = -1;
}

inline GLubyte getLuma(const GLubyte color[]) {
	return GLubyte((color[0] + color[0] + color[1] + color[1] + color[1] + color[2]) / 6);
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
			m_angle = int(atan2(m_prevTarget.y - target.y, m_prevTarget.x - target.x) / PI * 180);
			break;
	}

	m_width = pDoc->getWidth();
	
}
