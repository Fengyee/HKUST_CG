//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "paintview.h"
#include "ImpBrush.h"
#include <cmath>
#include <algorithm>    
#include <vector>
#include <ctime>
#include <cstdlib>


#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN	4
#define RIGHT_MOUSE_DRAG	5
#define RIGHT_MOUSE_UP		6


#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#define max(a, b)	( ( (a)>(b) ) ? (a) : (b) )
#endif

static int		eventToDo;
static int		isAnEvent=0;
static Point	coord;
static Point right_mouse_source;
static Point prev_right_mouse_source;


PaintView::PaintView(int			x, 
					 int			y, 
					 int			w, 
					 int			h, 
					 const char*	l)
						: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;

}


void PaintView::draw()
{
	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
	#endif // !MESA

	//printf("params: %d\t%d\t%d\t%d\n", x(), y(), m_nDrawWidth, m_nDrawHeight);
	//fl_push_no_clip();
	//fl_pop_clip();
	//fl_push_clip(x(), y(), m_nDrawWidth, m_nDrawHeight);
	if(!valid())
	{

		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		ortho();

		glClear( GL_COLOR_BUFFER_BIT );
	}

	Point scrollpos;// = GetScrollPosition();
	scrollpos.x = 0;
	scrollpos.y	= 0;

	m_nWindowWidth	= w();
	m_nWindowHeight	= h();

	int drawWidth, drawHeight;
	drawWidth = min( m_nWindowWidth, m_pDoc->m_nPaintWidth );
	drawHeight = min( m_nWindowHeight, m_pDoc->m_nPaintHeight );

	int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
	if ( startrow < 0 ) startrow = 0;


	m_pPaintBitstart = m_pDoc->m_ucPainting + 
		3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);
	m_nDrawWidth	= drawWidth;
	m_nDrawHeight	= drawHeight;

	m_nStartRow		= startrow;
	m_nEndRow		= startrow + drawHeight;
	m_nStartCol		= scrollpos.x;
	m_nEndCol		= m_nStartCol + drawWidth;

	if ( m_pDoc->m_ucPainting && !isAnEvent) 
	{
		RestoreContent();

	}

	//printf("x, y : (%d, %d)\n", coord.x - x(), coord.y - y());
	bool outsideDrawRegion = coord.x >= m_nDrawWidth || coord.y >= m_nDrawHeight;

	if ( !outsideDrawRegion && m_pDoc->m_ucPainting && isAnEvent) 
	{
		// Clear it after processing.
		isAnEvent	= 0;	

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// glShadeModel(GL_FLAT);
		// glClearColor(0.0, 0.0, 0.0, 0.0);

		Point source( coord.x + m_nStartCol, m_nEndRow - coord.y );
		Point target(coord.x, m_nWindowHeight - coord.y);
		//std::cout << coord.x << ' ' << coord.y << std::endl;
		//std::cout << m_nStartCol << ' ' << m_nEndRow << std::endl;
		//std::cout << source.x << ' ' << source.y << std::endl;
		// printf("start column: %d; Start row: %d", m_nStartCol, m_nStartRow);


		// This is the event handler
		switch (eventToDo) 
		{
		case LEFT_MOUSE_DOWN:
			SaveUndoContent();
			m_pDoc->m_pCurrentBrush->BrushBegin( source, target );
			break;
		case LEFT_MOUSE_DRAG:
			m_pDoc->m_pCurrentBrush->BrushMove( source, target );
			break;
		case LEFT_MOUSE_UP:
			m_pDoc->m_pCurrentBrush->BrushEnd( source, target );
			
			SaveCurrentContent();
			RestoreContent();
			break;
		case RIGHT_MOUSE_DOWN:
			if (m_pDoc->m_nBrushDirection == SLIDER_AND_RIGHT_MOUSE) {
				right_mouse_source.x = source.x;
				right_mouse_source.y = source.y;
				prev_right_mouse_source.x = right_mouse_source.x;
				prev_right_mouse_source.y = right_mouse_source.y;
				SaveCurrentContent();
			}
			break;
		case RIGHT_MOUSE_DRAG:
			if (m_pDoc->m_nBrushDirection == SLIDER_AND_RIGHT_MOUSE) {
				RestoreContent();
				glLineWidth(2);
				glBegin(GL_LINES);
				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex2f(right_mouse_source.x, right_mouse_source.y);
				glVertex2f(source.x, source.y);
				glEnd();
				//glBegin(GL_LINES);
				//glColor3f(0.0f, 0.0f, 0.0f);
				//glVertex2f(right_mouse_source.x, right_mouse_source.y);
				//glVertex2f(prev_right_mouse_source.x, prev_right_mouse_source.y);
				//glEnd();
				prev_right_mouse_source.x = source.x;
				prev_right_mouse_source.y = source.y;
			}
			break;
		case RIGHT_MOUSE_UP:
			if (m_pDoc->m_nBrushDirection == SLIDER_AND_RIGHT_MOUSE) {
				RestoreContent();
				int angle = (int)((atan((float)(source.y - right_mouse_source.y) / (float)(source.x - right_mouse_source.x))) * 180 / M_PI);
				if (angle < 0) angle = angle + 360;
				m_pDoc->setLineAngle(angle);
			}
			break;

		default:
			printf("Unknown event!!\n");		
			break;
		}
	}

	glFlush();

	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
	#endif // !MESA

}


