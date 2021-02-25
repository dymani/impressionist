#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "AlphaMappedBrush.h"

AlphaMappedBrush::AlphaMappedBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name) {
	m_alphaMap = nullptr;
}

void AlphaMappedBrush::BrushBegin(const IPoint source, const IPoint target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (!m_alphaMap)
		return;

	BrushMove(source, target);
}

void AlphaMappedBrush::BrushMove(const IPoint source, const IPoint target) {
	ImpressionistDoc* pDoc = GetDocument();

	if (pDoc == NULL) {
		printf("AlphaMappedBrush::BrushMove  document is NULL\n");
		return;
	}
	if (!m_alphaMap)
		return;

	GLubyte color3[3];
	GLubyte color4[4];
	glPointSize((float)1);
	glBegin(GL_POINTS);
		memcpy(color3, pDoc->GetOriginalPixel(source), 3);
		double R = pDoc->getRedVal();
		double G = pDoc->getGreenVal();
		double B = pDoc->getBlueVal();
		int alpha = pDoc->getAlpha();
		int luma;
		for (int y = 0; y < m_height; ++y) {
			for (int x = 0; x < m_width; ++x) {
				luma = 0.299 * m_alphaMap[(y * m_width + x) * 3] + 0.587 * m_alphaMap[(y * m_width + x) * 3 + 1] + 0.114 * m_alphaMap[(y * m_width + x) * 3 + 2];
				color4[0] = (int)(color3[0] * R );
				color4[1] = (int)(color3[1] * G );
				color4[2] = (int)(color3[2] * B );
				color4[3] = (int)(alpha * luma / 255.0);
				glColor4ubv(color4);
				glVertex2d(target.x + x - m_width / 2, target.y + y - m_height / 2);
			}
		}	
	glEnd();
}

void AlphaMappedBrush::BrushEnd(const IPoint source, const IPoint target) {
}

void AlphaMappedBrush::updateAlphaMap(unsigned char* map, int width, int height) {
	if (m_alphaMap)
		delete[] m_alphaMap;
	m_alphaMap = map;
	m_width = width;
	m_height = height;
}
