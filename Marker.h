#ifndef MARKER_H
#define MARKER_H

#include "ImpBrush.h"

class ImpressionistDoc;

class Marker {
public:
	Marker();
	void update(const Point source);
	void draw(int yOffset);
	ImpressionistDoc* m_pDoc;
private:
	const int SIZE = 4;
	Point m_source;
};

#endif