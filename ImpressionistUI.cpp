//
// impressionistUI.h
//
// The user interface part for the program.
//


#include <FL/fl_ask.h>

#include <math.h>
#include <string>

#include "impressionistUI.h"
#include "impressionistDoc.h"

/*
//------------------------------ Widget Examples -------------------------------------------------
Here is some example code for all of the widgets that you may need to add to the 
project.  You can copy and paste these into your code and then change them to 
make them look how you want.  Descriptions for all of the widgets here can be found 
in links on the fltk help session page.

//---------Window/Dialog and Menubar-----------------------------------
	
	//----To install a window--------------------------
	Fl_Window* myWindow = new Fl_Window(600, 300, "MyWindow");
		myWindow->user_data((void*)(this));	// record self to be used by static callback functions
		
		// install menu bar
		myMenubar = new Fl_Menu_Bar(0, 0, 600, 25);
		Fl_Menu_Item ImpressionistUI::myMenuItems[] = {
			{ "&File",		0, 0, 0, FL_SUBMENU },
				{ "&Load...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load },
				{ "&Save...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save }.
				{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
				{ 0 },
			{ "&Edit",		0, 0, 0, FL_SUBMENU },
				{ "&Copy",FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_copy, (void *)COPY },
				{ "&Cut",	FL_ALT + 'x', (Fl_Callback *)ImpressionistUI::cb_cut, (void *)CUT },
				{ "&Paste",	FL_ALT + 'v', (Fl_Callback *)ImpressionistUI::cb_paste, (void *)PASTE },
				{ 0 },
			{ "&Help",		0, 0, 0, FL_SUBMENU },
				{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
				{ 0 },
			{ 0 }
		};
		myMenubar->menu(myMenuItems);
    m_mainWindow->end();

	//----The window callback--------------------------
	// One of the callbacks
	void ImpressionistUI::cb_load(Fl_Menu_* o, void* v) 
	{	
		ImpressionistDoc *pDoc=whoami(o)->getDocument();

		char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
		if (newfile != NULL) {
			pDoc->loadImage(newfile);
		}
	}


//------------Slider---------------------------------------

	//----To install a slider--------------------------
	Fl_Value_Slider * mySlider = new Fl_Value_Slider(10, 80, 300, 20, "My Value");
	mySlider->user_data((void*)(this));	// record self to be used by static callback functions
	mySlider->type(FL_HOR_NICE_SLIDER);
    mySlider->labelfont(FL_COURIER);
    mySlider->labelsize(12);
	mySlider->minimum(1);
	mySlider->maximum(40);
	mySlider->step(1);
	mySlider->value(m_nMyValue);
	mySlider->align(FL_ALIGN_RIGHT);
	mySlider->callback(cb_MyValueSlides);

	//----The slider callback--------------------------
	void ImpressionistUI::cb_MyValueSlides(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyValue=int( ((Fl_Slider *)o)->value() ) ;
	}
	

//------------Choice---------------------------------------
	
	//----To install a choice--------------------------
	Fl_Choice * myChoice = new Fl_Choice(50,10,150,25,"&myChoiceLabel");
	myChoice->user_data((void*)(this));	 // record self to be used by static callback functions
	Fl_Menu_Item ImpressionistUI::myChoiceMenu[3+1] = {
	  {"one",FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)ONE},
	  {"two",FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)TWO},
	  {"three",FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)THREE},
	  {0}
	};
	myChoice->menu(myChoiceMenu);
	myChoice->callback(cb_myChoice);
	
	//-----The choice callback-------------------------
	void ImpressionistUI::cb_myChoice(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
		ImpressionistDoc* pDoc=pUI->getDocument();

		int type=(int)v;

		pDoc->setMyType(type);
	}


//------------Button---------------------------------------

	//---To install a button---------------------------
	Fl_Button* myButton = new Fl_Button(330,220,50,20,"&myButtonLabel");
	myButton->user_data((void*)(this));   // record self to be used by static callback functions
	myButton->callback(cb_myButton);

	//---The button callback---------------------------
	void ImpressionistUI::cb_myButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI*)(o->user_data()));
		ImpressionistDoc* pDoc = pUI->getDocument();
		pDoc->startPainting();
	}


//---------Light Button------------------------------------
	
	//---To install a light button---------------------
	Fl_Light_Button* myLightButton = new Fl_Light_Button(240,10,150,25,"&myLightButtonLabel");
	myLightButton->user_data((void*)(this));   // record self to be used by static callback functions
	myLightButton->callback(cb_myLightButton);

	//---The light button callback---------------------
	void ImpressionistUI::cb_myLightButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI *pUI=((ImpressionistUI*)(o->user_data()));

		if (pUI->myBool==TRUE) pUI->myBool=FALSE;
		else pUI->myBool=TRUE;
	}

//----------Int Input--------------------------------------

    //---To install an int input-----------------------
	Fl_Int_Input* myInput = new Fl_Int_Input(200, 50, 5, 5, "&My Input");
	myInput->user_data((void*)(this));   // record self to be used by static callback functions
	myInput->callback(cb_myInput);

	//---The int input callback------------------------
	void ImpressionistUI::cb_myInput(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyInputValue=int( ((Fl_Int_Input *)o)->value() );
	}

//------------------------------------------------------------------------------------------------
*/

