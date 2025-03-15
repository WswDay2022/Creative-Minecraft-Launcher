//
// Created by WswDay2022 on 2024/11/30.
//

#include "mainApplication.h"

mainApplication::mainApplication(QWidget *parent)
    : baseWindow(parent) {
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    QGraphicsDropShadowEffect *shadowEffect;
    shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(10);
    shadowEffect->setXOffset(0);
    shadowEffect->setYOffset(1);
    shadowEffect->setColor(QColor(0, 0, 0, 100));
    setGraphicsEffect(shadowEffect);

    setObjectName("application");
}

mainApplication::~mainApplication() {
    LogPrint("[MAIN APPLICATION]:窗口已关闭","INFO");
}

void mainApplication::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    QPainter painter(this);
    drawRoundedRect(painter);
}

void mainApplication::drawRoundedRect(QPainter &painter) {
    painter.setRenderHint(QPainter::Antialiasing);
    core core_;core_.globalInit();
    painter.setPen(core_.themeColor);
    QColor lighterColor = core_.themeColor.lighter(160);
    painter.setBrush(lighterColor);
    painter.drawRoundedRect(rect().adjusted(10,10,-10,-10), 10, 10);
}