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
	int filter_x[3][3] = { { -1, 0, 1 },{ -2, 0, 2 },{ -1, 0, 1 } };
	int filter_y[3][3] = { { 1, 2, 1 },{ 0, 0, 0 },{ -1, -2, -1 } };
//	int filter_x[5][5] = { {1, 2, 0, -2, -1}, {4, 8, 0, -8, -4}, {6, 12, 0, -12, -6}, {4, 8, 0, -8, -4},{ 1, 2, 0, -2, -1 } };
//	int filter_y[5][5] = { {-1, -4, -6, -4, -1}, {-2, -8, -12, -8, -2}, {0, 0, 0, 0, 0}, {2, 8, 12, 8, 2}, {1, 4, 6, 4, 1} };
//	int filter_x[7][7] = { {-1, -1, -1, 0, 1, 1, 1}, {-1, -2, -2, 0, 2, 2, 1 },{ -1, -2, -3, 0, 1, 2, 3 },
//		{ -1, -2, -3, 0, 1, 2, 3 },{ -1, -2, -3, 0, 1, 2, 3 },{ -1, -2, -2, 0, 2, 2, 1 },{ -1, -1, -1, 0, 1, 1, 1 } };
//	int filter_y[7][7] = { { 1, 1, 1, 1, 1, 1, 1 },{1, 2, 2, 2, 2, 2, 1},{1, 2, 3, 3, 3, 2, 1}, { 0, 0, 0, 0, 0, 0, 0 },
//		{ -1, -2, -3, -3, -3, -2, -1 },{ -1, -2, -2, -2, -2, -2, -1 },{ -1, -1, -1, -1, -1, -1, -1 } };
	GLubyte pixel_cur[4];
	GLubyte pixels_bw;
	int x_conv = 0;
	int y_conv = 0;

	int radius = (width + 1) / 2;
	int num_points = size / radius + 1;

	int ax = target.x;
	int ay = target.y;
	int bx = target.x;
	int by = target.y;
	for (int j = 0; j < num_points; j++)
	{
		glBegin(GL_POLYGON);
		SetColor(source);
		for (int i = 0; i < 90; i++)
		{
			float degInRad = i * 4 * M_PI / 180.0;
			glVertex2f(ax + cos(degInRad)*radius, ay + sin(degInRad)*radius);
			
		}
		glEnd();
		
		// calculate gradients
		for (int s = 0; s < radius; s++) {
			// calculate gradient
			x_conv = 0;
			y_conv = 0;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					memcpy(pixel_cur, pDoc->GetOriginalPixel(ax - 1 + j, ay - 1 + i), 3);
					if (ax - 1 + j >= 0 && ay - 1 + i >= 0 && ax-1+j <= pDoc->m_nPaintWidth && ay-1+i <= pDoc->m_nPaintHeight)
					{
						pixels_bw = (pixel_cur[0] + pixel_cur[1] + pixel_cur[2]) / 3;
					}
						
					else {
						pixels_bw = 0;
					}
						
						
					//glReadPixels(source.x-1+i, source.y-1+j, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel_cur);
					
					x_conv = x_conv + (int)pixels_bw * filter_x[i][j];
					y_conv = y_conv + (int)pixels_bw * filter_y[i][j];
					//std::cout << (int)pixels_bw << filter_x[i][j] << filter_y[i][j] << std::endl;
					//std::cout << x_conv << y_conv << std::endl;
				}
			}
			//float grad = sqrt(x_conv * x_conv + y_conv * y_conv);
			if (y_conv != 0)
				angle = atan((float)(y_conv) / (float)(x_conv)) + M_PI / 2;		//angle of drawing !!! not gradient !!!
			else
				angle = 0;

//			ax = target.x - size * cos(angle) / 2;
//			ay = target.y - size * sin(angle) / 2;
//			bx = target.x + size * cos(angle) / 2;
//			by = target.y + size * sin(angle) / 2;
//			ax = target.x - cos(angle);
//			ay = target.y - sin(angle);

			if (angle < (67.5 * M_PI / 180.0) || angle > (112.5 * M_PI / 180.0))
			{
				if (y_conv > 0) ax = ax + 1;
				else ax = ax - 1;

			}
			if (angle > (22.5 * M_PI / 180.0) && angle <(157.5 * M_PI / 180.0))
			{
				if (x_conv > 0) ay = ay + 1;
				else ay = ay - 1;

			}
		}
	}



	

}

void CurvedBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

