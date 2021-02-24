#include "WarpBrush.h"
#include "ConvolutionManager.h"
#include "ImpressionistDoc.h"
#include "ImpressionistUI.h"

#include <imgwarp/imgwarp_mls_rigid.h>
#include <imgwarp/imgwarp_mls_similarity.h>
#include <imgwarp/imgwarp_piecewiseaffine.h>

WarpBrush::WarpBrush(ImpressionistDoc* pDoc, char* name) 
	: ImpBrush(pDoc, name), m_size(1){
	m_prev = { -1, -1 };
	m_kernel = nullptr;
	m_size = 3;

	m_imageWarp = new ImgWarp_PieceWiseAffine();
	m_imageWarp->alpha = 255;
	m_imageWarp->gridSize = 5;
	((ImgWarp_PieceWiseAffine*)m_imageWarp)->backGroundFillAlg = ImgWarp_PieceWiseAffine::BGMLS;
}

void WarpBrush::BrushBegin(const IPoint source, const IPoint target) {
	ImpressionistDoc* pDoc = GetDocument();

	if (pDoc == NULL) {
		printf("WarpBrush::BrushMove  document is NULL\n");
		return;
	}

	m_prev = { -1, -1 };

	m_size = pDoc->getSize();
	if (m_size % 2 == 0)
		++m_size;

	m_anchors.clear();
	int step = sqrt(m_size) * 30;
	if (step < 100) step = 100;
	for (int x = 0; x < pDoc->m_nWidth; x += step) {
		for (int y = 0; y < pDoc->m_nHeight; y += step) {
			m_anchors.push_back(cv::Point(x, y));
		}
	}
	if (pDoc->m_nWidth % (step)) {
		m_anchors.push_back(cv::Point{ pDoc->m_nWidth - 1, 0 });
		if (pDoc->m_nHeight % (step))
			m_anchors.push_back(cv::Point{ pDoc->m_nWidth - 1, pDoc->m_nHeight - 1 });
	}
	if (pDoc->m_nHeight % (step))
		m_anchors.push_back(cv::Point{ 0, pDoc->m_nHeight - 1 });
	
	if (m_kernel) {
		delete[] m_kernel;
		m_kernel = nullptr;
	}
	initKernel();	
	m_prev = target;
	m_startX = target.x;
	m_startY = target.y;
	m_paintingMat = new Mat{ pDoc->m_nHeight, pDoc->m_nWidth, CV_8UC3, pDoc->getPainting() };
}

void WarpBrush::BrushMove(const IPoint source, const IPoint target) {
	initKernel();
	ImpressionistDoc* pDoc = GetDocument();

	if (m_prev.x == -1 && m_prev.y == -1) {
		m_prev = target;
		m_startX = target.x;
		m_startY = target.y;
		m_paintingMat = new Mat{ pDoc->m_nHeight, pDoc->m_nWidth, CV_8UC3, pDoc->getPainting() };
		return;
	}		

	int width = pDoc->m_nWidth;
	int height = pDoc->m_nHeight;

	int startX = m_prev.x;
	int startY = m_prev.y;
	int targetX = target.x;
	int targetY = target.y;

	int distX = targetX - startX;
	int distY = targetY - startY;

	int paintX, paintY;
	int colorX, colorY;

	/*glPointSize((float)1);
	glBegin(GL_POINTS);
	for (int dx = - m_size / 2; dx <= m_size / 2; ++dx) {
		paintX = startX + dx;
		if (paintX < 0 || paintX >= width)
			continue;
		for (int dy = -m_size / 2; dy <= m_size / 2; ++dy) {
			paintY = startY + dy;
			if (paintY < 0 || paintY >= height)
				continue;

			if ((double)dx * dx + (double)dy * dy >= m_size / 2.0 * m_size / 2.0)
				continue;

			colorX = int(paintX - distX * m_kernel[(dy + m_size / 2) * m_size + dx + m_size / 2]);
			colorY = int(paintY - distY * m_kernel[(dy + m_size / 2) * m_size + dx + m_size / 2]);

			
			GLubyte* c = pDoc->getPaintPixel(colorX, colorY);
			SetColor(c, pDoc->getAlpha());
			glVertex2d(paintX, paintY);
		}
	}
	glEnd();
	*/

	auto anchor1 = std::vector<cv::Point>(m_anchors);
	anchor1.push_back({ m_startX, m_startY });
	auto anchor2 = std::vector<cv::Point>(m_anchors);
	anchor2.push_back({ targetX, targetY });

	Mat result = m_imageWarp->setAllAndGenerate(
		*m_paintingMat,
		anchor1, anchor2,
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


	//for (int y = 0; y < height; ++y) {
	//	for (int x = 0; x < width; ++x) {
	//		cv::Vec3b color = result.at<cv::Vec3b>(y, x);
	//		c[0] = color[0];
	//		c[1] = color[1];
	//		c[2] = color[2];
	//		SetColor(c, pDoc->getAlpha());
	//		glVertex2d(x, y);
	//		/*pDoc->getBitmap()[(y * width + x) * 3 + 0] = color[0];
	//		pDoc->getBitmap()[(y * width + x) * 3 + 1] = color[1];
	//		pDoc->getBitmap()[(y * width + x) * 3 + 2] = color[2];*/
	//	}
	//}

	delete[] c;

	//pDoc->m_pUI->m_origView->refresh();
	m_prev = target;
}

void WarpBrush::BrushEnd(const IPoint source, const IPoint target) {
}

void WarpBrush::initKernel() {
	if (m_kernel)
		return;
	ImpressionistDoc* pDoc = GetDocument();
	float sigma = pDoc->getWidth() / 2.0f;
	if (sigma < 1) sigma = 1;
	m_kernel = Kernel::generateGaussian(m_size, sigma);

	double center = m_kernel[m_size * m_size / 2];
	for (int i = 0; i <= m_size / 2; ++i) {
		for (int j = 0; j <= m_size / 2; ++j) {
			if (i * i + j * j >= m_size / 2.0 * m_size / 2.0) {
				m_kernel[(m_size / 2 + i) * m_size + m_size / 2 + j] = 0;
				m_kernel[(m_size / 2 - i) * m_size + m_size / 2 + j] = 0;
				m_kernel[(m_size / 2 + i) * m_size + m_size / 2 - j] = 0;
				m_kernel[(m_size / 2 - i) * m_size + m_size / 2 - j] = 0;
			}
		}
	}
	for (int i = 0; i < m_size; ++i) {
		for (int j = 0; j < m_size; ++j) {
			m_kernel[i * m_size + j] /= center;
		}
	}
	m_kernel[m_size * m_size / 2] = 1;
}