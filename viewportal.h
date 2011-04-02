#ifndef VIEWPORTAL_H
#define VIEWPORTAL_H

#include <QtGui>

class Viewportal : public QLabel
{
   Q_OBJECT
   
  public:
   Viewportal(QWidget *parent = 0, int w = 640, int h = 480);

  protected:
   void mouseMoveEvent(QMouseEvent *event);
   void resizeEvent(QResizeEvent *event);

  private:
   int screenX;
   int screenY;
   int x;
   int y;
   QPixmap pixmap;
   QImage image;
};

#endif
