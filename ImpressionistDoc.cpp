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

#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

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
	default:
		break;
	}

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

	m_ucBitmap = data;

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

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel(const Point p)
{
	return GetOriginalPixel(p.x, p.y);
}

