#ifndef ALPHA_MAPPED_BRUSH_H
#define ALPHA_MAPPED_BRUSH_H

#include "ImpBrush.h"

class AlphaMappedBrush : public ImpBrush
{
public:
	AlphaMappedBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const IPoint source, const IPoint target);
	void BrushMove(const IPoint source, const IPoint target);
	void BrushEnd(const IPoint source, const IPoint target);
	char* BrushName(void);
	void updateAlphaMap(unsigned char* map, int width, int height);
protected:
	unsigned char* m_alphaMap;
	int m_width, m_height;
};

#endif#pragma once
