//
// Created by WswDay2022 on 2024/12/14.
//

#include "pageBase.h"

pageBase::pageBase(QWidget *parent)
    : QWidget(parent) {
    setObjectName("pageBase");
    initControls();
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

void pageBase::initControls() {
    mainLayout_ = new QHBoxLayout();
    mainLayout_->setContentsMargins(0,0,0,0);
    mainLayout_->setSpacing(0);
    list = new myContentList();
    list->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    myButton *button = new myButton("What Can I Say?");
    button->setFixedHeight(40);

    myContentCard *tipCard = new myContentCard(nullptr,true);
    tipCard->setTitle("公告");
    tipCard->setCanToggle(true);
    tipCard->addChildWidget(button);
    tipCard->setHeight(100);
    list->addAnCard(tipCard);

    mainLayout_->addWidget(list);
    setLayout(mainLayout_);
    list->loadWidget();
}

pageBase::~pageBase() = default;
