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
#include "CircleBrush.h"
#include "LineBrush.h"
#include "ScatteredCircleBrush.h"
#include "ScatteredLineBrush.h"
#include "ScatteredPointBrush.h"
#include "MosaicBrush.h"
#include "AlphaMappedBrush.h"
#include "CurvedBrush.h"



#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

// helper funciton declaration
int* getImageGradient(unsigned char* img, int width, int height);


ImpressionistDoc::ImpressionistDoc()
{
	// Set NULL image name as init. 
	m_imageName[0] = '\0';

	m_nWidth = -1;
	m_ucBitmap = NULL;
	m_ucPainting = NULL;
	m_ucUndoBitstart = NULL;
	m_ucAlphaMappedImage = NULL;
	m_nAlphaMappedImageWidth = -1;
	m_nAlphaMappedImageHeight = -1;
	alphaMappedImageLoaded = false;
	m_ucEdgeImg = NULL;
	m_ipGradient = NULL;
	m_ipGValue = NULL;
	m_ucAnotherImg = NULL;
	m_ucdisplayImage = NULL;

	// create one instance of each brush
	ImpBrush::c_nBrushCount = NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes = new ImpBrush*[ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS] = new PointBrush(this, "Points");

	// Note: You should implement these 5 brushes.  They are set the same (PointBrush) for now
	ImpBrush::c_pBrushes[BRUSH_LINES]
		= new LineBrush(this, "Lines");
	ImpBrush::c_pBrushes[BRUSH_CIRCLES]
		= new CircleBrush(this, "Circles");
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS]
		= new ScatteredPointBrush(this, "Scattered Points");
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES]
		= new ScatteredLineBrush(this, "Scattered Lines");
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES]
		= new ScatteredCircleBrush(this, "Scattered Circles");
	ImpBrush::c_pBrushes[BRUSH_MOSAIC]
		= new MosaicBrush(this, "Mosaic");
	ImpBrush::c_pBrushes[BRUSH_ALPHAMAPPED]
		= new AlphaMappedBrush(this, "Alpha Mapped");
	ImpBrush::c_pBrushes[BRUSH_CURVED]
		= new CurvedBrush(this, "Curved");

	// make one of the brushes current
	m_pCurrentBrush = ImpBrush::c_pBrushes[0];

}


