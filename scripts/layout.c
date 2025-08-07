// gcc layout.c -o layout -lX11 -lcairo -lX11-xcb -lxcb -lXau -lXdmcp

#include <X11/XKBlib.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cairo/cairo-xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define WINDOW_CLASS "KeyboardLayoutIndicator"

char *GetLayoutName(Display *display);
static void refresh(Display *display, Window win, int screen, cairo_t *cr,
                    cairo_surface_t *sfc);
Window find_existing_window(Display *display, Window root);

static const char *textfamily = "JetBrains Mono";
static const int textsize = 50;
static int sizex = 150, sizey = 70;

Window find_existing_window(Display *display, Window root) {
  Atom actual_type;
  int actual_format;
  unsigned long nitems, bytes_after;
  unsigned char *data = NULL;
  Window *children = NULL;
  Window found = 0;
  unsigned int nchildren;

  if (!XQueryTree(display, root, &root, &root, &children, &nchildren))
    return 0;

  for (unsigned int i = 0; i < nchildren; i++) {
    Atom prop = XInternAtom(display, "WM_CLASS", False);

    if (XGetWindowProperty(display, children[i], prop, 0, 1024, False,
                           XA_STRING, &actual_type, &actual_format, &nitems,
                           &bytes_after, &data) == Success &&
        data) {
      if (strstr((char *)data, WINDOW_CLASS) != NULL) {
        found = children[i];
        XFree(data);
        break;
      }
      XFree(data);
      data = NULL;
    }
  }

  if (children)
    XFree(children);
  return found;
}

static void refresh(Display *display, Window win, int screen, cairo_t *cr,
                    cairo_surface_t *sfc) {
  const char *layout = GetLayoutName(display);

  XClearWindow(display, win);

  cairo_select_font_face(cr, textfamily, CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size(cr, textsize);
  cairo_set_source_rgb(cr, 1, 1, 1);

  cairo_text_extents_t extents;
  cairo_text_extents(cr, layout, &extents);
  double xpos = (sizex - extents.width) / 2;
  double ypos = (sizey + extents.height) / 2.15;

  cairo_move_to(cr, xpos, ypos);
  cairo_show_text(cr, layout);

  cairo_surface_flush(sfc);
  XFlush(display);

  free((void *)layout);
}

char *GetLayoutName(Display *display) {
  XkbStateRec state;
  XkbGetState(display, XkbUseCoreKbd, &state);

  int layoutNumber = state.group;

  const char *layoutNames[] = {"US", "RU"};

  if (layoutNumber >= 0 &&
      layoutNumber < sizeof(layoutNames) / sizeof(layoutNames[0])) {
    return strdup(layoutNames[layoutNumber]);
  } else {
    return strdup("Unknown");
  }
}

int main(void) {
  Display *display;
  unsigned int statekb;

  if ((display = XOpenDisplay(NULL)) == NULL) {
    puts("Can not connect to the X server!\n");
    return 1;
  }

  int screen = DefaultScreen(display);
  Window root = RootWindow(display, screen);

  Window win = find_existing_window(display, root);
  int create_new = (win == 0);

  if (create_new) {
    XSetWindowAttributes attrs;
    attrs.override_redirect = True;
    attrs.background_pixel = 0x3b3b3b;

    int x = (DisplayWidth(display, screen) - sizex) / 2;
    int y = (DisplayHeight(display, screen) / 1.4);

    win = XCreateWindow(display, root, x, y, sizex, sizey, 0, CopyFromParent,
                        InputOutput, CopyFromParent,
                        CWOverrideRedirect | CWBackPixel, &attrs);

    XClassHint *class_hint = XAllocClassHint();
    class_hint->res_name = "kblayout";
    class_hint->res_class = WINDOW_CLASS;
    XSetClassHint(display, win, class_hint);
    XFree(class_hint);
  }

  cairo_surface_t *sfc = cairo_xlib_surface_create(
      display, win, DefaultVisual(display, screen), 300, 100);
  cairo_t *cr = cairo_create(sfc);

  if (create_new) {
    XMapWindow(display, win);
  }
  XFlush(display);

  refresh(display, win, screen, cr, sfc);

  if (create_new) {
    sleep(1);
  }

  cairo_destroy(cr);
  cairo_surface_destroy(sfc);

  if (create_new) {
    XDestroyWindow(display, win);
  }

  XCloseDisplay(display);

  return 0;
}
