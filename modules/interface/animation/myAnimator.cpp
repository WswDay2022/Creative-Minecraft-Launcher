//
// Created by WswDay2022 on 2025/1/17.
//

#include <qgraphicsscene.h>
#include "myAnimator.h"
#include <QGraphicsRotation>

myAnimator::myAnimator(int duration)
    : m_duration(duration) {}

myAnimator::~myAnimator() = default;

void myAnimator::setDuration(int duration) { m_duration = duration; }

QPropertyAnimation* myAnimator::scaleAnimation(QWidget *parent,const QSize &startSize, const QSize &endSize,bool start,QAbstractAnimation::DeletionPolicy startMode) const {
    if (parent == nullptr) return nullptr;
    auto *animation = new QPropertyAnimation(parent, "geometry");
    animation->setDuration(m_duration);
    parent->setGeometry(parent->x(),parent->y(),startSize.width(),startSize.height());
    QRect startRect = parent->geometry();
    animation->setStartValue(startRect);
    QRect endRect = QRect(
        startRect.x() - (endSize.width() - startSize.width()) / 2,
        startRect.y() - (endSize.height() - startSize.height()) / 2,
        endSize.width(),
        endSize.height()
    );
    animation->setEndValue(endRect);
    animation->setEasingCurve(QEasingCurve::Linear);
    if (start) animation->start(startMode);
    return animation;
}

QPropertyAnimation *myAnimator::rotateAnimation(QWidget *parent, const int &startDeg, const int &endDeg, bool start,QAbstractAnimation::DeletionPolicy startMode) const {
    if (parent == nullptr) return nullptr;
    auto *animation = new QPropertyAnimation(parent);
    animation->setStartValue(startDeg);
    animation->setTargetObject(parent);
    animation->setDuration(m_duration);
    animation->setEndValue(endDeg);
    parent->connect(animation,&QPropertyAnimation::valueChanged,[=](const QVariant &value) {
        double deg = value.toDouble();
        parent->setStyleSheet("transform: rotate(" + QString::number(deg) + "deg);");
    });
    animation->setEasingCurve(QEasingCurve::Linear);
    if (start) animation->start(startMode);
    return animation;
}

QPropertyAnimation* myAnimator::opacityAnimation(QWidget *parent,const double &startValue, const double &endValue, bool start,QAbstractAnimation::DeletionPolicy startMode) const {
    if (parent == nullptr) return nullptr;
    QGraphicsOpacityEffect effect(parent);
    effect.setOpacity(startValue);
    parent->setGraphicsEffect(&effect);
    auto *animation = new QPropertyAnimation(&effect,"opacity");
    animation->setStartValue(startValue);
    animation->setTargetObject(&effect);
    animation->setDuration(m_duration);
    animation->setParent(&effect);
    animation->setEndValue(endValue);
    animation->setEasingCurve(QEasingCurve::Linear);
    if (start) animation->start(startMode);
    return animation;
}

QPropertyAnimation *myAnimator::windowOpacityAnimation(QWidget *parent, const double &startValue, const double &endValue, bool start,QAbstractAnimation::DeletionPolicy startMode) const {
    if (parent == nullptr) return nullptr;
    auto *animation = new QPropertyAnimation(parent,"windowOpacity");
    animation->setStartValue(startValue);
    animation->setDuration(m_duration);
    animation->setParent(parent);
    animation->setEndValue(endValue);
    animation->setEasingCurve(QEasingCurve::Linear);
    if (start) animation->start(startMode);
    return animation;
}

QPropertyAnimation *myAnimator::scaleAnimation(QWidget *parent,const int &scaleSize, bool start,QAbstractAnimation::DeletionPolicy startMode) const {
    if (parent == nullptr) return nullptr;
    auto *animation = new QPropertyAnimation(parent,"geometry");
    animation->setStartValue(parent->geometry());
    animation->setEndValue(parent->geometry().adjusted(scaleSize/2,scaleSize/2,-scaleSize,-scaleSize));
    animation->setDuration(m_duration);
    animation->setTargetObject(parent);
    animation->setEasingCurve(QEasingCurve::Linear);
    if (start) animation->start(startMode);
    return animation;
}

QPropertyAnimation *myAnimator::fontSizeAnimation(QWidget *parent, const int &startPointSize, const int &endPointSize, bool start, QAbstractAnimation::DeletionPolicy startMode) const {
    if (parent == nullptr) return nullptr;
    auto *animation = new QPropertyAnimation(parent);
    animation->setParent(parent);
    animation->setStartValue(startPointSize);
    animation->setEndValue(endPointSize);
    animation->setDuration(m_duration);
    animation->setTargetObject(parent);
    parent->connect(animation,&QPropertyAnimation::valueChanged,parent,[=](const QVariant &variant) {
        int size = variant.toInt();
        QFont font = parent->font();
        font.setPointSize(size);
        parent->setFont(font);
    });
    animation->setEasingCurve(QEasingCurve::Linear);
    if (start) animation->start(startMode);
    return animation;
}

