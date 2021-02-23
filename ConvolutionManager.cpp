#include "ConvolutionManager.h"
#include <cmath>

ConvolutionManager::ConvolutionManager() {
	m_image = nullptr;
	m_iWidth = m_iHeight = 0;

	m_presets[GRADIENT_GAUSSIAN_5] = new Convolution(Kernel::GAUSSIAN_5, 5, 5);
	m_presets[GRADIENT_GAUSSIAN_5]->setValueFunction(Convolution::LUMA);
	m_presets[GRADIENT_GAUSSIAN_5]->setNormalized(true);
	m_presets[GRADIENT_GAUSSIAN_5]->setRgbOutput(false);

	m_presets[GRADIENT_SOBEL_X] = new Convolution(Kernel::SOBEL_X, 3, 3);
	m_presets[GRADIENT_SOBEL_X]->setNormalized(false);
	m_presets[GRADIENT_SOBEL_X]->setRgbOutput(false);

	m_presets[GRADIENT_SOBEL_Y] = new Convolution(Kernel::SOBEL_Y, 3, 3);
	m_presets[GRADIENT_SOBEL_Y]->setNormalized(false);
	m_presets[GRADIENT_SOBEL_Y]->setRgbOutput(false);

	m_filters[FILTER_GAUSSIAN_3] = new Convolution(Kernel::GAUSSIAN_3, 3, 3);
	m_filters[FILTER_GAUSSIAN_3]->setValueFunction(Convolution::RGB);
	m_filters[FILTER_GAUSSIAN_3]->setRgbOutput(true);

	m_filters[FILTER_GAUSSIAN_5] = new Convolution(Kernel::GAUSSIAN_5, 5, 5);
	m_filters[FILTER_GAUSSIAN_5]->setValueFunction(Convolution::RGB);
	m_filters[FILTER_GAUSSIAN_5]->setRgbOutput(true);

	m_filters[FILTER_SOBEL_X] = new Convolution(Kernel::SOBEL_X, 3, 3);
	m_filters[FILTER_SOBEL_X]->setValueFunction(Convolution::RGB);
	m_filters[FILTER_SOBEL_X]->setRgbOutput(true);

	m_filters[FILTER_SOBEL_Y] = new Convolution(Kernel::SOBEL_Y, 3, 3);
	m_filters[FILTER_SOBEL_Y]->setValueFunction(Convolution::RGB);
	m_filters[FILTER_SOBEL_Y]->setRgbOutput(true);

	m_filters[FILTER_EDGE] = nullptr;

	m_filters[FILTER_CUSTOM] = nullptr;

	for (int i = 0; i < NUM_FILTER_PRESET; ++i) {
		m_presetImages[i] = nullptr;
	}

	m_brushes[BRUSH_SHARPEN] = new Convolution(Kernel::SHARPEN, 3, 3);
	m_brushes[BRUSH_SHARPEN]->setValueFunction(Convolution::RGB);
	m_brushes[BRUSH_SHARPEN]->setRgbOutput(true);
	m_brushes[BRUSH_SHARPEN]->setNormalized(true);

	m_brushes[BRUSH_BLUR] = new Convolution(Kernel::GAUSSIAN_3, 3, 3);
	m_brushes[BRUSH_BLUR]->setValueFunction(Convolution::RGB);
	m_brushes[BRUSH_BLUR]->setRgbOutput(true);
	m_brushes[BRUSH_BLUR]->setNormalized(true);
}

void ConvolutionManager::initializePresets(unsigned char* image, int iWidth, int iHeight) {
	m_image = image;
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_presets[GRADIENT_GAUSSIAN_5]->setImage(m_image, m_iWidth, m_iHeight, true);
	double* gaussian = m_presets[GRADIENT_GAUSSIAN_5]->getResult();

	m_presets[GRADIENT_SOBEL_X]->setImage(gaussian, m_iWidth, m_iHeight, false);
	m_presets[GRADIENT_SOBEL_X]->compute();

	m_presets[GRADIENT_SOBEL_Y]->setImage(gaussian, m_iWidth, m_iHeight, false);
	m_presets[GRADIENT_SOBEL_Y]->compute();

	for (int i = 0; i < NUM_FILTER_PRESET; ++i) {
		if (m_presetImages[i]) delete[] m_presetImages[i];
		m_presetImages[i] = nullptr;
	}
}

