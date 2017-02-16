//
// ScatteredCircleBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredCircleBrush.h"
#include <math.h>
#include <ctime>

extern float frand();

ScatteredCircleBrush::ScatteredCircleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatteredCircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();



	glPointSize((float)size);

	BrushMove(source, target);
}

void ScatteredCircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredCircleBrush::BrushMove  document is NULL\n");
		return;
	}
	int size = pDoc->getSize();
	int radius = size / 2;
	GLfloat alpha = pDoc->getAlpha();

	
	int number = rand() % 3 + 3;
	//srand(time(NULL));

	for (size_t i = 0; i < number; i++)
	{
		
		int offset_x = rand() % size;
		int offset_y = rand() % size;
		glBegin(GL_POLYGON);
		const Point s(source.x - radius + offset_x, source.y - radius + offset_y);
		const Point t(target.x - radius + offset_x, target.y - radius + offset_y);
		SetColor(s);
		for (int i = 0; i < 360; i++)
		{
			float degInRad = i * M_PI / 180.0;
			glVertex2f(t.x + cos(degInRad)*radius, t.y + sin(degInRad)*radius);
		}
		glEnd();
	}
	
}

void ScatteredCircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

