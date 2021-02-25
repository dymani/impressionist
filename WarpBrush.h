#ifndef WARP_BRUSH_H
#define WARP_BRUSH_H

#include "ImpBrush.h"
#include <FL/Fl.H>
#include <imgwarp/imgwarp_mls.h>

class WarpBrush : public ImpBrush {
public:
	WarpBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	void BrushBegin(const IPoint source, const IPoint target);
	void BrushMove(const IPoint source, const IPoint target);
	void BrushEnd(const IPoint source, const IPoint target);
protected:
	int m_size;
	int m_strength;

	cv::Point m_start;

	ImgWarp_MLS* m_imageWarp;
	cv::Mat* m_paintingMat;
	unsigned char* m_paintingCopy;
};

#endif