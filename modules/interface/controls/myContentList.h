//
// Created by wsz on 2024/12/14.
//

#ifndef CMCL_MYCONTENTLIST_H
#define CMCL_MYCONTENTLIST_H


#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include "myControls.h"

class myContentList : public QScrollArea {
    Q_OBJECT

public:
    myContentList(QWidget *parent = nullptr);
    ~myContentList();

    void initControl();
    void addAnCard(myContentCard *card);
    void addControl(QWidget *widget);

private:
    QVBoxLayout *contentList;
};


#endif //CMCL_MYCONTENTLIST_H
