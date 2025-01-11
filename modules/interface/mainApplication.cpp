//
// Created by WswDay2022 on 2024/11/30.
//

#include "mainApplication.h"

mainApplication::mainApplication(QWidget *parent)
    : baseWindow(parent) {
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(10);
    shadowEffect->setXOffset(0);
    shadowEffect->setYOffset(1);
    shadowEffect->setColor(QColor(0, 0, 0, 100));
    setGraphicsEffect(shadowEffect);

    setObjectName("application");
    core core_;
    core_.globalInit();
}

mainApplication::~mainApplication() {
    qDebug() << "Application destructed.";
}

void mainApplication::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    QPainter painter(this);
    drawRoundedRect(painter);
}

void mainApplication::drawRoundedRect(QPainter &painter) {
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    core core_;core_.globalInit();
    int m_colorR = core_.getSettingJson()["themeColor"][0].asInt();
    int m_colorG = core_.getSettingJson()["themeColor"][1].asInt();
    int m_colorB = core_.getSettingJson()["themeColor"][2].asInt();
    QColor color(m_colorR,m_colorG,m_colorB);
    QColor lighterColor = color.lighter(160);
    painter.setBrush(lighterColor);
    painter.drawRoundedRect(rect().adjusted(10,10,-10,-10), 10, 10);
}