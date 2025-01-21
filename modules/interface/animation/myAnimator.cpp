//
// Created by wsz on 2025/1/17.
//

#include <qgraphicsscene.h>
#include "myAnimator.h"
#include <QGraphicsRotation>

myAnimator::myAnimator(int duration)
    : m_duration(duration) {}

myAnimator::~myAnimator() = default;

void myAnimator::setDuration(int duration) { m_duration = duration; }

QPropertyAnimation* myAnimator::scaleAnimation(QWidget *parent,const QSize &startSize, const QSize &endSize,bool start,QAbstractAnimation::DeletionPolicy startMode) {
    if (parent == nullptr) return nullptr;
    auto *animation = new QPropertyAnimation(parent, "geometry");
    animation->setDuration(m_duration);

    parent->setGeometry(parent->x(),parent->y(),startSize.width(),startSize.height());
    QRect startRect = parent->geometry();
    animation->setStartValue(startRect);
    if (startSize == endSize) return nullptr;
    int x = (startSize.width() > endSize.width()) ? startSize.width() - endSize.width() : endSize.width() - startSize.width();
    int y = (startSize.height() > endSize.height()) ? startSize.height() - endSize.height() : endSize.height() - startSize.height();
    x = (startRect.x() > 0) ? startRect.x() + x : x - startRect.x();
    y = (startRect.y() > 0) ? startRect.y() + y : y - startRect.y();
    QRect endRect = startRect.adjusted(x,y,endSize.width(),endSize.height());
    animation->setEndValue(endRect);

    if (start) animation->start(startMode);
    return animation;
}

QPropertyAnimation* myAnimator::opacityAnimation(QWidget *parent,const int &startValue, const int &endValue, bool start,QAbstractAnimation::DeletionPolicy startMode) {
    if (parent == nullptr) return nullptr;
    auto *effect = new QGraphicsOpacityEffect(parent);
    effect->setOpacity(startValue);
    parent->setGraphicsEffect(effect);
    auto *animation = new QPropertyAnimation(effect,"opacity");
    animation->setStartValue(startValue);
    animation->setTargetObject(parent);
    animation->setDuration(m_duration);
    animation->setEndValue(endValue);
    if (start) animation->start(startMode);
    return animation;
}

QPropertyAnimation *myAnimator::scaleAnimation(QWidget *parent,const int &scaleSize, bool start,QAbstractAnimation::DeletionPolicy startMode) {
    if (parent == nullptr) return nullptr;
    auto *animation = new QPropertyAnimation(parent,"geometry");
    animation->setStartValue(parent->geometry());
    animation->setEndValue(parent->geometry().adjusted(scaleSize/2,scaleSize/2,-scaleSize,-scaleSize));
    animation->setDuration(m_duration);
    animation->setTargetObject(parent);

    if (start) animation->start(startMode);
    return animation;
}

QPropertyAnimation *myAnimator::sizeFontAnimation(QWidget *parent,const int &startPointSize, const int &endPointSize, bool start,QAbstractAnimation::DeletionPolicy startMode) {
    if (parent == nullptr) return nullptr;
    auto *animation = new QPropertyAnimation(parent);
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

    if (start) {animation->start(startMode);}
    return animation;
}

QPropertyAnimation *myAnimator::fontColorAnimation(QWidget *parent,const QColor &startColor, const QColor &endColor, bool start,QAbstractAnimation::DeletionPolicy startMode) {
    if (parent == nullptr) return nullptr;
    auto *animation = new QPropertyAnimation(parent);
    animation->setStartValue(startColor);
    animation->setEndValue(endColor);
    animation->setDuration(m_duration);
    animation->setTargetObject(parent);
    parent->connect(animation,&QPropertyAnimation::valueChanged,[=](const QVariant &value) {
        QColor color = value.value<QColor>();
        QPalette palette = parent->palette();
        palette.setColor(QPalette::WindowText, color);
        palette.setColor(QPalette::ButtonText, color);
        palette.setColor(QPalette::Text, color);
        parent->setPalette(palette);
        parent->update();
    });

    if (start) animation->start(startMode);
    return animation;
}

QPropertyAnimation *myAnimator::iconColorButtonAnimation(QPushButton *button, const QColor &startColor, const QColor &endColor, bool start,QAbstractAnimation::DeletionPolicy startMode) {
    if (button == nullptr) return nullptr;
    auto *animation = new QPropertyAnimation(button);
    animation->setTargetObject(button);
    animation->setStartValue(startColor);
    animation->setEndValue(endColor);
    animation->setDuration(m_duration);
    button->connect(animation,&QPropertyAnimation::valueChanged,button,[=](const QVariant &value) {
        QColor color = value.value<QColor>(); // 编译器这里会报错，但不影响运行，奇怪了
        QPixmap pixmap = button->icon().pixmap(QSize(64,64));
        QPainter painter(&pixmap);
        painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
        painter.fillRect(pixmap.rect(), color);
        QIcon colorIcon = QIcon(pixmap);
        button->setIcon(colorIcon);
    });

    if (start) animation->start(startMode);
    return animation;
}

QPropertyAnimation *myAnimator::iconSizeButtonAnimation(QPushButton *button, const QSize &startSize, const QSize &endSize, bool start,QAbstractAnimation::DeletionPolicy startMode) {
    if (button == nullptr) return nullptr;
    auto *animation = new QPropertyAnimation(button);
    animation->setStartValue(startSize);
    animation->setTargetObject(button);
    animation->setEndValue(endSize);
    animation->setDuration(m_duration);
    button->connect(animation,&QPropertyAnimation::valueChanged,button,[=](const QVariant &value) {
        button->setIconSize(value.toSize());
    });
    if (start) animation->start(startMode);
    return animation;
}