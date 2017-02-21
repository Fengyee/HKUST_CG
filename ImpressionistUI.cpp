//
// impressionistUI.h
//
// The user interface part for the program.
//


#include <FL/fl_ask.h>

#include <math.h>

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
	return ((ImpressionistUI*)(o->parent()->user_data()));
}


//--------------------------------- Callback Functions --------------------------------------------

//------------------------------------------------------------------
// Brings up a file chooser and then loads the chosen image
// This is called by the UI when the load image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_load_image(Fl_Menu_* o, void* v)
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL) {
		pDoc->loadImage(newfile);
	}
}

void ImpressionistUI::cb_load_another_image(Fl_Menu_* o, void* v)
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();
	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL) {
		pDoc->loadAnotherImage(newfile);
	}
}


//------------------------------------------------------------------
// Brings up a file chooser and then saves the painted image
// This is called by the UI when the save image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_save_image(Fl_Menu_* o, void* v)
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Save File?", "*.bmp", "save.bmp");
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

void ImpressionistUI::cb_filter_kernel(Fl_Menu_* o, void* v)
{
	whoami(o)->m_filterDialog->show();
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	pDoc->clearCanvas();
}

//------------------------------------------------------------
// Causes the Impressionist program to exit
// Called by the UI when the quit menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_exit(Fl_Menu_* o, void* v)
{
	whoami(o)->m_mainWindow->hide();
	whoami(o)->m_brushDialog->hide();
	whoami(o)->m_colorSelectorDialog->hide();
}



//-----------------------------------------------------------
// Brings up an about dialog box
// Called by the UI when the about menu item is chosen
//-----------------------------------------------------------
void ImpressionistUI::cb_about(Fl_Menu_* o, void* v)
{
	fl_message("Impressionist FLTK version for CS341, Spring 2002");
}

//------- UI should keep track of the current for all the controls for answering the query from Doc ---------
//-------------------------------------------------------------
// Sets the type of brush to use to the one chosen in the brush 
// choice.  
// Called by the UI when a brush is chosen in the brush choice
//-------------------------------------------------------------
void ImpressionistUI::cb_brushChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	int type = (int)v;

	if (type == BRUSH_ALPHAMAPPED && pDoc->alphaMappedImageLoaded == false) {
		fl_alert("Please load the alpha map image first. Brush type not changed");
		return;
	}
	pDoc->setBrushType(type);
}

void ImpressionistUI::cb_StrokeDirectionChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	int type = (int)v;

	pDoc->setBrushDirection(type);
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


// Undo one previous step
void ImpressionistUI::cb_undo_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->undoPainting();
}

// Start automatic painting
void ImpressionistUI::cb_auto_paint(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->autoPainting();
}

void ImpressionistUI::cb_size_rand(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nRand = int(((Fl_Light_Button *)o)->value());
}
//-----------------------------------------------------------
// Updates the brush size to use from the value of the size
// slider
// Called by the UI when the size slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_sizeSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nSize = int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_lineWidthSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nLineWidth = int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_lineAngleSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nLineAngle = int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_alphaSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nAlpha = float(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_mosaicSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nMosasiLevel = int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_spacingSlider(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nSpacing = int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_swap_image(Fl_Menu_* o, void* v)
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();
	unsigned char* temp = pDoc->m_ucBitmap;
	pDoc->m_ucBitmap = pDoc->m_ucPainting;
	pDoc->m_ucPainting = temp;
	
	pDoc->m_pUI->m_paintView->refresh();
	pDoc->m_pUI->m_origView->refresh();
}

void ImpressionistUI::cb_color_selector(Fl_Menu_* o, void* v) {
	// ImpressionistDoc *pDoc = whoami(o)->getDocument();
	whoami(o)->m_colorSelectorDialog->show();
}

void ImpressionistUI::cb_loadAlphaMappedImage(Fl_Menu_* o, void* v)
{
	ImpressionistDoc *pDoc = whoami(o)->getDocument();
	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL) {
		pDoc->loadAlphaMappedImage(newfile);
	}
}

