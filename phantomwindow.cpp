// Christopher Rabl
// 001051542

#include "phantomwindow.h"

PhantomWindow::PhantomWindow(QWidget *parent) {
    // NO MAGIC NUMBERS, BRADLEY
    setMinimumSize(MIN_WIDTH, MIN_HEIGHT);

    // Create transparent window
    setStyleSheet("background:transparent;");
    setAttribute(Qt::WA_TranslucentBackground);

    // Window is always on top
    setWindowFlags(Qt::WindowStaysOnTopHint);

    // Uncomment this to make it a "Frameless" window
    //setWindowFlags(Qt::FramelessWindowHint);


    // WARNING: GYPSY MAGIC BELOW! PROCEED WITH CAUTION!
    Pixmap mask;

    // Create a new X11 pixmap (NOT A QPixmap!) of the entire display area
    // that is beneath the current window (which is always on top, although
    // not always in focus because of the magic below)
    mask = XCreatePixmap(
            QX11Info::display(),
            winId(),
            1, // width
            1, // height
            1  // depth
    );

    // Combine the pixmap (which catches the input received in the top-level
    // window) with a new pixmap encompassing the bottom layers of windows
    XShapeCombineMask(
            QX11Info::display(),
            winId(),
            ShapeInput,
            0, // x-offset
            0, // y-offset
            mask,
            ShapeSet
    );

    // Delete the mask
    XFreePixmap(QX11Info::display(), mask);

    // Mind... blown. You're welcome.
}

void PhantomWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    // This works too, input just goes right through it. Uncomment to see it in action!


    painter.setBrush(QBrush(Qt::red));
    painter.setOpacity(0.4);
    painter.drawRect(0, 0, QMainWindow::width(), QMainWindow::height());

}
