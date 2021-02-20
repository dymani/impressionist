#include "Convolution.h"
#include <cstring>
#include <iostream>

Convolution::Convolution(const int kernel[], int kWidth, int kHeight)
	: m_kWidth(kWidth), m_kHeight(kHeight) {
	
	m_kernel = new int[m_kWidth * m_kHeight];
	m_normalizeFactor = 0;
	for (int i = 0; i < m_kWidth * m_kHeight; ++i) {
		m_kernel[i] = kernel[i];
		m_normalizeFactor += kernel[i];
	}		
	m_normalizeFactor = 1 / m_normalizeFactor;

	m_isCharImage = false;
	m_imageC = nullptr;
	m_imageD = nullptr;
	m_iWidth = m_iHeight = 0;
	m_isRgbInput = false;

	m_rgbInputChannel = RgbInputChannel::LUMA;

	m_isNormalized = true;

	m_isRgbOutput = false;

	m_wasModified = true;

	m_result = nullptr;
}

Convolution::~Convolution() {
	if (m_kernel) delete[] m_kernel;
	if (m_imageC) delete[] m_imageC;
	if (m_imageD) delete[] m_imageD;
	if (m_result) delete[] m_result;
}

void Convolution::setImage(const unsigned char* image, int iWidth, int iHeight, bool isRgbInput) {
	m_isCharImage = true;
	bool canReuse = (m_iWidth == iWidth && m_iHeight == iHeight && m_isRgbInput == isRgbInput);
	if (!canReuse) {
		if (m_imageC) {
			delete[] m_imageC;
			m_imageC = nullptr;
		}
	}
	if (m_imageD) {
		delete[] m_imageD;
		m_imageD = nullptr;
	}
	if (m_result) {
		delete[] m_result;
		m_result = nullptr;
	}
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_isRgbInput = isRgbInput;
	if (m_isRgbInput) {
		if(!m_imageC)
			m_imageC = new unsigned char[m_iWidth * m_iHeight * 3];
		for (int i = 0; i < m_iHeight * m_iWidth; ++i) {
			m_imageC[i] = image[i * 3];
			m_imageC[m_iWidth * m_iHeight + i] = image[i * 3 + 1];
			m_imageC[m_iWidth * m_iHeight * 2 + i] = image[i * 3 + 2];
		}
	}
	else {
		if (!m_imageC)
			m_imageC = new unsigned char[m_iWidth * m_iHeight];
		memcpy(m_imageC, image, m_iWidth * m_iHeight * sizeof(unsigned char));
	}
	m_wasModified = true;
}

void Convolution::setImage(const double* image, int iWidth, int iHeight, bool isRgbInput) {
	m_isCharImage = false;
	bool canReuse = (m_iWidth == iWidth && m_iHeight == iHeight && m_isRgbInput == isRgbInput);
	if (!canReuse) {
		if (m_imageD) {
			delete[] m_imageD;
			m_imageD = nullptr;
		}
	}
	if (m_imageC) {
		delete[] m_imageC;
		m_imageC = nullptr;
	}
	if (m_result) {
		delete[] m_result;
		m_result = nullptr;
	}
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_isRgbInput = isRgbInput;
	if (m_isRgbInput) {
		if (!m_imageD) {
			m_imageD = new double[m_iWidth * m_iHeight * 3];
		}
		for (int i = 0; i < m_iHeight * m_iWidth; ++i) {
			m_imageD[i] = image[i * 3];
			m_imageD[m_iWidth * m_iHeight + i] = image[i * 3 + 1];
			m_imageD[m_iWidth * m_iHeight * 2 + i] = image[i * 3 + 2];
		}
	}
	else {
		if (!m_imageD) {
			m_imageD = new double[m_iWidth * m_iHeight];
		}
		memcpy(m_imageD, image, m_iWidth * m_iHeight * sizeof(double));
	}
	m_wasModified = true;
}

void Convolution::setValueFunction(RgbInputChannel choice) {
	if(m_rgbInputChannel != choice)
		m_wasModified = true;
	m_rgbInputChannel = choice;
}

void Convolution::setNormalized(bool isNormalized) {
	if (m_isNormalized != isNormalized)
		m_wasModified = true;
	m_isNormalized = isNormalized;
}

void Convolution::setRgbOutput(bool isRgbOutput) {
	if (!m_wasModified) {
		if (m_isRgbOutput == isRgbOutput) {
			return;
		}
	}
	if (m_result) {
		delete[] m_result;
		m_result = nullptr;
	}
	m_isRgbOutput = isRgbOutput;
	m_wasModified = true;
}