QPropertyAnimation *myAnimator::geometryAnimation(QWidget *parent, const QRect &startRect, const QRect &endRect, bool start,QAbstractAnimation::DeletionPolicy startMode) const {
    if (parent == nullptr) return nullptr;
    auto *animation = new QPropertyAnimation(parent,"geometry");
    animation->setParent(parent);
    animation->setStartValue(startRect);
    animation->setEndValue(endRect);
    animation->setDuration(m_duration);
    animation->setTargetObject(parent);
    animation->setEasingCurve(QEasingCurve::Linear);
    if (start) animation->start(startMode);
    return animation;
}

QPropertyAnimation *myAnimator::fixedHeightAnimation(QWidget *parent, const int &startHeight, const int &endHeight, bool start, QAbstractAnimation::DeletionPolicy startMode) const {
    if (parent == nullptr) return nullptr;
    auto *animation = new QPropertyAnimation(parent);
    animation->setParent(parent);
    animation->setStartValue(startHeight);
    animation->setEndValue(endHeight);
    animation->setDuration(m_duration);
    animation->setTargetObject(parent);
    parent->connect(animation,&QPropertyAnimation::valueChanged,[=](const QVariant &value) {
        int height = value.toInt();
        parent->setFixedHeight(height);
    });
    animation->setEasingCurve(QEasingCurve::Linear);
    if (start) animation->start(startMode);
    return animation;
}

QPropertyAnimation *myAnimator::fixedWidthAnimation(QWidget *parent, const int &startWidth, const int &endWidth, bool start, QAbstractAnimation::DeletionPolicy startMode) const {
    if (parent == nullptr) return nullptr;
    auto *animation = new QPropertyAnimation(parent);
    animation->setParent(parent);
    animation->setStartValue(startWidth);
    animation->setEndValue(endWidth);
    animation->setDuration(m_duration);
    animation->setTargetObject(parent);
    parent->connect(animation,&QPropertyAnimation::valueChanged,[=](const QVariant &value) {
        int width = value.toInt();
        parent->setFixedWidth(width);
    });
    animation->setEasingCurve(QEasingCurve::Linear);
    if (start) animation->start(startMode);
    return animation;
}

QPropertyAnimation *myAnimator::fontColorAnimation(QWidget *parent,const QColor &startColor, const QColor &endColor, bool start,QAbstractAnimation::DeletionPolicy startMode) const {
    if (parent == nullptr) return nullptr;
    auto *animation = new QPropertyAnimation(parent);
    animation->setParent(parent);
    animation->setStartValue(startColor);
    animation->setEndValue(endColor);
    animation->setDuration(m_duration);
    animation->setTargetObject(parent);
    parent->connect(animation,&QPropertyAnimation::valueChanged,[=](const QVariant &value) {
        QColor color = value.value<QColor>(); // 编译器这里会报错，但不影响运行，奇怪了
        QPalette palette = parent->palette();
        palette.setColor(QPalette::WindowText, color);
        palette.setColor(QPalette::ButtonText, color);
        palette.setColor(QPalette::Text, color);
        parent->setPalette(palette);
        parent->update();
    });
    animation->setEasingCurve(QEasingCurve::Linear);
    if (start) animation->start(startMode);
    return animation;
}

QPropertyAnimation *myAnimator::iconColorButtonAnimation(QPushButton *button, const QColor &startColor, const QColor &endColor, bool start,QAbstractAnimation::DeletionPolicy startMode) const {
    if (button == nullptr) return nullptr;
    auto *animation = new QPropertyAnimation(button);
    animation->setParent(button);
    animation->setStartValue(startColor);
    animation->setEndValue(endColor);
    animation->setDuration(m_duration);
    animation->setTargetObject(button);
    button->connect(animation,&QPropertyAnimation::valueChanged,button,[=](const QVariant &value) {
        QColor color = value.value<QColor>(); // 编译器这里会报错，但不影响运行，奇怪了
        QPixmap pixmap = button->icon().pixmap(QSize(64,64));
        QPainter painter(&pixmap);
        painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
        painter.fillRect(pixmap.rect(), color);
        QIcon colorIcon = QIcon(pixmap);
        button->setIcon(colorIcon);
    });
    animation->setEasingCurve(QEasingCurve::Linear);
    if (start) animation->start(startMode);
    return animation;
}

QPropertyAnimation *myAnimator::iconSizeButtonAnimation(QPushButton *button, const QSize &startSize, const QSize &endSize, bool start,QAbstractAnimation::DeletionPolicy startMode) const {
    if (button == nullptr) return nullptr;
    auto *animation = new QPropertyAnimation(button);
    animation->setParent(button);
    animation->setStartValue(startSize);
    animation->setEndValue(endSize);
    animation->setDuration(m_duration);
    animation->setTargetObject(button);
    button->connect(animation,&QPropertyAnimation::valueChanged,button,[=](const QVariant &value) {
        button->setIconSize(value.toSize());
    });
    animation->setEasingCurve(QEasingCurve::Linear);
    if (start) animation->start(startMode);
    return animation;
}