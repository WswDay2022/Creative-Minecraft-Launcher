//
// Created by WswDay2022 on 2024/11/30.
//

#ifndef CMCL_MAINLAYOUT_H
#define CMCL_MAINLAYOUT_H


#include <qlayout.h>

class mainLayout : public QVBoxLayout {
    Q_OBJECT

public:
    mainLayout(QWidget *parent = nullptr);
    void initTitleBar();
};


#endif //CMCL_MAINLAYOUT_H
