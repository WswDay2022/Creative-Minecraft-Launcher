//
// Created by WswDay2022 on 2024/12/14.
//

#include <QGraphicsView>
#include "myMainSideBar.h"
#include "myControls.h"

myMainSideBar::myMainSideBar(QWidget *parent)
    : QFrame(parent) {
    setObjectName("myMainSideBar");
    initControl();
    core core_;core_.globalInit();
    setFrameShape(QFrame::NoFrame);
    setAutoFillBackground(true);
    setStyleSheet("background-color:white");
    setMaximumWidth(260);
    setMinimumWidth(50);
}

void myMainSideBar::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), Qt::transparent); // 清除背景
    return QFrame::paintEvent(event);
}

void myMainSideBar::toggleSidebar() {
    QPropertyAnimation *animation = new QPropertyAnimation(this, "rect");
    animation->setDuration(170);

    QRectF startRect = this->rect();

    if (startRect.width() == 260) {
        // 收起侧边栏
        animation->setStartValue(startRect);
        animation->setEndValue(QRectF(0, 0, 0, height()));  // 将宽度改为0
    } else {
        // 展开侧边栏
        animation->setStartValue(startRect);
        animation->setEndValue(QRectF(0, 0, 260, height()));  // 恢复宽度为260
    }
    // connect(animation1,&QPropertyAnimation::finished,[this](){ update(); });
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->start(QAbstractAnimation::DeleteWhenStopped); // 启动动画
}

void myMainSideBar::initControl() {
    // 折叠按钮
    QIcon icon = QApplication::style()->standardIcon(QStyle::SP_MediaPlay);
    myIconButton *toggleButton = new myIconButton(icon);
    toggleButton->setFixedSize(20,20);
    toggleButton->setIconSize(QSize(20,20));
    connect(toggleButton, &QPushButton::clicked,this,&myMainSideBar::toggleSidebar);

    myButton *button = new myButton("wf");
    button->setSize(100,50);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(toggleButton);
    vbox->addWidget(button);
    setLayout(vbox);
}

myMainSideBar::~myMainSideBar() {
}
