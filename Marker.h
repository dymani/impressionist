#ifndef MARKER_H
#define MARKER_H

#include "ImpBrush.h"

class ImpressionistDoc;

class Marker {
public:
	Marker();
	void update(const IPoint source);
	void draw(int yOffset);
	ImpressionistDoc* m_pDoc;
private:
	const int SIZE = 4;
	IPoint m_source;
};

#endif