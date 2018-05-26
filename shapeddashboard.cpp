#include <QtWidgets>
#include <iostream>
#include <cmath>

#include "shapeddashboard.h"
#include "wifiapcollection.h"

int dots = 0;

# define MATH_PI 3.14159265358979323846  /* pi */

double doubleDegreeToDoubleRadian(double degree);

ShapedDashboard::ShapedDashboard(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint | Qt::WindowSystemMenuHint) {

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);

    QAction *quitAction = new QAction(tr("E&xit"), this);
    quitAction->setShortcut(tr("Ctrl+Q"));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    addAction(quitAction);

    setContextMenuPolicy(Qt::ActionsContextMenu);
    setToolTip(tr("Drag the Wifi Scanner window with the left mouse button.\n"
                  "Use the right mouse button to open a context menu."));
    setWindowTitle(tr("Wifi Scanner"));

}

void ShapedDashboard::setWifiAPCollection(WifiAPCollection *wifiAPCollection) {
    this->wifiAPCollection = wifiAPCollection;
}

void ShapedDashboard::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void ShapedDashboard::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void ShapedDashboard::paintEvent(QPaintEvent *) {

    QPainter painter(this);
    QRectF rectangle(0, 0, width(), height());

    std::vector<WifiAP> APs = this->wifiAPCollection->getAPs();

    if (APs.size() > 0) {

        // calculate maximal and minimal RSSI and sum of RSSIs
        signed int maxRSSI = -1000;
        signed int minRSSI = 1000;
        signed int sumRSSI = 0;
        for (auto it = APs.begin(); it != APs.end(); it++) {
            int currentRSSI = (*it).getRSSI();
            if (currentRSSI > maxRSSI) maxRSSI = currentRSSI;
            if (currentRSSI < minRSSI) minRSSI = currentRSSI;
            sumRSSI += currentRSSI;
        }

        // set ratio
        for (auto it = APs.begin(); it != APs.end(); it++) {
            double ratio = ((double)(*it).getRSSI() - (double)maxRSSI) / ((double)minRSSI - (double)maxRSSI);
            (*it).setRatio(ratio);
        }

        // calculate multiplier
        double multiplier = 360.0 / (double)sumRSSI;

        // draw pies
        QPen pen;
        QColor textColor = QColor(0, 0, 0, 255);

        double nextStartAngle = 0.0;
        for (auto it = APs.begin(); it != APs.end(); it++) {
            double currentAngle = ((double)(*it).getRSSI()) * multiplier;
            int startAngle = (int)(nextStartAngle * 16.0);
            int spanAngle = (int)(currentAngle * 16.0);
            QColor drawColor = QColor(0, 192, 0, (int)((*it).getRatio()*255.0));
            pen.setBrush(drawColor);
            painter.setPen(pen);
            painter.setBrush(drawColor);

            painter.drawPie(rectangle, startAngle, spanAngle);

            // draw text
            double textAngle = nextStartAngle + currentAngle / 2.0;
            double divider = 3.0;
            if (textAngle <= 90.0 || textAngle > 270.0) divider = 5.0;
            int textXCoord = (int) (((double)width() / 2.0) + (double)width() / divider * std::cos(doubleDegreeToDoubleRadian(textAngle)));

            // Take care! We need to subtract (-) the y coordinate (the part calculated using sin) from the center!)
            int textYCoord = (int) (((double)height() / 2.0) - (double)height() / divider * std::sin(doubleDegreeToDoubleRadian(textAngle)));
            pen.setBrush(textColor);
            painter.setPen(pen);
            painter.drawText(textXCoord, textYCoord, QString("• ") + QString((*it).getName().c_str()));
            nextStartAngle += currentAngle;
        }

    } else {
        QFont font = painter.font() ;

        /* twice the size than the current font size */
        font.setPointSize(font.pointSize() * 3);

        /* set the modified font to the painter */
        painter.setFont(font);

        /* draw text */
        QString scanning;
        if (dots == 0) {
            scanning = QString("Scanning Wifi.  ");
            dots++;
        } else if (dots == 1) {
            scanning = QString("Scanning Wifi . ");
            dots++;
        } else if (dots == 2) {
            scanning = QString("Scanning Wifi  .");
            dots++;
        } else if (dots == 3) {
            scanning = QString("Scanning Wifi . ");
            dots = 0;
        }

        painter.drawText(rectangle, Qt::AlignCenter, scanning);
    }

}

void ShapedDashboard::resizeEvent(QResizeEvent * /* event */) {
    int side = qMin(width(), height());
    QRegion maskedRegion(width() / 2 - side / 2, height() / 2 - side / 2, side,
                         side, QRegion::Ellipse);
    setMask(maskedRegion);
}

QSize ShapedDashboard::sizeHint() const {
    return QSize(450, 450);
}

double doubleDegreeToDoubleRadian(double degree) {
     return degree * MATH_PI / 180.0;
}
