//
// AlphaMappedBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "AlphaMappedBrush.h"

extern float frand();

AlphaMappedBrush::AlphaMappedBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void AlphaMappedBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();

	//printf("source%d %d\n", source.x, source.y);

	glPointSize((float)size);

	BrushMove(source, target);
}

void AlphaMappedBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	GLubyte color[4];

	memcpy(color, pDoc->GetOriginalPixel(source), 3);
	// 
	Fl_Color_Chooser* CC = pDoc->m_pUI->m_colorChooser;
	color[0] *= CC->r();
	color[1] *= CC->g();
	color[2] *= CC->b();
	int x = pDoc->m_ucAlphaMappedImage[source.x + source.y * pDoc->m_nWidth];
	color[3] = x;
	glColor4ubv(color);

	glBegin(GL_POINTS);
	glVertex2d(target.x, target.y);
	glEnd();

}

void AlphaMappedBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