//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui)
{
	m_pUI = ui;
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName()
{
	return m_imageName;
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	m_pCurrentBrush = ImpBrush::c_pBrushes[type];

	m_pUI->m_BrushAlphaSlider->deactivate();
	m_pUI->m_BrushSizeSlider->deactivate();
	m_pUI->m_BrushLineWidthSlider->deactivate();
	m_pUI->m_BrushLineAngleSlider->deactivate();

	m_pUI->m_StrokeDirectionChoice->deactivate();
	m_pUI->m_EdgeClippingButton->deactivate();
	m_pUI->m_AnotherGradientButton->deactivate();
	m_pUI->m_MosaicSlider->deactivate();

	switch (type)
	{
	case BRUSH_POINTS:
	case BRUSH_CIRCLES:
	case BRUSH_SCATTERED_POINTS:
	case BRUSH_SCATTERED_CIRCLES:
		m_pUI->m_BrushSizeSlider->activate();
		m_pUI->m_BrushAlphaSlider->activate();
		break;
	case BRUSH_LINES:
	case BRUSH_SCATTERED_LINES:
		m_pUI->m_StrokeDirectionChoice->activate();
		m_pUI->m_BrushSizeSlider->activate();
		m_pUI->m_BrushLineWidthSlider->activate();
		m_pUI->m_BrushLineAngleSlider->activate();
		m_pUI->m_BrushAlphaSlider->activate();
		m_pUI->m_EdgeClippingButton->activate();
		m_pUI->m_AnotherGradientButton->activate();
		break;
	case BRUSH_MOSAIC:
		m_pUI->m_BrushSizeSlider->activate();
		m_pUI->m_BrushAlphaSlider->activate();
		m_pUI->m_MosaicSlider->activate();
		break;
	case BRUSH_ALPHAMAPPED:
		m_pUI->m_BrushSizeSlider->activate();
	case BRUSH_CURVED:
		m_pUI->m_BrushSizeSlider->activate();
		m_pUI->m_BrushLineWidthSlider->activate();
		m_pUI->m_BrushAlphaSlider->activate();
		m_pUI->m_EdgeClippingButton->activate();
		m_pUI->m_AnotherGradientButton->activate();
	default:
		break;
	}
}

void ImpressionistDoc::setDisplayImage(int type)
{
	switch (type)
	{
	case DISPLAY_ORIGINAL:
		m_ucdisplayImage = m_ucBitmap;
		break;
	case DISPLAY_ANOTHER:
		if (m_ucAnotherImg)
			m_ucdisplayImage = m_ucAnotherImg;
		break;
	case DISPLAY_EDGE:
		if (m_ucEdgeImg)
			m_ucdisplayImage = m_ucEdgeImg;
		break;
	default:
		break;
	} 
	m_pUI->m_origView->refresh();
}

void ImpressionistDoc::setBrushDirection(int type)
{
	m_nBrushDirection = type;
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}


int ImpressionistDoc::getMosasicLevel()
{
	return m_pUI->getMosaicLevel();
}


void ImpressionistDoc::setSize(int size)
{
	m_pUI->setSize(size);
}

int ImpressionistDoc::getLineWidth()
{
	return m_pUI->getLineWidth();
}

void ImpressionistDoc::setLineWidth(int width)
{
	m_pUI->setLineWidth(width);
}

int ImpressionistDoc::getLineAngle()
{
	return m_pUI->getLineAngle();
}

void ImpressionistDoc::setLineAngle(int angle)
{
	m_pUI->setLineAngle(angle);
}

float ImpressionistDoc::getAlpha()
{
	return m_pUI->getAlpha();
}

void ImpressionistDoc::setAlpha(float alpha)
{
	m_pUI->setAlpha(alpha);
}

int ImpressionistDoc::getSpacing()
{
	return m_pUI->getSpacing();
}

int ImpressionistDoc::getRand()
{
	return m_pUI->getRand();
}

int ImpressionistDoc::getAnotherGradient()
{
	return m_pUI->getAnotherGradient();
}

void ImpressionistDoc::recalEdgeImg()
{
	if (m_ucEdgeImg)
	{
		memset(m_ucEdgeImg, 0, 3 * m_nWidth * m_nHeight * sizeof(unsigned char));
		int threshold = m_pUI->getEdgeThreshold();
		for (int i = 0; i < m_nWidth * m_nHeight; i++)
		{
			if (m_ipGValue[i] > threshold)
			{
				m_ucEdgeImg[3 * i] = 255;
				m_ucEdgeImg[3 * i + 1] = 255;
				m_ucEdgeImg[3 * i + 2] = 255;
			}
		}
	}

	if (m_ucEdgeImg == m_ucdisplayImage)
	{
		m_pUI->m_origView->refresh();
	}

}

void ImpressionistDoc::rePaint()
{
	m_pUI->m_paintView->rePaint();
}

int	ImpressionistDoc::getFilter()
{
	return m_pUI->getFilter();
}
int* ImpressionistDoc::getFilterValue() {
	return m_pUI->getFilterValue();
}
int	ImpressionistDoc::getFilterHeight() {
	return m_pUI->getFilterHeight();
}
int ImpressionistDoc::getFilterWidth() {
	return m_pUI->getFilterWidth();
}

int ImpressionistDoc::getResolution() {
	return m_pUI->getResolution();
}

int ImpressionistDoc::getMural() {
	return m_pUI->getMural();
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

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nWidth = width;
	m_nPaintWidth = width;
	m_nHeight = height;
	m_nPaintHeight = height;

	// release old storage
	if (m_ucBitmap) delete[] m_ucBitmap;
	if (m_ucPainting) delete[] m_ucPainting;
	if (m_ucUndoBitstart) delete[] m_ucUndoBitstart;
	if (m_ucEdgeImg) delete[] m_ucEdgeImg;
	if (m_ipGradient) delete[] m_ipGradient;
	if (m_ipGValue) delete[] m_ipGValue;

	// allocate memory for edge img 
	m_ucEdgeImg = new unsigned char[3 * width * height];
	memset(m_ucEdgeImg, 0, 3 * width * height * sizeof(unsigned char));
	// get the gradient bmp and magnitude map
	m_ipGradient = getImageGradient(data, width, height);
	m_ipGValue = new int[width * height];
	memset(m_ipGValue, 0, width* height * sizeof(int));
	for (int i = 0; i < width * height; i++)
	{
		m_ipGValue[i] = sqrt(m_ipGradient[i * 2] * m_ipGradient[i * 2]
			+ m_ipGradient[i * 2 + 1] * m_ipGradient[i * 2 + 1]);
	}

	recalEdgeImg();


	m_ucBitmap = data;
	m_ucdisplayImage = m_ucBitmap;

	// allocate space for draw view (undo)
	m_ucUndoBitstart = new unsigned char[width * height * 3];
	memset(m_ucUndoBitstart, 0, width * height * 3);

	// allocate space for draw view
	m_ucPainting = new unsigned char[width*height * 3];
	memset(m_ucPainting, 0, width*height * 3);

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(),
		m_pUI->m_mainWindow->y(),
		width * 2,
		height + 25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);
	m_pUI->m_paintView->refresh();

	alphaMappedImageLoaded = false;
	m_pUI->setAlphaMappedBrushState();

	return 1;
}

