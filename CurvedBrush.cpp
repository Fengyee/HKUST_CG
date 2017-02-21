//
// LineBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "CurvedBrush.h"
#include <math.h>

extern float frand();

CurvedBrush::CurvedBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void CurvedBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize();

	glPointSize((float)size);

	BrushMove(source, target);
}

void CurvedBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("LineBrush::BrushMove  document is NULL\n");
		return;
	}

	int size = pDoc->getSize();
	size = 100;
	int width = pDoc->getLineWidth();
	//double angle = calcAngle(pDoc, source_prev_c, source);

	float alpha = pDoc->getAlpha();
/*
	GLfloat ax, ay, bx, by;
	ax = target.x - size * cos(angle) / 2;
	ay = target.y - size * sin(angle) / 2;
	bx = target.x + size * cos(angle) / 2;
	by = target.y + size * sin(angle) / 2;
*/
	double angle = 45 * M_PI / 180.0;
//	int filter_x[3][3] = { { -1, 0, 1 },{ -1, 0, 1 },{ -1, 0, 1 } };
//	int filter_y[3][3] = { { 1, 1, 1 },{ 0, 0, 0 },{ -1, -1, -1 } };
	int filter_x[5][5] = { {-1, -1, 0, 1, 1}, {-1, -2, 0, 2, 1}, {-1, -2, 0, 2, 1}, {-1, -2, 0, 2, 1},{ -1, -1, 0, 1, 1 } };
	int filter_y[5][5] = { {1, 1, 1, 1, 1}, {1, 2, 2, 2, 1}, {0, 0, 0, 0, 0}, {-1, -2, -2, -2, -1}, {-1, -1, -1, -1, -1} };
//	int filter_x[7][7] = { {-1, -1, -1, 0, 1, 1, 1}, {-1, -2, -2, 0, 2, 2, 1 },{ -1, -2, -3, 0, 1, 2, 3 },
//		{ -1, -2, -3, 0, 1, 2, 3 },{ -1, -2, -3, 0, 1, 2, 3 },{ -1, -2, -2, 0, 2, 2, 1 },{ -1, -1, -1, 0, 1, 1, 1 } };
//	int filter_y[7][7] = { { 1, 1, 1, 1, 1, 1, 1 },{1, 2, 2, 2, 2, 2, 1},{1, 2, 3, 3, 3, 2, 1}, { 0, 0, 0, 0, 0, 0, 0 },
//		{ -1, -2, -3, -3, -3, -2, -1 },{ -1, -2, -2, -2, -2, -2, -1 },{ -1, -1, -1, -1, -1, -1, -1 } };
	GLubyte pixel_cur[4];
	GLubyte pixels_bw;
	int x_conv = 0;
	int y_conv = 0;

	int radius = (width + 1) / 2;
	int num_points = size;
	if (radius > 2) num_points = size / (radius / 2);

	int ax = target.x;
	int ay = target.y;
	int bx = target.x;
	int by = target.y;
	for (int j = 0; j < (num_points + 1) / 2; j++)
	{
		glBegin(GL_POLYGON);
		SetColor(source);
		for (int i = 0; i < 360; i++)
		{
			float degInRad = i * M_PI / 180.0;
			glVertex2f(ax + cos(degInRad)*radius, ay + sin(degInRad)*radius);
			
		}
		glEnd();
		glBegin(GL_POLYGON);
		SetColor(source);
		for (int i = 0; i < 360; i++)
		{
			float degInRad = i * M_PI / 180.0;
			glVertex2f(bx + cos(degInRad)*radius, by + sin(degInRad)*radius);

		}
		glEnd();
		
		// calculate gradient
		x_conv = 0;
		y_conv = 0;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				memcpy(pixel_cur, pDoc->GetOriginalPixel(ax - 2 + j, ay - 2 + i), 3);
				//glReadPixels(source.x-1+i, source.y-1+j, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel_cur);
				pixels_bw = (pixel_cur[0] + pixel_cur[1] + pixel_cur[2]) / 3;
				x_conv = x_conv + (int)pixels_bw * filter_x[i][j];
				y_conv = y_conv + (int)pixels_bw * filter_y[i][j];
				//std::cout << (int)pixels_bw << filter_x[i][j] << filter_y[i][j] << std::endl;
				//std::cout << x_conv << y_conv << std::endl;
			}
		}
		if (y_conv != 0)
			angle = atan((float)(x_conv) / (float)(y_conv));
		else
//			if (x_conv > 0) angle = M_PI / 2;
//			else angle = -1 * M_PI / 2;
			if (ay < target.y) angle = M_PI / 2;
			else angle = -1 * M_PI / 2;
		ax = ax - ((radius + 1) / 2) * cos(angle);
		ay = ay - ((radius + 1) / 2) * sin(angle);
		x_conv = 0;
		y_conv = 0;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				memcpy(pixel_cur, pDoc->GetOriginalPixel(bx - 2 + j, by - 2 + i), 3);
				//glReadPixels(source.x-1+i, source.y-1+j, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel_cur);
				pixels_bw = (pixel_cur[0] + pixel_cur[1] + pixel_cur[2]) / 3;
				x_conv = x_conv + (int)pixels_bw * filter_x[i][j];
				y_conv = y_conv + (int)pixels_bw * filter_y[i][j];
				//std::cout << (int)pixels_bw << filter_x[i][j] << filter_y[i][j] << std::endl;
				//std::cout << x_conv << y_conv << std::endl;
			}
		}
		if (y_conv != 0)
			angle = atan((float)(x_conv) / (float)(y_conv));
		else
			if (by > target.y) angle = M_PI / 2;
			else angle = -1 * M_PI / 2;
		bx = bx + ((radius + 1) / 2) * cos(angle);
		by = by + ((radius + 1) / 2) * sin(angle);
		
//		ax = ax - (radius / 2) * cos(angle);
//		ay = ay - (radius / 2) * sin(angle);
//		bx = bx + (radius / 2) * cos(angle);
//		by = by + (radius / 2) * sin(angle);
	}




}

void CurvedBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

