#include "viewportal.h"

Viewportal::Viewportal(QWidget *parent, int w, int h) : QLabel(parent) {
    setMouseTracking (true);

    screenX = QApplication::desktop() -> screenGeometry().width();
    screenY = QApplication::desktop() -> screenGeometry().height();

    if (w < 0 || w > screenX) {
        x = 640;
    } else {
        x = w;
    }

    if (h < 0 || h > screenY) {
        y = 480;
    } else {
        y = h;
    }

    pixmap = QPixmap::grabWindow (QApplication::desktop()->winId(), 0, 0, x, y);
    setPixmap (pixmap);
}

void Viewportal::mouseMoveEvent(QMouseEvent *event) {
   int left = event -> globalX() - x / 2;
   if (left < 0)
      left = 0;
   else if (left + x > screenX)
      left = screenX - x;
   
   int top = event -> globalY () - y / 2;
   if (top < 0)
      top = 0;
   else if (top + y > screenY)
      top = screenY - y;
   pixmap = QPixmap::grabWindow (QApplication::desktop () -> winId (), left, top, x, y);
   //image = pixmap.toImage ();
   //image.invertPixels ();
   //pixmap = QPixmap::fromImage (image);
   setPixmap (pixmap);
}

void Viewportal::resizeEvent(QResizeEvent *event) {
   x = width ();
   y = height ();
}
