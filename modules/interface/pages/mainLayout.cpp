//
// Created by WswDay2022 on 2024/11/30.
//

#include <QPushButton>
#include <QLabel>
#include "mainLayout.h"
#include "../../../core/io/fileReader.h"
#include "../../../core/json/reader.h"
#include "../../../core/json/value.h"
#include "../../../core/core.h"

mainLayout::mainLayout(QWidget *parent)
    : QVBoxLayout(parent) {
    setObjectName("mainLayout");
    this->setSpacing(0);
    initTitleBar();
}

void mainLayout::initTitleBar() {
    QHBoxLayout *hBox = new QHBoxLayout;
    core core_;core_.globalInit();
    Json::Value value = core_.getSettingJson();

    QPushButton *close = new QPushButton("X");
    QPushButton *hide = new QPushButton("_");
    QLabel *title = new QLabel(QString::fromStdString(value["launcherShortName"].asString()));

    close->connect(close, SIGNAL(clicked()),this, SLOT(close()));
    hide->connect(hide, SIGNAL(clicked()), this, SLOT(showMinimized();));

    close->setObjectName("closeButton");
    hide->setObjectName("hideButton");
    title->setObjectName("titleText");
    title->setFixedHeight(50);
    hide->setFixedHeight(50);
    close->setFixedHeight(50);

    title->setAlignment(Qt::AlignmentFlag::AlignCenter);

    std::string color;
    color += std::to_string(core_.getSettingJson()["themeColor"][0].asInt()) + ",";
    color += std::to_string(core_.getSettingJson()["themeColor"][1].asInt()) + ",";
    color += std::to_string(core_.getSettingJson()["themeColor"][2].asInt());

    close->setStyleSheet(QString::fromStdString("#closeButton {background-color:rgb("+color+");}"));
    title->setStyleSheet(QString::fromStdString("#titleBar {background-color:rgb("+color+");}"));
    hide->setStyleSheet(QString::fromStdString("#hideButton {background-color:rgb("+color+");}"));

    hBox->addWidget(title);
    hBox->addWidget(hide);
    hBox->addWidget(close);
    setAlignment(hBox,Qt::AlignmentFlag::AlignTop);
    addLayout(hBox);
}

