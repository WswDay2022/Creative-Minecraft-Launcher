//
// Created by WswDay2022 on 2024/11/29.
//

#ifndef CMCL_PAGES_H
#define CMCL_PAGES_H

#include <QVBoxLayout>

class mainPage : public QVBoxLayout {
    Q_OBJECT
public:
    mainPage(QWidget *parent = nullptr);

    void addChildren(QWidget *w) {
        centerLayout.addWidget(w);
    };

private:
    QHBoxLayout centerLayout;
};

#endif //CMCL_PAGES_H
