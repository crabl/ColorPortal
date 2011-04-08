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

    cbType = "Deuteranope";

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePixmap()));
    timer->start(MAX_UPDATE_INTERVAL);
}

QImage Viewportal::daltonize(const QImage &input, QString type) {
    float protanope[9] = {0.0, 2.02344, -2.52581, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
    float deuteranope[9] = {1.0, 0.0, 0.0, 0.494207, 0.0, 1.24827, 0.0, 0.0, 1.0};
    float tritanope[9] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -0.395913, 0.801109, 0.0};

    QImage temp = input.convertToFormat(QImage::Format_RGB32);

    float *cvd;

    if(type == "Protanope") {
        cvd = protanope;
    } else if(type == "Deuteranope") {
        cvd = deuteranope;
    } else if(type == "Tritanope") {
        cvd = tritanope;
    }

    float L, M, S, l, m, s, R, G, B, RR, GG, BB;
    for(int y = 0; y < temp.height(); ++y) {
        for(int x = 0; x < temp.width(); ++x) {
            QRgb origValue = temp.pixel(x,y);
            float r = qRed(origValue);
            float g = qGreen(origValue);
            float b = qBlue(origValue);

            // Convert the RGB values to LMS colorspace
            L = (17.8824 * r) + (43.5161 * g) + (4.11935 * b);
            M = (3.45565 * r) + (27.1554 * g) + (3.86714 * b);
            S = (0.0299566 * r) + (0.184309 * g) + (1.46709 * b);

            // Simulate color blindness using the corresponding matrix
            l = (cvd[0] * L) + (cvd[1] * M) + (cvd[2] * S);
            m = (cvd[3] * L) + (cvd[4] * M) + (cvd[5] * S);
            s = (cvd[6] * L) + (cvd[7] * M) + (cvd[8] * S);

            // Convert again, from LMS to RGB colorspace
            R = (0.0809444479 * l) + (-0.130504409 * m) + (0.116721066 * s);
            G = (-0.0102485335 * l) + (0.0540193266 * m) + (-0.113614708 * s);
            B = (-0.000365296938 * l) + (-0.00412161469 * m) + (0.693511405 * s);

            // Isolate invisible colors to color vision deficiency (calculate error matrix)
            R = r - R;
            G = g - G;
            B = b - B;

            // Shift colors towards visible spectrum (apply error modifications)
            RR = 0;
            GG = (0.7 * R) + G;
            BB = (0.7 * R) + B;

            // Add compensation to original values
            R = RR + r;
            G = GG + g;
            B = BB + b;

            // Clamp values

            R = fmin(fmax(R,0),255);
            G = fmin(fmax(G,0),255);
            B = fmin(fmax(B,0),255);

            // Record color
            QRgb newValue = qRgb(R,G,B);
            temp.setPixel(QPoint(x,y),newValue);
        }
    }

    return temp;
}

void Viewportal::resizeEvent(QResizeEvent *event) {
   x = width();
   y = height();
}

QString Viewportal::getColorblindType() const { return cbType; }

void Viewportal::setColorblindType(QString str) {
    if(str == "Deuteranope" || str == "Tritanope" || str == "Protanope") {
        cbType = str;
    } else {
        cbType = "Deuteranope";
    }
}

void Viewportal::updatePixmap() {
    pixmap = QPixmap::grabWindow (QApplication::desktop () -> winId (), 0, 51, 640, 800);
    QImage image = daltonize(pixmap.toImage(), getColorblindType());
    pixmap = QPixmap::fromImage(image);
    setPixmap (pixmap);
}
