//
// Created by WswDay2022 on 2024/11/30.
//

#ifndef CMCL_MAINAPPLICATION_H
#define CMCL_MAINAPPLICATION_H

#include <QMainWindow>
#include <QApplication>
#include <QWidget>
#include "../../core/io/fileReader.h"
#include "../../core/json/json.h"
#include "../../core/core.h"
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include "pages/mainLayout.h"
#include "controls/myControls.h"
#include "controls/myTitleBar.h"
#include "baseWindow.h"
#include <QPushButton>

class mainApplication : public baseWindow {
    Q_OBJECT

public:
    mainApplication(QWidget *parent = nullptr);
    ~mainApplication();

protected:
    void paintEvent(QPaintEvent *event) override;
    void drawRoundedRect(QPainter &painter);
};


#endif //CMCL_MAINAPPLICATION_H
