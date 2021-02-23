// 
// impressionistDoc.h
//
// header file for Doc 
//

#ifndef ImpressionistDoc_h
#define ImpressionistDoc_h

#include "impressionist.h"
#include "bitmap.h"

class ImpressionistUI;

class ConvolutionManager;

class ImpressionistDoc 
{
public:
	ImpressionistDoc();

	void	setUI(ImpressionistUI* ui);		// Assign the UI to use

	int		loadImage(char *iname);			// called by the UI to load image
	int		saveImage(char *iname);			// called by the UI to save image

	int     clearCanvas();                  // called by the UI to clear the drawing canvas
	int swapContents();
	int changeImage(char* name);
	int loadAnotherImage(char* name);
	int loadEdgeImage(char* name);

	int applyFilter(int filterType, int filterSource, bool isNormalized);
	int applyCustomFilter(int kernel[], int width, int height, int filterSource, bool isNormalized);

	void	setBrushType(int type);			// called by the UI to set the brushType
	void    setStrokeDirectionType(int type); // called by the UI to set the stroke direction type
	int		getSize();						// get the UI size
	void	setSize(int size);				// set the UI size
	int		getWidth();						// get the line width
	void	setWidth(int width);			// set the line width
	int		getAngle();						// get the line angle
	void	setAngle(int angle);			// set the line angle
	int 	getAlpha();						// get the alpha value
	void	setAlpha(int alpha);			// set the alpha value
	void updateConvolutionPresetImage(bool isAnotherImage);
	void setEdgeClipping(bool isEdgeClippingOn);

	double	getRedVal();				
	void	setRedVal(double R);
	double	getGreenVal();
	void	setGreenVal(double G);
	double	getBlueVal();
	void	setBlueVal(double B);
	char*	getImageName();					// get the current image name


// Attributes
public:
	// Dimensions of original window.
	int				m_nWidth, 
					m_nHeight;
	// Dimensions of the paint window.
	int				m_nPaintWidth, 
					m_nPaintHeight;	
	// Bitmaps for original image and painting.
private:
	unsigned char*	m_ucBitmap;
	unsigned char*	m_ucPainting;
	unsigned char* m_ucAnotherImage;
	unsigned char* m_ucEdgeImage;
public:
	unsigned char* getBitmap();
	unsigned char* getPainting();
	unsigned char* getAnotherImage();
	unsigned char* getEdgeImage();

	// The current active brush.
	ImpBrush*			m_pCurrentBrush;	
	// Size of the brush.
	int m_nSize;	
	int m_brushWidth;
	int m_angle;
	int m_alpha;
	bool m_isUsingAnotherGradient;
	bool m_isEdgeClippingOn;

	double	m_redVal;
	double	m_greenVal;
	double	m_blueVal;

	ConvolutionManager* m_convolutionManager;

	ImpressionistUI*	m_pUI;

// Operations
public:
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel( int x, int y );   
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel( const Point p );  
	
	GLubyte* getEdgePixel(int x, int y);  


private:
	char			m_imageName[256];

};

extern void MessageBox(char *message);

#endif
