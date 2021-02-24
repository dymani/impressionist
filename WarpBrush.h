#ifndef WARP_BRUSH_H
#define WARP_BRUSH_H

#include "ImpBrush.h"
#include <FL/Fl.H>
#include <imgwarp/imgwarp_mls.h>
#include <vector>

class WarpBrush : public ImpBrush {
public:
	WarpBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);
	void BrushBegin(const IPoint source, const IPoint target);
	void BrushMove(const IPoint source, const IPoint target);
	void BrushEnd(const IPoint source, const IPoint target);
protected:
	void initKernel();
	int m_size;
	IPoint m_prev;
	double* m_kernel;

	int m_startX, m_startY;

	ImgWarp_MLS* m_imageWarp;
	cv::Mat* m_paintingMat;

	std::vector<cv::Point> m_anchors;
};

#endif