//
// Created by WswDay2022 on 2024/12/14.
//

#ifndef CMCL_MYMAINSIDEBAR_H
#define CMCL_MYMAINSIDEBAR_H

#include <QWidget>
#include <QFrame>
#include "../../../core/core.h"
#include <QPainter>
#include <QLabel>
#include <QVBoxLayout>
#include <QPainterPath>
#include <QPushButton>
#include <QPropertyAnimation>

/* 侧边菜单栏 */
class myMainSideBar : public QFrame { // 搞了半天，还是继承QFrame好
    Q_OBJECT

public:
    myMainSideBar(QWidget *parent = nullptr);
    ~myMainSideBar();

    void initControl();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void toggleSidebar();

signals:
    void widthChanged();

private:
    int m_width;
    Q_PROPERTY(int getWidth READ getWidth WRITE setWidth NOTIFY widthChanged);

    int getWidth() const;
    void setWidth(const int &width);
    void animateWidth(const int &startWidth, const int &endWidth, int duration);
};



#endif //CMCL_MYMAINSIDEBAR_H