int PaintView::handle(int event)
{
	switch(event)
	{
	case FL_ENTER:
	    redraw();
		break;
	case FL_PUSH:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DOWN;
		else
			eventToDo=LEFT_MOUSE_DOWN;
		isAnEvent = 1;
		redraw();
		break;
	case FL_DRAG:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_DRAG;
		else
			eventToDo=LEFT_MOUSE_DRAG;
		isAnEvent=1;
		redraw();
		break;
	case FL_RELEASE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_UP;
		else
			eventToDo=LEFT_MOUSE_UP;
		isAnEvent=1;
		redraw();
		break;
	case FL_MOVE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		break;
	default:
		return 0;
		break;

	}

	// printf("Source is %d, %d ; \n Target is %d, %d ;\n", source.x, source.y, target.x, target.y);
	this->m_pDoc->m_pUI->m_origView->update_indicator(coord.x, m_pDoc->m_nHeight - coord.y);

	return 1;
}

void PaintView::refresh()
{
	redraw();
}

void PaintView::resizeWindow(int width, int height)
{
	resize(x(), y(), width, height);
}

void PaintView::SaveCurrentContent()
{
	// Tell openGL to read from the front buffer when capturing
	// out paint strokes
	glReadBuffer(GL_FRONT);

	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	
	glReadPixels( 0, 
				  m_nWindowHeight - m_nDrawHeight, 
				  m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart );
}

void PaintView::SaveUndoContent() 
{
	// Tell openGL to read from the front buffer when capturing
	// out paint strokes
/*	glReadBuffer(GL_FRONT);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);

	glReadPixels(0,
		m_nWindowHeight - m_nDrawHeight,
		m_nDrawWidth,
		m_nDrawHeight,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		m_pUndoBitstart);
		*/
	if (m_pDoc->m_ucPainting) {
		memcpy(m_pDoc->m_ucUndoBitstart, m_pPaintBitstart, m_pDoc->m_nPaintWidth * m_pDoc->m_nPaintHeight * 3);
	}

}

void PaintView::RestoreContent()
{
	glDrawBuffer(GL_BACK);

	glClear( GL_COLOR_BUFFER_BIT );

	glRasterPos2i( 0, m_nWindowHeight - m_nDrawHeight );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	glDrawPixels( m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart);

//	glDrawBuffer(GL_FRONT);
}

