//
// Created by WswDay2022 on 2024/12/14.
//

#include "myControls.h"

myIconButton::myIconButton(const QIcon &icon,QWidget *parent)
    : QPushButton(parent),m_icon(icon) {
    setObjectName("myIconButton");
    setControlStyle(CONTROL_INFO);
    setButtonIcon(icon);
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

void myIconButton::setControlStyle(controlType type) {
    type_ = type;
    update();
}

void myIconButton::setSize(QSize size) {
    setIconSize(size);
    setMaximumSize(size);
    setMinimumSize(size-QSize(4,4));
}

void myIconButton::setSize(int w, int h) {
    setIconSize(QSize(w,h));
    setMaximumSize(w,h);
    setMinimumSize(w-4,h-4);
}

void myIconButton::animateIconColor(const QColor &startColor, const QColor &endColor, int duration) {
    myAnimator animator(duration);
    animator.iconColorButtonAnimation(this,startColor,endColor,true)->setEasingCurve(QEasingCurve::InOutQuad);
}

void myIconButton::enterEvent(QEnterEvent *event) {
    setCursor(Qt::PointingHandCursor);
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

void myIconButton::mousePressEvent(QMouseEvent *event) {
    myAnimator animator(100);
    auto *animation = animator.iconSizeButtonAnimation(this,iconSize(),iconSize()-QSize(4,4),false);
    auto *returnAnimation = animator.iconSizeButtonAnimation(this,animation->endValue().toSize(),animation->startValue().toSize(),false);

    if (event->button() == Qt::LeftButton) {
        if (!m_isAnimating) { // 防止多次触发
            m_isAnimating = true;
            //animation->start(QAbstractAnimation::DeleteWhenStopped);
            animation->start(QAbstractAnimation::DeleteWhenStopped);
            connect(animation, &QPropertyAnimation::finished, [=]() {
                returnAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            });
            connect(returnAnimation, &QPropertyAnimation::finished, [=]() {
                m_isAnimating = false; // 动画结束，允许再次点击
                returnAnimation->deleteLater();
            });
        }
    }

    return QPushButton::mousePressEvent(event);
}

void myIconButton::rotationAnimation(const int &startValue, const int &endValue, int duration) {
    QPropertyAnimation* animation = new QPropertyAnimation(this, "rotationAngle");
    animation->setDuration(duration);
    animation->setStartValue(startValue);
    animation->setEndValue(endValue);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void myIconButton::setRotationAngle(qreal angle) {
    if (m_rotationAngle != angle) {
        m_rotationAngle = angle;
        update();
    }
}