//------------------------------------- Help Functions --------------------------------------------

//------------------------------------------------------------
// This returns the UI, given the menu item.  It provides a
// link from the menu items to the UI
//------------------------------------------------------------
ImpressionistUI* ImpressionistUI::whoami(Fl_Menu_* o)	
{
	return ( (ImpressionistUI*)(o->parent()->user_data()) );
}


//--------------------------------- Callback Functions --------------------------------------------

//------------------------------------------------------------------
// Brings up a file chooser and then loads the chosen image
// This is called by the UI when the load image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_load_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
	if (newfile != NULL) {
		pDoc->loadImage(newfile);
	}
}


//------------------------------------------------------------------
// Brings up a file chooser and then saves the painted image
// This is called by the UI when the save image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_save_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Save File?", "*.bmp", "save.bmp" );
	if (newfile != NULL) {
		pDoc->saveImage(newfile);
	}
}

//-------------------------------------------------------------
// Brings up the paint dialog
// This is called by the UI when the brushes menu item
// is chosen
//-------------------------------------------------------------
void ImpressionistUI::cb_brushes(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_brushDialog->show();
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc=whoami(o)->getDocument();

	pDoc->clearCanvas();
}

void ImpressionistUI::cb_swap_contents(Fl_Menu_* o, void* v) {
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	pDoc->swapContents();
}

void ImpressionistUI::cb_change_image(Fl_Menu_* o, void* v) {
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL) {
		pDoc->changeImage(newfile);
	}
}

//------------------------------------------------------------
// Causes the Impressionist program to exit
// Called by the UI when the quit menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_exit(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_mainWindow->hide();
	whoami(o)->m_brushDialog->hide();

}



//-----------------------------------------------------------
// Brings up an about dialog box
// Called by the UI when the about menu item is chosen
//-----------------------------------------------------------
void ImpressionistUI::cb_about(Fl_Menu_* o, void* v) 
{
	fl_message("Impressionist FLTK version for CS341, Spring 2002");
}

void ImpressionistUI::cb_colors(Fl_Menu_* o, void* v)
{
	whoami(o)->m_colorDialog->show();
}

void ImpressionistUI::cb_filters(Fl_Menu_* o, void* v) {
	whoami(o)->m_filterDialog->show();
}


//------- UI should keep track of the current for all the controls for answering the query from Doc ---------
//-------------------------------------------------------------
// Sets the type of brush to use to the one chosen in the brush 
// choice.  
// Called by the UI when a brush is chosen in the brush choice
//-------------------------------------------------------------
void ImpressionistUI::cb_brushChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc=pUI->getDocument();

	int type=(int)v;


	pDoc->setBrushType(type);
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->clearCanvas();
}



void ImpressionistUI::cb_strokeDirectionChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	int type = (int)v;

	pDoc->setStrokeDirectionType(type);
}

