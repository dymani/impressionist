// 
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//

#include <FL/fl_ask.H>

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "ImpBrush.h"

// Include individual brush headers here.
#include "PointBrush.h"
#include "LineBrush.h"
#include "CircleBrush.h"
#include "ScatteredPointBrush.h"
#include "ScatteredCircleBrush.h"
#include "ScatteredLineBrush.h"
#include "RandomPolygonBrush.h"
#include "SharpenBrush.h"
#include "BlurBrush.h"

#include "LineOverlay.h"
#include "InputTable.h"
#include "ConvolutionManager.h"

#include <vector>


#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc() 
{
	// Set NULL image name as init. 
	m_imageName[0]	='\0';	

	m_nWidth		= -1;
	m_ucBitmap		= NULL;
	m_ucPainting	= NULL;
	m_ucPaintingUndo = nullptr;
	m_ucAnotherImage = nullptr;
	m_ucDissolveImage = nullptr;
	m_isUsingAnotherGradient = false;
	m_ucEdgeImage = nullptr;
	m_isEdgeClippingOn = false;


	// create one instance of each brush
	ImpBrush::c_nBrushCount	= NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes	= new ImpBrush* [ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS]	= new PointBrush( this, "Points" );

	// Note: You should implement these 5 brushes.  They are set the same (PointBrush) for now
	ImpBrush::c_pBrushes[BRUSH_LINES]				
		= new LineBrush( this, "Lines" );
	ImpBrush::c_pBrushes[BRUSH_CIRCLES]				
		= new CircleBrush( this, "Circles" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS]	
		= new ScatteredPointBrush( this, "Scattered Points" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES]		
		= new ScatteredLineBrush( this, "Scattered Lines" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES]	
		= new ScatteredCircleBrush( this, "Scattered Circles" );

	// New brush(es)
	ImpBrush::c_pBrushes[BRUSH_RANDOM_POLYGON]
		= new RandomPolygonBrush(this, "Random Polygon");
	ImpBrush::c_pBrushes[BRUSH_SHARPEN]
		= new SharpenBrush(this, "Sharpen");
	ImpBrush::c_pBrushes[BRUSH_BLUR]
		= new BlurBrush(this, "Blur");

	// make one of the brushes current
	m_pCurrentBrush	= ImpBrush::c_pBrushes[0];	

	m_convolutionManager = new ConvolutionManager();
}


//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui) 
{
	m_pUI	= ui;
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName() 
{
	return m_imageName;
}


unsigned char* ImpressionistDoc::getBitmap() {
	return m_ucBitmap; 
}

unsigned char* ImpressionistDoc::getPainting() { 
	return m_ucPainting;
}

unsigned char* ImpressionistDoc::getAnotherImage() { 
	return m_ucAnotherImage;
}

unsigned char* ImpressionistDoc::getDissolveImage() {
	return m_ucDissolveImage;
}

unsigned char* ImpressionistDoc::getEdgeImage() {
	return m_ucEdgeImage;
}

void ImpressionistDoc::setPainting(unsigned char* p) {
	m_ucPainting = p;
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	m_pCurrentBrush	= ImpBrush::c_pBrushes[type];
}

void ImpressionistDoc::setStrokeDirectionType(int type) {
	if (dynamic_cast<LineBrush*>(m_pCurrentBrush)) {
		dynamic_cast<LineBrush*>(ImpBrush::c_pBrushes[BRUSH_LINES])->setMode(type);
		dynamic_cast<LineBrush*>(ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES])->setMode(type);
	}
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}

void ImpressionistDoc::setSize(int size) {
	m_pUI->setSize(size);
}

//---------------------------------------------------------
// Returns the line width.
//---------------------------------------------------------
int ImpressionistDoc::getWidth()
{
	return m_pUI->getWidth();
}

void ImpressionistDoc::setWidth(int width) {
	m_pUI->setWidth(width);
}

//---------------------------------------------------------
// Returns the line angle.
//---------------------------------------------------------
int ImpressionistDoc::getAngle()
{
	return m_pUI->getAngle();
}

void ImpressionistDoc::setAngle(int angle) {
	m_pUI->setAngle(angle);
}

//---------------------------------------------------------
// Returns the alpha value of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getAlpha()
{
	return m_pUI->getAlpha();
}

void ImpressionistDoc::setAlpha(int alpha) {
	m_pUI->setAlpha(alpha);
}

void ImpressionistDoc::updateConvolutionPresetImage(bool isUsingAnotherGradient) {
	if (m_ucAnotherImage)
		m_isUsingAnotherGradient = isUsingAnotherGradient;
	if (m_isUsingAnotherGradient)
		m_convolutionManager->initializePresets(m_ucAnotherImage, m_nWidth, m_nHeight);
	else
		m_convolutionManager->initializePresets(m_ucBitmap, m_nWidth, m_nHeight);
}

