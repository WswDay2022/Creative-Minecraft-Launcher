//
// Created by WswDay2022 on 2024/12/14.
//

#include "myContentList.h"

myContentList::myContentList(QWidget *parent)
    : QScrollArea(parent) {
    setObjectName("myScrollPane");
    initControl();
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded); // 横向滚动条根据需要出现
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setWidgetResizable(true);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

void myContentList::initControl() {
    contentWidget = new QWidget();
    contentWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    contentList = new QVBoxLayout();
    contentWidget->setLayout(contentList);
    contentList->setContentsMargins(5,5,5,5);
    contentList->setSpacing(5);
    contentList->setEnabled(true);

    setStyleSheet( // 横向滚动条
        "QScrollBar::handle:horizontal { background: gray; border-radius: 5px; }"
        "QScrollBar:horizontal { border: none; height: 10px; }"
    );

    setStyleSheet( // 纵向滚动条
        "QScrollBar::handle:vertical { background: gray; border-radius: 5px; padding:5px 5px 5px 5px; }"
        "QScrollBar:vertical { width: 10px; border-bottom-right-radius: 6px; }"
        "QScrollBar::groove:vertical { background: lightgray; border-bottom-right-radius: 6px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { background: none; border-bottom-right-radius: 6px; }"

    );

}

void myContentList::addAnCard(myContentCard *card) {
    contentList->addWidget(card);
}

void myContentList::addControl(QWidget *widget) {
    contentList->addWidget(widget);
}

void myContentList::loadWidget() {
    setWidget(contentWidget);
}

myContentList::~myContentList() = default;
