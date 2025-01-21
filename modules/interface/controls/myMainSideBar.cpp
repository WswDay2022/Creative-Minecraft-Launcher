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
    QIcon icon = QApplication::style()->standardIcon(QStyle::SP_TitleBarCloseButton);
    myIconButton *toggleButton = new myIconButton(icon);
    toggleButton->setSize(20,20);
    toggleButton->setIconSize(QSize(20,20));
    connect(toggleButton, &QPushButton::clicked,this,&myMainSideBar::toggleSidebar);

    myTextButton *textButton = new myTextButton("傻逼");
    myButton *button = new myButton("Test");
    myRoundButton *roundButton = new myRoundButton();
    myRoundIconButton *roundIconButton = new myRoundIconButton(icon);
    roundButton->setText("SB");
    textButton->setFixedSize(100,50);
    roundButton->setSize(50,50);
    roundIconButton->setSize(50,50);
    button->setSize(100,50);

    connect(button,&QPushButton::clicked,this,[=]() {
        // myMessageBox *box = new myMessageBox(this->parentWidget(),500,500);
        // box->show();
        // box->setControlStyle(CONTROL_WARING);
    });

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(toggleButton);
    vbox->addWidget(textButton);
    vbox->addWidget(roundButton);
    vbox->addWidget(roundIconButton);
    vbox->addWidget(button);
    setLayout(vbox);
}

myMainSideBar::~myMainSideBar() = default;

int myMainSideBar::getWidth() const {
    return m_width;
}

void myMainSideBar::setWidth(const int &width) {
    if (m_width != width) {
        m_width = width;
        setFixedWidth(width);
        emit widthChanged();
    }
}

void myMainSideBar::animateWidth(const int &startWidth, const int &endWidth, int duration) {
    QPropertyAnimation *animation = new QPropertyAnimation(this, "maximumWidth");
    animation->setDuration(duration);
    animation->setStartValue(startWidth);
    animation->setEndValue(endWidth);
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, this, [=] {
        //move(this->x()-1,this->y());
    });
}