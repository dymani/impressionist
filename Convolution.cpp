#include "Convolution.h"
#include <cstring>

Convolution::Convolution(const int kernel[], int kWidth, int kHeight, bool isNormalized)
	: m_kWidth(kWidth), m_kHeight(kHeight) {
	
	m_kernel = new int[m_kWidth * m_kHeight];
	for (int i = 0; i < m_kWidth * m_kHeight; ++i) {
		m_kernel[i] = kernel[i];
	}
	
	setNormalized(isNormalized);

	m_image = nullptr;
	m_iWidth = m_iHeight = 0;

	m_convolution = nullptr;
}

Convolution::~Convolution() {
	if (m_kernel) delete[] m_kernel;
	if (m_image) delete[] m_image;
	if (m_convolution) delete[] m_convolution;
}

void Convolution::setImage(const unsigned char* image, int iWidth, int iHeight, bool isRgb) {
	if (m_image) {
		if (m_iWidth * m_iHeight != iWidth * iHeight) {
			m_iWidth = iWidth;
			m_iHeight = iHeight;
			delete[] m_image;
			m_image = new unsigned char[m_iWidth * m_iHeight];
		}
	}
	else {
		m_iWidth = iWidth;
		m_iHeight = iHeight;
		m_image = new unsigned char[m_iWidth * m_iHeight];
	}
	if (m_convolution) {
		delete[] m_convolution;
		m_convolution = nullptr;
	}
	if (isRgb) {
		for (int i = 0; i < m_iWidth * m_iHeight; ++i) {
			m_image[i] = getValueFromRgb(image + i * 3);
		}
	}
	else {
		for (int i = 0; i < m_iWidth * m_iHeight; ++i) {
			m_image[i] = image[i];
		}
	}
}

void Convolution::setNormalized(bool isNormalized) {
	if (isNormalized) {
		m_normalizeFactor = 0;
		for (int i = 0; i < m_kWidth * m_kHeight; ++i)
			m_normalizeFactor += m_kernel[i];
		m_normalizeFactor = 1 / m_normalizeFactor;
	}
	else {
		m_normalizeFactor = 1;
	}
}

void Convolution::compute() {
	if (!m_image)
		return;
	if (m_convolution)
		return;
	m_convolution = new double[m_iWidth * m_iHeight];
	memset(m_convolution, 0, m_iWidth * m_iHeight * sizeof(double));
	
	for (int row = 0; row < m_iHeight; ++row) {
		for (int col = 0; col < m_iWidth; ++col) { // for each position
			int sum = 0;
			for (int dy = -(m_kHeight / 2); dy <= m_kHeight / 2; ++dy) {
				for (int dx = -(m_kWidth / 2); dx <= m_kWidth / 2; ++dx) {
					int x = col + dx;
					int y = row+ dy;
					if (x < 0) x = (-x - 1) % m_iWidth;
					if (x >= m_iWidth) x = (-x - 1) % m_iWidth + m_iWidth;
					if (y < 0) y = (-y - 1) % m_iHeight;
					if (y >= m_iHeight) y = (-y - 1) % m_iHeight + m_iHeight;
					sum += m_image[y * m_iWidth + x] * m_kernel[(dy + (m_kHeight / 2)) * m_kWidth + dx + (m_kWidth / 2)];
				}
			}
			m_convolution[row * m_iWidth + col] = sum * m_normalizeFactor;
		}
	}
}

double Convolution::getPixelResult(int x, int y) {
	if (!m_image)
		return 0;
	if (m_convolution) {
		if (x < 0) x = (-x - 1) % m_iWidth;
		if (x >= m_iWidth) x = (-x - 1) % m_iWidth + m_iWidth;
		if (y < 0) y = (-y - 1) % m_iHeight;
		if (y >= m_iHeight) y = (-y - 1) % m_iHeight + m_iHeight;
		return m_convolution[y * m_iWidth + x];
	}
	int sum = 0;
	for (int dy = -(m_kHeight / 2); dy <= m_kHeight / 2; ++dy) {
		for (int dx = -(m_kWidth / 2); dx <= m_kWidth / 2; ++dx) {
			int sx = x + dx;
			int sy = y + dy;
			if (sx < 0) sx = (-sx - 1) % m_iWidth;
			if (sx >= m_iWidth) sx = (-sx - 1) % m_iWidth + m_iWidth;
			if (sy < 0) sy = (-sy - 1) % m_iHeight;
			if (sy >= m_iHeight) sy = (-sy - 1) % m_iHeight + m_iHeight;
			sum += m_image[sy * m_iWidth + sx] * m_kernel[(dy + (m_kHeight / 2)) * m_kWidth + dx + (m_kWidth / 2)];
		}
	}
	return sum * m_normalizeFactor;	
}

unsigned char* Convolution::generateResultImage() {
	if (!m_image)
		return nullptr;
	compute();
	unsigned char* result = new unsigned char[m_iWidth * m_iHeight];
	double r;
	for (int row = 0; row < m_iHeight; ++row) {
		for (int col = 0; col < m_iWidth; ++col) {
			r = m_convolution[row * m_iWidth + col];
			r = r < 0 ? 0 : r;
			r = r > 255 ? 255 : r;
			result[row * m_iWidth + col] = (unsigned char)r;
		}
	}
	return result;
}

unsigned char Convolution::getValueFromRgb(const unsigned char rgb[]) {
	return (unsigned char)(0.299 * rgb[0] + 0.587 * rgb[1] + 0.114 * rgb[2]);
}