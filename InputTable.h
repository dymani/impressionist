#ifndef TABLE_H
#define TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Table.H>
#include <FL/Fl_Int_Input.H>
#include <FL/fl_draw.H>

class InputTable : public Fl_Table {
public:
    static const int MAX_ROWS = 9;
    static const int MAX_COLS = 9;
    InputTable(int X, int Y, int W, int H, const char* L = 0);
    ~InputTable();
    void update();
    void beginEditing(int r, int c);
    void endEditing();

    int m_values[MAX_ROWS][MAX_COLS];
private:
    void draw_cell(TableContext context, int r = 0, int c = 0, int x = 0, int y = 0, int w = 0, int h = 0);
    void event_callback2();
    static void event_callback(Fl_Widget*, void* v) {
        ((InputTable*)v)->event_callback2();
    }
    static void cb_input(Fl_Widget*, void* v) {
        ((InputTable*)v)->update();
    }

    Fl_Int_Input* m_input;
    int m_currRow, m_currCol;

};

#endif