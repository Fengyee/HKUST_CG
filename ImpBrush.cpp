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

	memcpy(color, pDoc->GetOriginalPixel(source), 3);
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