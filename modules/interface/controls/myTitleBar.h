//
// Created by WswDay2022 on 2024/11/30.
//

#ifndef CMCL_MYTITLEBAR_H
#define CMCL_MYTITLEBAR_H

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

class myTitleBar : public QWidget {
    Q_OBJECT

public:
    myTitleBar(QWidget *parent);
    ~myTitleBar();

    void setBackgroundColor(bool isTransparent = false);
    void setTitleIcon(QString filePath, QSize IconSize = QSize(30, 30));
    void setTitleContent(QString titleContent, int titleFontSize = 11);
    void setTitleWidth(int width);
    void setTitleRoll();
    void setWindowBorderWidth(int borderWidth);
    void saveRestoreInfo(const QPoint point, const QSize size);
    void getRestoreInfo(QPoint &point, QSize &size);

private:
    void paintEvent(QPaintEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void initControl();
    void initConnections();
    void loadStyleSheet();

signals:
    void signalButtonMinClicked();
    void signalButtonCloseClicked();

private slots:
    void onButtonMinClicked();
    void onButtonCloseClicked();
    void onRollTitle();

private:
    QLabel *m_pIcon;
    QLabel *m_pTitleContent;
    QPushButton *m_pButtonMin;
    QPushButton *m_pButtonClose;

    int m_colorR;
    int m_colorG;
    int m_colorB;

    QPoint m_restorePos;
    QSize m_restoreSize;
    bool m_isPressed;
    QPoint m_startMovePos;
    QTimer m_titleRollTimer;
    QString m_titleContent;
    int m_windowBorderWidth;
    bool m_isTransparent;
};

#endif //CMCL_MYTITLEBAR_H
