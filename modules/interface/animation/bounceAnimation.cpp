//
// Created by WswDay2022 on 2024/12/13.
//

#include "bounceAnimation.h"

bounceAnimation::bounceAnimation(QWidget *target, QObject *parent)
        : QObject(parent), m_target(target), m_currentScale(1.0) {
    m_animation = new QPropertyAnimation(this, "scale");
    m_animation->setDuration(150);
    m_animation->setKeyValueAt(0.0, 1.0); // 起始缩放
    m_animation->setKeyValueAt(0.5, 0.8); // 按下缩放
    m_animation->setKeyValueAt(1.0, 1.0); // 释放时恢复
    m_animation->setEasingCurve(QEasingCurve::OutBounce); // 设置为回弹效果
}

void bounceAnimation::startForward() {
    m_animation->setDirection(QPropertyAnimation::Forward);
    m_animation->start();
}

void bounceAnimation::startBackward() {
    m_animation->setDirection(QPropertyAnimation::Backward);
    m_animation->start();
}

qreal bounceAnimation::scale() const {
    return m_currentScale;
}

void bounceAnimation::setScale(qreal value) {
    if (value != m_currentScale) {
        m_currentScale = value;
        applyScale(value); // 更新控件尺寸
    }
}

void bounceAnimation::applyScale(qreal scale) {
    QSize size = m_target->size();
    QSize newSize(size.width() * scale, size.height() * scale);
    m_target->resize(newSize); // 根据缩放值调整大小
    m_target->update(); // 更新目标控件
}