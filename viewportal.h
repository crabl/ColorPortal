#ifndef VIEWPORTAL_H
#define VIEWPORTAL_H

#include <QtGui>
#include <QTimer>

class Viewportal : public QLabel {
   Q_OBJECT
   
  public:
   Viewportal(QWidget *parent = 0, int w = 640, int h = 480);

  protected:
   void mouseMoveEvent(QMouseEvent *event);
   void resizeEvent(QResizeEvent *event);

public slots:
   void updatePixmap();

  private:
   int screenX;
   int screenY;
   int x;
   int y;
   QPixmap pixmap;
   QImage image;
};

#endif
