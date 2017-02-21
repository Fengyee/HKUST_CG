//
// LineBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "LineBrush.h"
#include <math.h>

Point source_prev;
extern float frand();

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void LineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();


	BrushMove(source, target);
}

void LineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("LineBrush::BrushMove  document is NULL\n");
		return;
	}

	int size = pDoc->getSize();
	int width = pDoc->getLineWidth();
	double angle = calcAngle(pDoc, source_prev, source);
	//double angle = pDoc->getLineAngle() * M_PI / 180.0;
	float alpha = pDoc->getAlpha();

	GLfloat ax, ay, bx, by;
	ax = target.x - size * cos(angle) / 2;
	ay = target.y - size * sin(angle) / 2;
	bx = target.x + size * cos(angle) / 2;
	by = target.y + size * sin(angle) / 2;

	glLineWidth(width);

	glBegin(GL_LINES);

	SetColor(source);
	glVertex2f(ax, ay);
	glVertex2f(bx, by);

	glEnd();

	source_prev.x = source.x;
	source_prev.y = source.y;
}

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

