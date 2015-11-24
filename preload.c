// preload.c
// Alistair Buxton <a.j.buxton@gmail.com>

#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

void __attribute__ ((constructor)) load(void);

// Called when the library is loaded and before dlopen() returns
void load(void)
{
    fprintf(stderr, "Decocolour preload hack loaded...\n");
}


/* NOT USED */

/*
typedef Display *(* xod_func)(char *);

Display *XOpenDisplay(char *display_name)
{
    static xod_func xod = NULL;
    if (!xod) xod = dlsym(RTLD_NEXT, "XOpenDisplay");

    Display *ret = xod(display_name);

    return ret;
}



typedef int (* xcd_func)(Display *);

int XCloseDisplay(Display *display)
{
    static xcd_func xcd = NULL;
    if (!xcd) xcd = dlsym(RTLD_NEXT, "XCloseDisplay");

    int ret = xcd(display);

    return ret;
}
*/



static void set_decocolour(Display *display, Window window)
{
    static Atom active_atom = 0, inactive_atom = 0;

    const unsigned char active_colour[] = "#ff0000";
    const unsigned char inactive_colour[] = "#ff7777";

    if(!active_atom) {
        active_atom = XInternAtom(display, "ACTIVE_DECOCOLOUR", False);
        inactive_atom = XInternAtom(display, "INACTIVE_DECOCOLOUR", False);
    }

    XChangeProperty(display, window, active_atom, XA_STRING, 8, PropModeReplace, active_colour, 8);
    XChangeProperty(display, window, inactive_atom, XA_STRING, 8, PropModeReplace, inactive_colour, 8);
}




typedef Window (* xcw_func)(Display *, Window, int, int, unsigned int, unsigned int, unsigned int, int,
                            unsigned int, Visual *, unsigned long, XSetWindowAttributes *);

Window XCreateWindow(Display *display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth,
                     unsigned int class, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes)
{
    static xcw_func xcw = NULL;

    if (!xcw) {
        xcw = dlsym(RTLD_NEXT, "XCreateWindow");
    }

    Window window = xcw(display, parent, x, y, width, height, border_width, depth, class, visual, valuemask, attributes);

    set_decocolour(display, window);

    return window;
}



typedef Window (* xcsw_func)(Display *, Window, int, int, unsigned int, unsigned int, unsigned int, unsigned long, unsigned long);

Window XCreateSimpleWindow(Display *display, Window parent, int x, int y, unsigned int width, unsigned int height,
                           unsigned int border_width, unsigned long border, unsigned long background)
{
    static xcsw_func xcsw = NULL;

    if (!xcsw) {
        xcsw = dlsym(RTLD_NEXT, "XCreateSimpleWindow");
    }

    Window window = xcsw(display, parent, x, y, width, height, border_width, border, background);

    // if atom is good and window is good...

    set_decocolour(display, window);

    return window;
}

