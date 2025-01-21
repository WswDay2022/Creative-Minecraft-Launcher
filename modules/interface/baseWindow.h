//
// Created by wsw on 2024/11/30.
//

#ifndef CMCL_BASEWINDOW_H
#define CMCL_BASEWINDOW_H

#include <QWidget>
#include "controls/myTitleBar.h"
#include "../../core/core.h"
#include "controls/myMainSideBar.h"
#include "controls/pageChange.h"
#include <QPainterPath>
#include <QApplication>
#include <QPainter>
#include <QFile>
#include <QPropertyAnimation>

/****
 窗口基类
 - 标题栏
 - 可调整大小
 - 可拖动
 - 加载样式表
 ***/

class baseWindow : public QWidget {
    Q_OBJECT

public:
    baseWindow(QWidget *parent = nullptr);
    ~baseWindow();

private:
    void initTitleBar();
    void initMenuBar();
    void paintEvent(QPaintEvent *event);
    void loadStyleSheet();

    void initPageChanger();
    void initPanel();

    bool isResizing = false;
    QPoint mousePressPos;
    QSize windowSize;

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void onButtonMinClicked();
    void onButtonCloseClicked();

protected:
    myTitleBar* m_titleBar;
    myMainSideBar* m_menuBar;
    pageChange* m_pageChange;
    QHBoxLayout* m_panel;
};


#endif //CMCL_BASEWINDOW_H
