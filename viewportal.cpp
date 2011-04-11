#include "viewportal.h"
#include <iostream>

Viewportal::Viewportal(QWidget *parent) :
        QLabel(parent), cbType("Deuteranope"), simulate(false) {
    screenX = QApplication::desktop() -> screenGeometry().width();
    screenY = QApplication::desktop() -> screenGeometry().height();

    setFixedSize(screenX/2,screenY);
    setGeometry(QRect(screenX/2, 0, screenX, screenY));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePixmap()));
    timer->start(MAX_UPDATE_INTERVAL);
}

QImage Viewportal::daltonize(const QImage &input, QString type) {
    float protanope[9] = {0.0, 2.02344, -2.52581, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
    float deuteranope[9] = {1.0, 0.0, 0.0, 0.494207, 0.0, 1.24827, 0.0, 0.0, 1.0};
    float tritanope[9] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -0.395913, 0.801109, 0.0};

    QImage temp = input.convertToFormat(QImage::Format_RGB32);

    float *cMatrix = NULL;

    if(type == "Protanope") {
        cMatrix = protanope;
    } else if(type == "Deuteranope") {
        cMatrix = deuteranope;
    } else if(type == "Tritanope") {
        cMatrix = tritanope;
    }

    // This code was adapted by Christopher Rabl from the Javascript implementation of
    // the LMS Daltonization Algorithm found at http://www.daltonize.org/
    // Code: http://mudcu.be/labs/Color/Vision/Javascript/Color.Vision.Daltonize.js
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
            l = (cMatrix[0] * L) + (cMatrix[1] * M) + (cMatrix[2] * S);
            m = (cMatrix[3] * L) + (cMatrix[4] * M) + (cMatrix[5] * S);
            s = (cMatrix[6] * L) + (cMatrix[7] * M) + (cMatrix[8] * S);

            // Convert again, from LMS to RGB colorspace
            R = (0.0809444479 * l) + (-0.130504409 * m) + (0.116721066 * s);
            G = (-0.0102485335 * l) + (0.0540193266 * m) + (-0.113614708 * s);
            B = (-0.000365296938 * l) + (-0.00412161469 * m) + (0.693511405 * s);

            if(!isSimulation()) {
                // Isolate invisible colors to color vision deficiency (calculate error matrix)
                R = r - R;
                G = g - G;
                B = b - B;
            }

            // Shift colors towards visible spectrum (apply error modifications)
            RR = 0;
            GG = (0.7 * R) + G;
            BB = (0.7 * R) + B;

            // Add compensation to original values
            R = RR + r;
            G = GG + g;
            B = BB + b;

            // Ensure values fall within the correct range
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

bool Viewportal::event(QEvent *e) {

    bool result = QLabel::event(e);
    if(e->type() == QEvent::MouseButtonPress) {
        QString type = getColorblindType();
        if(type == "Deuteranope") {
            setColorblindType("Protanope");
        } else if(type == "Protanope") {
            setColorblindType("Tritanope");
        } else {
            setColorblindType("Deuteranope");
        }

        result = true;
    }

    return result;
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
    pixmap = QPixmap::grabWindow(QApplication::desktop()->winId (), 0, 51, screenX/2, screenY);
    setSimulation(false);
    QImage image = daltonize(pixmap.toImage(), getColorblindType());
    pixmap = QPixmap::fromImage(image);
    setPixmap (pixmap);
}
