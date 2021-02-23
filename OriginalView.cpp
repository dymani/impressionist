//
// originalview.cpp
//
// The code maintaining the original view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "originalview.h"

#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#endif

OriginalView::OriginalView(int			x, 
						   int			y, 
						   int			w, 
						   int			h, 
						   const char*	l)
							: Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;
	m_view = ORIGINAL;

}

void OriginalView::setView(View v) {
	m_view = v;
	refresh();
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

	if ( m_pDoc->getBitmap() ) 
	{
		// note that both OpenGL pixel storage and the Windows BMP format
		// store pixels left-to-right, BOTTOM-to-TOP!!  thus all the fiddling
		// around with startrow.

		m_nWindowWidth=w();
		m_nWindowHeight=h();

		int drawWidth, drawHeight;
		GLvoid* bitstart = nullptr;

		// we are not using a scrollable window, so ignore it
		Point scrollpos;	// = GetScrollPosition();
		scrollpos.x=scrollpos.y=0;

		drawWidth	= min( m_nWindowWidth, m_pDoc->m_nWidth );
		drawHeight	= min( m_nWindowHeight, m_pDoc->m_nHeight );

		int	startrow	= m_pDoc->m_nHeight - (scrollpos.y + drawHeight);
		if ( startrow < 0 ) 
			startrow = 0;

		switch (m_view)
		{
			case OriginalView::ANOTHER:
				if (m_pDoc->getAnotherImage())
					bitstart = m_pDoc->getAnotherImage() + 3 * ((m_pDoc->m_nWidth * startrow) + scrollpos.x);
				break;
			case OriginalView::EDGE:
				if (m_pDoc->getEdgeImage())
					bitstart = m_pDoc->getEdgeImage() + 3 * ((m_pDoc->m_nWidth * startrow) + scrollpos.x);
				break;
			case OriginalView::ORIGINAL:
			default:
				bitstart = m_pDoc->getBitmap() + 3 * ((m_pDoc->m_nWidth * startrow) + scrollpos.x);
				break;
		}

		if (bitstart) {
			// just copy image to GLwindow conceptually
			glRasterPos2i(0, m_nWindowHeight - drawHeight);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glPixelStorei(GL_UNPACK_ROW_LENGTH, m_pDoc->m_nWidth);
			glDrawBuffer(GL_BACK);
			glDrawPixels(drawWidth, drawHeight, GL_RGB, GL_UNSIGNED_BYTE, bitstart);
		}
		else {
			glBegin(GL_QUADS);
				glColor3ub(0, 0, 0);
				glVertex2f(0, (float)m_nWindowHeight - drawHeight);
				glVertex2f((float)m_pDoc->m_nWidth, (float)m_nWindowHeight - drawHeight);
				glVertex2f((float)m_pDoc->m_nWidth, (float)m_nWindowHeight);
				glVertex2f(0, (float)m_nWindowHeight);
			glEnd();
		}
		m_pDoc->m_pUI->m_marker->draw(m_nWindowHeight - drawHeight);

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