void ImpressionistDoc::setEdgeClipping(bool isEdgeClippingOn) {
	/*if (!m_ucEdgeImage)
		return;*/
	m_isEdgeClippingOn = isEdgeClippingOn;
}

void ImpressionistDoc::applyEdgeDetection(int threshold) {
	if (!m_ucBitmap)
		return;
	if (m_ucEdgeImage)
		delete[] m_ucEdgeImage;
	m_ucEdgeImage = m_convolutionManager->generateFilterImage(ConvolutionManager::FILTER_EDGE, m_ucBitmap, m_nWidth, m_nHeight, true, threshold);
}


//-------------------------------------------------
// Get the red value
//-------------------------------------------------
double ImpressionistDoc::getRedVal()
{
	return m_pUI->getRedVal();
}

//-------------------------------------------------
// Set the red value
//-------------------------------------------------
void ImpressionistDoc::setRedVal(double R)
{
	m_pUI->setRedVal(R);
}

//-------------------------------------------------
// Get the green value
//-------------------------------------------------
double ImpressionistDoc::getGreenVal()
{
	return m_pUI->getGreenVal();
}

//-------------------------------------------------
// Set the green value
//-------------------------------------------------
void ImpressionistDoc::setGreenVal(double G)
{
	m_pUI->setGreenVal(G);
}

//-------------------------------------------------
// Get the blue value
//-------------------------------------------------
double ImpressionistDoc::getBlueVal()
{
	return m_pUI->getBlueVal();
}

//-------------------------------------------------
// Set the blue value
//-------------------------------------------------
void ImpressionistDoc::setBlueVal(double B)
{
	m_pUI->setBlueVal(B);;
}

//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(char *iname) 
{
	// try to open the image to read
	unsigned char*	data;
	int				width, 
					height;

	if ( (data=readBMP(iname, width, height))==NULL ) 
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nWidth		= width;
	m_nPaintWidth	= width;
	m_nHeight		= height;
	m_nPaintHeight	= height;

	// release old storage
	if ( m_ucBitmap ) delete [] m_ucBitmap;
	if ( m_ucPainting ) delete [] m_ucPainting;
	if (m_ucPaintingUndo) delete[] m_ucPaintingUndo;
	if (m_ucAnotherImage) delete[] m_ucAnotherImage;

	m_ucBitmap		= data;

	// allocate space for draw view
	m_ucPainting	= new unsigned char [width*height*3];
	memset(m_ucPainting, 0, width*height*3);

	// allocate space for undo memory
	m_ucPaintingUndo = new unsigned char[width * height * 3];
	memset(m_ucPaintingUndo, 0, width * height * 3);

	m_ucAnotherImage = nullptr;
	m_isUsingAnotherGradient = false;

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(), 
								m_pUI->m_mainWindow->y(), 
								width*2, 
								height+25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);	
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);	
	m_pUI->m_paintView->refresh();

	updateConvolutionPresetImage(m_isUsingAnotherGradient);

	return 1;
}


//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(char *iname) 
{

	writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);

	return 1;
}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() 
{

	// Release old storage
	if ( m_ucPainting ) 
	{
		delete [] m_ucPainting;

		// allocate space for draw view
		m_ucPainting	= new unsigned char [m_nPaintWidth*m_nPaintHeight*3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight*3);

		// refresh paint view as well	
		m_pUI->m_paintView->refresh();
	}
	
	return 0;
}

int ImpressionistDoc::swapContents() {
	unsigned char* temp = m_ucPainting;
	m_ucPainting = m_ucBitmap;
	m_ucBitmap = temp;
	m_pUI->m_origView->refresh();
	m_pUI->m_paintView->refresh();
	updateConvolutionPresetImage(m_isUsingAnotherGradient);
	return 1;
}

int ImpressionistDoc::changeImage(char* iname) {
	// try to open the image to read
	unsigned char* data;
	int width,height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	if (m_nWidth != width || m_nHeight != height) {
		fl_alert("Mural image should have the same dimension");
		return 0;
	}

	// release old storage
	if (m_ucBitmap) delete[] m_ucBitmap;
	m_ucBitmap = data;

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);
	m_pUI->m_origView->refresh();

	updateConvolutionPresetImage(m_isUsingAnotherGradient);

	return 1;
}

