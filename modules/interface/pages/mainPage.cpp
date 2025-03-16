//
// Created by wsw on 2024/11/29.
//

#include "pages.h"
#include "mainLayout.h"
#include "../controls/myControls.h"

mainPage::mainPage(QWidget *parent)
    : pageBase(parent) {
    initPage();
}
/*
void mainPage::initPage() {
    myButton *button = new myButton("What Can I Say?");
    button->setFixedHeight(40);

    myContentCard *tipCard = new myContentCard(nullptr,true);
    tipCard->setTitle("公告");
    tipCard->setCanToggle(true);
    tipCard->addChildWidget(button);
    tipCard->setHeight(500);
    list->addAnCard(tipCard);
}*/

void mainPage::initPage() {
    myButton *button = new myButton("What Can I Say?");
    button->setFixedHeight(40);

    myContentCard *tipCard = new myContentCard(nullptr, true);
    tipCard->setTitle("公告");
    tipCard->setCanToggle(true);
    tipCard->addChildWidget(button);
    tipCard->setHeight(500);
    list->addAnCard(tipCard);
}