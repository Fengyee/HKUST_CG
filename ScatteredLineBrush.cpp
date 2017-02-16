//
// ScatteredLineBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredLineBrush.h"
#include "math.h"
#include <iostream>

Point target_prev_s;
//enum direction {GRADIENT, SLIDER, BRUSH};
extern float frand();

ScatteredLineBrush::ScatteredLineBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatteredLineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();


	BrushMove(source, target);
}

void ScatteredLineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredLineBrush::BrushMove  document is NULL\n");
		return;
	}

	float alpha = pDoc->getAlpha();
	int size = pDoc->getSize();
	int width = pDoc->getLineWidth();
	double angle = calcAngle(pDoc, target_prev_s, target);

	int number = rand() % 3 + 3;
	for (size_t i = 0; i < number; i++)
	{
		int offset_x = rand() % size;
		int offset_y = rand() % size;
		
		//randomize the length of line
		int percent_left = rand() % 100 + 1;
		int percent_right = rand() % 100 + 1;
		const Point s(source.x - size/2 + offset_x, source.y - size/2 + offset_y);
		
		GLfloat ax, ay, bx, by;
		ax = s.x - sqrt((float)percent_left / 100) * size * cos(angle) / 2;
		ay = s.y - sqrt((float)percent_left / 100) * size * sin(angle) / 2;
		bx = s.x + sqrt((float)percent_right / 100) * size * cos(angle) / 2;
		by = s.y + sqrt((float)percent_right / 100) * size * sin(angle) / 2;
		glLineWidth(width);

		glBegin(GL_LINES);
		SetColor(s);
		glVertex2f(ax, ay);
		glVertex2f(bx, by);
		glEnd();
	}
	target_prev_s.x = target.x;
	target_prev_s.y = target.y;
}

void ScatteredLineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

