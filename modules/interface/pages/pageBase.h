//
// Created by wsz on 2024/12/14.
//

#ifndef CMCL_PAGEBASE_H
#define CMCL_PAGEBASE_H

#include <QWidget>
#include "../controls/myControls.h"
#include "../controls/myContentList.h"

/* 页面的基类 */
class pageBase : public QWidget {
    Q_OBJECT

public:
    pageBase(QWidget *parent = nullptr);
    ~pageBase();

    void initControls();

private:
    QHBoxLayout *mainLayout_;
    myContentList *list;
    QVBoxLayout *contentLayout;
};

#endif //CMCL_PAGEBASE_H
