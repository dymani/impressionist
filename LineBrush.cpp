#include "LineBrush.h"

#include "impressionistDoc.h"
#include "impressionistUI.h"

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name)
	: ImpBrush(pDoc, name) {
}

void LineBrush::BrushBegin(const Point source, const Point target) {
	ImpressionistDoc* pDoc = GetDocument();

	m_size = pDoc->getSize();

	BrushMove(source, target);
}

void LineBrush::BrushMove(const Point source, const Point target) {
	ImpressionistDoc* pDoc = GetDocument();

	if (pDoc == NULL) {
		printf("LineBrush::BrushMove  document is NULL\n");
		return;
	}

	glBegin(GL_LINE_STRIP);
		SetColor(source);
		glVertex2d(target.x - m_size / 2.0, target.y);
		glVertex2d(target.x + m_size / 2.0, target.y);
	glEnd();
}

void LineBrush::BrushEnd(const Point source, const Point target) {
}
