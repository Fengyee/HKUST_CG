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
	int		loadAnotherImage(char *iname);
	int		saveImage(char *iname);			// called by the UI to save image
	int		loadAlphaMappedImage(char *name);


	int     clearCanvas();                  // called by the UI to clear the drawing canvas
	int		undoPainting();
	void	autoPainting();					// called by the UI to start auto painting
	void	setBrushType(int type);			// called by the UI to set the brushType
	void	setBrushDirection(int type);
	bool	alphaMappedImageLoaded;
	void	recalEdgeImg();
	void	setDisplayImage(int type);

	void	rePaint();


	int		getSize();						
	void	setSize(int size);				
	int		getLineWidth();					
	void	setLineWidth(int width);		
	int		getLineAngle();					
	void	setLineAngle(int angle);			
	float	getAlpha();							
	void	setAlpha(float alpha);				
	int		getMosasicLevel();
	int		getSpacing();					// get the spacing of auto painting
	int		getRand();
	int		getFilter();
	int*	getFilterValue();
	int		getFilterHeight();
	int		getFilterWidth();
	int		getResolution();
	int		getMural();
	int		getAnotherGradient();

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
	unsigned char*  m_ucUndoBitstart;
	unsigned char*	m_ucEdgeImg;
	unsigned char*	m_ucAnotherImg;
	unsigned char*	m_ucdisplayImage;

	unsigned char*  m_ucAlphaMappedImage;
	int				m_nAlphaMappedImageWidth;
	int				m_nAlphaMappedImageHeight;

	int*			m_ipGradient;
	int*			m_ipGValue;

	// The current active brush.
	ImpBrush*		m_pCurrentBrush;

	int m_nBrushDirection;
	// Size of the brush.
	int m_nSize;
	int m_nLineWidth;
	int m_nAngle;
	int m_nMosaicLevel;

	// Spacing of automatic painting
	////int m_nSpacing;

	ImpressionistUI*	m_pUI;

// Operations
public:
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel( int x, int y );   
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel( const Point p );  

	GLubyte* GetDisplayPixel(int x, int y);
	GLubyte* GetAnotherPixel(int x, int y);
	GLubyte* GetAnotherPixel(const Point p);


private:
	char			m_imageName[256];

};

extern void MessageBox(char *message);

#endif
