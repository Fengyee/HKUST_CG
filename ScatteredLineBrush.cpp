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

enum direction {GRADIENT, SLIDER, BRUSH};
extern float frand();

ScatteredLineBrush::ScatteredLineBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

double ScatteredLineBrush::calcAngle(ImpressionistDoc* pDoc, const Point source, const Point target)
{
	direction d = GRADIENT;
	double angle = pDoc->getLineAngle() * M_PI / 180.0;

	int filter_x[3][3] = { { -1, 0, 1 },{ -2, 0, 2 },{ -1, 0, 1 } };
	int filter_y[3][3] = { { 1, 2, 1 },{ 0, 0, 0 },{ -1, -2, -1 } };
	GLubyte pixel_cur[4];
	GLubyte pixels_bw;
	int x_conv = 0;
	int y_conv = 0;

	switch (d)
	{
	case GRADIENT:

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				memcpy(pixel_cur, pDoc->GetOriginalPixel(source.x-1+i, source.y-1+j), 3);
				//glReadPixels(source.x-1+i, source.y-1+j, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel_cur);
				pixels_bw = (pixel_cur[0] + pixel_cur[1] + pixel_cur[2]) / 3;
				x_conv = x_conv + (int)pixels_bw * filter_x[i][j];
				y_conv = y_conv + (int)pixels_bw * filter_y[i][j];
				//std::cout << (int)pixels_bw << filter_x[i][j] << filter_y[i][j] << std::endl;
				//std::cout << x_conv << y_conv << std::endl;
			}
		}
		angle = atan((float)(y_conv) / (float)(x_conv));
		//std::cout << std::endl << angle << std::endl;
		//std::cout << x_conv << std::endl;
		//std::cout << y_conv << std::endl;
		
		break;
	case SLIDER:
		break;
	case BRUSH:
		angle = atan((float)(target.y - source.y) / (float)(target.x - source.x));
		std::cout << angle << std::endl;
		std::cout << target.x << source.x << std::endl;
		std::cout << target.y << source.y << std::endl;
		break;
	default:
		break;
	}
    
	return angle;
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
	double angle = calcAngle(pDoc, source, target);

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
}

void ScatteredLineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