//-----------------------------------------------------------
// Updates the brush size to use from the value of the size
// slider
// Called by the UI when the size slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_sizeSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nSize=int( ((Fl_Slider *)o)->value() ) ;
}

//-----------------------------------------------------------
// Updates the line width to use from the value of the line 
// width slider
// Called by the UI when the line width slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_widthSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_brushWidth = int(((Fl_Slider*)o)->value());
}

//-----------------------------------------------------------
// Updates the line angle to use from the value of the line 
// angle slider
// Called by the UI when the line angle slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_angleSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_angle = int(((Fl_Slider*)o)->value());
}

//-----------------------------------------------------------
// Updates the alpha value to use from the value of the alpha 
// value slider
// Called by the UI when the alpha slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_alphaSlides(Fl_Widget* o, void* v)
{
	int convert = (int)(((Fl_Slider*)o)->value() * 255);
	((ImpressionistUI*)(o->user_data()))->m_alpha = int(convert);
}

//-----------------------------------------------------------
// Updates the color choice
//-----------------------------------------------------------
void ImpressionistUI::cb_color_chooser(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_redVal = ((Fl_Color_Chooser*)o)->r();
	((ImpressionistUI*)(o->user_data()))->m_greenVal = ((Fl_Color_Chooser*)o)->g();
	((ImpressionistUI*)(o->user_data()))->m_blueVal = ((Fl_Color_Chooser*)o)->b();
}

//---------------------------------- filter dialog functions --------------------------------------

void ImpressionistUI::cb_filter_type_choice(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();
	pUI->m_filterType = (int)v;
	if (pUI->m_filterType == ConvolutionManager::FILTER_CUSTOM) {
		pUI->m_filterInputTable->rows(pUI->m_filterHeight);
		pUI->m_filterInputTable->cols(pUI->m_filterWidth);
		pUI->m_filterInputTable->activate();
		pUI->m_filterWidthInput->activate();
		pUI->m_filterHeightInput->activate();
	}
	else {
		pUI->m_filterInputTable->rows(0);
		pUI->m_filterInputTable->cols(0);
		pUI->m_filterInputTable->deactivate();
		pUI->m_filterWidthInput->deactivate();
		pUI->m_filterHeightInput->deactivate();
	}
}

void ImpressionistUI::cb_filter_source_choice(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	pUI->m_filterSource = (int)v;
}

void ImpressionistUI::cb_filter_width_input(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	pUI->m_filterWidth = atoi(((Fl_Int_Input*)o)->value());
	pUI->m_filterInputTable->rows(pUI->m_filterHeight);
	pUI->m_filterInputTable->cols(pUI->m_filterWidth);
}

void ImpressionistUI::cb_filter_height_input(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	pUI->m_filterHeight = atoi(((Fl_Int_Input*)o)->value());
	pUI->m_filterInputTable->rows(pUI->m_filterHeight);
	pUI->m_filterInputTable->cols(pUI->m_filterWidth);
}

void ImpressionistUI::cb_filter_normalize_light_button(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	pUI->m_isFilterNormalized = !pUI->m_isFilterNormalized;
}

void ImpressionistUI::cb_filter_apply_button(Fl_Widget* o, void* v) {
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();
	if (pUI->m_filterType == ConvolutionManager::FILTER_CUSTOM) {
		if(pUI->m_filterWidth % 2 == 0 || pUI->m_filterHeight % 2 == 0)
			fl_alert("Dimension of kernel must be odd!");
		else
			pDoc->applyCustomFilter(*pUI->m_filterInputTable->m_values, pUI->m_filterWidth, pUI->m_filterHeight, pUI->m_filterSource, pUI->m_isFilterNormalized);
	}
	else {
		pDoc->applyFilter(pUI->m_filterType, pUI->m_filterSource, pUI->m_isFilterNormalized);
	}
}


//---------------------------------- per instance functions --------------------------------------

//------------------------------------------------
// Return the ImpressionistDoc used
//------------------------------------------------
ImpressionistDoc* ImpressionistUI::getDocument()
{
	return m_pDoc;
}

