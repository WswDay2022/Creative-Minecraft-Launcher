//
// Created by WswDay2022 on 2024/12/14.
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
    explicit pageBase(QWidget *parent = nullptr);
    ~pageBase();

    void initControls();

    void startPage();
    void closePage();

protected:
    void paintEvent(QPaintEvent *event) override;

    QHBoxLayout *mainLayout_;
    myContentList *list;
};

#endif //CMCL_PAGEBASE_H
