//
// Created by WswDay2022 on 2024/12/14.
//

#include <utility>

#include "myControls.h"

myIconButton::myIconButton(const QIcon &icon,QWidget *parent)
    : QPushButton(parent),m_icon(icon) {
    setStyle(CONTROL_INFO);
    setButtonIcon(icon);
    setObjectName("myIconButton");
}

myIconButton::~myIconButton() = default;

void myIconButton::paintEvent(QPaintEvent *event) {
    return QPushButton::paintEvent(event);
}

void myIconButton::setButtonIcon(QIcon icon) {
    core core_;core_.globalInit();
    myControls tools;
    icon = tools.setIconColor(icon,core_.fontColor);
    setIcon(icon);
    setIconSize(QSize(this->width(),this->height()));
}

QColor myIconButton::iconColor() const { return m_iconColor; }

void myIconButton::setStyle(controlType type) {
    type_ = type;
    update();
}

void myIconButton::setIconColor(const QColor &color) {
    if (m_iconColor != color) {
        m_iconColor = color;
        myControls tools;
        setIcon(tools.setIconColor(m_icon, m_iconColor)); // 更新图标颜色
        emit iconColorChanged(); // 发出通知
    }
}

void myIconButton::animateIconColor(const QColor &startColor, const QColor &endColor, int duration) {
    QPropertyAnimation *animation = new QPropertyAnimation(this, "iconColor");
    animation->setDuration(duration);
    animation->setStartValue(startColor);
    animation->setEndValue(endColor);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->start();
}

void myIconButton::enterEvent(QEnterEvent *event) {
    QWidget::enterEvent(event);
    core core_;
    core_.globalInit();
    switch (type_) {
        case CONTROL_INFO:
            animateIconColor(core_.fontColor,core_.themeColor,150);
            break;
        case CONTROL_ERROR:
            animateIconColor(core_.fontColor,core_.errColor,150);
            break;
        case CONTROL_WARING:
            animateIconColor(core_.fontColor,core_.warnColor,150);
            break;
    }
}

void myIconButton::leaveEvent(QEvent *event) {
    QWidget::leaveEvent(event);
    core core_;
    core_.globalInit();
    switch (type_) {
        case CONTROL_INFO:
            animateIconColor(core_.themeColor,core_.fontColor,150);
            break;
        case CONTROL_ERROR:
            animateIconColor(core_.errColor,core_.fontColor,150);
            break;
        case CONTROL_WARING:
            animateIconColor(core_.warnColor,core_.fontColor,150);
            break;
    }
}