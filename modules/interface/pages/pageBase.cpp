//
// Created by wsz on 2024/12/14.
//

#include "pageBase.h"

pageBase::pageBase(QWidget *parent)
    : QWidget(parent) {
    setObjectName("pageBase");
    initControls();

    setAutoFillBackground(true);
    setStyleSheet("background-color:cyan"); // 用于测试

    setFixedSize(500,500);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

void pageBase::initControls() {
    mainLayout_ = new QHBoxLayout();
    mainLayout_->setContentsMargins(0,0,0,0);
    mainLayout_->setSpacing(0);
    list = new myContentList();
    list->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    mainLayout_->addWidget(list);
    setLayout(mainLayout_);
}

pageBase::~pageBase() {
}