//------------------------------------------------
// Draw the main window
//------------------------------------------------
void ImpressionistUI::show() {
	m_mainWindow->show();
	m_paintView->show();
	m_origView->show();
}

//------------------------------------------------
// Change the paint and original window sizes to 
// w by h
//------------------------------------------------
void ImpressionistUI::resize_windows(int w, int h) {
	m_paintView->size(w,h);
	m_origView->size(w,h);
}

//------------------------------------------------ 
// Set the ImpressionistDoc used by the UI to 
// communicate with the brushes 
//------------------------------------------------
void ImpressionistUI::setDocument(ImpressionistDoc* doc)
{
	m_pDoc = doc;

	m_origView->m_pDoc = doc;
	m_paintView->m_pDoc = doc;

	m_lineOverlay->m_pDoc = doc;
	m_marker->m_pDoc = doc;
}

//------------------------------------------------
// Return the brush size
//------------------------------------------------
int ImpressionistUI::getSize()
{
	return m_nSize;
}

//-------------------------------------------------
// Set the brush size
//-------------------------------------------------
void ImpressionistUI::setSize( int size )
{
	m_nSize=size;

	if (size<=40) 
		m_BrushSizeSlider->value(m_nSize);
}

//------------------------------------------------
// Return the line width
//------------------------------------------------
int ImpressionistUI::getWidth()
{
	return m_brushWidth;
}

//-------------------------------------------------
// Set the line width
//-------------------------------------------------
void ImpressionistUI::setWidth(int width)
{
	if (width < 0) width = 0;
	if (width > 40) width = 40;
	m_brushWidth = width;
	m_LineWidthSlider->value(m_brushWidth);
}

//------------------------------------------------
// Return the line angle
//------------------------------------------------
int ImpressionistUI::getAngle()
{
	return m_angle;
}

//-------------------------------------------------
// Set the line angle
//-------------------------------------------------
void ImpressionistUI::setAngle(int angle)
{
	while (angle >= 359) angle -= 360;
	while (angle < 0) angle += 360;

	m_angle = angle;
	m_LineAngleSlider->value(m_angle);
}

//------------------------------------------------
// Return the alpha value
//------------------------------------------------
int ImpressionistUI::getAlpha()
{
	return m_alpha;
}

//-------------------------------------------------
// Set the alpha value
//-------------------------------------------------
void ImpressionistUI::setAlpha(int alpha)
{
	m_alpha = alpha;
}

//-------------------------------------------------
// Get the red value
//-------------------------------------------------
double ImpressionistUI::getRedVal()
{
	return m_redVal;
}

//-------------------------------------------------
// Set the red value
//-------------------------------------------------
void ImpressionistUI::setRedVal(double R)
{
	m_redVal = R;
}

//-------------------------------------------------
// Get the green value
//-------------------------------------------------
double ImpressionistUI::getGreenVal()
{
	return m_greenVal;
}

//-------------------------------------------------
// Set the green value
//-------------------------------------------------
void ImpressionistUI::setGreenVal(double G)
{
	m_greenVal = G;
}

//-------------------------------------------------
// Get the blue value
//-------------------------------------------------
double ImpressionistUI::getBlueVal()
{
	return m_blueVal;
}

//-------------------------------------------------
// Set the blue value
//-------------------------------------------------
void ImpressionistUI::setBlueVal(double B)
{
	m_blueVal = B;
}

