//
// Created by wsz on 2025/1/16.
//

#include "myControls.h"

myRoundIconButton::myRoundIconButton(const QIcon &icon,QWidget *parent)
    : myRoundButton(parent) {
    setObjectName("myRoundIconButton");
    setControlStyle(CONTROL_INFO);
    setButtonIcon(icon);
}

myRoundIconButton::~myRoundIconButton() = default;

void myRoundIconButton::setButtonIcon(QIcon icon) {
    core core_;core_.globalInit();
    myControls tools;

    switch (getControlStyle()) {
        case CONTROL_INFO:
            icon = tools.setIconColor(icon,core_.themeColor);
            break;
        case CONTROL_WARING:
            icon = tools.setIconColor(icon,core_.warnColor);
            break;
        case CONTROL_ERROR:
            icon = tools.setIconColor(icon,core_.errColor);
            break;
    }

    setIconSize(QSize(font().pointSize()*1.5,font().pointSize()*1.5));
    m_iconSize = QSize(font().pointSize()*1.5,font().pointSize()*1.5);
    setIcon(icon);
}

void myRoundIconButton::mousePressEvent(QMouseEvent *event) {
    // 创建动画对象，控制按钮的缩放效果
    setIconSize(QSize(font().pointSize(),font().pointSize()));
    auto *animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(100); // 动画持续时间
    animation->setStartValue(geometry()); // 起始位置
    animation->setEndValue(geometry().adjusted(2, 2, -4, -4));

    auto *returnAnimation = new QPropertyAnimation(this, "geometry");
    returnAnimation->setDuration(100);
    returnAnimation->setStartValue(animation->endValue());
    returnAnimation->setEndValue(geometry()); // 返回到原来的位置

    myAnimator animator(100);
    auto animation1 = animator.iconSizeButtonAnimation(this,iconSize(),iconSize()-QSize(4,4),false);
    auto returnAnimation1 = animator.iconSizeButtonAnimation(this,animation1->endValue().toSize(),animation1->startValue().toSize(),false);

    if (event->button() == Qt::LeftButton) {
        if (!m_isAnimating) { // 防止多次触发
            m_isAnimating = true;
            animation->start(QAbstractAnimation::DeleteWhenStopped);
            animation1->start(QAbstractAnimation::DeleteWhenStopped);
            connect(animation, &QPropertyAnimation::finished, [=]() {
                returnAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            });
            connect(animation1, &QPropertyAnimation::finished, [=]() {
                returnAnimation1->start(QAbstractAnimation::DeleteWhenStopped);
            });
            connect(returnAnimation, &QPropertyAnimation::finished, [=]() {
                connect(returnAnimation1, &QPropertyAnimation::finished, [=]() {
                    m_isAnimating = false; // 动画结束，允许再次点击
                    returnAnimation->deleteLater();
                    returnAnimation1->deleteLater();
                });
            });
        }
    }

    return QPushButton::mousePressEvent(event);
}

void myRoundIconButton::setSizeIcon(const QSize &size) {
    if (m_iconSize != size) {
        m_iconSize = size;
        setIconSize(size);
        emit iconSizeChanged();
    }
}

QSize myRoundIconButton::sizeIcon() const {
    return m_iconSize;
}

void myRoundIconButton::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    QStyleOptionButton option;
    initStyleOption(&option);

    core core_;core_.globalInit();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::white);
    painter.setPen(Qt::white);

    switch (getControlStyle()) {
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

    painter.drawEllipse(rect());
    QSize iconSize(font().pointSize()*1.5,font().pointSize()*1.5);
    QRect iconRect(QPoint(0, 0), this->iconSize());
    iconRect.moveCenter(rect().center()); // 将图标居中
    QIcon::Mode mode = option.state & QStyle::State_Enabled ? QIcon::Normal : QIcon::Disabled;
    QIcon::State state = option.state & QStyle::State_Selected ? QIcon::On : QIcon::Off;
    icon().paint(&painter, iconRect, Qt::AlignCenter, mode, state);
}