//
// Created by WswDay2022 on 2025/1/16.
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
    static core core_;
    core_.globalInit();
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

    setIconSize(QSize(this->font().pointSize()*1.5,this->font().pointSize()*1.5));
    setIcon(icon);
}

void myRoundIconButton::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
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
    QRect iconRect(QPoint(0, 0), this->iconSize());
    iconRect.moveCenter(rect().center()); // 将图标居中
    QIcon::Mode mode = option.state & QStyle::State_Enabled ? QIcon::Normal : QIcon::Disabled;
    QIcon::State state = option.state & QStyle::State_Selected ? QIcon::On : QIcon::Off;
    icon().paint(&painter, iconRect, Qt::AlignCenter, mode, state);
}