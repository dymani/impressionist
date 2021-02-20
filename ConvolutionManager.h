#ifndef FILTERS_H
#define FILTERS_H

#include "Convolution.h"

namespace Kernel {

	static const int GAUSSIAN_3[] = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };
	static const int GAUSSIAN_5[] = { 1, 4, 7, 4, 1,
											  4, 16, 26, 16, 4,
											  7, 26, 41, 26, 7,
											  4, 16, 26, 16, 4,
											  1, 4, 7, 4, 1 };
	static const int SOBEL_X[] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
	static const int SOBEL_Y[] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 };
	static const int SHARPEN[] = { 0, -1, 0, -1, 5, -1, 0, -1, 0};
}

class ConvolutionManager {
public:
	enum FilterPreset {
		GRADIENT_GAUSSIAN_5,
		GRADIENT_SOBEL_X,
		GRADIENT_SOBEL_Y,
		NUM_FILTER_PRESET
	};
	enum FilterChoice {
		FILTER_GAUSSIAN_3,
		FILTER_GAUSSIAN_5,
		FILTER_SOBEL_X,
		FILTER_SOBEL_Y,
		FILTER_CUSTOM,
		NUM_FILTER_CHOICE
	};
	enum FilterBrush {
		BRUSH_SHARPEN,
		BRUSH_BLUR,
		NUM_FILTER_BRUSH
	};

	ConvolutionManager();

	void initializePresets(unsigned char* image, int iWidth, int iHeight);

	unsigned char* generateFilterImage(FilterChoice choice, unsigned char* source, int iWidth, int iHeight, bool isNormalized);
	unsigned char* generateCustomFilterImage(int kernel[], int kWidth, int kHeight, unsigned char* image, int iWidth, int iHeight, bool isNormalized);

	unsigned char* generatePresetImage(FilterPreset preset);
	unsigned char* getPresetImagePixel(FilterPreset preset, int x, int y);

	double* getPresetResult(FilterPreset preset);
	double getPresetResultPixel(FilterPreset preset, int x, int y);

	void updateFilterBrushImage(FilterBrush brush, unsigned char* image, int iWidth, int iHeight);
	unsigned char* getFilterBrushPixel(FilterBrush brush, int x, int y);
private:
	unsigned char* m_image;
	int m_iWidth, m_iHeight;

	Convolution* m_presets[NUM_FILTER_PRESET];
	unsigned char* m_presetImages[NUM_FILTER_PRESET];

	Convolution* m_filters[NUM_FILTER_CHOICE];

	Convolution* m_brushes[NUM_FILTER_BRUSH];
};

#endif