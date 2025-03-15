//
// Created by WswDay2022 on 2024/12/14.
//

#include <QGraphicsRotation>
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
    static core core_;
    core_.globalInit();
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
    static core core_;
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
    static core core_;
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
    // 创建动画对象，控制按钮的缩放效果
    myAnimator animator(100);
    auto *animation = animator.iconSizeButtonAnimation(this,iconSize(),iconSize()-QSize(2,2),false);
    auto *returnAnimation = animator.iconSizeButtonAnimation(this,animation->endValue().toSize(),animation->startValue().toSize(),false);

    if (event->button() == Qt::LeftButton) {
        if (!m_isAnimating) { // 防止多次触发
            m_isAnimating = true;
            auto *iconGroup = new QSequentialAnimationGroup(this);
            iconGroup->addAnimation(animation);
            iconGroup->addAnimation(returnAnimation);
            iconGroup->start(QAbstractAnimation::DeleteWhenStopped);

            connect(iconGroup, &QPropertyAnimation::finished, [=]() {
                m_isAnimating = false; // 动画结束，允许再次点击
                iconGroup->deleteLater();
            });
        }
    }

    return QPushButton::mousePressEvent(event);
}