int ImpressionistDoc::loadAnotherImage(char* name) {
	// try to open the image to read
	unsigned char* data;
	int width, height;

	if ((data = readBMP(name, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	if (m_nWidth != width || m_nHeight != height) {
		fl_alert("Another image should have the same dimension");
		return 0;
	}

	// release old storage
	if (m_ucAnotherImage) delete[] m_ucAnotherImage;
	m_ucAnotherImage = data;

	updateConvolutionPresetImage(m_isUsingAnotherGradient);

	return 1;
}

int ImpressionistDoc::loadDissolveImage(char* name) {
	// try to open the image to read
	unsigned char* data;
	int width, height;

	if ((data = readBMP(name, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	if (m_nWidth != width || m_nHeight != height) {
		fl_alert("Edge image should have the same dimension");
		return 0;
	}

	// release old storage
	if (m_ucDissolveImage) delete[] m_ucDissolveImage;
	m_ucDissolveImage = data;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int index = (i * width + j) * 3;
			m_ucBitmap[index] = 0.5 * m_ucBitmap[index] + 0.5 * m_ucDissolveImage[index];
			m_ucBitmap[index + 1] = 0.5 * m_ucBitmap[index + 1] + 0.5 * m_ucDissolveImage[index + 1];
			m_ucBitmap[index + 2] = 0.5 * m_ucBitmap[index + 2] + 0.5 * m_ucDissolveImage[index + 2];
		}
	}

	//refresh display
	m_pUI->m_origView->refresh();

	return 1;
}

int ImpressionistDoc::loadEdgeImage(char* name) {
	// try to open the image to read
	unsigned char* data;
	int width, height;

	if ((data = readBMP(name, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	if (m_nWidth != width || m_nHeight != height) {
		fl_alert("Edge image should have the same dimension");
		return 0;
	}

	// release old storage
	if (m_ucEdgeImage) delete[] m_ucEdgeImage;
	m_ucEdgeImage = data;

	return 1;
}

int ImpressionistDoc::applyFilter(int filterType, int filterSource, bool isNormalized) {
	if (!m_ucBitmap)
		return 0;
	
	unsigned char* result;

	if (filterSource == 0) {
		result = m_convolutionManager->generateFilterImage(ConvolutionManager::FilterChoice(filterType), m_ucBitmap, m_nWidth, m_nHeight, isNormalized);
	}
	else {
		result = m_convolutionManager->generateFilterImage(ConvolutionManager::FilterChoice(filterType), m_ucPainting, m_nWidth, m_nHeight, isNormalized);
	}
	
	for (int i = 0; i < m_nPaintWidth * m_nPaintHeight * 3; ++i) {
		m_ucPainting[i] = result[i];
	}
	m_pUI->m_paintView->refresh();
	delete[] result;
	return 1;
}

int ImpressionistDoc::applyCustomFilter(int kernel[], int width, int height, int filterSource, bool isNormalized) {
	if (!m_ucBitmap)
		return 0;
	int* kern = new int[width * height];
	for (int r = 0; r < height; ++r) {
		for (int c = 0; c < width; ++c) {
			kern[r * width + c] = kernel[r * InputTable::MAX_COLS + c];
		}
	}
	unsigned char* result;
	if (filterSource == 0) {
		result = m_convolutionManager->generateCustomFilterImage(kern, width, height, m_ucBitmap, m_nWidth, m_nHeight, isNormalized);
	}
	else {
		result = m_convolutionManager->generateCustomFilterImage(kern, width, height, m_ucBitmap, m_nWidth, m_nHeight, isNormalized);
	}
	for (int i = 0; i < m_nPaintWidth * m_nPaintHeight * 3; ++i) {
		m_ucPainting[i] = result[i];
	}
	m_pUI->m_paintView->refresh();
	delete[] kern;
	delete[] result;
	return 1;
}

//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( int x, int y )
{
	if ( x < 0 ) 
		x = 0;
	else if ( x >= m_nWidth ) 
		x = m_nWidth-1;

	if ( y < 0 ) 
		y = 0;
	else if ( y >= m_nHeight ) 
		y = m_nHeight-1;

	return (GLubyte*)(m_ucBitmap + 3 * (y*m_nWidth + x));
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( const Point p )
{
	return GetOriginalPixel( p.x, p.y );
}


GLubyte* ImpressionistDoc::getEdgePixel(int x, int y) {
	if (x < 0)
		x = 0;
	else if (x >= m_nWidth)
		x = m_nWidth - 1;

	if (y < 0)
		y = 0;
	else if (y >= m_nHeight)
		y = m_nHeight - 1;
	if (!m_ucEdgeImage) {
		applyEdgeDetection(255);
	}
	return (GLubyte*)(m_ucEdgeImage + 3 * (y * m_nWidth + x));
}