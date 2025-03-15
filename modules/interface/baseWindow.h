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
#include "pages/pageBase.h"
#include <QPainterPath>
#include <QApplication>
#include <QPainter>
#include <QCursor>
#include <QFile>
#include "pages/pages.h"
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
    enum class ResizeEdge {
        None,
        Left,
        Right,
        Top,
        Bottom,
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

    // 工具函数
    ResizeEdge calculateResizeEdge(const QPoint &pos) const;
    void updateCursorShape(const QPoint &pos);
    void handleResize(const QPoint &globalMousePos);

    void initTitleBar();
    void initMenuBar();
    void paintEvent(QPaintEvent *event) override;
    void changePage(pageBase *page);
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

private:
    ResizeEdge m_resizeEdge = ResizeEdge::None;
    QPoint m_mousePressPos;
    QSize m_windowSize;
    myTitleBar* m_titleBar;
    myMainSideBar* m_menuBar;
    pageChange* m_pageChange;
    QHBoxLayout* m_panel;
};


#endif //CMCL_BASEWINDOW_H
