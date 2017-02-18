// 
// impressionistDoc.h
//
// header file for Doc 
//

#ifndef IMPRESSIONISTDOC_H
#define IMPRESSIONISTDOC_H

#include "impressionist.h"
#include "bitmap.h"

class ImpressionistUI;

class ImpressionistDoc 
{
public:
	ImpressionistDoc();

	void	setUI(ImpressionistUI* ui);		// Assign the UI to use

	int		loadImage(char *iname);			// called by the UI to load image
	int		saveImage(char *iname);			// called by the UI to save image


	int     clearCanvas();                  // called by the UI to clear the drawing canvas
	int		undoPainting();
	void	setBrushType(int type);			// called by the UI to set the brushType
	void	setBrushDirection(int type);
	int		getSize();						// get the UI size
	void	setSize(int size);				// set the UI size
	int		getLineWidth();					// get the UI size
	void	setLineWidth(int width);			// set the UI size
	int		getLineAngle();					// get the UI size
	void	setLineAngle(int angle);			// set the UI size
	float	getAlpha();							// get the UI size
	void	setAlpha(float alpha);				// set the UI size
	int		getSpacing();					// get the spacing of auto painting

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
	unsigned char*	m_ucBitmap;
	unsigned char*	m_ucPainting;


	// The current active brush.
	ImpBrush*			m_pCurrentBrush;

	int m_nBrushDirection;

	// Size of the brush.
	int m_nSize;
	
	int m_nLineWidth;
	int m_nAngle;

	// Spacing of automatic painting
	////int m_nSpacing;

	ImpressionistUI*	m_pUI;

// Operations
public:
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel( int x, int y );   
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel( const Point p );  


private:
	char			m_imageName[256];

};

extern void MessageBox(char *message);

#endif
