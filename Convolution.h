#ifndef CONVOLUTION_H
#define CONVULOTUION_H

class Convolution {
public:
	Convolution(const int kernel[], int kWidth, int kHeight, bool isNormalized);
	~Convolution();

	void changeImage(const unsigned char* image, int iWidth, int iHeight, bool isRgb);
		
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

#endif 