void ImpressionistUI::cb_edge_threshold(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nEdgeThreshold = int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_recal_edge(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->recalEdgeImg();
}


void ImpressionistUI::setAlphaMappedBrushState()
{
	if (m_pDoc->alphaMappedImageLoaded)
	{
		brushTypeMenu[7].activate();
	}
}

// filter kernel
void ImpressionistUI::cb_filterWidthInput(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nFilterWidth = int(((Fl_Value_Input *)o)->value());
}
void ImpressionistUI::cb_filterHeightInput(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nFilterHeight = int(((Fl_Value_Input *)o)->value());
}
void ImpressionistUI::cb_filter_value(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nFilterValue = strdup(((Fl_Multiline_Input *)o)->value());
}
void ImpressionistUI::cb_filter_normal_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->clearCanvas();
	int filter_width = ((ImpressionistUI*)(o->user_data()))->m_nFilterWidth;
	int filter_height = ((ImpressionistUI*)(o->user_data()))->m_nFilterHeight;
	std::string s1("");
	for (int i = 0; i < filter_height; i++) {
		for (int j = 0; j < filter_width; j++) {
			if (j != filter_width - 1)
				s1.append("1, ");
			else
				s1.append("1;\n");
		}
	}
	
	((ImpressionistUI*)(o->user_data()))->m_nFilterValue = new char[s1.size() + 1]; 
	std::copy(s1.begin(), s1.end(), ((ImpressionistUI*)(o->user_data()))->m_nFilterValue);
	((ImpressionistUI*)(o->user_data()))->m_nFilterValue[s1.size()] = '\0';
	((ImpressionistUI*)(o->user_data()))->m_FilterValue->value(((ImpressionistUI*)(o->user_data()))->m_nFilterValue);
}
void ImpressionistUI::cb_filter_apply(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nFilterApply = (int)((Fl_Light_Button *)o)->value();
	int apply = int(((ImpressionistUI*)(o->user_data()))->m_nFilterApply);
	if (apply)
	{
		int filter_width = ((ImpressionistUI*)(o->user_data()))->m_nFilterWidth;
		int filter_height = ((ImpressionistUI*)(o->user_data()))->m_nFilterHeight;
		if (filter_width <= 0 || filter_height <= 0)
		{
			fl_alert("Invalid size of filter kernel");
			return;
		}
		char* filter_value = ((ImpressionistUI*)(o->user_data()))->m_nFilterValue;

		if (!((ImpressionistUI*)(o->user_data()))->construct_filter(filter_value, filter_width, filter_height))
		{
			fl_alert("Invalid input for filter kernel");
			((Fl_Light_Button *)o)->value(0);
			return;
		}
	}

}

bool ImpressionistUI::construct_filter(char* filter_data, int filter_width, int filter_height)
{


	std::string str(filter_data);
	std::smatch sm;
	std::regex e("[\-0-9, ]*;");
	m_nFilter = new int[filter_width * filter_height];

	int lineCount = 0;
	while (std::regex_search(str, sm, e)) {

		std::smatch sint;
		std::regex eint("(-?[0-9]+)[,;]");
		std::string sub_str(sm[0]);
		lineCount++;
		if (lineCount > filter_height)
		{
			delete[] m_nFilter;
			return false;
		}
		int itemCount = 0;
		while (std::regex_search(sub_str, sint, eint)) {
				
			std::string item(sint[1]);
			int newItem = std::stoi(item);
			itemCount++;
			if (itemCount > filter_width || newItem < 0)
			{
				delete[] m_nFilter;
				return false;
			}
			m_nFilter[(lineCount-1)*filter_width+itemCount-1] = newItem;
			sub_str = sint.suffix().str();
		}
		if (itemCount < filter_width)
		{
			delete[] m_nFilter;
			return false;
		}
		str = sm.suffix().str();
	}
	if (lineCount < filter_height)
	{
		delete[] m_nFilter;
		return false;
	}
	return true;
}

void ImpressionistUI::cb_resolution_slider(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nResolution = int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_re_paint_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();
	pDoc->rePaint();
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
	m_paintView->size(w, h);
	m_origView->size(w, h);
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
}

