//
// MosaicBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "MosaicBrush.h"

extern float frand();

MosaicBrush::MosaicBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void MosaicBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();

	glPointSize((float)size);

	BrushMove(source, target);
}

void MosaicBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("MosaicBrush::BrushMove  document is NULL\n");
		return;
	}
	
	int size_p = pDoc->getSize();
	// 4 pixels length 
	int mosaicLevel = size_p / pDoc->getMosasicLevel();
	glPointSize(size_p / mosaicLevel);

	glBegin(GL_POINTS);
	for (int i = 0; i < mosaicLevel; i++) {
		for (int j = 0; j < mosaicLevel; j++) {
			SetColor(Point(source.x - size_p / 2 + size_p / 2 / mosaicLevel + j * size_p / mosaicLevel, source.y - size_p / 2 + size_p / 2 / mosaicLevel + i * size_p / mosaicLevel));
			glVertex2d(target.x - size_p / 2 + size_p / 2 / mosaicLevel + j * size_p / mosaicLevel, target.y - size_p / 2 + size_p / 2 / mosaicLevel + i * size_p / mosaicLevel);
		}
	}
	glEnd();
}

void MosaicBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

