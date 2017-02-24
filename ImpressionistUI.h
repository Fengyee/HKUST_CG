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

#include <string>
#include <regex>

enum
{
	DISPLAY_ORIGINAL,
	DISPLAY_EDGE,
	DISPLAY_ANOTHER,
	NUM_DISPLAY_TYPE // Make sure this stays at the end!
};

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
	Fl_Window*			m_filterDialog;
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
	Fl_Slider*			m_ResolutionSlider;
	Fl_Button*			m_RePaintButton;
	Fl_Button*			m_Mural;

	// filterDialog
//	Fl_Button*			m_FilterSize;
	Fl_Button*			m_FilterApply;
	Fl_Value_Input*		m_FilterWidth;
	Fl_Value_Input*		m_FilterHeight;
	Fl_Input*			m_FilterValue;
	Fl_Button*			m_FilterNormal;

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
	void				setAlphaMappedBrushState();
	int					getEdgeThreshold();
	int					getResolution();

	void				setRand(int rand);
	int					getRand();
	int					getFilterWidth();
	int					getFilterHeight();
	int					getFilter();
	int*				getFilterValue();
	bool				construct_filter(char* filter_data, int filter_width, int filter_height);
	bool				getEdgeClipping();
	int					getMural();
	int					getAnotherGradient();

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
	int		m_nRand;
	int		m_nEdgeThreshold;
	int		m_nFilterWidth;
	int		m_nFilterHeight;
	char*	m_nFilterValue;
	int		m_nFilterApply;
	int*	m_nFilter;
	bool	m_bEdgeClipping;
	int		m_nResolution;
	int		m_nMural;
	int		m_nAnotherGradient;
	// Static class members
	static Fl_Menu_Item		menuitems[];
	static Fl_Menu_Item		brushTypeMenu[NUM_BRUSH_TYPE + 1];
	static Fl_Menu_Item		directionTypeMenu[NUM_DIRECTION_TYPE + 1];

	static ImpressionistUI*	whoami(Fl_Menu_* o);

	// All callbacks here.  Callbacks are declared 
	// static
	static void	cb_load_image(Fl_Menu_* o, void* v);
	static void	cb_load_another_image(Fl_Menu_* o, void* v);
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
	static void	cb_auto_paint(Fl_Widget* o, void* v);
	static void cb_size_rand(Fl_Widget* o, void* v);
	static void	cb_swap_image(Fl_Menu_* o, void* v);
	static void cb_color_selector(Fl_Menu_* o, void * v);
	static void	cb_mosaicSlides(Fl_Widget* o, void* v);
	static void cb_loadAlphaMappedImage(Fl_Menu_* o, void* v);
	static void cb_edge_threshold(Fl_Widget* o, void* v);
	static void cb_recal_edge(Fl_Widget* o, void* v);
	static void cb_filter_kernel(Fl_Menu_* o, void* v);
	static void cb_filter_apply(Fl_Widget* o, void* v);
	static void cb_filterWidthInput(Fl_Widget* o, void* v);
	static void cb_filterHeightInput(Fl_Widget* o, void* v);
	static void cb_filter_value(Fl_Widget* o, void* v);
	static void	cb_filter_normal_button(Fl_Widget* o, void* v);
	static void cb_imageChoice(Fl_Menu_* o, void* v);
	static void cb_edgeclipping_button(Fl_Widget* o, void* v);
	static void cb_resolution_slider(Fl_Widget* o, void* v);
	static void cb_re_paint_button(Fl_Widget* o, void* v);
	static void cb_mural(Fl_Widget* o, void* v);
	static void cb_anotherGradient(Fl_Widget* o, void* v);
};

#endif
