#ifndef VIEWPORTAL_H
#define VIEWPORTAL_H

#include "math.h"
#include <QtGui>
#include <QTimer>

const int MAX_UPDATE_INTERVAL = 40; // milliseconds

class Viewportal : public QLabel {
    Q_OBJECT

public:
    Viewportal(QWidget *parent = 0);

protected:
    QImage daltonize(const QImage&, QString);
    QString getColorblindType() const;
    bool event(QEvent*);

public slots:
    void updatePixmap();
    void setColorblindType(QString);

private:
    int screenX;
    int screenY;
    int x;
    int y;
    QPixmap pixmap;
    QString cbType;
};

#endif
