#include "viewportal.h"

Viewportal::Viewportal(QWidget *parent, int w, int h) : QLabel(parent) {
    setMouseTracking (true);
    setFixedSize(640,800);
    setGeometry(QRect(640, 0, 1280, 800));

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

    cbType = "d";

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePixmap()));
    timer->start(MAX_UPDATE_INTERVAL);
}

void Viewportal::resizeEvent(QResizeEvent *event) {
   x = width ();
   y = height ();
}

QString Viewportal::getColorblindType() const { return cbType; }

void Viewportal::setColorblindType(QString str) {
    if(str == "d" || str == "t" || str == "p") {
        cbType = str;
    } else {
        cbType = "d";
    }
}

void Viewportal::updatePixmap() {
    pixmap = QPixmap::grabWindow (QApplication::desktop () -> winId (), 0, 51, 640, 800);
    image = pixmap.toImage ();

    // image.invertPixels(); // Just to test

    // PUT THIS INTO ANOTHER THREAD!!!
    QString oldFile = QDir::currentPath()+"/image.jpg";
    QString newFile = QDir::currentPath()+"/"+getColorblindType()+"-image.jpg";

    image.save(oldFile);
    std::string daltonizeCmd = ("python /home/crabl/Colorblind/daltonize.py "+getColorblindType()+" "+oldFile).toStdString();
    std::string moveCmd = ("mv "+newFile+" "+oldFile).toStdString();

    system(daltonizeCmd.c_str());
    system(moveCmd.c_str());

    QImage image2 = QImage(oldFile);
    pixmap = QPixmap::fromImage (image2);
    setPixmap (pixmap);
}
