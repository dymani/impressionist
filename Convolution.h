#ifndef CONVOLUTION_H
#define CONVOLUTION_H

class Convolution {
public:
	Convolution(const int kernel[], int kWidth, int kHeight, bool isNormalized);
	~Convolution();

	void setImage(const unsigned char* image, int iWidth, int iHeight, bool isRgb);
	void setNormalized(bool isNormalized);
		
	void compute();
		
	double getPixelResult(int x, int y);
	unsigned char* generateResultImage();

protected:
	unsigned char getValueFromRgb(const unsigned char rgb[]);

	int* m_kernel; // copy
	const int m_kWidth, m_kHeight; // const, assume both odd
	double m_normalizeFactor;
	
	unsigned char* m_image;
	int m_iWidth, m_iHeight;

	double* m_convolution;
};

namespace FilterTypes {
	enum Type {
		GAUSSIAN_3,
		GAUSSIAN_5,
		SOBEL_X,
		SOBEL_Y,
		CUSTOM,
		NUM_FILTER_TYPE
	};

	static const int KERNEL_GAUSSIAN_3[] = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };
	static const int KERNEL_GAUSSIAN_5[] = {  1, 4, 7, 4, 1,
											  4, 16, 26, 16, 4,
											  7, 26, 41, 26, 7,
											  4, 16, 26, 16, 4,
											  1, 4, 7, 4, 1 };
	static const int KERNEL_SOBEL_X[] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
	static const int KERNEL_SOBEL_Y[] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 };

}



#endif 