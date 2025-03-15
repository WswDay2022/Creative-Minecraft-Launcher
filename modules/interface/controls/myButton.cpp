//
// Created by WswDay2022 on 2024/11/30.
//

#include "myControls.h"

myButton::myButton(const QString &text, QWidget *parent)
    : QPushButton(text,parent) {
    setObjectName("myButton");
    setControlStyle(CONTROL_INFO);

    shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(10);
    shadowEffect->setXOffset(0);
    shadowEffect->setYOffset(1);
    shadowEffect->setColor(QColor(0, 0, 0,150));
    setGraphicsEffect(shadowEffect);

    colorAnimation = new QPropertyAnimation(shadowEffect, "color");
    colorAnimation->setTargetObject(shadowEffect);
    colorAnimation->setParent(shadowEffect);
    colorAnimation->setDuration(150);
    setFont(core::getFont());
}

myButton::~myButton() = default;

void myButton::setControlStyle(controlType type) {
    type_ = type;
    update();
}

void myButton::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    QPainter painter(this);
    QStyleOptionButton option;
    initStyleOption(&option);
    static core core_; // 静态对象避免重复创建
    core_.globalInit();

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
    static core core_; // 静态对象避免重复创建
    core_.globalInit();
    colorAnimation->setStartValue(QColor(0, 0, 0, 150));

    if (type_ == CONTROL_INFO) {
        colorAnimation->setEndValue(core_.themeColor);
    } else if (type_ == CONTROL_ERROR) {
        colorAnimation->setEndValue(core_.errColor);
    } else if (type_ == CONTROL_WARING) {
        colorAnimation->setEndValue(core_.warnColor);
    }

    colorAnimation->start();
    return QPushButton::enterEvent(event);
}

void myButton::leaveEvent(QEvent *event) {
    colorAnimation->setEndValue(QColor(0, 0, 0, 150));
    static core core_; // 静态对象避免重复创建
    core_.globalInit();

    if (type_ == CONTROL_INFO) {
        colorAnimation->setStartValue(core_.themeColor);
    } else if (type_ == CONTROL_ERROR) {
        colorAnimation->setStartValue(core_.errColor);
    } else if (type_ == CONTROL_WARING){
        colorAnimation->setStartValue(core_.warnColor);
    }

    colorAnimation->start();
    return QPushButton::leaveEvent(event);
}

void myButton::setSize(QSize size) {
    setMaximumSize(size);
    setMinimumSize(size-QSize(4,4));
}

void myButton::setSize(int w, int h) {
    setMaximumSize(w,h);
    setMinimumSize(w-4,h-4);
}

void myButton::mousePressEvent(QMouseEvent *event) {
    // 创建动画对象，控制按钮的缩放效果
    myAnimator animator(100);
    auto *animation = animator.geometryAnimation(this,this->geometry(),this->geometry().adjusted(2,2,-4,-4),false);
    auto *returnAnimation = animator.geometryAnimation(this,animation->endValue().toRect(),animation->startValue().toRect(),false);

    if (event->button() == Qt::LeftButton) {
        if (!m_isAnimating) { // 防止多次触发
            m_isAnimating = true;
            auto *sizeGroup = new QSequentialAnimationGroup(this);
            sizeGroup->addAnimation(animation);
            sizeGroup->addAnimation(returnAnimation);

            auto *groups = new QParallelAnimationGroup(this);
            groups->setParent(this);
            groups->addAnimation(sizeGroup);
            groups->start(QAbstractAnimation::DeleteWhenStopped);
            animation->start(QAbstractAnimation::DeleteWhenStopped);
            connect(sizeGroup, &QParallelAnimationGroup::finished,[=]() {
                m_isAnimating = false; // 动画结束，允许再次点击
                sizeGroup->deleteLater();
            });
        }
    }

    shadowEffect->setBlurRadius(10);
    return QPushButton::mousePressEvent(event);
}

void myButton::mouseReleaseEvent(QMouseEvent *event) {
    shadowEffect->setBlurRadius(10);
    return QPushButton::mouseReleaseEvent(event);
}