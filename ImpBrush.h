#ifndef IMPBRUSH_H
#define IMPBRUSH_H

//
// ImpBrush.h
//
// The header file of virtual brush. All the other brushes inherit from it.
//

#include <stdlib.h>

// Each brush type has an associated constant.
enum
{
	BRUSH_POINTS = 0,
	BRUSH_LINES,
	BRUSH_CIRCLES,
	BRUSH_SCATTERED_POINTS,
	BRUSH_SCATTERED_LINES,
	BRUSH_SCATTERED_CIRCLES,
	BRUSH_MOSAIC,
	BRUSH_ALPHAMAPPED,
	BRUSH_CURVED,
	NUM_BRUSH_TYPE // Make sure this stays at the end!
};

enum
{
	SLIDER_AND_RIGHT_MOUSE = 0,
	GRADIENT,
	BRUSH_DIRECTION,
	NUM_DIRECTION_TYPE // Make sure this stays at the end!
};


class ImpressionistDoc; // Pre-declaring class

class Point 
{
public:
	Point() {};
	Point(int xx, int yy) { x = xx; y = yy; };

	int x, y;
};

class ImpBrush 
{
protected:
	ImpBrush::ImpBrush( ImpressionistDoc* pDoc = NULL, char* name = NULL );

public:
	// The implementation of your brush should realize these virtual functions
	virtual void BrushBegin( const Point source, const Point target ) = 0;
	virtual void BrushMove( const Point source, const Point target ) = 0;
	virtual void BrushEnd( const Point source, const Point target ) = 0;

	// according to the source image and the position, determine the draw color
	void SetColor( const Point source);

	// get Doc to communicate with it
	ImpressionistDoc* GetDocument( void );

	// Return the name of the brush (not used in this version).
	char* BrushName( void );

	//calculate the angle of line brush
	double calcAngle(ImpressionistDoc* pDoc, const Point source_prev, const Point source);
	
	static int			c_nBrushCount;	// How many brushes we have,
	static ImpBrush**	c_pBrushes;		// and what they are.

private:
	ImpressionistDoc*	m_pDoc;

	// Brush's name (not used in this version).
	char*				m_pBrushName;
};

#endif