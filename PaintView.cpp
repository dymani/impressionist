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
#include "LineOverlay.h"


#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN	4
#define RIGHT_MOUSE_DRAG	5
#define RIGHT_MOUSE_UP		6
#define MOUSE_MOVE_			7


#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#define max(a, b)	( ( (a)>(b) ) ? (a) : (b) )


static int		eventToDo;
static int		isAnEvent=0;
static IPoint	coord;

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

	if(!valid())
	{

		glClearColor(0.7f, 0.7f, 0.7f, 1.0);

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		ortho();

		glClear( GL_COLOR_BUFFER_BIT );

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	m_scrollpos.x = 0;
	m_scrollpos.y	= 0;

	m_nWindowWidth	= w();
	m_nWindowHeight	= h();

	int drawWidth, drawHeight;
	drawWidth = min( m_nWindowWidth, m_pDoc->m_nPaintWidth );
	drawHeight = min( m_nWindowHeight, m_pDoc->m_nPaintHeight );

	m_startrow = m_pDoc->m_nPaintHeight - (m_scrollpos.y + drawHeight);
	if ( m_startrow < 0 ) m_startrow = 0;

	m_pPaintingBitstart = m_pDoc->getPainting() + 
		3 * ((m_pDoc->m_nPaintWidth * m_startrow) + m_scrollpos.x);

	m_nDrawWidth	= drawWidth;
	m_nDrawHeight	= drawHeight;

	m_nStartRow		= m_startrow;
	m_nEndRow		= m_startrow + drawHeight;
	m_nStartCol		= m_scrollpos.x;
	m_nEndCol		= m_nStartCol + drawWidth;

	if ( m_pDoc->getPainting() && !isAnEvent)
	{
		RestoreContent();
		m_pDoc->updateViewImage();
		drawView();
	}
	bool save = false;
	if ( m_pDoc->getPainting() && isAnEvent)
	{

		// Clear it after processing.
		isAnEvent	= 0;	

		IPoint source( coord.x + m_nStartCol, m_nEndRow - coord.y );
		IPoint target( coord.x, m_nWindowHeight - coord.y );
		
		// This is the event handler
		switch (eventToDo) 
		{
		case LEFT_MOUSE_DOWN:
			drawPainting();
			saveUndo();
			m_pDoc->m_pCurrentBrush->BrushBegin( source, target );
			m_pDoc->m_pUI->m_marker->update(source);
			m_pDoc->m_pUI->m_origView->refresh();
			SaveCurrentContent();
			m_pDoc->updateViewImage();
			drawView();
			break;
		case LEFT_MOUSE_DRAG:
			drawPainting();
			m_pDoc->m_pCurrentBrush->BrushMove( source, target );
			m_pDoc->m_pUI->m_marker->update(source);
			m_pDoc->m_pUI->m_origView->refresh();
			SaveCurrentContent();
			m_pDoc->updateViewImage();
			drawView();
			break;
		case LEFT_MOUSE_UP:
			m_pDoc->m_pCurrentBrush->BrushEnd( source, target );
			m_pDoc->m_pUI->m_marker->update(source);
			m_pDoc->m_pUI->m_origView->refresh();
			drawPainting();
			SaveCurrentContent();
			RestoreContent();
			save = true;
			break;
		case RIGHT_MOUSE_DOWN:
			m_pDoc->updateViewImage();
			drawView();
			m_pDoc->m_pUI->m_lineOverlay->setStart(target);
			m_pDoc->m_pUI->m_marker->update(source);
			m_pDoc->m_pUI->m_origView->refresh();
			break;
		case RIGHT_MOUSE_DRAG:
			drawPainting();
			m_pDoc->updateViewImage();
			drawView();
			m_pDoc->m_pUI->m_lineOverlay->draw(target);
			m_pDoc->m_pUI->m_marker->update(source);
			m_pDoc->m_pUI->m_origView->refresh();
			break;
		case RIGHT_MOUSE_UP:
			drawPainting();
			m_pDoc->updateViewImage();
			drawView();
			m_pDoc->m_pUI->m_lineOverlay->release(target);
			m_pDoc->m_pUI->m_marker->update(source);
			m_pDoc->m_pUI->m_origView->refresh();
			break;
		case MOUSE_MOVE_:
			break;
		default:
			printf("Unknown event!!\n");		
			break;
		}
	}

	
	glBegin(GL_QUADS);
		glColor3ub(178, 178, 178);
		glVertex2f(0, 0);
		glVertex2f((float)m_nWindowWidth, 0);
		glVertex2f((float)m_nWindowWidth, (float)m_nWindowHeight - m_nDrawHeight);
		glVertex2f(0, (float)m_nWindowHeight - m_nDrawHeight);
	glEnd();
	glBegin(GL_QUADS);
		glColor3ub(178, 178, 178);
		glVertex2f((float)m_nDrawWidth, (float)m_nWindowHeight - m_nDrawHeight);
		glVertex2f((float)m_nWindowWidth, (float)m_nWindowHeight - m_nDrawHeight);
		glVertex2f((float)m_nWindowWidth, (float)m_nWindowHeight);
		glVertex2f((float)m_nDrawWidth, (float)m_nWindowHeight);
	glEnd();

	glFlush();

	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
	#endif // !MESA

	if (save) {
		SaveCurrentContent();
		glFlush();
		m_pDoc->updateViewImage();
		drawView();
	}

}

