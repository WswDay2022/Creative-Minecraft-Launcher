//
// Created by WswDay2022 on 2024/12/13.
//

#ifndef CMCL_BOUNCEANIMATION_H
#define CMCL_BOUNCEANIMATION_H

#include <QPropertyAnimation>
#include <QObject>
#include <QWidget>

class bounceAnimation : public QObject {
Q_OBJECT

public:
    explicit bounceAnimation(QWidget *target, QObject *parent = nullptr);

    void startForward();  // 启动前向动画
    void startBackward(); // 启动后向动画

protected:
    qreal scale() const; // 获取当前缩放比例
    void setScale(qreal value); // 设置缩放比例

private:
    QPropertyAnimation *m_animation; // 动画对象
    QWidget *m_target; // 目标控件
    qreal m_currentScale; // 当前缩放比例

    void applyScale(qreal scale); // 应用缩放
};

#endif //CMCL_BOUNCEANIMATION_H
