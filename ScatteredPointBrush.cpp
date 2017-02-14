//
// ScatteredPointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredPointBrush.h"
#include <ctime>

extern float frand();

ScatteredPointBrush::ScatteredPointBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatteredPointBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	
	glPointSize(1.0);
	
	BrushMove(source, target);
	
}

void ScatteredPointBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredPointBrush::BrushMove  document is NULL\n");
		return;
	}
	
	float alpha = pDoc->getAlpha();

	int size = pDoc->getSize();
	int count = size * size / 5 + 1;

	//srand(time(NULL));
	for (size_t i = 0; i < count; i++)
	{
		glBegin(GL_POINTS);
		
		int offset_x = rand() % size;
		int offset_y = rand() % size;
		const Point s(target.x - size / 2 + offset_x, target.y - size / 2 + offset_y);
		SetColor(s);
		glVertex2d(target.x - size / 2 + offset_x, target.y - size / 2 + offset_y);
		glEnd();
	}
	
}

void ScatteredPointBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