// Main menu definition
Fl_Menu_Item ImpressionistUI::menuitems[] = {
	{ "&File",		0, 0, 0, FL_SUBMENU },
		{ "&Load Image...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load_image },
		{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save_image },
		{ "&Brushes...",	FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_brushes },
		{ "&Clear Canvas", FL_ALT + 'c', (Fl_Callback*)ImpressionistUI::cb_clear_canvas , 0, FL_MENU_DIVIDER },
		{ "&Colors...", FL_ALT + 'k', (Fl_Callback *)ImpressionistUI::cb_colors, 0, FL_MENU_DIVIDER },
		{ "S&wap contents", FL_ALT + 'w', (Fl_Callback *)ImpressionistUI::cb_swap_contents,},
		{ "Change &mural image", FL_ALT + 'm', (Fl_Callback*)ImpressionistUI::cb_change_image, 0, FL_MENU_DIVIDER },
		{ "&Filters...", FL_ALT + 'f', (Fl_Callback*)ImpressionistUI::cb_filters, 0, FL_MENU_DIVIDER },

		{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
		{ 0 },

	{ "&Help",		0, 0, 0, FL_SUBMENU },
		{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
		{ 0 },

	{ 0 }
};

// Brush choice menu definition
Fl_Menu_Item ImpressionistUI::brushTypeMenu[NUM_BRUSH_TYPE+1] = {
  {"Points",			FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_POINTS},
  {"Lines",				FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_LINES},
  {"Circles",			FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_CIRCLES},
  {"Scattered Points",	FL_ALT+'q', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_POINTS},
  {"Scattered Lines",	FL_ALT+'m', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_LINES},
  {"Scattered Circles",	FL_ALT+'d', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_CIRCLES},
  {"Random Polygon",	FL_ALT+'r', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_RANDOM_POLYGON},
  {"Sharpen",	FL_ALT + 's', (Fl_Callback*)ImpressionistUI::cb_brushChoice, (void*)BRUSH_SHARPEN},
  {"Blur",	FL_ALT + 'u', (Fl_Callback*)ImpressionistUI::cb_brushChoice, (void*)BRUSH_BLUR},
  {0}
};

Fl_Menu_Item ImpressionistUI::strokeDirectionTypeMenu[4] = {
  {"Slider/Right Mouse", FL_ALT + 's', (Fl_Callback*)ImpressionistUI::cb_strokeDirectionChoice, (void*)0},
  {"Gradient", FL_ALT + 'g', (Fl_Callback*)ImpressionistUI::cb_strokeDirectionChoice, (void*)1},
  {"Brush Direction", FL_ALT + 'b', (Fl_Callback*)ImpressionistUI::cb_strokeDirectionChoice, (void*)2},
  {0}
};

Fl_Menu_Item ImpressionistUI::filterTypeMenu[ConvolutionManager::NUM_FILTER_CHOICE + 1] = {
  {"Gaussian 3x3", FL_ALT + 'g', (Fl_Callback*)ImpressionistUI::cb_filter_type_choice, (void*)ConvolutionManager::FILTER_GAUSSIAN_3},
  {"Gaussian 5x5", FL_ALT + 'h', (Fl_Callback*)ImpressionistUI::cb_filter_type_choice, (void*)ConvolutionManager::FILTER_GAUSSIAN_3},
  {"Sobel X", FL_ALT + 'x', (Fl_Callback*)ImpressionistUI::cb_filter_type_choice, (void*)ConvolutionManager::FILTER_SOBEL_X},
  {"Sobel Y", FL_ALT + 'y', (Fl_Callback*)ImpressionistUI::cb_filter_type_choice, (void*)ConvolutionManager::FILTER_SOBEL_Y},
  {"Custom...", FL_ALT + 'c', (Fl_Callback*)ImpressionistUI::cb_filter_type_choice, (void*)ConvolutionManager::FILTER_CUSTOM},
  {0}
};

Fl_Menu_Item ImpressionistUI::filterSourceMenu[2 + 1] = {
  {"Original image", FL_ALT + 'o', (Fl_Callback*)ImpressionistUI::cb_filter_source_choice, (void*)0},
  {"Canvas", FL_ALT + 'v', (Fl_Callback*)ImpressionistUI::cb_filter_source_choice, (void*)1},
  {0}
};


//----------------------------------------------------
// Constructor.  Creates all of the widgets.
// Add new widgets here
//----------------------------------------------------
ImpressionistUI::ImpressionistUI() {
	// Create the main window
	m_mainWindow = new Fl_Window(600, 300, "Impressionist");
		m_mainWindow->user_data((void*)(this));	// record self to be used by static callback functions
		// install menu bar
		m_menubar = new Fl_Menu_Bar(0, 0, 600, 25);
		m_menubar->menu(menuitems);

		// Create a group that will hold two sub windows inside the main
		// window
		Fl_Group* group = new Fl_Group(0, 25, 600, 275);

			// install paint view window
			m_paintView = new PaintView(300, 25, 300, 275, "This is the paint view");//0jon
			m_paintView->box(FL_DOWN_FRAME);

			// install original view window
			m_origView = new OriginalView(0, 25, 300, 275, "This is the orig view");//300jon
			m_origView->box(FL_DOWN_FRAME);
			m_origView->deactivate();

		group->end();
		Fl_Group::current()->resizable(group);
    m_mainWindow->end();

	// init values

	m_nSize = 10;
	m_brushWidth = 1;
	m_angle = 0;
	m_alpha = 255;
	m_redVal = 1.0;
	m_greenVal = 1.0;
	m_blueVal = 1.0;

	// brush dialog definition
	m_brushDialog = new Fl_Window(400, 325, "Brush Dialog");
		// Add a brush type choice to the dialog
		m_BrushTypeChoice = new Fl_Choice(50,10,150,25,"&Brush");
		m_BrushTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushTypeChoice->menu(brushTypeMenu);
		m_BrushTypeChoice->callback(cb_brushChoice);

		m_ClearCanvasButton = new Fl_Button(240,10,150,25,"&Clear Canvas");
		m_ClearCanvasButton->user_data((void*)(this));
		m_ClearCanvasButton->callback(cb_clear_canvas_button);

		m_strokeDirectionChoice = new Fl_Choice(117, 45, 150, 25, "&Stroke Direction");
		m_strokeDirectionChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_strokeDirectionChoice->menu(strokeDirectionTypeMenu);
		m_strokeDirectionChoice->callback(cb_strokeDirectionChoice);
		m_strokeDirectionChoice->deactivate();


		// Add brush size slider to the dialog 
		m_BrushSizeSlider = new Fl_Value_Slider(10, 80, 300, 20, "Size");
		m_BrushSizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushSizeSlider->type(FL_HOR_NICE_SLIDER);
        m_BrushSizeSlider->labelfont(FL_COURIER);
        m_BrushSizeSlider->labelsize(12);
		m_BrushSizeSlider->minimum(1);
		m_BrushSizeSlider->maximum(40);
		m_BrushSizeSlider->step(1);
		m_BrushSizeSlider->value(m_nSize);
		m_BrushSizeSlider->align(FL_ALIGN_RIGHT);
		m_BrushSizeSlider->callback(cb_sizeSlides);

		// Add line width slider for line brush to the dialog
		m_LineWidthSlider = new Fl_Value_Slider(10, 110, 300, 20, "Line Width");
		m_LineWidthSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_LineWidthSlider->type(FL_HOR_NICE_SLIDER);
		m_LineWidthSlider->labelfont(FL_COURIER);
		m_LineWidthSlider->labelsize(12);
		m_LineWidthSlider->minimum(1);
		m_LineWidthSlider->maximum(40);
		m_LineWidthSlider->step(1);
		m_LineWidthSlider->value(m_brushWidth);
		m_LineWidthSlider->align(FL_ALIGN_RIGHT);
		m_LineWidthSlider->callback(cb_widthSlides);
		m_LineWidthSlider->deactivate();

		// Add line angle slider for line brush to the dialog
		m_LineAngleSlider = new Fl_Value_Slider(10, 140, 300, 20, "Line Angle");
		m_LineAngleSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_LineAngleSlider->type(FL_HOR_NICE_SLIDER);
		m_LineAngleSlider->labelfont(FL_COURIER);
		m_LineAngleSlider->labelsize(12);
		m_LineAngleSlider->minimum(0);
		m_LineAngleSlider->maximum(359);
		m_LineAngleSlider->step(1);
		m_LineAngleSlider->value(m_angle);
		m_LineAngleSlider->align(FL_ALIGN_RIGHT);
		m_LineAngleSlider->callback(cb_angleSlides);
		m_LineAngleSlider->deactivate();

		// Add alpha slider to the dialog
		m_AlphaSlider = new Fl_Value_Slider(10, 170, 300, 20, "Alpha");
		m_AlphaSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_AlphaSlider->type(FL_HOR_NICE_SLIDER);
		m_AlphaSlider->labelfont(FL_COURIER);
		m_AlphaSlider->labelsize(12);
		m_AlphaSlider->minimum(0.00);
		m_AlphaSlider->maximum(1.00);
		m_AlphaSlider->step(0.01);
		m_AlphaSlider->value(1);
		m_AlphaSlider->align(FL_ALIGN_RIGHT);
		m_AlphaSlider->callback(cb_alphaSlides);

    m_brushDialog->end();	

	// color dialog definition
	m_colorDialog = new Fl_Window(300, 225, "Color Select");
		m_colorChooser = new Fl_Color_Chooser(50, 20, 150, 150, "Color Blending");
		m_colorChooser->user_data((void*)(this));	// record self to be used by static callback functions
		m_colorChooser->rgb(m_redVal, m_greenVal, m_blueVal);
		m_colorChooser->callback(cb_color_chooser);
	m_colorDialog->end();

	// init values

	m_filterType = 0;
	m_filterSource = 0;
	m_filterWidth = 3;
	m_filterHeight = 3;
	m_isFilterNormalized = true;

	// filter dialog definition
	m_filterDialog = new Fl_Double_Window(400, 400, "Filter Dialog");
		m_filterSourceChoice = new Fl_Choice(50, 10, 150, 25, "&Source");
		m_filterSourceChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_filterSourceChoice->menu(filterSourceMenu);
		m_filterSourceChoice->callback(cb_filter_source_choice);
			
		m_filterTypeChoice = new Fl_Choice(50, 45, 150, 25, "&Filter");
		m_filterTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_filterTypeChoice->menu(filterTypeMenu);
		m_filterTypeChoice->callback(cb_filter_type_choice);

		m_filterWidthInput = new Fl_Int_Input(100, 80, 80, 25, "&Width (max 9)");
		m_filterWidthInput->user_data((void*)(this));   // record self to be used by static callback functions
		m_filterWidthInput->callback(cb_filter_width_input);
		m_filterWidthInput->value(std::to_string(m_filterWidth).c_str());
		m_filterWidthInput->deactivate();

		m_filterHeightInput = new Fl_Int_Input(300, 80, 80, 25, "&Height (max 9)");
		m_filterHeightInput->user_data((void*)(this));   // record self to be used by static callback functions
		m_filterHeightInput->callback(cb_filter_height_input);
		m_filterHeightInput->value(std::to_string(m_filterHeight).c_str());
		m_filterHeightInput->deactivate();

		m_filterInputTable = new InputTable(50, 140, 300, 170, "Kernel");
#if FLTK_ABI_VERSION >= 10303
		m_filterInputTable->tab_cell_nav(1);		// enable tab navigation of table cells (instead of fltk widgets)
#endif
		m_filterInputTable->tooltip("Use keyboard to navigate cells:\n"
			"Arrow keys or Tab/Shift-Tab");
		m_filterInputTable->row_header(1);
		m_filterInputTable->row_header_width(25);
		m_filterInputTable->row_resize(0);
		m_filterInputTable->rows(0);
		m_filterInputTable->row_height_all(25);
		m_filterInputTable->col_header(1);
		m_filterInputTable->col_header_height(25);
		m_filterInputTable->col_resize(0);
		m_filterInputTable->cols(0);
		m_filterInputTable->col_width_all(50);
		m_filterInputTable->deactivate();
		m_filterInputTable->m_values[1][1] = 1;

		m_filterNormalizeLightButton = new Fl_Light_Button(50, 320, 150, 25, "&Normalized");
		m_filterNormalizeLightButton->user_data((void*)(this));   // record self to be used by static callback functions
		m_filterNormalizeLightButton->value(1);
		m_filterNormalizeLightButton->callback(cb_filter_normalize_light_button);

		m_filterApplyButton = new Fl_Button(220, 355, 150, 25, "&Apply");
		m_filterApplyButton->user_data((void*)(this));
		m_filterApplyButton->callback(cb_filter_apply_button);

	m_filterDialog->end();


	m_lineOverlay = new LineOverlay();
	m_marker = new Marker();

}