void Convolution::compute() {
	if (!m_imageC && !m_imageD)
		return;
	if (!m_wasModified)
		return;
	if (m_result)
		delete[] m_result;
	if (m_isRgbOutput) {
		m_result = new double[m_iWidth * m_iHeight * 3];
		memset(m_result, 0, m_iWidth * m_iHeight * 3 * sizeof(double));
	}
	else {
		m_result = new double[m_iWidth * m_iHeight];
		memset(m_result, 0, m_iWidth * m_iHeight * sizeof(double));
	}
	for (int row = 0; row < m_iHeight; ++row) {
		for (int col = 0; col < m_iWidth; ++col) { // for each position
			if (!m_isRgbInput && !m_isRgbOutput) { // grey to grey
				m_result[row * m_iWidth + col] = computePixel(col, row, 0);
			} 
			else if (!m_isRgbInput && m_isRgbOutput) { // grey to rgb, copy 3 times
				m_result[(row * m_iWidth + col) * 3] = computePixel(col, row, 0);
				m_result[(row * m_iWidth + col) * 3 + 1] = m_result[row * m_iWidth + col];
				m_result[(row * m_iWidth + col) * 3 + 2] = m_result[row * m_iWidth + col];
			}
			else if (m_isRgbInput && !m_isRgbOutput) { // rgb to grey
				m_result[row * m_iWidth + col] = computePixel(col, row, 0);
			}
			else if (m_isRgbInput && m_isRgbOutput) { // rgb to rgb
				m_result[(row * m_iWidth + col) * 3] = computePixel(col, row, 0);
				m_result[(row * m_iWidth + col) * 3 + 1] = computePixel(col, row, 1);
				m_result[(row * m_iWidth + col) * 3 + 2] = computePixel(col, row, 2);
			}			
		}
	}
	m_wasModified = false;
}

double Convolution::getPixelResult(int x, int y, int rgbOffset) {
	if (!m_imageC && !m_imageD)
		return 0;
	if (m_result) {
		if (x < 0) x = (-x - 1) % m_iWidth;
		if (x >= m_iWidth) x = (-x - 1) % m_iWidth + m_iWidth;
		if (y < 0) y = (-y - 1) % m_iHeight;
		if (y >= m_iHeight) y = (-y - 1) % m_iHeight + m_iHeight;
		if (m_isRgbOutput)
			return m_result[(y * m_iWidth + x) * 3 + rgbOffset];
		else
			return m_result[y * m_iWidth + x];
	}
	return computePixel(x, y, rgbOffset);
}

double Convolution::computePixel(int x, int y, int rgbOffset) {
	double sum = 0;
	for (int dy = -(m_kHeight / 2); dy <= m_kHeight / 2; ++dy) {
		for (int dx = -(m_kWidth / 2); dx <= m_kWidth / 2; ++dx) {
			int sx = x + dx;
			int sy = y + dy;
			if (sx < 0) sx = (-sx - 1) % m_iWidth;
			if (sx >= m_iWidth) sx = (-sx - 1) % m_iWidth + m_iWidth;
			if (sy < 0) sy = (-sy - 1) % m_iHeight;
			if (sy >= m_iHeight) sy = (-sy - 1) % m_iHeight + m_iHeight;

			int pixelPos = sy * m_iWidth + sx;
			double kernelFactor = m_kernel[(dy + (m_kHeight / 2)) * m_kWidth + dx + (m_kWidth / 2)];
			int resolution = m_iWidth * m_iHeight;
			
			if (!m_isRgbInput) { // grey input, ignore rgb offset
				if (m_isCharImage) {
					sum += kernelFactor * m_imageC[pixelPos];
				}
				else {
					sum += kernelFactor * m_imageD[pixelPos];
				}
			}
			else if (m_isRgbOutput && m_rgbInputChannel == RgbInputChannel::RGB) { // rgb to rgb, separate channels
				if (m_isCharImage) {
					sum += kernelFactor * getValue(m_imageC[pixelPos],
						m_imageC[resolution + pixelPos],
						m_imageC[resolution * 2 + pixelPos],
						RgbInputChannel(rgbOffset));
				}
				else {
					sum += kernelFactor * getValue(m_imageD[pixelPos],
						m_imageD[resolution + pixelPos],
						m_imageD[resolution * 2 + pixelPos],
						RgbInputChannel(rgbOffset));
				}
			}
			else { // rgb to grey or rgb to rgb mix, ignore rgb offset
				if (m_isCharImage) {
					sum += kernelFactor * getValue(m_imageC[pixelPos],
						m_imageC[resolution + pixelPos],
						m_imageC[resolution * 2 + pixelPos],
						m_rgbInputChannel);
				}
				else {
					sum += kernelFactor * getValue(m_imageD[pixelPos],
						m_imageD[resolution + pixelPos],
						m_imageD[resolution * 2 + pixelPos],
						m_rgbInputChannel);
				}
			}
		}
	}
	return (m_isNormalized ? sum * m_normalizeFactor : sum);
}

double* Convolution::getResult() {
	if (!m_imageC && !m_imageD)
		return nullptr;
	compute();
	return m_result;
}

unsigned char* Convolution::generateImage() {
	if (!m_imageC && !m_imageD)
		return nullptr;
	compute();
	int size = m_iWidth * m_iHeight;
	if (m_isRgbOutput) {
		size *= 3;
	}
	unsigned char* result = new unsigned char[size];
	double r;
	for (int i = 0; i < size; ++i) {
		r = m_result[i];
		r = r < 0 ? 0 : r;
		r = r > 255 ? 255 : r;
		result[i] = (unsigned char)r;
	}
	return result;
}