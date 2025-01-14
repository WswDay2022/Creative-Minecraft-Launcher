//
// Created by WswDay2022 on 2024/11/30.
//

#ifndef CMCL_MYCONTROLS_H
#define CMCL_MYCONTROLS_H

#include "../animation/bounceAnimation.h"
#include "../../../core/io/fileReader.h"
#include "../../../core/json/json.h"
#include "../../../core/core.h"

#include <QTimer>
#include <QPainterPath>
#include <QMouseEvent>
#include <QMainWindow>
#include <QApplication>
#include <QWidget>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPushButton>
#include <QFontDatabase>
#include <QPropertyAnimation>
#include <QUndoView>
#include <QLabel>
#include <QGridLayout>

class myControls {
public:
    QIcon setIconColor(QIcon icon,QColor color);
    QColor lighterColor(QColor color,double f);
};

enum controlType {
    CONTROL_WARING,
    CONTROL_INFO,
    CONTROL_ERROR
};

class myButton : public QPushButton {
    Q_OBJECT

public:
    myButton(const QString &text, QWidget *parent = nullptr);
    ~myButton();

    void setSize(QSize size);
    void setSize(int w,int h);
    void setStyle(controlType type);

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    controlType type_;
    bool m_isAnimating = false; // 跟踪动画状态
    QGraphicsDropShadowEffect *shadowEffect;
    QPropertyAnimation *colorAnimation;
};

class myIconButton : public QPushButton {
    Q_OBJECT

public:
    myIconButton(const QIcon &icon,QWidget *parent = nullptr);
    ~myIconButton();

    void setButtonIcon(QIcon icon);
    void setStyle(controlType type);

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    Q_PROPERTY(QColor iconColor READ iconColor WRITE setIconColor NOTIFY iconColorChanged)

    QColor iconColor() const;
    void setIconColor(const QColor &color);
    void animateIconColor(const QColor &startColor, const QColor &endColor, int duration);

signals:
    void iconColorChanged(); // 自定义信号，图标颜色改变时发出

private:
    controlType type_;
    QIcon m_icon;
    QColor m_iconColor;
};

class myTextButton : public QPushButton {
    Q_OBJECT

public:
    myTextButton(const QString &text, QWidget *parent = nullptr);
    ~myTextButton();

    void setStyle(controlType type);
    void setTextColor(QColor color);
    void setTextFont(QFont font);
    void setTextSize(int pointSize);

private:
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextFillColor NOTIFY textColorChanged)

    QColor textColor() const;
    void setTextFillColor(QColor color);
    void animateTextColor(const QColor &startColor, const QColor &endColor, int duration);

signals:
    void textColorChanged();

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    controlType type_;
    QColor m_color;
    QPropertyAnimation *colorAnimation;
};

class myContentCard : public QWidget {
    Q_OBJECT

public:
    myContentCard(QWidget *parent = nullptr);
    ~myContentCard();

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QGraphicsDropShadowEffect *shadowEffect;
    QPropertyAnimation *colorAnimation;
};

class myMessageBox : public QWidget {
    Q_OBJECT

public:
    myMessageBox(QWidget *parent,int w = 500,int h = 300);
    ~myMessageBox();

    void initControl();
    void setStyle(controlType type);
    void addChildWidget(QWidget *widget);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

private slots:
    void onCloseButtonClicked();

private:
    bool m_isPressed;
    QPoint m_startMovePos;
    controlType type_;
    int width;
    int height;
    QGridLayout *mainPane;
};

#endif //CMCL_MYCONTROLS_H
