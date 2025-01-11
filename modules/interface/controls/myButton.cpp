//
// Created by WswDay2022 on 2024/11/30.
//

#include "myControls.h"

myButton::myButton(const QString &text, QWidget *parent)
    : QPushButton(text,parent), tempRect(this->rect()) {
    setObjectName("myButton");
    setStyle(CONTROL_INFO);
    setFixedHeight(40);
    shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(10);
    shadowEffect->setXOffset(0);
    shadowEffect->setYOffset(1);

    shadowEffect->setColor(QColor(0, 0, 0,200));
    setGraphicsEffect(shadowEffect);

    colorAnimation = new QPropertyAnimation(shadowEffect,"color");
    colorAnimation->setDuration(150);

    int fontId = QFontDatabase::addApplicationFont("../assets/font.ttf");
    QFont customFont(QFontDatabase::applicationFontFamilies(fontId).at(0),10);
    setFont(customFont);
}

void myButton::setStyle(controlType type) {
    type_ = type;
    update();
}

void myButton::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    QPainter painter(this);
    QStyleOptionButton option;
    initStyleOption(&option);

    core core_;core_.globalInit();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::white);
    painter.setPen(Qt::white);
    painter.drawRoundedRect(rect(), 6, 6);
    switch (type_) {
        case CONTROL_INFO:
            painter.setPen(core_.themeColor);
            painter.drawRoundedRect(rect(), 6, 6);
            painter.drawText(rect(), Qt::AlignCenter, option.text);
            break;
        case CONTROL_ERROR:
            painter.setPen(core_.errColor);
            painter.drawRoundedRect(rect(), 6, 6);
            painter.drawText(rect(), Qt::AlignCenter, option.text);
            break;
        case CONTROL_WARING:
            painter.setPen(core_.warnColor);
            painter.drawRoundedRect(rect(), 6, 6);
            painter.drawText(rect(), Qt::AlignCenter, option.text);
            break;
    }
}

void myButton::enterEvent(QEnterEvent *event) {
    setCursor(Qt::PointingHandCursor);
    core core_;core_.globalInit();
    Q_UNUSED(event)
    if (type_ == CONTROL_INFO) {
        colorAnimation->setStartValue(QColor(0, 0, 0, 200));
        colorAnimation->setEndValue(core_.themeColor);
        colorAnimation->start();
    } else if (type_ == CONTROL_ERROR) {
        colorAnimation->setStartValue(QColor(0, 0, 0, 200));
        colorAnimation->setEndValue(core_.errColor);
        colorAnimation->start();
    } else if (type_ == CONTROL_WARING) {
        colorAnimation->setStartValue(QColor(0, 0, 0, 200));
        colorAnimation->setEndValue(core_.warnColor);
        colorAnimation->start();
    }
}

void myButton::leaveEvent(QEvent *event) {
    core core_;core_.globalInit();
    Q_UNUSED(event)
    if (type_ == CONTROL_INFO) {
        colorAnimation->setStartValue(core_.themeColor);
        colorAnimation->setEndValue(QColor(0, 0, 0, 200));
        colorAnimation->start();
    } else if (type_ == CONTROL_ERROR) {
        colorAnimation->setStartValue(core_.errColor);
        colorAnimation->setEndValue(QColor(0, 0, 0, 200));
        colorAnimation->start();
    } else if (type_ == CONTROL_WARING){
        colorAnimation->setStartValue(core_.warnColor);
        colorAnimation->setEndValue(QColor(0, 0, 0, 200));
        colorAnimation->start();
    }
}

void myButton::setSize(QSize size) {
    setMaximumSize(size);
    setMinimumSize(size-QSize(20,20));
    tempRect = QRect(this->x(),this->y(),size.width(),size.height());
}

void myButton::setSize(int w, int h) {
    setMaximumSize(w,h);
    setMinimumSize(w-20,h-20);
    tempRect = QRect(this->x(),this->y(),w,h);
}

void myButton::mousePressEvent(QMouseEvent *event) {
    auto *pressAnimation = new QPropertyAnimation(this, "geometry");
    pressAnimation->setDuration(150);  // 动画持续时间
    pressAnimation->setStartValue(tempRect);
    pressAnimation->setEndValue(QRect(this->x()+3,this->y()+3,this->width()-6,this->height()-6));  // 按下时缩小90%
    tempRect = this->rect();
    pressAnimation->start();
    shadowEffect->setBlurRadius(10);
    return QPushButton::mousePressEvent(event);
}

void myButton::mouseReleaseEvent(QMouseEvent *event) {
    auto *releaseAnimation = new QPropertyAnimation(this, "geometry");
    releaseAnimation->setDuration(150);
    releaseAnimation->setStartValue(tempRect);
    releaseAnimation->setEndValue(QRect(this->x()-3,this->y()-3,this->width()+6,this->height()+6));  // 按下时缩小90
    tempRect = this->rect();
    releaseAnimation->start();
    shadowEffect->setBlurRadius(10);
    return QPushButton::mouseReleaseEvent(event);
}

myButton::~myButton() {
}


