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
    setFrameShape(QFrame::NoFrame);
    setFrameStyle(QFrame::NoFrame);
    setFrameShadow(QFrame::Plain);
    setAutoFillBackground(true);
    setStyleSheet("background-color:white;");
    setMaximumWidth(260);
    setMinimumWidth(50);
}

void myMainSideBar::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // painter.fillRect(event->rect(), Qt::transparent); // 清除背景
    return QFrame::paintEvent(event);
}

void myMainSideBar::toggleSidebar() {
    if (this->width() == 260) { // 收起侧边栏
        animateWidth(260,50,170);
    } else { // 展开侧边栏
        animateWidth(50,260,170);
    }
}

void myMainSideBar::initControl() {
    // 折叠按钮
    QIcon icon = QApplication::style()->standardIcon(QStyle::SP_FileDialogInfoView);
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

int myMainSideBar::getWidth() const {
    return m_width;
}

void myMainSideBar::setWidth(const int &width) {
    if (m_width != width) {
        m_width = width;
        setGeometry(this->x(),this->y(),width,this->height());;
        emit widthChanged();
    }
}

void myMainSideBar::animateWidth(const int &startWidth, const int &endWidth, int duration) {
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(duration);
    animation->setStartValue(this->geometry());
    animation->setEndValue(QRect(this->x()+1,this->y(),endWidth,this->height()));
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    connect(animation, &QPropertyAnimation::finished, this, [=] {
        move(this->x()-1,this->y());
    });
}