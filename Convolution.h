#ifndef CONVOLUTION_H
#define CONVOLUTION_H

class Convolution {
public:
	enum RgbInputChannel {
		RED, GREEN, BLUE, LUMA, RGB
	};

	Convolution(const int kernel[], int kWidth, int kHeight);
	~Convolution();

	void setImage(const unsigned char* image, int iWidth, int iHeight, bool isRgbInput);
	void setImage(const double* image, int iWidth, int iHeight, bool isRgbInput);
	void setValueFunction(RgbInputChannel choice);
	void setNormalized(bool isNormalized);
	void setRgbOutput(bool isRgbOutput);
		
	void compute();
		
	double getPixelResult(int x, int y, int rgbOffset = 0);
	double* getResult();

	unsigned char* generateImage();

protected:
	double computePixel(int x, int y, int rgbOffset);
	/*unsigned char getValue(unsigned char r, unsigned char g, unsigned char b) {
		return (unsigned char)getValue((double)r, (double)g, (double)b, m_rgbInputChannel);
	}*/
	inline unsigned char getValue(unsigned char r, unsigned char g, unsigned char b, RgbInputChannel choice) {
		return (unsigned char)getValue((double)r, (double)g, (double)b, choice);
	}
	/*double getValue(double r, double g, double b) {
		return getValue(r, g, b, m_rgbInputChannel);
	}*/
	inline double getValue(double r, double g, double b, RgbInputChannel choice) {
		switch (choice)
		{
		case RgbInputChannel::RED:
			return r;
		case RgbInputChannel::GREEN:
			return g;
		case RgbInputChannel::BLUE:
			return b;
		case RgbInputChannel::LUMA:
		default:
			return 0.299 * r + 0.587 * g + 0.114 * b;
		}
	}

	int* m_kernel; // copy
	const int m_kWidth, m_kHeight; // const, assume both odd
	
	bool m_isCharImage;
	unsigned char* m_imageC;
	double* m_imageD;
	int m_iWidth, m_iHeight;
	bool m_isRgbInput;

	RgbInputChannel m_rgbInputChannel;

	bool m_isNormalized;
	double m_normalizeFactor;

	bool m_isRgbOutput;

	bool m_wasModified;

	double* m_result;
};

#endif 