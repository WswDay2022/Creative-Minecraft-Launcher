//
// Created by WswDay2022 on 2024/11/30.
//

#include "myControls.h"

myButton::myButton(const QString &text, QWidget *parent)
    : QPushButton(text,parent) {
    setObjectName("myButton");
    setStyle(CONTROL_INFO);
    setFixedHeight(40);

    shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(10);
    shadowEffect->setXOffset(0);
    shadowEffect->setYOffset(1);
    shadowEffect->setColor(QColor(0, 0, 0,150));
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
            break;
        case CONTROL_ERROR:
            painter.setPen(core_.errColor);
            break;
        case CONTROL_WARING:
            painter.setPen(core_.warnColor);
            break;
    }

    painter.drawRoundedRect(rect(), 6, 6);
    painter.drawText(rect(), Qt::AlignCenter, option.text);
}

void myButton::enterEvent(QEnterEvent *event) {
    setCursor(Qt::PointingHandCursor);
    core core_;core_.globalInit();
    colorAnimation->setStartValue(QColor(0, 0, 0, 150));
    Q_UNUSED(event)

    if (type_ == CONTROL_INFO) {
        colorAnimation->setEndValue(core_.themeColor);
    } else if (type_ == CONTROL_ERROR) {
        colorAnimation->setEndValue(core_.errColor);
    } else if (type_ == CONTROL_WARING) {
        colorAnimation->setEndValue(core_.warnColor);
    }

    colorAnimation->start();
}

void myButton::leaveEvent(QEvent *event) {
    core core_;core_.globalInit();
    colorAnimation->setEndValue(QColor(0, 0, 0, 150));
    Q_UNUSED(event)

    if (type_ == CONTROL_INFO) {
        colorAnimation->setStartValue(core_.themeColor);
    } else if (type_ == CONTROL_ERROR) {
        colorAnimation->setStartValue(core_.errColor);
    } else if (type_ == CONTROL_WARING){
        colorAnimation->setStartValue(core_.warnColor);
    }

    colorAnimation->start();
}

void myButton::setSize(QSize size) {
    setMaximumSize(size);
    setMinimumSize(size-QSize(20,20));
}

void myButton::setSize(int w, int h) {
    setMaximumSize(w,h);
    setMinimumSize(w-20,h-20);
}

void myButton::mousePressEvent(QMouseEvent *event) {
    // 创建动画对象，控制按钮的缩放效果
    auto *animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(100); // 动画持续时间
    animation->setStartValue(geometry()); // 起始位置
    animation->setEndValue(geometry().adjusted(2, 2, -4, -4));

    auto *returnAnimation = new QPropertyAnimation(this, "geometry");
    returnAnimation->setDuration(100);
    returnAnimation->setStartValue(animation->endValue());
    returnAnimation->setEndValue(geometry()); // 返回到原来的位置

    if (!m_isAnimating) { // 防止多次触发
        m_isAnimating = true;
        animation->start(QAbstractAnimation::DeleteWhenStopped);
        connect(animation, &QPropertyAnimation::finished, [=]() {
            returnAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        });
        connect(returnAnimation, &QPropertyAnimation::finished, [=]() {
            m_isAnimating = false; // 动画结束，允许再次点击
            returnAnimation->deleteLater();
        });
    }

    shadowEffect->setBlurRadius(10);
    return QPushButton::mousePressEvent(event);
}

void myButton::mouseReleaseEvent(QMouseEvent *event) {
    shadowEffect->setBlurRadius(10);
    return QPushButton::mouseReleaseEvent(event);
}

myButton::~myButton() = default;