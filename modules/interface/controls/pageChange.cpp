//
// Created by WswDay2022 on 2024/12/14.
//

#include "pageChange.h"

pageChange::pageChange(QWidget *parent)
    : QStackedWidget(parent) {
    setObjectName("pageChange");
    setAutoFillBackground(true);
    initControl();
}

void pageChange::initControl() {
    core core_;core_.globalInit();
    myControls controlTools;
    setStyleSheet("background-color:"+controlTools.lighterColor(core_.themeColor,160).name());
}

pageChange::~pageChange() {
}