void PaintView::drawView() {
	m_pPaintingBitstart = m_pDoc->getViewImage() +
		3 * ((m_pDoc->m_nPaintWidth * m_startrow) + m_scrollpos.x);
	RestoreContent();
	m_pPaintingBitstart = m_pDoc->getPainting() +
		3 * ((m_pDoc->m_nPaintWidth * m_startrow) + m_scrollpos.x);
}

void PaintView::drawPainting() {
	m_pPaintingBitstart = m_pDoc->getPainting()+
		3 * ((m_pDoc->m_nPaintWidth * m_startrow) + m_scrollpos.x);
	RestoreContent();
	glFlush();
}

int PaintView::handle(int event)
{

	switch (event)
		{
		case FL_ENTER:
			break;
		case FL_PUSH:
			coord.x = Fl::event_x();
			coord.y = Fl::event_y();
			if (Fl::event_button() > 1)
				eventToDo = RIGHT_MOUSE_DOWN;
			else
				eventToDo = LEFT_MOUSE_DOWN;
			isAnEvent = 1;
			redraw();
			break;
		case FL_DRAG:
			coord.x = Fl::event_x();
			coord.y = Fl::event_y();
			if (Fl::event_button() > 1)
				eventToDo = RIGHT_MOUSE_DRAG;
			else
				eventToDo = LEFT_MOUSE_DRAG;
			isAnEvent = 1;
			redraw();
			break;
		case FL_RELEASE:
			coord.x = Fl::event_x();
			coord.y = Fl::event_y();
			if (Fl::event_button() > 1)
				eventToDo = RIGHT_MOUSE_UP;
			else
				eventToDo = LEFT_MOUSE_UP;
			isAnEvent = 1;
			redraw();
			break;
		case FL_MOVE:
			coord.x = Fl::event_x();
			coord.y = Fl::event_y();
			{
				int drawWidth, drawHeight;
				drawWidth = min(m_nWindowWidth, m_pDoc->m_nPaintWidth);
				drawHeight = min(m_nWindowHeight, m_pDoc->m_nPaintHeight);

				int m_startrow = m_pDoc->m_nPaintHeight - (drawHeight);
				if (m_startrow < 0) m_startrow = 0;

				IPoint source(coord.x, m_startrow + drawHeight - coord.y);
				m_pDoc->m_pUI->m_marker->update(source);
				m_pDoc->m_pUI->m_origView->refresh();
			}			
			break;
		default:
			return 0;
			break;

		}


	return 1;
}

void PaintView::saveUndo()
{
	// get the bitmap char* size before releasing previous data
	int size = m_pDoc->m_nPaintHeight * m_pDoc->m_nPaintWidth * 3;	

	// release previous data
	delete[] m_pDoc->m_ucPaintingUndo;							

	// save the bitmap
	m_pDoc->m_ucPaintingUndo = new unsigned char[size];
	memcpy(m_pDoc->m_ucPaintingUndo, m_pDoc->getPainting(), size);
}

void PaintView::refresh()
{
	m_pDoc->updateViewImage();
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
	glReadBuffer(GL_BACK);

	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	
	glReadPixels( 0, 
				  m_nWindowHeight - m_nDrawHeight, 
				  m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintingBitstart );
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
				  m_pPaintingBitstart);

//	glDrawBuffer(GL_FRONT);
}
