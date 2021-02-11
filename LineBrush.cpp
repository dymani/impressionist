#include "LineBrush.h"

#include "impressionistDoc.h"
#include "impressionistUI.h"

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name)
	: ImpBrush(pDoc, name) {
}

void LineBrush::BrushBegin(const Point source, const Point target) {
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	
	BrushMove(source, target);
}

void LineBrush::BrushMove(const Point source, const Point target) {
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	int size = pDoc->getSize();

	glBegin(GL_LINE_STRIP);
	SetColor(source);

	glVertex2d(target.x- size / 2.0, target.y);
	glVertex2d(target.x+ size / 2.0, target.y);

	glEnd();
}

void LineBrush::BrushEnd(const Point source, const Point target) {

}
