//
// Created by WswDay2022 on 2024/12/6.
//

#include "myControls.h"

myContentCard::myContentCard(QWidget *parent)
    : QWidget(parent) {
    setObjectName("myContentCard");
    setStyleSheet("background-color:white");
    setAutoFillBackground(true);
    shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(10);
    shadowEffect->setXOffset(0);
    shadowEffect->setYOffset(1);
    shadowEffect->setColor(QColor(0, 0, 0, 100));

    setGraphicsEffect(shadowEffect);
    colorAnimation = new QPropertyAnimation(shadowEffect, "color");
    colorAnimation->setDuration(150);

    auto *layout = new QHBoxLayout();
    setLayout(layout);
}

myContentCard::~myContentCard() = default;

void myContentCard::paintEvent(QPaintEvent *event) {
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