//------------------------------------------------
// Return the brush size
//------------------------------------------------
int ImpressionistUI::getSize()
{
	return m_nSize;
}

int ImpressionistUI::getLineWidth()
{
	return m_nLineWidth;
}

int ImpressionistUI::getLineAngle()
{
	return m_nLineAngle;
}

float ImpressionistUI::getAlpha()
{
	return m_nAlpha;
}

int ImpressionistUI::getMosaicLevel()
{
	return m_nMosasiLevel;
}

int ImpressionistUI::getSpacing()
{
	return m_nSpacing;
}

int ImpressionistUI::getRand()
{
	return m_nRand;
}

int ImpressionistUI::getFilterHeight()
{
	return m_nFilterHeight;
}
int ImpressionistUI::getFilterWidth()
{
	return m_nFilterWidth;
}
int ImpressionistUI::getFilter()
{
	return m_nFilterApply;
}
int* ImpressionistUI::getFilterValue()
{
	return m_nFilter;
}

int ImpressionistUI::getResolution()
{
	return m_nResolution;
}
//-------------------------------------------------
// Set the brush size
//-------------------------------------------------
void ImpressionistUI::setSize(int size)
{
	m_nSize = size;

	if (size <= 40)
		m_BrushSizeSlider->value(m_nSize);
}

void ImpressionistUI::setLineWidth(int width)
{
	m_nLineWidth = width;
	if (width <= 40)
		m_BrushLineWidthSlider->value(m_nLineWidth);
}

void ImpressionistUI::setLineAngle(int angle)
{
	m_nLineAngle = angle;

	if (angle <= 359)
		m_BrushLineAngleSlider->value(m_nLineAngle);
}

void ImpressionistUI::setAlpha(float alpha)
{
	m_nAlpha = alpha;

	if (alpha <= 1.00)
		m_BrushAlphaSlider->value(m_nAlpha);
}

void ImpressionistUI::setRand(int rand)
{
	if (rand == 0 || rand == 1)
		m_SizeRandButton->value(m_nRand);
}

int ImpressionistUI::getEdgeThreshold()
{
	return m_nEdgeThreshold;
}

