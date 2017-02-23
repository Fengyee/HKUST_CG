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

Point source_prev_s;
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
	double angle = calcAngle(pDoc, source_prev_s, source);

	int number = rand() % 3 + 3;
	for (size_t i = 0; i < number; i++)
	{
		int offset_x = rand() % size;
		int offset_y = rand() % size;
		
		//randomize the length of line
		int percent_left = rand() % 100 + 1;
		int percent_right = rand() % 100 + 1;
		const Point s(source.x - size/2 + offset_x, source.y - size/2 + offset_y);
		const Point t(target.x - size / 2 + offset_x, target.y - size / 2 + offset_y);

		GLfloat ax, ay, bx, by;
		ax = t.x - sqrt((float)percent_left / 100) * size * cos(angle) / 2;
		ay = t.y - sqrt((float)percent_left / 100) * size * sin(angle) / 2;
		bx = t.x + sqrt((float)percent_right / 100) * size * cos(angle) / 2;
		by = t.y + sqrt((float)percent_right / 100) * size * sin(angle) / 2;
		glLineWidth(width);

		glBegin(GL_LINES);
		SetColor(s);
		if (!dlg->getEdgeClipping())
		{
			glVertex2f(ax, ay);
			glVertex2f(bx, by);
		}
		else
		{
			Point end;
			end.x = t.x;
			end.y = t.y;
			bool drawOnEdge = pDoc->m_ucEdgeImg[(t.y * pDoc->m_nWidth + t.x) * 3] != 0;
			for (int i = 0; i <= size / 2; i++)
			{
				end.x = t.x - i * cos(angle);
				end.y = t.y - i * sin(angle);
				if (end.x <= 0 || end.x >= pDoc->m_nWidth
					|| end.y <= 0 || end.y >= pDoc->m_nHeight
					|| !drawOnEdge && (pDoc->m_ucEdgeImg[3 * (end.x + end.y * pDoc->m_nWidth)] != 0))
				{
					break;
				}
			}

			glVertex2d(end.x, end.y);
			end.x = t.x;
			end.y = t.y;
			for (int i = 0; i <= size / 2; i++)
			{
				end.x = t.x + i * cos(angle);
				end.y = t.y + i * sin(angle);
				if (end.x <= 0 || end.x >= pDoc->m_nWidth
					|| end.y <= 0 || end.y >= pDoc->m_nHeight
					|| !drawOnEdge && (pDoc->m_ucEdgeImg[3 * (end.x + end.y * pDoc->m_nWidth)] != 0))
				{
					break;
				}
			}
			glVertex2d(end.x, end.y);
		}
		glEnd();
	}
	source_prev_s.x = source.x;
	source_prev_s.y = source.y;
}

void ScatteredLineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

