#ifndef SHAPEDDASHBOARD_H
#define SHAPEDDASHBOARD_H

#include <QWidget>

#include "wifiapcollection.h"

class ShapedDashboard : public QWidget {
    Q_OBJECT

public:
    ShapedDashboard(QWidget *parent = 0);
    QSize sizeHint() const override;
    void setWifiAPCollection(WifiAPCollection *wifiAPCollection);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QPoint dragPosition;
    WifiAPCollection *wifiAPCollection;
};

#endif
