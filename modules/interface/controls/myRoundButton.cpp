//
// Created by WswDay2022 on 2025/1/15.
//

#include "myControls.h"

myRoundButton::myRoundButton(QWidget *parent)
    : myButton(nullptr,parent) {
    setObjectName("myRoundButton");
    setControlStyle(CONTROL_INFO);
}

myRoundButton::~myRoundButton() = default;

void myRoundButton::paintEvent(QPaintEvent *event) {
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
    painter.drawText(rect(), Qt::AlignCenter, option.text);
}
