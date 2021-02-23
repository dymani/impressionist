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
#include <FL/Fl_Color_Chooser.H>
#include "InputTable.h"

#include "Impressionist.h"
#include "OriginalView.h"
#include "PaintView.h"
#include "LineOverlay.h"
#include "Marker.h"
#include "ConvolutionManager.h"

#include "ImpBrush.h"

class ImpressionistUI {
public:
	ImpressionistUI();

	// The FLTK widgets
	Fl_Window*			m_mainWindow;
	Fl_Menu_Bar*		m_menubar;
								
	PaintView*			m_paintView;
	OriginalView*		m_origView;

	LineOverlay* m_lineOverlay;
	Marker* m_marker;

// for brush dialog
	Fl_Window*			m_brushDialog;
	Fl_Choice*			m_BrushTypeChoice;
	Fl_Choice*          m_strokeDirectionChoice;

	Fl_Slider*			m_BrushSizeSlider;
	Fl_Slider*			m_LineWidthSlider;
	Fl_Slider*			m_LineAngleSlider;
	Fl_Slider*			m_AlphaSlider;

	Fl_Light_Button* m_anotherGradientLightButton;
	Fl_Light_Button* m_edgeClipLightButton;

	Fl_Slider* m_edgeThresholdSlider;
	Fl_Button* m_edgeDetectionButton;

	Fl_Button*          m_ClearCanvasButton;

// for color dialog
	Fl_Window*			m_colorDialog;
	Fl_Color_Chooser*	m_colorChooser;

// for filter dialog
	Fl_Window* m_filterDialog;
	Fl_Choice* m_filterSourceChoice;
	Fl_Choice* m_filterTypeChoice;
	Fl_Int_Input* m_filterWidthInput;
	Fl_Int_Input* m_filterHeightInput;
	InputTable* m_filterInputTable;
	Fl_Light_Button* m_filterNormalizeLightButton;
	Fl_Button* m_filterApplyButton;

	// Member functions
	void				setDocument(ImpressionistDoc* doc);
	ImpressionistDoc*	getDocument();

	void				show();
	void				resize_windows(int w, int h);

	// Interface to get attribute

	int					getSize();
	void				setSize(int size);
	int					getWidth();
	void				setWidth(int width);
	int					getAngle();
	void				setAngle(int angle);
	int					getAlpha();
	void				setAlpha(int alpha);
	int getEdgeThreshold();
	
	double				getRedVal();
	void				setRedVal(double R);
	double				getGreenVal();
	void				setGreenVal(double G);
	double				getBlueVal();
	void				setBlueVal(double B);
	
private:
	ImpressionistDoc*	m_pDoc;		// pointer to document to communicate with the document

	// All attributes here
	int m_brushType;
	int		m_nSize;
	int		m_brushWidth;
	int		m_angle;
	int		m_alpha;
	bool m_isUsingAnotherGradient;
	bool m_isAnotherImageLoaded;
	bool m_isEdgeClippingOn;
	bool m_isEdgeImageLoaded;
	int m_edgeThreshold;


	double	m_redVal;
	double	m_greenVal;
	double	m_blueVal;

	int m_filterType;
	int m_filterSource;
	int m_filterWidth;
	int m_filterHeight;
	bool m_isFilterNormalized;

	// Static class members
	static Fl_Menu_Item		menuitems[];
	static Fl_Menu_Item		brushTypeMenu[NUM_BRUSH_TYPE+1];
	static Fl_Menu_Item		strokeDirectionTypeMenu[4];
	static Fl_Menu_Item		filterTypeMenu[ConvolutionManager::NUM_FILTER_CHOICE + 1];
	static Fl_Menu_Item		filterSourceMenu[2 + 1];

	static ImpressionistUI*	whoami(Fl_Menu_* o);

	// All callbacks here.  Callbacks are declared 
	// static
	static void	cb_load_image(Fl_Menu_* o, void* v);
	static void	cb_save_image(Fl_Menu_* o, void* v);
	static void	cb_brushes(Fl_Menu_* o, void* v);
	static void	cb_clear_canvas(Fl_Menu_* o, void* v);
	static void cb_undo(Fl_Menu_* o, void* v);
	static void cb_colors(Fl_Menu_* o, void* v);
	static void	cb_swap_contents(Fl_Menu_* o, void* v);
	static void cb_change_image(Fl_Menu_* o, void* v);
	static void cb_load_another_image(Fl_Menu_* o, void* v);
	static void cb_load_edge_image(Fl_Menu_* o, void* v);
	static void	cb_exit(Fl_Menu_* o, void* v);
	static void	cb_about(Fl_Menu_* o, void* v);
	static void	cb_filters(Fl_Menu_* o, void* v);
	static void	cb_view_original(Fl_Menu_* o, void* v);
	static void	cb_view_another(Fl_Menu_* o, void* v);
	static void	cb_view_edge(Fl_Menu_* o, void* v);

	static void	cb_brushChoice(Fl_Widget* o, void* v);
	static void	cb_clear_canvas_button(Fl_Widget* o, void* v);
	static void	cb_strokeDirectionChoice(Fl_Widget* o, void* v);
	static void	cb_sizeSlides(Fl_Widget* o, void* v);
	static void cb_widthSlides(Fl_Widget* o, void* v);
	static void cb_angleSlides(Fl_Widget* o, void* v);
	static void cb_alphaSlides(Fl_Widget* o, void* v);
	static void cb_another_gradient_light_button(Fl_Widget* o, void* v);
	static void cb_edge_clip_light_button(Fl_Widget* o, void* v);
	static void cb_edge_threshold_slides(Fl_Widget* o, void* v);
	static void	cb_edge_detection_button(Fl_Widget* o, void* v);

	static void cb_color_chooser(Fl_Widget* o, void* v);

	static void cb_filter_type_choice(Fl_Widget* o, void* v);
	static void cb_filter_source_choice(Fl_Widget* o, void* v);
	static void cb_filter_width_input(Fl_Widget* o, void* v);
	static void cb_filter_height_input(Fl_Widget* o, void* v);
	static void cb_filter_normalize_light_button(Fl_Widget* o, void* v);
	static void cb_filter_apply_button(Fl_Widget* o, void* v);

};

#endif
