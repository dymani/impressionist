#include "SmudgeBrush.h"
#include "ConvolutionManager.h"
#include "ImpressionistDoc.h"
#include "ImpressionistUI.h"


SmudgeBrush::SmudgeBrush(ImpressionistDoc* pDoc, char* name)
	: ImpBrush(pDoc, name), m_size(1) {
	m_prev = { -1, -1 };
	m_kernel = nullptr;
}

void SmudgeBrush::BrushBegin(const IPoint source, const IPoint target) {
	ImpressionistDoc* pDoc = GetDocument();

	if (pDoc == NULL) {
		printf("SmudgeBrush::BrushMove  document is NULL\n");
		return;
	}
	

	m_size = pDoc->getSize();
	if (m_size % 2 == 0)
		++m_size;

	
	if (m_kernel) {
		delete[] m_kernel;
		m_kernel = nullptr;
	}
	initKernel();
	m_prev = target;
}

void SmudgeBrush::BrushMove(const IPoint source, const IPoint target) {
	initKernel();
	ImpressionistDoc* pDoc = GetDocument();

	if (m_prev.x == -1 && m_prev.y == -1) {
		m_prev = target;
		return;
	}

	int width = pDoc->m_nWidth;
	int height = pDoc->m_nHeight;

	int startX = m_prev.x;
	int startY = m_prev.y;
	int targetX = target.x;
	int targetY = target.y;

	int distX = targetX - startX;
	int distY = targetY - startY;

	int paintX, paintY;
	int colorX, colorY;

	glPointSize((float)1);
	glBegin(GL_POINTS);
	for (int dx = - m_size / 2; dx <= m_size / 2; ++dx) {
		paintX = startX + dx;
		if (paintX < 0 || paintX >= width)
			continue;
		for (int dy = -m_size / 2; dy <= m_size / 2; ++dy) {
			paintY = startY + dy;
			if (paintY < 0 || paintY >= height)
				continue;

			if ((double)dx * dx + (double)dy * dy >= m_size / 2.0 * m_size / 2.0)
				continue;

			colorX = int(paintX - distX * m_kernel[(dy + m_size / 2) * m_size + dx + m_size / 2]);
			colorY = int(paintY - distY * m_kernel[(dy + m_size / 2) * m_size + dx + m_size / 2]);


			GLubyte* c = pDoc->getPaintPixel(colorX, colorY);
			SetColor(c, pDoc->getAlpha());
			glVertex2d(paintX, paintY);
		}
	}
	glEnd();

	pDoc->m_pUI->m_paintView->SaveCurrentContent();
	
	m_prev = target;
}

void SmudgeBrush::BrushEnd(const IPoint source, const IPoint target) {
	m_prev = { -1, -1 };
}

void SmudgeBrush::initKernel() {
	if (m_kernel)
		return;
	ImpressionistDoc* pDoc = GetDocument();
	float sigma = pDoc->getStrength() * 2;
	if (sigma < 1) sigma = 1;
	m_kernel = Kernel::generateGaussian(m_size, sigma);

	double center = m_kernel[m_size * m_size / 2];
	for (int i = 0; i <= m_size / 2; ++i) {
		for (int j = 0; j <= m_size / 2; ++j) {
			if (i * i + j * j >= m_size / 2.0 * m_size / 2.0) {
				m_kernel[(m_size / 2 + i) * m_size + m_size / 2 + j] = 0;
				m_kernel[(m_size / 2 - i) * m_size + m_size / 2 + j] = 0;
				m_kernel[(m_size / 2 + i) * m_size + m_size / 2 - j] = 0;
				m_kernel[(m_size / 2 - i) * m_size + m_size / 2 - j] = 0;
			}
		}
	}
	if (center == 0) center = 1;
	for (int i = 0; i < m_size; ++i) {
		for (int j = 0; j < m_size; ++j) {
			m_kernel[i * m_size + j] /= center;
		}
	}
	m_kernel[m_size * m_size / 2] = 1;
}