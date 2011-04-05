#include "viewportal.h"

Viewportal::Viewportal(QWidget *parent, int w, int h) : QLabel(parent) {
    setMouseTracking (true);

    setFixedSize(640,800);


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

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePixmap()));
    timer->start(40);


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
    updatePixmap();
}

void Viewportal::resizeEvent(QResizeEvent *event) {
   x = width ();
   y = height ();
}

void Viewportal::updatePixmap() {
    pixmap = QPixmap::grabWindow (QApplication::desktop () -> winId (), 0, 51, 640, 800);
    image = pixmap.toImage ();
    image.invertPixels();
    pixmap = QPixmap::fromImage (image);
    setPixmap (pixmap);
}
