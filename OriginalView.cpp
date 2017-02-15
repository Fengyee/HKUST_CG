//
// originalview.cpp
//
// The code maintaining the original view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "originalview.h"

#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#endif

#define IND_X 5
#define IND_Y 5

GLubyte indicator_preserve[3 * IND_X * IND_Y];

OriginalView::OriginalView(int			x, 
						   int			y, 
						   int			w, 
						   int			h, 
						   const char*	l)
							: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;

}

void OriginalView::draw()
{
	if(!valid())
	{
		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		// Tell openGL to read from the front buffer when capturing
		// out paint strokes 
		glReadBuffer( GL_FRONT );
		ortho();

	}

	glClear( GL_COLOR_BUFFER_BIT );

	if ( m_pDoc->m_ucBitmap ) 
	{
		// note that both OpenGL pixel storage and the Windows BMP format
		// store pixels left-to-right, BOTTOM-to-TOP!!  thus all the fiddling
		// around with startrow.

		m_nWindowWidth=w();
		m_nWindowHeight=h();

		int drawWidth, drawHeight;
		GLvoid* bitstart;

		// we are not using a scrollable window, so ignore it
		Point scrollpos;	// = GetScrollPosition();
		scrollpos.x=scrollpos.y=0;

		drawWidth	= min( m_nWindowWidth, m_pDoc->m_nWidth );
		drawHeight	= min( m_nWindowHeight, m_pDoc->m_nHeight );

		int	startrow	= m_pDoc->m_nHeight - (scrollpos.y + drawHeight);
		if ( startrow < 0 ) 
			startrow = 0;


		bitstart = m_pDoc->m_ucBitmap + 3 * ((m_pDoc->m_nWidth * startrow) + scrollpos.x);
		GLubyte* image = (GLubyte*)bitstart;
		
		int start_x = indicator_x - 5 >= 0 ? indicator_x - 5 : 0;
		int end_x = indicator_x + 6 >= m_pDoc->m_nWidth ? m_pDoc->m_nWidth : indicator_x + 6;
		int start_y = indicator_y - 5 >= 0 ? indicator_y - 5 : 0;
		int end_y = indicator_y + 6 >= m_pDoc->m_nHeight ? m_pDoc->m_nHeight : indicator_y + 6;

		memset(indicator_preserve, 0, 3 * IND_X * IND_Y);

		for (int b = 0; b < IND_Y; b++)
		{
			for (int a = 0; a < IND_X; a++)
			{
				if (start_x + a < end_x && start_y + b < end_y)
				{
					GLubyte* point = m_pDoc->GetOriginalPixel(start_x + a, start_y + b);
					// printf("Point(%d, %d) %d, %d, %d \n", start_x + a, start_y + b, point[0], point[1], point[2]);
					indicator_preserve[(a + b * IND_X) * 3] = point[0];
					indicator_preserve[(a + b * IND_X) * 3 + 1] = point[1];
					indicator_preserve[(a + b * IND_X) * 3 + 2] = point[2];
					image[((start_x + a) + (start_y - startrow + b) * m_pDoc->m_nWidth) * 3] = 255;
					image[((start_x + a) + (start_y - startrow + b) * m_pDoc->m_nWidth) * 3 + 1] = 0;
					image[((start_x + a) + (start_y - startrow + b) * m_pDoc->m_nWidth) * 3 + 2] = 0;
				}
			}
		}

		// just copy image to GLwindow conceptually
		glRasterPos2i( 0, m_nWindowHeight - drawHeight );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nWidth );
		glDrawBuffer( GL_BACK );
		glDrawPixels( drawWidth, drawHeight, GL_RGB, GL_UNSIGNED_BYTE, bitstart );

		for (int b = 0; b < IND_Y; b++)
		{
			for (int a = 0; a < IND_X; a++)
			{
				if (start_x + a < end_x && start_y + b < end_y)
				{
					image[((start_x + a) + (start_y - startrow + b) * m_pDoc->m_nWidth) * 3] = indicator_preserve[(a + b * IND_X) * 3];
					image[((start_x + a) + (start_y - startrow + b) * m_pDoc->m_nWidth) * 3 + 1] = indicator_preserve[(a + b * IND_X) * 3 + 1];
					image[((start_x + a) + (start_y - startrow + b) * m_pDoc->m_nWidth) * 3 + 2] = indicator_preserve[(a + b * IND_X) * 3 + 2];
				}
			}
		}
	}
			
	glFlush();
}

void OriginalView::refresh()
{
	redraw();
}

void OriginalView::resizeWindow(int	width, 
								int	height)
{
	resize(x(), y(), width, height);
}

void OriginalView::update_indicator(int x, int y)
{
	indicator_x = x;
	indicator_y = y;
	redraw();
}