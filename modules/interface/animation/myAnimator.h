//
// Created by WswDay2022 on 2025/1/17.
//

#ifndef CMCL_MYANIMATOR_H
#define CMCL_MYANIMATOR_H

#include <QTimer>
#include <QPainterPath>
#include <QMouseEvent>
#include <QMainWindow>
#include <QApplication>
#include <QWidget>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPushButton>
#include <QFontDatabase>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include <QPropertyAnimation>
#include <QUndoView>
#include <QLabel>
#include <QGridLayout>

#include "../../../core/core.h"

using namespace Log;

class myAnimator {
public:
    myAnimator(int duration);
    ~myAnimator();

    void setDuration(int duration);

    QPropertyAnimation *opacityAnimation(QWidget *parent,const double &startValue,const double &endValue,bool start,QAbstractAnimation::DeletionPolicy startMode = QAbstractAnimation::DeleteWhenStopped) const;
    QPropertyAnimation *scaleAnimation(QWidget *parent,const QSize &startSize,const QSize &endSize,bool start,QAbstractAnimation::DeletionPolicy startMode = QAbstractAnimation::DeleteWhenStopped) const;
    QPropertyAnimation *scaleAnimation(QWidget *parent,const int &scaleSize,bool start,QAbstractAnimation::DeletionPolicy startMode = QAbstractAnimation::DeleteWhenStopped) const;
    QPropertyAnimation *geometryAnimation(QWidget *parent,const QRect &startRect,const QRect &endRect,bool start,QAbstractAnimation::DeletionPolicy startMode = QAbstractAnimation::DeleteWhenStopped) const;
    QPropertyAnimation *rotateAnimation(QWidget *parent,const int &startDeg,const int &endDeg,bool start,QAbstractAnimation::DeletionPolicy startMode = QAbstractAnimation::DeleteWhenStopped) const;
    QPropertyAnimation *fixedHeightAnimation(QWidget *parent,const int &startHeight, const int &endHeight, bool start, QAbstractAnimation::DeletionPolicy startMode = QAbstractAnimation::DeleteWhenStopped) const;
    QPropertyAnimation *fixedWidthAnimation(QWidget *parent,const int &startWidth,const int &endWidth,bool start,QAbstractAnimation::DeletionPolicy startMode = QAbstractAnimation::DeleteWhenStopped) const;

    QPropertyAnimation *fontColorAnimation(QWidget *parent,const QColor &startColor,const QColor &endColor,bool start,QAbstractAnimation::DeletionPolicy startMode = QAbstractAnimation::DeleteWhenStopped) const;
    QPropertyAnimation *fontSizeAnimation(QWidget *parent, const int &startPointSize, const int &endPointSize, bool start, QAbstractAnimation::DeletionPolicy startMode = QAbstractAnimation::DeleteWhenStopped) const;
    QPropertyAnimation *iconColorButtonAnimation(QPushButton *button,const QColor &startColor,const QColor &endColor,bool start,QAbstractAnimation::DeletionPolicy startMode = QAbstractAnimation::DeleteWhenStopped) const;
    QPropertyAnimation *iconSizeButtonAnimation(QPushButton *button,const QSize &startSize,const QSize &endSize,bool start,QAbstractAnimation::DeletionPolicy startMode = QAbstractAnimation::DeleteWhenStopped) const;


private:
    Q_PROPERTY(QColor fontColor READ fontColor WRITE setFontColor NOTIFY fontColorChanged);
    Q_PROPERTY(QColor iconColor READ iconColor WRITE setIconColor NOTIFY iconColorChanged);

    QColor m_parent_fontColor;
    QColor m_parent_iconColor;
    QColor fontColor() const { return m_parent_fontColor; }
    QColor iconColor() const { return m_parent_iconColor; }
    void setFontColor(const QColor &color);
    void setIconColor(const QColor &color);

signals:
    void fontColorChanged();
    void iconColorChanged();

private:

    QSize m_iconSize;
    int m_fontSize;

    int m_endValue;
    int m_startValue;

    int m_duration;

};


#endif //CMCL_MYANIMATOR_H
