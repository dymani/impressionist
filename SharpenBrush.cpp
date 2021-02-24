#include "SharpenBrush.h"

#include "impressionistDoc.h"
#include "impressionistUI.h"

SharpenBrush::SharpenBrush(ImpressionistDoc* pDoc, char* name)
	: ImpBrush(pDoc, name) {
	m_size = 0;
}

void SharpenBrush::BrushBegin(const IPoint source, const IPoint target) {
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	m_size = pDoc->getSize();
	pDoc->m_convolutionManager->updateFilterBrushImage(ConvolutionManager::BRUSH_SHARPEN, pDoc->getPainting(), pDoc->m_nPaintWidth, pDoc->m_nPaintHeight);


	BrushMove(source, target);
}

void SharpenBrush::BrushMove(const IPoint source, const IPoint target) {
	ImpressionistDoc* pDoc = GetDocument();

	if (pDoc == NULL) {
		printf("SharpenBrush::BrushMove  document is NULL\n");
		return;
	}

	unsigned char* color3;
	GLubyte color[4];
	glPointSize((float)1);
	glBegin(GL_POINTS);
	for (int dx = -m_size / 2; dx <= m_size / 2; ++dx) {
		for (int dy = -m_size / 2; dy <= m_size / 2; ++dy) {
			color3 = pDoc->m_convolutionManager->getFilterBrushPixel(ConvolutionManager::BRUSH_SHARPEN, target.x + dx, target.y + dy);
			memcpy(color, color3, 3 * sizeof(unsigned char));
			color[3] = pDoc->getAlpha();
			glColor4ubv(color);
			glVertex2d((double)target.x + dx, (double)target.y + dy);
			delete[] color3;
		}
	}		
	glEnd();
}

void SharpenBrush::BrushEnd(const IPoint source, const IPoint target) {
	m_size = 0;
}