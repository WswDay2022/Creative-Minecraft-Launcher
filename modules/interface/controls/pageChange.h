//
// Created by WswDay2022 on 2024/12/14.
//

#ifndef CMCL_PAGECHANGE_H
#define CMCL_PAGECHANGE_H

#include <QStackedWidget>
#include "../../../core/core.h"
#include "myControls.h"
#include "../pages/pageBase.h"

/* 界面切换器 */
class pageChange : public QStackedWidget {
    Q_OBJECT

public:
    pageChange(QWidget *parent = nullptr);
    ~pageChange();

    void changePage(pageBase *page);
    void initControl();
};

#endif //CMCL_PAGECHANGE_H
