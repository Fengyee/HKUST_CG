//
// ImpBrush.cpp
//
// The implementation of virtual brush. All the other brushes inherit from it.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ImpBrush.h"
#include <cmath>


// Static class member initializations
int			ImpBrush::c_nBrushCount	= 0;
ImpBrush**	ImpBrush::c_pBrushes	= NULL;

ImpBrush::ImpBrush(ImpressionistDoc*	pDoc, 
				   char*				name) :
					m_pDoc(pDoc), 
					m_pBrushName(name)
{
}

//---------------------------------------------------
// Return m_pDoc, which connects the UI and brushes
//---------------------------------------------------
ImpressionistDoc* ImpBrush::GetDocument(void)
{
	return m_pDoc;
}

//---------------------------------------------------
// Return the name of the current brush
//---------------------------------------------------
char* ImpBrush::BrushName(void)
{
	return m_pBrushName;
}

//----------------------------------------------------
// Set the color to paint with to the color at source,
// which is the coord at the original window to sample 
// the color from
//----------------------------------------------------
void ImpBrush::SetColor (const Point source)
{
	ImpressionistDoc* pDoc = GetDocument();

	GLubyte color[4];
	GLubyte pixel_cur[4];
	int conv_sum = 0;
	int r_conv = 0;
	int g_conv = 0;
	int b_conv = 0;
	if (pDoc->getFilter() == 0) {
		if (pDoc->getMural()) {
			memcpy(color, pDoc->GetAnotherPixel(source), 3);
		}
		else {
			memcpy(color, pDoc->GetOriginalPixel(source), 3);
		}
		
	}
	else {
		int filterHeight = pDoc->getFilterHeight();
		int filterWidth = pDoc->getFilterWidth();
		int* filterValue = pDoc->getFilterValue();
		int offset_x = filterWidth / 2;
		int offset_y = filterHeight / 2;
		for (int i = 0; i < filterHeight; i++) {
			for (int j = 0; j < filterWidth; j++) {
				if (pDoc->getMural()) {
					memcpy(pixel_cur, pDoc->GetAnotherPixel(source.x - offset_x + i, source.y - offset_y + j), 3);
				}
				else {
					memcpy(pixel_cur, pDoc->GetOriginalPixel(source.x - offset_x + i, source.y - offset_y + j), 3);
				}
				
				//glReadPixels(source.x-1+i, source.y-1+j, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel_cur);
				//pixels_bw = (pixel_cur[0] + pixel_cur[1] + pixel_cur[2]) / 3;
				r_conv = r_conv + (int)pixel_cur[0] * filterValue[i*filterWidth+j];
				g_conv = g_conv + (int)pixel_cur[1] * filterValue[i*filterWidth + j];
				b_conv = b_conv + (int)pixel_cur[2] * filterValue[i*filterWidth + j];
				conv_sum = conv_sum + filterValue[i*filterWidth + j];
				//std::cout << (int)pixels_bw << filter_x[i][j] << filter_y[i][j] << std::endl;
				//std::cout << x_conv << y_conv << std::endl;
			}
		}
		if (conv_sum == 0) conv_sum = 1;
		color[0] = r_conv / conv_sum;
		color[1] = g_conv / conv_sum;
		color[2] = b_conv / conv_sum;


	}
	color[3] = m_pDoc->getAlpha() * 255;
	Fl_Color_Chooser* CC = pDoc->m_pUI->m_colorChooser;
	color[0] *= CC->r();
	color[1] *= CC->g();
	color[2] *= CC->b();

	glColor4ubv(color);

}

double ImpBrush::calcAngle(ImpressionistDoc* pDoc, const Point source_prev, const Point source)
{
	int d = pDoc->m_nBrushDirection;
	double angle = pDoc->getLineAngle() * M_PI / 180.0;

	int filter_x[3][3] = { { -1, 0, 1 },{ -2, 0, 2 },{ -1, 0, 1 } };
	int filter_y[3][3] = { { 1, 2, 1 },{ 0, 0, 0 },{ -1, -2, -1 } };
	GLubyte pixel_cur[4];
	GLubyte pixels_bw;
	int x_conv = 0;
	int y_conv = 0;

	switch (d)
	{
	case SLIDER_AND_RIGHT_MOUSE:
		break;
	case GRADIENT:

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				memcpy(pixel_cur, pDoc->GetOriginalPixel(source.x - 1 + i, source.y - 1 + j), 3);
				//glReadPixels(source.x-1+i, source.y-1+j, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel_cur);
				if (source.x - 1 + j >= 0 && source.y - 1 + i >= 0 && source.x - 1 + j <= pDoc->m_nPaintWidth && source.y - 1 + i <= pDoc->m_nPaintHeight)
				{
					pixels_bw = (pixel_cur[0] + pixel_cur[1] + pixel_cur[2]) / 3;
				}
				else
				{
					pixels_bw = 0;
				}
				
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

	case BRUSH_DIRECTION:
		angle = atan((float)(source.y - source_prev.y) / (float)(source.x - source_prev.x));
		//std::cout << angle << std::endl;
		//std::cout << target.x << source_prev.x << std::endl;
		//std::cout << target.y << source_prev.y << std::endl;
		break;
	default:
		break;
	}

	return angle;
}