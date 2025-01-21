//
// Created by WswDay2022 on 2024/12/6.
//

#include "myControls.h"

myTextButton::myTextButton(const QString &text, QWidget *parent)
    : QPushButton(text,parent) {
    setObjectName("myTextButton");
    int fontId = QFontDatabase::addApplicationFont("../assets/font.ttf");// 设置字体
    QFont customFont(QFontDatabase::applicationFontFamilies(fontId).at(0),10);
    setFont(customFont);
}

void myTextButton::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    QPainter painter(this);
    QStyleOptionButton option;
    initStyleOption(&option);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::NoPen);

    core core_;core_.globalInit();
    painter.setPen(core_.fontColor);
    painter.drawText(rect(), Qt::AlignCenter, option.text);
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
    core core_;core_.globalInit();
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
    core core_;core_.globalInit();
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

myTextButton::~myTextButton() {
}

void myTextButton::setTextColor(QColor color) {
    QPalette palette1 = palette();
    palette1.setColor(QPalette::ButtonText,color);
    setPalette(palette1);
}

QColor myTextButton::textColor() const {
    return m_color;
}

void myTextButton::setTextFillColor(QColor color) {
    if (m_color != color) {
        m_color = color;
        setTextColor(color);
        emit textColorChanged();
    }
}

void myTextButton::animateTextColor(const QColor &startColor, const QColor &endColor, int duration) {
    myAnimator animator(duration);
    animator.fontColorAnimation(this,startColor,endColor,true);
}