void PaintView::RestoreUndoContent()
{
	if (m_pDoc->m_ucUndoBitstart) {
		memcpy(m_pPaintBitstart, m_pDoc->m_ucUndoBitstart, m_pDoc->m_nPaintWidth * m_pDoc->m_nPaintHeight * 3);
		//std::cout << "finish memcpy" << std::endl;
	}
	RestoreContent();
//	glFlush();
	redraw();
}

void PaintView::autoPainting()
{
	std::srand(unsigned(std::time(0)));
	std::vector<int> rand_order;

	
	//std::cout << "start auto painting" << std::endl;
	int spacing = m_pDoc->getSpacing();
	//std::cout << "spacing is " << spacing << std::endl;
	int x_num = m_pDoc->m_nPaintWidth / spacing + 1;
	int y_num = m_pDoc->m_nPaintHeight / spacing + 1;

	// Generate a random vector for random painting order
	for (int i = 0; i < x_num * y_num; i++)
	{
		rand_order.push_back(i);
	}
	std::random_shuffle(rand_order.begin(), rand_order.end());

	//isAnEvent = 0;
	
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// glShadeModel(GL_FLAT);
	// glClearColor(0.0, 0.0, 0.0, 0.0);
	
	if (m_pDoc->getRand() == 0)
	{


		for (std::vector<int>::iterator it = rand_order.begin(); it != rand_order.end(); ++it)
		{
			//std::cout << ' ' << *it;
			int cor_x = *it % x_num * spacing;
			int cor_y = *it / x_num * spacing;
			//Point source(coord.x + m_nStartCol, m_nEndRow - coord.y);
			//Point target(coord.x, m_nWindowHeight - coord.y);
			Point auto_source(cor_x + m_nStartCol, m_nEndRow - cor_y);
			Point auto_target(cor_x, m_nWindowHeight - cor_y);
			m_pDoc->m_pCurrentBrush->BrushBegin(auto_source, auto_target);
		}
	}
	else
	{
		int originSize = m_pDoc->getSize();
		int minSize = originSize;
		if (originSize > 1) minSize = originSize / 2;
		for (std::vector<int>::iterator it = rand_order.begin(); it != rand_order.end(); ++it)
		{
			int cor_x = *it % x_num * spacing;
			int cor_y = *it / x_num * spacing;
			int currentSize = rand() % originSize + minSize;
			m_pDoc->setSize(currentSize);
			//printf("current size %d\n", currentSize);
			Point auto_source(cor_x + m_nStartCol, m_nEndRow - cor_y);
			Point auto_target(cor_x, m_nWindowHeight - cor_y);
			m_pDoc->m_pCurrentBrush->BrushBegin(auto_source, auto_target);
		}
		m_pDoc->setSize(originSize);
			
	}
	//std::cout << isAnEvent << ' ';
//	isAnEvent = 1;
//	redraw();


	SaveCurrentContent();
	m_pDoc->m_pUI->m_origView->redraw();
	redraw();
	RestoreContent();



//	glFlush();

	//std::cout << isAnEvent << std::endl;

	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);



//	m_nStartRow = startrow;
//	m_nEndRow = startrow + drawHeight;
//	m_nStartCol = scrollpos.x;
//	m_nEndCol = m_nStartCol + drawWidth;

	
}

