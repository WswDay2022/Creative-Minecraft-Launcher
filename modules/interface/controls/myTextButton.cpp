//
// Created by WswDay2022 on 2024/12/6.
//

#include "myControls.h"

myTextButton::myTextButton(const QString &text, QWidget *parent)
    : QPushButton(text,parent) {
    setControlStyle(CONTROL_INFO);
    setObjectName("myTextButton");
    setTextSize(this->width()/2);
    setFont(core::getFont());
}

myTextButton::~myTextButton() = default;

void myTextButton::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setPen(Qt::NoPen);  // 设置无笔，忽略文本绘制
    return QPushButton::paintEvent(event);
}

void myTextButton::setTextSize(int pointSize) {
    QFont font(this->font());
    font.setPointSize(pointSize);
    setFont(font);
}

void myTextButton::setTextFont(QFont font) {
    setFont(font);
}

void myTextButton::setControlStyle(controlType type) {
    type_ = type;
    update();
}

void myTextButton::enterEvent(QEnterEvent *event) {
    setCursor(Qt::PointingHandCursor);
    static core core_;
    core_.globalInit();
    switch (type_) {
        case CONTROL_INFO:
            animateTextColor(core_.fontColor,core_.themeColor,150);
        case CONTROL_ERROR:
            animateTextColor(core_.fontColor,core_.errColor,150);
        case CONTROL_WARING:
            animateTextColor(core_.fontColor,core_.warnColor,150);
    }
    return QPushButton::enterEvent(event);
}

void myTextButton::leaveEvent(QEvent *event) {
    static core core_;
    core_.globalInit();
    switch (type_) {
        case CONTROL_INFO:
            animateTextColor(core_.themeColor,core_.fontColor,150);
        case CONTROL_ERROR:
            animateTextColor(core_.errColor,core_.fontColor,150);
        case CONTROL_WARING:
            animateTextColor(core_.warnColor,core_.fontColor,150);
    }
    return QPushButton::leaveEvent(event);
}

void myTextButton::setTextColor(QColor color) {
    QPalette palette1 = palette();
    palette1.setColor(QPalette::ButtonText,color);
    setPalette(palette1);
}

void myTextButton::animateTextColor(const QColor &startColor, const QColor &endColor, int duration) {
    myAnimator animator(duration);
    animator.fontColorAnimation(this,startColor,endColor,true);
}



