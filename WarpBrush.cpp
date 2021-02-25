#include "WarpBrush.h"
#include "ConvolutionManager.h"
#include "ImpressionistDoc.h"
#include "ImpressionistUI.h"

#include <imgwarp/imgwarp_mls_rigid.h>
#include <imgwarp/imgwarp_mls_similarity.h>
#include <imgwarp/imgwarp_piecewiseaffine.h>
#include <vector>

const double PI = 3.1415926536;

WarpBrush::WarpBrush(ImpressionistDoc* pDoc, char* name) 
	: ImpBrush(pDoc, name), m_size(1) {
	m_start = { -1, -1 };
	m_imageWarp = new ImgWarp_PieceWiseAffine();
	m_imageWarp->alpha = 255;
	m_imageWarp->gridSize = 5;
	((ImgWarp_PieceWiseAffine*)m_imageWarp)->backGroundFillAlg = ImgWarp_PieceWiseAffine::BGMLS;
	m_paintingCopy = nullptr;
}

void WarpBrush::BrushBegin(const IPoint source, const IPoint target) {
	ImpressionistDoc* pDoc = GetDocument();

	if (pDoc == NULL) {
		printf("WarpBrush::BrushMove  document is NULL\n");
		return;
	}

	m_size = pDoc->getSize() * 2;
	m_strength = pDoc->getStrength();

	m_start = { target.x, target.y };

	if (m_paintingCopy) delete[] m_paintingCopy;
	m_paintingCopy = new unsigned char[pDoc->m_nWidth * pDoc->m_nHeight * 3];
	memcpy(m_paintingCopy, pDoc->getPainting(), pDoc->m_nWidth * pDoc->m_nHeight * 3);
	m_paintingMat = new Mat{ pDoc->m_nHeight, pDoc->m_nWidth, CV_8UC3, m_paintingCopy };
}

cv::Point project(cv::Point distance, cv::Point line) {
	float proj = distance.x * line.x + distance.y * line.y;
	if (proj <= 0)
		return { 0, 0 };
	proj /= (line.x * line.x + line.y * line.y);
	return cv::Point{ int(line.x * proj), int(line.y * proj) };
}


void WarpBrush::BrushMove(const IPoint source, const IPoint target) {
	ImpressionistDoc* pDoc = GetDocument();

	if (m_start.x == -1 && m_start.y == -1) {
		BrushBegin(source, target);
		return;
	}

	int width = pDoc->m_nWidth;
	int height = pDoc->m_nHeight;

	int Dx = target.x - m_start.x;
	int Dy = target.y - m_start.y;
	double dist = sqrt(Dx * Dx + Dy * Dy);
	double distLimit = m_size / 2.0 * m_strength;
	if (dist > distLimit) {
		Dx = Dx * distLimit / dist;
		Dy = Dy * distLimit / dist;
	}

	int left = m_start.x - distLimit * 2;
	if (left < 0) left = 0;
	int right = m_start.x + distLimit * 2;
	if (right >= width) right = width - 1;
	int top = m_start.y + distLimit * 2;
	if (top >= height) top = height - 1;
	int down = m_start.x - distLimit * 2;
	if (down < 0) down = 0;

	std::vector<cv::Point> anchorBefore;
	std::vector<cv::Point> anchorAfter;
	{
		anchorBefore.push_back(cv::Point{ 0, 0 });
		anchorBefore.push_back(cv::Point{ 0, height - 1 });
		anchorBefore.push_back(cv::Point{ width - 1, 0 });
		anchorBefore.push_back(cv::Point{ width - 1, height - 1 });

		anchorAfter.push_back(cv::Point{ 0, 0 });
		anchorAfter.push_back(cv::Point{ 0, height - 1 });
		anchorAfter.push_back(cv::Point{ width - 1, 0 });
		anchorAfter.push_back(cv::Point{ width - 1, height - 1 });;
	}
	{
		anchorBefore.push_back(cv::Point{ left, top });
		anchorBefore.push_back(cv::Point{ right, top });
		anchorBefore.push_back(cv::Point{ left, down });
		anchorBefore.push_back(cv::Point{ right, down });

		anchorAfter.push_back(cv::Point{ left, top });
		anchorAfter.push_back(cv::Point{ right, top });
		anchorAfter.push_back(cv::Point{ left, down });
		anchorAfter.push_back(cv::Point{ right, down });
	}
	{
		anchorBefore.push_back(m_start);
		anchorAfter.push_back(m_start + cv::Point{ Dx, Dy });
	}
	cv::Point line, proj;
	int dir;
	int dot = 0;
	for (int i = 0; i < 8; ++i) {
		line = cv::Point{ int(m_size / 2.0 * cos(i * PI / 4)) , int(m_size / 2.0 * sin(i * PI / 4)) };
		if (line.dot({ Dx, Dy }) > dot) {
			dir = i;
			dot = line.dot({ Dx, Dy });
		}
	}
	for (int i = 0; i < 8; ++i) {		
		if ((i - dir + 8) % 8 <= 1 || (i - dir + 8) % 8 == 7) {
			line = cv::Point{ int(m_size / 2.0 * cos(dir * PI / 4)) , int(m_size / 2.0 * sin(dir * PI / 4)) };
			anchorBefore.push_back(m_start + line);
			anchorAfter.push_back(m_start + line + cv::Point{ Dx, Dy });
		}
		else {
			line = cv::Point{ int(m_size / 2.0 * cos(i * PI / 4)) , int(m_size / 2.0 * sin(i * PI / 4)) };
			anchorBefore.push_back(m_start + line);
			anchorAfter.push_back(m_start + line);
		}
	}

	Mat result = m_imageWarp->setAllAndGenerate(
		*m_paintingMat,
		anchorBefore, anchorAfter,
		width, height, 1);
	GLubyte* c = new unsigned char[3];
	glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(0, pDoc->m_pUI->m_paintView->getWindowHeight() - pDoc->m_pUI->m_paintView->getDrawHeight());
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, pDoc->m_nPaintWidth);
	glDrawPixels(pDoc->m_pUI->m_paintView->getDrawWidth(),
		pDoc->m_pUI->m_paintView->getDrawHeight(),
		GL_RGB,
		GL_UNSIGNED_BYTE,
		result.ptr());

	delete[] c;
}

void WarpBrush::BrushEnd(const IPoint source, const IPoint target) {
	m_start = { -1, -1 };
}