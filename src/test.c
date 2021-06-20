#include <stdio.h>
#include <curses.h>
#include <string.h>
#include "../include/window.h"

#define WINDOWS                                             10
#define WINDOW_HEIGHT                                       20
#define WINDOW_WIDTH                                        10
int main()
{
    window_t *windows[WINDOWS], *process_window;
    int index;

    window_init();
    for(index = 0; index < WINDOWS; index++)
    {
        windows[index] = window_create(WINDOW_HEIGHT, WINDOW_WIDTH, 0, WINDOW_WIDTH * index);
        wnoutrefresh(windows[index]->border_window);
    }
    process_window = window_create(WINDOW_HEIGHT, WINDOW_WIDTH * 4, 0, WINDOW_WIDTH * index);
    wnoutrefresh(process_window->border_window);
    doupdate();
    wgetch(windows[0]->user_window);
    window_end();
}