int ImpressionistDoc::loadAnotherImage(char *iname)
{
	// try to open the image to read
	unsigned char*	data;
	int				width,
		height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	if (m_nPaintWidth != width || m_nPaintHeight != height)
	{
		fl_alert("Different dimension");
		return 0;
	}

	if (m_ucAnotherImg) delete[] m_ucAnotherImg;
	m_ucAnotherImg = data;

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

int ImpressionistDoc::loadAlphaMappedImage(char *iname)
{
	// try to open the image to read
	unsigned char*	data;
	int				width, height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	if (m_nWidth == -1) {
		fl_alert("Load the original image first");
		return 0;
	}
	if (m_nWidth != width || m_nHeight != height) {
		fl_alert("Different dimension...");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nAlphaMappedImageHeight = height;
	m_nAlphaMappedImageWidth = width;

	if (m_ucAlphaMappedImage)
	{
		delete[] m_ucAlphaMappedImage;
	}
	m_ucAlphaMappedImage = new unsigned char[width * height];

	for (int i = 0; i < width * height; i++)
	{
		m_ucAlphaMappedImage[i] = ((int)data[i * 3] + (int)data[i * 3 + 1] + (int)data[i * 3 + 2]) / 3;
	}
	alphaMappedImageLoaded = true;
	m_pUI->setAlphaMappedBrushState();

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
	if (m_ucPainting)
	{
		delete[] m_ucPainting;
		delete[] m_ucUndoBitstart;

		m_ucUndoBitstart = new unsigned char[m_nPaintWidth*m_nPaintHeight * 3];
		memset(m_ucUndoBitstart, 0, m_nPaintWidth*m_nPaintHeight * 3);
		// allocate space for draw view
		m_ucPainting = new unsigned char[m_nPaintWidth*m_nPaintHeight * 3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight * 3);


		// refresh paint view as well	
		m_pUI->m_paintView->refresh();
	}

	return 0;
}

int ImpressionistDoc::undoPainting()
{
	if (m_ucPainting)
	{
		m_pUI->m_paintView->RestoreUndoContent();
		
	}
	return 0;
}

void ImpressionistDoc::autoPainting()
{
	m_pUI->m_paintView->autoPainting();
}
//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel(int x, int y)
{
	if (x < 0)
		x = 0;
	else if (x >= m_nWidth)
		x = m_nWidth - 1;

	if (y < 0)
		y = 0;
	else if (y >= m_nHeight)
		y = m_nHeight - 1;

	return (GLubyte*)(m_ucBitmap + 3 * (y*m_nWidth + x));
}

GLubyte* ImpressionistDoc::GetAnotherPixel(int x, int y)
{
	if (x < 0)
		x = 0;
	else if (x >= m_nWidth)
		x = m_nWidth - 1;

	if (y < 0)
		y = 0;
	else if (y >= m_nHeight)
		y = m_nHeight - 1;

	return (GLubyte*)(m_ucAnotherImg + 3 * (y*m_nWidth + x));
}

GLubyte* ImpressionistDoc::GetAnotherPixel(const Point p)
{
	return GetAnotherPixel(p.x, p.y);
}
//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel(const Point p)
{
	return GetOriginalPixel(p.x, p.y);
}

GLubyte* ImpressionistDoc::GetDisplayPixel(int x, int y)
{
	if (x < 0)
		x = 0;
	else if (x >= m_nWidth)
		x = m_nWidth - 1;

	if (y < 0)
		y = 0;
	else if (y >= m_nHeight)
		y = m_nHeight - 1;

	return (GLubyte*)(m_ucdisplayImage + 3 * (y*m_nWidth + x));
}

inline int getBW(unsigned char* img, int index)
{
	return img[index * 3] * 0.299 + img[index * 3 + 1] * 0.587 + img[index * 3 + 2] * 0.144;
}

int* getImageGradient(unsigned char* img, int width, int height)
{
	int* result = new int[width * height * 2];
	memset(result, 0, width*height * 2 * sizeof(int));

	for (int y = 1; y < height - 1; y++)
	{
		for (int x = 1; x < width - 1; x++)
		{
			/*-1  0  1
			- 2  0  2
			- 1  0  1*/

			int index = (x + y * width);
			result[index * 2] = getBW(img, index + 1) * 2 - getBW(img, index - 1) * 2
				+ getBW(img, index + width + 1) - getBW(img, index + width - 1)
				+ getBW(img, index - width + 1) - getBW(img, index - width - 1);
			/*1  2  1
			0  0  0
			- 1 - 2 - 1*/

			result[index * 2 + 1] = getBW(img, index + width + 1) + 2 * getBW(img, index + width)
				+ getBW(img, index + width - 1) - getBW(img, index - width - 1)
				- 2 * getBW(img, index - width) - getBW(img, index - width + 1);
		}
	}
	return result;
}