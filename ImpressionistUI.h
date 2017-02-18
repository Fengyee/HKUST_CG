//
// impressionistUI.h
//
// The header file for the UI part
//

#ifndef ImpressionistUI_h
#define ImpressionistUI_h

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_file_chooser.H>		// FLTK file chooser
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Color_Chooser.H>

#include "Impressionist.h"
#include "OriginalView.h"
#include "PaintView.h"

#include "ImpBrush.h"

class ImpressionistUI {
public:
	ImpressionistUI();

	// The FLTK widgets
	Fl_Window*			m_mainWindow;
	Fl_Menu_Bar*		m_menubar;

	PaintView*			m_paintView;
	OriginalView*		m_origView;

	// for brush dialog
	Fl_Window*			m_brushDialog;
	Fl_Window*			m_colorSelectorDialog;
	Fl_Choice*			m_BrushTypeChoice;
	Fl_Choice*			m_StrokeDirectionChoice;
	Fl_Light_Button*	m_EdgeClippingButton;
	Fl_Light_Button*	m_AnotherGradientButton;

	Fl_Color_Chooser*	m_colorChooser;

	Fl_Slider*			m_BrushSizeSlider;
	Fl_Slider*			m_BrushLineWidthSlider;
	Fl_Slider*			m_BrushLineAngleSlider;
	Fl_Slider*			m_BrushAlphaSlider;
	Fl_Button*          m_ClearCanvasButton;
	Fl_Button*			m_SizeRandButton;
	Fl_Button*			m_PaintButton;
	Fl_Button*			m_UndoButton;
	Fl_Slider*          m_SpacingSlider;
	Fl_Slider*			m_EdgeThresholdSlider;
	Fl_Button*			m_DoItButton;
	Fl_Slider*			m_MosaicSlider;

	// Member functions
	void				setDocument(ImpressionistDoc* doc);
	ImpressionistDoc*	getDocument();

	void				show();
	void				resize_windows(int w, int h);

	// Interface to get attribute

	int					getSize();
	void				setSize(int size);
	int					getLineWidth();
	void				setLineWidth(int width);
	int					getLineAngle();
	void				setLineAngle(int angle);
	float				getAlpha();
	void				setAlpha(float alpha);
	int					getMosaicLevel();
	int					getSpacing();

private:
	ImpressionistDoc*	m_pDoc;		// pointer to document to communicate with the document

									// All attributes here
	int		m_nSize;
	int		m_nLineWidth;
	int		m_nLineAngle;
	float	m_nAlpha;
	int		m_nBrushDirection;
	int		m_nMosasiLevel;
	int		m_nSpacing;

	// Static class members
	static Fl_Menu_Item		menuitems[];
	static Fl_Menu_Item		brushTypeMenu[NUM_BRUSH_TYPE + 1];
	static Fl_Menu_Item		directionTypeMenu[NUM_DIRECTION_TYPE + 1];

	static ImpressionistUI*	whoami(Fl_Menu_* o);

	// All callbacks here.  Callbacks are declared 
	// static
	static void	cb_load_image(Fl_Menu_* o, void* v);
	static void	cb_save_image(Fl_Menu_* o, void* v);
	static void	cb_brushes(Fl_Menu_* o, void* v);
	static void	cb_clear_canvas(Fl_Menu_* o, void* v);
	static void	cb_exit(Fl_Menu_* o, void* v);
	static void	cb_about(Fl_Menu_* o, void* v);
	static void	cb_brushChoice(Fl_Widget* o, void* v);
	static void cb_StrokeDirectionChoice(Fl_Widget* o, void* v);
	static void	cb_clear_canvas_button(Fl_Widget* o, void* v);
	static void	cb_undo_button(Fl_Widget* o, void* v);
	static void	cb_sizeSlides(Fl_Widget* o, void* v);
	static void	cb_lineWidthSlides(Fl_Widget* o, void* v);
	static void	cb_lineAngleSlides(Fl_Widget* o, void* v);
	static void	cb_alphaSlides(Fl_Widget* o, void* v);
	static void cb_spacingSlider(Fl_Widget* o, void* v);
	static void	cb_swap_image(Fl_Menu_* o, void* v);
	static void cb_color_selector(Fl_Menu_* o, void * v);
	static void	cb_mosaicSlides(Fl_Widget* o, void* v);

};

#endif
