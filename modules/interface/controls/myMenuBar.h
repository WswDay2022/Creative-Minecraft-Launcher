//
// Created by WswDay2022 on 2024/11/30.
//

#ifndef CMCL_MYMENUBAR_H
#define CMCL_MYMENUBAR_H

#include "../../../core/io/fileReader.h"
#include "../../../core/json/reader.h"
#include "../../../core/json/value.h"
#include "../../../core/core.h"

#include <Qt>
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>
#include <QWidget>
#include <QHBoxLayout>
#include <QPainter>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QStyle>
#include <QMouseEvent>
#include <QFontDatabase>
#include <QApplication>

/* 左侧菜单栏 */
class myMenuBar : public QWidget {
    Q_OBJECT

public:
    myMenuBar(QWidget *parent = nullptr);
    ~myMenuBar();

private:
    void paintEvent(QPaintEvent *event);
    void initControl();

};


#endif //CMCL_MYMENUBAR_H