unsigned char* ConvolutionManager::generateFilterImage(FilterChoice choice, unsigned char* source, int iWidth, int iHeight, bool isNormalized, int threshold) {
	if (choice == FILTER_CUSTOM || choice == NUM_FILTER_CHOICE)
		return nullptr;
	if (choice == FILTER_EDGE) {
		m_presets[GRADIENT_GAUSSIAN_5]->setImage(source, iWidth, iHeight, true);
		double* gaussian = m_presets[GRADIENT_GAUSSIAN_5]->getResult();

		m_presets[GRADIENT_SOBEL_X]->setImage(gaussian, iWidth, iHeight, false);
		m_presets[GRADIENT_SOBEL_X]->compute();

		m_presets[GRADIENT_SOBEL_Y]->setImage(gaussian, iWidth, iHeight, false);
		m_presets[GRADIENT_SOBEL_Y]->compute();

		unsigned char* result = new unsigned char[iWidth * iHeight * 3];

		for (int y = 0; y < iHeight; ++y) {
			for (int x = 0; x < iWidth; ++x) {
				double tx = m_presets[GRADIENT_SOBEL_X]->getPixelResult(x, y, 0);
				double ty = m_presets[GRADIENT_SOBEL_Y]->getPixelResult(x, y, 0);
				double value = sqrt(tx * tx + ty * ty);
				if (isNormalized) {
					if (value >= threshold)
						value = 255;
					else
						value = 0;
				}
				else {
					if (value > 255)
						value = 255;
				}
				result[(y * iWidth + x) * 3 + 0] = (unsigned char)value;
				result[(y * iWidth + x) * 3 + 1] = (unsigned char)value;
				result[(y * iWidth + x) * 3 + 2] = (unsigned char)value;
			}
		}

		m_presets[GRADIENT_GAUSSIAN_5]->setImage(m_image, m_iWidth, m_iHeight, true);
		gaussian = m_presets[GRADIENT_GAUSSIAN_5]->getResult();
		m_presets[GRADIENT_SOBEL_X]->setImage(gaussian, m_iWidth, m_iHeight, false);
		m_presets[GRADIENT_SOBEL_X]->compute();
		m_presets[GRADIENT_SOBEL_Y]->setImage(gaussian, m_iWidth, m_iHeight, false);
		m_presets[GRADIENT_SOBEL_Y]->compute();
		return result;
	}
	m_filters[choice]->setImage(source, iWidth, iHeight, true);
	m_filters[choice]->setNormalized(isNormalized);
	return m_filters[choice]->generateImage();
}


unsigned char* ConvolutionManager::generateCustomFilterImage(int kernel[], int kWidth, int kHeight, unsigned char* image, int iWidth, int iHeight, bool isNormalized) {
	if (m_filters[FILTER_CUSTOM])
		delete m_filters[FILTER_CUSTOM];
	m_filters[FILTER_CUSTOM] = new Convolution(kernel, kWidth, kHeight);
	m_filters[FILTER_CUSTOM]->setImage(image, iWidth, iHeight, true);
	m_filters[FILTER_CUSTOM]->setValueFunction(Convolution::RGB);
	m_filters[FILTER_CUSTOM]->setNormalized(isNormalized);
	m_filters[FILTER_CUSTOM]->setRgbOutput(true);
	return m_filters[FILTER_CUSTOM]->generateImage();
}

unsigned char* ConvolutionManager::generatePresetImage(FilterPreset preset) {
	if (preset == NUM_FILTER_PRESET)
		return nullptr;
	if (!m_presetImages[preset])
		m_presetImages[preset] = m_presets[preset]->generateImage();
	return m_presetImages[preset];
}

unsigned char* ConvolutionManager::getPresetImagePixel(FilterPreset preset, int x, int y) {
	switch (preset)	{
		case GRADIENT_GAUSSIAN_5:
		case GRADIENT_SOBEL_X:
		case GRADIENT_SOBEL_Y:
			return generatePresetImage(preset) + y * m_iWidth + x; // grey image
		default:
			return nullptr;
	}
}

double* ConvolutionManager::getPresetResult(FilterPreset preset) {
	if (preset == NUM_FILTER_PRESET)
		return nullptr;
	return m_presets[preset]->getResult();
}

double ConvolutionManager::getPresetResultPixel(FilterPreset preset, int x, int y) {
	switch (preset) {
		case GRADIENT_GAUSSIAN_5:
		case GRADIENT_SOBEL_X:
		case GRADIENT_SOBEL_Y:
			return m_presets[preset]->getPixelResult(x, y, 0);
		default:
			return 0;
	}
}

void ConvolutionManager::updateFilterBrushImage(FilterBrush brush, unsigned char* image, int iWidth, int iHeight) {
	if (brush == NUM_FILTER_BRUSH)
		return;
	m_brushes[brush]->setImage(image, iWidth, iHeight, true);
	m_brushes[brush]->compute();
}

unsigned char* ConvolutionManager::getFilterBrushPixel(FilterBrush brush, int x, int y) {
	if (brush == NUM_FILTER_BRUSH)
		return nullptr;
	unsigned char* pixel = new unsigned char[3];
	double r;
	r = m_brushes[brush]->getPixelResult(x, y, 0);
	r = r < 0 ? 0 : r;
	r = r > 255 ? 255 : r;
	pixel[0] = (unsigned char)r;
	r = m_brushes[brush]->getPixelResult(x, y, 1);
	r = r < 0 ? 0 : r;
	r = r > 255 ? 255 : r;
	pixel[1] = (unsigned char)r;
	r = m_brushes[brush]->getPixelResult(x, y, 2);
	r = r < 0 ? 0 : r;
	r = r > 255 ? 255 : r;
	pixel[2] = (unsigned char)r;
	return pixel;
}