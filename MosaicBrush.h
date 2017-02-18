//
// MosaicBrush.h
//
// The header file for Point Brush. 
//

#ifndef MOSAICBRUSH_H
#define MOSAICBRUSH_H

#include "ImpBrush.h"

class MosaicBrush : public ImpBrush
{
public:
	MosaicBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif