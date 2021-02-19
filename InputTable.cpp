#include "InputTable.h"

InputTable::InputTable(int x, int y, int w, int h, const char* l)
	: Fl_Table(x, y, w, h, l) {
    callback(&event_callback, (void*)this);
    when(FL_WHEN_NOT_CHANGED | when());
    m_input = new Fl_Int_Input(w / 2, h / 2, 0, 0);
    m_input->hide();
    m_input->callback(cb_input, (void*)this);
    m_input->when(FL_WHEN_ENTER_KEY_ALWAYS);		// callback triggered when user hits Enter
    m_input->maximum_size(5);
    m_input->color(FL_YELLOW);
    for (int c = 0; c < MAX_COLS; c++)
        for (int r = 0; r < MAX_ROWS; r++)
            // m_values[r][c] = c + (r * MAX_COLS);		// initialize cells
            m_values[r][c] = 0;
    end();
    m_currCol= m_currRow = 0;
    set_selection(0, 0, 0, 0);
}

InputTable::~InputTable() {
}

void InputTable::update() {
    m_values[m_currRow][m_currCol] = atoi(m_input->value());
    m_input->hide();
    window()->cursor(FL_CURSOR_DEFAULT);
}

void InputTable::beginEditing(int r, int c) {
    m_currRow = r;
    m_currCol= c;
    set_selection(r, c, r, c);
    int x, y, w, h;
    find_cell(CONTEXT_CELL, r, c, x, y, w, h);
    m_input->resize(x, y, w, h);
    char s[30];
    sprintf(s, "%d", m_values[r][c]);
    m_input->value(s);
    m_input->position(0, strlen(s));
    m_input->show();
    m_input->take_focus();
}

void InputTable::endEditing() {
    if (m_input->visible()) {
        update();
    }
}

void InputTable::draw_cell(TableContext context, int r, int c, int x, int y, int w, int h) {
    static char s[30];
    switch (context) {
    case CONTEXT_STARTPAGE:
        break;
    case CONTEXT_COL_HEADER:
        fl_font(FL_HELVETICA | FL_BOLD, 14);
        fl_push_clip(x, y, w, h);
        fl_draw_box(FL_THIN_UP_BOX, x, y, w, h, col_header_color());
        fl_color(FL_BLACK);
        sprintf(s, "%d", c + 1);
        fl_draw(s, x, y, w, h, FL_ALIGN_CENTER);
        fl_pop_clip();
        return;
    case CONTEXT_ROW_HEADER:
        fl_font(FL_HELVETICA | FL_BOLD, 14);
        fl_push_clip(x, y, w, h);
        fl_draw_box(FL_THIN_UP_BOX, x, y, w, h, row_header_color());
        fl_color(FL_BLACK);
        sprintf(s, "%d", r + 1);
        fl_draw(s, x, y, w, h, FL_ALIGN_CENTER);
        fl_pop_clip();
        return;
    case CONTEXT_CELL:
        if (r == m_currRow && c == m_currCol && m_input->visible()) {
            return;
        }
        fl_draw_box(FL_THIN_UP_BOX, x, y, w, h, is_selected(r, c) ? FL_YELLOW : FL_WHITE);
        fl_push_clip(x + 3, y + 3, w - 6, h - 6);
        fl_color(FL_BLACK);
        fl_font(FL_HELVETICA, 14);
        sprintf(s, "%d", m_values[r][c]);
        fl_draw(s, x + 3, y + 3, w - 6, h - 6, FL_ALIGN_RIGHT);
        fl_pop_clip();
        return;
    case CONTEXT_RC_RESIZE:
        return;
    default:
        return;
    }
}

void InputTable::event_callback2() {
    int r = callback_row();
    int c = callback_col();
    TableContext context = callback_context();
    switch (context) {
        case CONTEXT_CELL: {
            switch (Fl::event()) {
                case FL_PUSH:
                    endEditing();
                    beginEditing(r, c);
                    return;
                case FL_KEYBOARD:
                    endEditing();
                    if (Fl::event_key() == FL_Escape)
                        return;
                    switch (Fl::e_text[0]) {
                        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '+': case '-':
                            beginEditing(r, c);
                            m_input->handle(Fl::event());
                            break;
                        case '\r': case '\n':
                            beginEditing(r, c);
                            break;
                    }
                    return;
            }
            return;
        }
        case CONTEXT_TABLE:
        case CONTEXT_ROW_HEADER:
        case CONTEXT_COL_HEADER:
            endEditing();
            return;
        default:
            return;
    }
}