// Main menu definition
Fl_Menu_Item ImpressionistUI::menuitems[] = {
	{ "&File",		0, 0, 0, FL_SUBMENU },
	{ "&Load Image...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load_image },
	{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save_image },
	{ "&Brushes...",	FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_brushes },
	{ "&Clear Canvas",	FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_clear_canvas, 0, FL_MENU_DIVIDER },
	{ "&Colors...",		FL_ALT + 'k', (Fl_Callback *)ImpressionistUI::cb_color_selector },
	{ "&Paintly...",	FL_ALT + 'p', 0 },
	{ "&Filter...",		FL_ALT + 'f', (Fl_Callback *)ImpressionistUI::cb_filter_kernel, 0, FL_MENU_DIVIDER },
	{ "Load Edge Image...",		FL_ALT + 'e', 0 },
	{ "Load Alphamap Image...",		FL_CTRL + 'b', (Fl_Callback *)ImpressionistUI::cb_loadAlphaMappedImage },
	//{ "Load Another Image...",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_load_another_image, 0, FL_MENU_DIVIDER },
	{ "Load Another Image...",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_load_another_image },
	{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
	{ 0 },

	{ "&Display", 0, 0, 0, FL_SUBMENU},
	{ "&Swap Paintings",	FL_CTRL + 'a', (Fl_Callback *)ImpressionistUI::cb_swap_image },
	{ "&Original Image...",	FL_ALT + 'o', 0 },
	{ "&Edge Image...",	FL_ALT + 'e',0 },
	{ "&Another Image...",	FL_ALT + 'a', 0},
	{ 0 },

	{ "&Options", 0, 0, 0, FL_SUBMENU },
	{ "&Faster",	FL_ALT + 'f', 0 },
	{ "&Safter",	FL_ALT + 's',0 },
	{ 0 },

	{ "&Help",		0, 0, 0, FL_SUBMENU },
	{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
	{ 0 },

	{ 0 }
};

// Brush choice menu definition
Fl_Menu_Item ImpressionistUI::brushTypeMenu[NUM_BRUSH_TYPE + 1] = {
	{ "Points",				FL_ALT + 'p', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_POINTS },
	{ "Lines",				FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_LINES },
	{ "Circles",			FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_CIRCLES },
	{ "Scattered Points",	FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_POINTS },
	{ "Scattered Lines",	FL_ALT + 'm', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_LINES },
	{ "Scattered Circles",	FL_ALT + 'd', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_CIRCLES },
	{ "Mosaic",				FL_CTRL + 'm', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_MOSAIC },
	{ "Alpha Mapped",		FL_CTRL + 'e', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_ALPHAMAPPED },
	{ "Curved",		FL_CTRL + 'v', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_CURVED },
	{ 0 }
};



Fl_Menu_Item ImpressionistUI::directionTypeMenu[NUM_DIRECTION_TYPE + 1] = {
	{ "Slider/Right Mouse",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_StrokeDirectionChoice, (void *)SLIDER_AND_RIGHT_MOUSE },
	{ "Gradient",			FL_ALT + 'g', (Fl_Callback *)ImpressionistUI::cb_StrokeDirectionChoice, (void *)GRADIENT },
	{ "Brush Direction",	FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_StrokeDirectionChoice, (void *)BRUSH_DIRECTION },
	{ 0 }
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
	m_nLineWidth = 1;
	m_nLineAngle = 0;
	m_nAlpha = 1.00;
	m_nMosasiLevel = 5;
	m_nSpacing = 1;
	m_nRand = 0;
	m_nEdgeThreshold = 200;
	m_nFilterHeight = 3;
	m_nFilterWidth = 3;
	m_nFilterApply = 0;
	m_nFilterValue = "1, 1, 1;\n1, 1, 1;\n1, 1, 1;";
	m_nResolution = 3;
	
//	m_nFilterValue = "1, 1, 1;\n1, 1, 1;\n1, 1, 1;"

	m_nBrushDirection = SLIDER_AND_RIGHT_MOUSE;

	// brush dialog definition
	m_brushDialog = new Fl_Window(400, 380, "Brush Dialog");
	// Add a brush type choice to the dialog
	m_BrushTypeChoice = new Fl_Choice(50, 10, 150, 25, "&Brush");
	m_BrushTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
	m_BrushTypeChoice->menu(brushTypeMenu);
	m_BrushTypeChoice->callback(cb_brushChoice);

	m_StrokeDirectionChoice = new Fl_Choice(113, 40, 150, 25, "Stroke Direction");
	m_StrokeDirectionChoice->user_data((void*)(this));	// record self to be used by static callback functions
	m_StrokeDirectionChoice->menu(directionTypeMenu);
	m_StrokeDirectionChoice->callback(cb_StrokeDirectionChoice);
	m_StrokeDirectionChoice->deactivate();

	m_ClearCanvasButton = new Fl_Button(240, 10, 150, 25, "&Clear Canvas");
	m_ClearCanvasButton->user_data((void*)(this));
	m_ClearCanvasButton->callback(cb_clear_canvas_button);

	m_UndoButton = new Fl_Button(150, 200, 50, 25, "&Undo");
	m_UndoButton->user_data((void*)(this));
	m_UndoButton->callback(cb_undo_button);

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

	// Add brush size slider to the dialog 
	m_BrushLineWidthSlider = new Fl_Value_Slider(10, 110, 300, 20, "Line Width");
	m_BrushLineWidthSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_BrushLineWidthSlider->type(FL_HOR_NICE_SLIDER);
	m_BrushLineWidthSlider->labelfont(FL_COURIER);
	m_BrushLineWidthSlider->labelsize(12);
	m_BrushLineWidthSlider->minimum(1);
	m_BrushLineWidthSlider->maximum(40);
	m_BrushLineWidthSlider->step(1);
	m_BrushLineWidthSlider->value(m_nLineWidth);
	m_BrushLineWidthSlider->align(FL_ALIGN_RIGHT);
	m_BrushLineWidthSlider->callback(cb_lineWidthSlides);
	m_BrushLineWidthSlider->deactivate();

	// Add brush size slider to the dialog 
	m_BrushLineAngleSlider = new Fl_Value_Slider(10, 140, 300, 20, "Line Angle");
	m_BrushLineAngleSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_BrushLineAngleSlider->type(FL_HOR_NICE_SLIDER);
	m_BrushLineAngleSlider->labelfont(FL_COURIER);
	m_BrushLineAngleSlider->labelsize(12);
	m_BrushLineAngleSlider->minimum(0);
	m_BrushLineAngleSlider->maximum(359);
	m_BrushLineAngleSlider->step(1);
	m_BrushLineAngleSlider->value(m_nLineAngle);
	m_BrushLineAngleSlider->align(FL_ALIGN_RIGHT);
	m_BrushLineAngleSlider->callback(cb_lineAngleSlides);
	m_BrushLineAngleSlider->deactivate();

	// Add brush size slider to the dialog 
	m_BrushAlphaSlider = new Fl_Value_Slider(10, 170, 300, 20, "Alpha");
	m_BrushAlphaSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_BrushAlphaSlider->type(FL_HOR_NICE_SLIDER);
	m_BrushAlphaSlider->labelfont(FL_COURIER);
	m_BrushAlphaSlider->labelsize(12);
	m_BrushAlphaSlider->minimum(0);
	m_BrushAlphaSlider->maximum(1.00);
	m_BrushAlphaSlider->step(0.01);
	m_BrushAlphaSlider->value(m_nAlpha);
	m_BrushAlphaSlider->align(FL_ALIGN_RIGHT);
	m_BrushAlphaSlider->callback(cb_alphaSlides);

	m_EdgeClippingButton = new Fl_Light_Button(10, 200, 120, 25, "&Edge Clipping");

	m_AnotherGradientButton = new Fl_Light_Button(240, 200, 150, 25, "&Another Gradient");

	m_SpacingSlider =  new Fl_Value_Slider(10, 240, 160, 25, "Spacing");
	m_SpacingSlider->user_data((void*)(this));
	m_SpacingSlider->type(FL_HOR_NICE_SLIDER);
	m_SpacingSlider->labelfont(FL_COURIER);
	m_SpacingSlider->labelsize(12);
	m_SpacingSlider->minimum(1);
	m_SpacingSlider->maximum(16);
	m_SpacingSlider->step(1);
	m_SpacingSlider->value(m_nSpacing);
	m_SpacingSlider->align(FL_ALIGN_RIGHT);
	m_SpacingSlider->callback(cb_spacingSlider);

	m_SizeRandButton = new Fl_Light_Button(230, 240, 100, 25, "&Size Rand.");
	m_SizeRandButton->user_data((void*)(this));
	m_SizeRandButton->value(m_nRand);
	m_SizeRandButton->callback(cb_size_rand);

	m_PaintButton = new Fl_Button(340, 240, 50, 25, "&Paint");
	m_PaintButton->user_data((void*)(this));
	m_PaintButton->callback(cb_auto_paint);

	m_ResolutionSlider = new Fl_Value_Slider(10, 275, 160, 25, "Resolution Level");
	m_ResolutionSlider->user_data((void*)(this));
	m_ResolutionSlider->type(FL_HOR_NICE_SLIDER);
	m_ResolutionSlider->labelfont(FL_COURIER);
	m_ResolutionSlider->labelsize(12);
	m_ResolutionSlider->minimum(1);
	m_ResolutionSlider->maximum(5);
	m_ResolutionSlider->step(1);
	m_ResolutionSlider->value(m_nResolution);
	m_ResolutionSlider->align(FL_ALIGN_RIGHT);
	m_ResolutionSlider->callback(cb_resolution_slider);

	m_RePaintButton = new Fl_Button(320, 275, 70, 25, "&Paint(R)");
	m_RePaintButton->user_data((void*)(this));
	m_RePaintButton->callback(cb_re_paint_button);

	m_EdgeThresholdSlider = new Fl_Value_Slider(10, 310, 170, 25, "Edge Threshold");
	m_EdgeThresholdSlider->user_data((void*)(this));
	m_EdgeThresholdSlider->type(FL_HOR_NICE_SLIDER);
	m_EdgeThresholdSlider->labelfont(FL_COURIER);
	m_EdgeThresholdSlider->labelsize(12);
	m_EdgeThresholdSlider->minimum(0);
	m_EdgeThresholdSlider->maximum(500);
	m_EdgeThresholdSlider->step(1);
	m_EdgeThresholdSlider->value(m_nEdgeThreshold);
	m_EdgeThresholdSlider->align(FL_ALIGN_RIGHT);
	m_EdgeThresholdSlider->callback(cb_edge_threshold);

	m_DoItButton = new Fl_Button(340, 310, 50, 25, "&Do it");
	m_DoItButton->user_data((void*)(this));
	m_DoItButton->callback(cb_recal_edge);
	
	m_MosaicSlider = new Fl_Value_Slider(10, 345, 300, 20, "Mosaic Level");
	m_MosaicSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_MosaicSlider->type(FL_HOR_NICE_SLIDER);
	m_MosaicSlider->labelfont(FL_COURIER);
	m_MosaicSlider->labelsize(12);
	m_MosaicSlider->minimum(3);
	m_MosaicSlider->maximum(8);
	m_MosaicSlider->step(1);
	m_MosaicSlider->value(m_nMosasiLevel);
	m_MosaicSlider->align(FL_ALIGN_RIGHT);
	m_MosaicSlider->callback(cb_mosaicSlides);
	m_MosaicSlider->deactivate();

	// End of brush dialog
	m_brushDialog->end();

	// Color selector dialog 
	m_colorSelectorDialog = new Fl_Window(240, 260, "Color Selector");

	m_colorChooser = new Fl_Color_Chooser(10, 20, 220, 230, "Color Blending");
	m_colorChooser->rgb(1.000, 1.000, 1.000);

	m_colorSelectorDialog->end();

	brushTypeMenu[7].deactivate();

	// Filter Dialog
	m_filterDialog = new Fl_Window(300, 300, "Filter Kernel");
	m_FilterWidth = new Fl_Value_Input(60, 10, 40, 20, "Width");
	m_FilterWidth->user_data((void*)(this));
	m_FilterWidth->type(FL_INT_INPUT);
	m_FilterWidth->labelfont(FL_COURIER);
	m_FilterWidth->labelsize(12);
	m_FilterWidth->callback(cb_filterWidthInput);
	m_FilterWidth->value(m_nFilterWidth);
	m_FilterHeight = new Fl_Value_Input(160, 10, 40, 20, "Height");
	m_FilterHeight->user_data((void*)(this));
	m_FilterHeight->type(FL_INT_INPUT);
	m_FilterHeight->labelfont(FL_COURIER);
	m_FilterHeight->labelsize(12);
	m_FilterHeight->callback(cb_filterHeightInput);
	m_FilterHeight->value(m_nFilterHeight);
	m_FilterValue = new Fl_Multiline_Input(20, 50, 260, 100, "Please enter kernel values separated with ',' for each element and ';' for each row\n");
	m_FilterValue->user_data((void*)(this));
	m_FilterValue->labelfont(FL_COURIER);
	m_FilterValue->labelsize(12);
	m_FilterValue->callback(cb_filter_value);
	m_FilterValue->value("1, 1, 1;\n1, 1, 1;\n1, 1, 1;");
	m_FilterApply = new Fl_Light_Button(210, 10, 60, 20, "Apply");
	m_FilterApply->user_data((void*)(this));
	m_FilterApply->labelfont(FL_COURIER);
	m_FilterApply->labelsize(12);
	m_FilterApply->callback(cb_filter_apply);
	m_FilterApply->value(m_nFilterApply);
	m_FilterNormal = new Fl_Button(100, 170, 100, 30, "Normalize");
	m_FilterNormal->user_data((void*)(this));
	m_FilterNormal->labelfont(FL_COURIER);
	m_FilterNormal->labelsize(12);
	m_FilterNormal->callback(cb_filter_normal_button);
	m_filterDialog->end();
}
