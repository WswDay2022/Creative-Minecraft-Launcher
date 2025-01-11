//
// Created by WswDay2022 on 2024/12/6.
//

#include "myControls.h"

myContentCard::myContentCard(QWidget *parent)
    : QWidget(parent) {
    setObjectName("contentCard");
    setStyleSheet("background-color:white");
    setAutoFillBackground(true);
    shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(10);
    shadowEffect->setXOffset(0);
    shadowEffect->setYOffset(1);
    shadowEffect->setColor(QColor(0, 0, 0, 100));
    setGraphicsEffect(shadowEffect);
    colorAnimation = new QPropertyAnimation(shadowEffect, "color");
    colorAnimation->setDuration(300);
}

myContentCard::~myContentCard() {
}

void myContentCard::paintEvent(QPaintEvent *event) {
    /*
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(255, 255,255));

    painter.setPen(Qt::NoPen);
    core core_;core_.globalInit();
    QColor lighterColor = core_.themeColor.lighter(160);
    painter.setBrush(lighterColor);
    painter.drawRoundedRect(rect(), 6, 6);

    QPainterPath pathBack;
    pathBack.setFillRule(Qt::FillRule::WindingFill);
    pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()+10).adjusted(10,10,-10,10), 10, 10);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(pathBack, QBrush(QColor(255,255,255)));*/

    return QWidget::paintEvent(event);
}

void myContentCard::enterEvent(QEnterEvent *event) {
    core core_;core_.globalInit();
    colorAnimation->setStartValue(QColor(0, 0, 0,200));
    colorAnimation->setEndValue(core_.themeColor);
    colorAnimation->start();
    return QWidget::enterEvent(event);
}

void myContentCard::leaveEvent(QEvent *event) {
    core core_;core_.globalInit();
    colorAnimation->setStartValue(core_.themeColor);
    colorAnimation->setEndValue(QColor(0, 0, 0,200));
    colorAnimation->start();
    return QWidget::leaveEvent(event);
}