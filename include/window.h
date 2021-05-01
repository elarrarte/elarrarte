#include <curses.h>
#include <panel.h>
#include <stdlib.h>

typedef struct
{
    PANEL *panel;
    WINDOW *border_window;
    WINDOW *user_window;
    size_t width, height;

} window_t;

void window_init()
{
    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);
}

void window_end()
{
    endwin();
}

window_t *window_create(int height, int width, int position_y, int position_x)
{
    window_t *window;

    window = (window_t *) malloc(sizeof(window_t));
    window->width = width;
    window->height = height;
    window->border_window = newwin(height, width, position_y, position_x);
    window->user_window = derwin(window->border_window, height - 2, width - 2, 1, 1);
    window->panel = new_panel(window->border_window);
    box(window->border_window, '|', '-');
    return(window);
}

void window_print(window_t *window, char *format, char *text)
{
    wprintw(window->user_window, format, text);
}

void window_title(window_t *window, char *title)
{
    box(window->border_window, '|', '-');
    mvwprintw(window->border_window, 0, (window->width - strlen(title)) / 2, "%s", title);
    wrefresh(window->border_window);
}