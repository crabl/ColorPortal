// Christopher Rabl
// 001051542
// This class contains gypsy magic

#ifndef PHANTOMWINDOW_H_
#define PHANTOMWINDOW_H_

#include <QWidget>
#include <QMainWindow>
#include <QPainter>

// DON'T MOVE THESE ANYWHERE
#include <X11/Xlib.h>
#include <X11/extensions/shape.h>
#include <QX11Info>

// These will be set to the height and width of the user's screen
const int MIN_WIDTH = 640;
const int MIN_HEIGHT = 480;

class PhantomWindow : public QMainWindow
{
    Q_OBJECT

public:
    PhantomWindow(QWidget *parent = 0);

private:
    void paintEvent(QPaintEvent *);

};

#endif // PHANTOMWINDOW_H_
