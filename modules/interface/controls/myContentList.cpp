//
// Created by wsz on 2024/12/14.
//

#include "myContentList.h"

myContentList::myContentList(QWidget *parent)
    : QScrollArea(parent) {
    initControl();
    setObjectName("myScrollPane");
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded); // 横向滚动条根据需要出现
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setWidgetResizable(true);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

void myContentList::initControl() {
    contentList = new QVBoxLayout();
    setLayout(contentList);
    contentList->setContentsMargins(5,5,5,5);
    contentList->setSpacing(10);
}

void myContentList::addAnCard(myContentCard *card) {
    contentList->addWidget(card);
}

void myContentList::addControl(QWidget *widget) {
    contentList->addWidget(widget);
}

myContentList::~myContentList() {
}
