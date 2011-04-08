#ifndef VIEWPORTAL_H
#define VIEWPORTAL_H

#include "math.h"
#include <QtGui>
#include <QTimer>

const int MAX_UPDATE_INTERVAL = 30; // milliseconds

class Viewportal : public QLabel {
    Q_OBJECT

public:
    Viewportal(QWidget *parent = 0, int w = 640, int h = 480);

protected:
    void resizeEvent(QResizeEvent *event);
    QImage daltonize(const QImage&, QString);
    QString getColorblindType() const;

public slots:
    void updatePixmap();
    void setColorblindType(QString);

private:
    int screenX;
    int screenY;
    int x;
    int y;
    QPixmap pixmap;
    QImage image;
    QString cbType;
};

#endif
