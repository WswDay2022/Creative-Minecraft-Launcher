//
// Created by WswDay2022 on 2024/11/30.
//

#include "myMenuBar.h"

myMenuBar::myMenuBar(QWidget *parent)
    : QWidget(parent) {
    initControl();
    setObjectName("myMenuBar");
    core core_;core_.globalInit();
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setStyleSheet("background-color:white;");
    setFixedWidth(300);
}

myMenuBar::~myMenuBar() {}

void myMenuBar::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
}

void myMenuBar::initControl() {

}