void PaintView::multiPaint(int spacing, bool first=false)
{
	GLubyte pixel_original[4];
	GLubyte pixel_draw[4];

#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
#endif // !MESA


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//std::cout << first << std::endl;

	
	int x_num = m_pDoc->m_nPaintWidth / spacing + 1;
	int y_num = m_pDoc->m_nPaintHeight / spacing + 1;

	// Generate a random vector for random painting order
	std::vector<int> rand_order;
	for (int i = 0; i < x_num * y_num; i++)
	{
		rand_order.push_back(i);
	}
	std::random_shuffle(rand_order.begin(), rand_order.end());


	int originSize = m_pDoc->getSize();

	for (std::vector<int>::iterator it = rand_order.begin(); it != rand_order.end(); ++it)
	{
		int cor_x = *it % x_num * spacing;
		int cor_y = *it / x_num * spacing;

		if (strcmp(m_pDoc->m_pCurrentBrush->BrushName(), "Curved") != 0)
		{
			int currentSize = (rand() % originSize) + ((originSize + 1) / 2);
			m_pDoc->setSize(currentSize);
		}

		Point auto_source(cor_x + m_nStartCol, m_nEndRow - cor_y);
		Point auto_target(cor_x, m_nWindowHeight - cor_y);

		if (!first) {
			memcpy(pixel_original, m_pDoc->GetOriginalPixel(auto_source.x, auto_source.y), 3);
			memcpy(pixel_draw, (GLubyte*)(m_pDoc->m_ucPainting + 3 * (auto_target.y*m_pDoc->m_nWidth + auto_target.x)), 3);
			//glReadPixels(auto_target.x, auto_target.y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel_draw);
			//std::cout << (int)pixel_draw[0] << (int)pixel_draw[1] << (int)pixel_draw[2] << std::endl;
			//std::cout << (int)pixel_original[0] << (int)pixel_original[1] << (int)pixel_original[2] << std::endl;
			if (abs(pixel_original[0] - pixel_draw[0]) < 5 && abs(pixel_original[1] - pixel_draw[1]) < 5 && abs(pixel_original[2] - pixel_draw[2]) < 5) {
				continue;
			}

		}

		m_pDoc->m_pCurrentBrush->BrushBegin(auto_source, auto_target);
		
	}



	//std::cout << isAnEvent << ' ';
	//isAnEvent = 1;
	//redraw();

	SaveCurrentContent();
	RestoreContent();
	redraw();
	m_pDoc->m_pUI->m_origView->redraw();

	//std::cout << first << std::endl;

#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
#endif // !MESA
}

void PaintView::rePaint()
{
	//isAnEvent = 0;

	glReadBuffer(GL_FRONT);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth);
	// glShadeModel(GL_FLAT);
	// glClearColor(0.0, 0.0, 0.0, 0.0);
	int size = m_pDoc->getSize();
	int width = m_pDoc->getLineWidth();
	std::srand(unsigned(std::time(0)));
	int spacing;
	if (strcmp(m_pDoc->m_pCurrentBrush->BrushName(), "Curved") == 0)
	{
		m_pDoc->setSize(100);
		m_pDoc->setLineWidth(20);
		spacing = 16;
	}
	else
	{
		if ((strcmp(m_pDoc->m_pCurrentBrush->BrushName(), "Lines") == 0) || (strcmp(m_pDoc->m_pCurrentBrush->BrushName(), "Scattered Lines") == 0))
		{
			//printf("setting line\n");
			m_pDoc->setSize(50);
			m_pDoc->setLineWidth(24);
			spacing = 12;
		}
		else
		{
			m_pDoc->setSize(24);
			spacing = 12;
		}
	}
	int resol = m_pDoc->getResolution();
	for (int j = 0; j < resol; j++)
	{
		if (j != 0)
		{
			spacing = (spacing + 1) / 2;

			if (strcmp(m_pDoc->m_pCurrentBrush->BrushName(), "Curved") == 0)
			{
				m_pDoc->setSize((m_pDoc->getSize() + 1 - 40) / 2 + 40);
				m_pDoc->setLineWidth((m_pDoc->getLineWidth() + 1) / 2);
			}
			else
			{
				m_pDoc->setSize((m_pDoc->getSize() + 1) / 2);
				m_pDoc->setLineWidth((m_pDoc->getLineWidth() + 1) / 2);
			}

			multiPaint(spacing);
		}
		else
		{
			multiPaint(spacing, true);
		}

	}

	


	m_pDoc->setSize(size);
	m_pDoc->setLineWidth(width);

	
	//std::cout << isAnEvent << std::endl;


	//	m_nStartRow = startrow;
	//	m_nEndRow = startrow + drawHeight;
	//	m_nStartCol = scrollpos.x;
	//	m_nEndCol = m_nStartCol + drawWidth;	

}