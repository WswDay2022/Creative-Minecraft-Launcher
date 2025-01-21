//
// Created by WswDay2022 on 2024/11/30.
//

// 工程量巨大啊qwq

#ifndef CMCL_MYCONTROLS_H
#define CMCL_MYCONTROLS_H

#include "../animation/bounceAnimation.h"
#include "../../../core/io/fileReader.h"
#include "../../../core/json/json.h"
#include "../../../core/core.h"
#include "../animation/myAnimator.h"

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

// 控件类型
enum controlType {
    CONTROL_WARING,
    CONTROL_INFO,
    CONTROL_ERROR
};

// #########################
// ###### BUTTONS 按钮 ######
// #########################

// 常规按钮
class myButton : public QPushButton {
    Q_OBJECT

public:
    myButton(const QString &text, QWidget *parent = nullptr);
    ~myButton();

    void setSize(QSize size);
    void setSize(int w,int h);
    void setControlStyle(controlType type);
    controlType getControlStyle() { return type_; };

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Q_PROPERTY(int textSize READ textSize WRITE setTextSize NOTIFY textSizeChanged)

    int textSize() const;
    void setTextSize(const int &size);

signals:
    void textSizeChanged();

private:
    controlType type_;
    bool m_isAnimating = false; // 跟踪动画状态
    QGraphicsDropShadowEffect *shadowEffect;
    QPropertyAnimation *colorAnimation;
    int m_textSize;
};

// 图标按钮
class myIconButton : public QPushButton {
    Q_OBJECT

public:
    myIconButton(const QIcon &icon,QWidget *parent = nullptr);
    ~myIconButton();

    void setSize(QSize size);
    void setSize(int w,int h);
    void setButtonIcon(QIcon icon);
    void setControlStyle(controlType type);
    controlType getControlStyle() { return type_; };

    void rotationAnimation(const int &startValue,const int &endValue,int duration);

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    Q_PROPERTY(qreal rotationAngle READ rotationAngle WRITE setRotationAngle)

    void setRotationAngle(qreal angle);
    qreal rotationAngle() { return m_rotationAngle; }
    void animateIconColor(const QColor &startColor, const QColor &endColor, int duration);

signals:
    void rotationAngleChanged();

private:
    controlType type_;
    QIcon m_icon;
    qreal m_rotationAngle = 0;
    bool m_isAnimating = false; // 跟踪动画状态
};

// 图标文字按钮
class myTextIconButton : public QPushButton {
    Q_OBJECT

public:
    myTextIconButton(QWidget *parent = nullptr);
    ~myTextIconButton();
};

// 文字按钮
class myTextButton : public QPushButton {
    Q_OBJECT

public:
    myTextButton(const QString &text, QWidget *parent = nullptr);
    ~myTextButton();

    void setControlStyle(controlType type);
    void setTextColor(QColor color);
    void setTextFont(QFont font);
    void setTextSize(int pointSize);
    controlType getControlStyle() { return type_; };

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

// 圆形按钮 & 圆形图标按钮
// 直接继承myButton,改一下paintEvent即可
// 其他属性均与myButton相同
class myRoundButton : public myButton {
    Q_OBJECT

public:
    myRoundButton(QWidget *parent = nullptr);
    ~myRoundButton();

protected:
    void paintEvent(QPaintEvent *event) override;
};
class myRoundIconButton : public myRoundButton {
    Q_OBJECT

public:
    myRoundIconButton(const QIcon &icon,QWidget *parent = nullptr);
    ~myRoundIconButton();

    void setButtonIcon(QIcon icon);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    Q_PROPERTY(QSize sizeIcon READ sizeIcon WRITE setSizeIcon NOTIFY iconSizeChanged)

    QSize sizeIcon() const;
    void setSizeIcon(const QSize &size);

signals:
    void iconSizeChanged();

private:
    bool m_isAnimating = false;
    QSize m_iconSize;
};

// ############################
// ###### 布局/面板 PANEL ######
// ############################

class myContentCard : public QWidget {
    Q_OBJECT

public:
    myContentCard(QWidget *parent = nullptr,bool canToggle = false,bool canClose = true);
    ~myContentCard();

    void initControl();
    void addChildWidget(QWidget *widget);
    QGridLayout *getContentLayout() { return contentLayout; };

    void setCanToggle(bool b);
    void setHaveCloseButton(bool b);
    void setTitle(const QString &content);

    void setHeight(int h);
    void setWidth(int w);

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private slots:
    void toggleCard();

private:
    Q_PROPERTY(int fixedHeight READ fixedHeight WRITE setHeightFixed NOTIFY fixedHeightChanged)

    int fixedHeight() const;
    void setHeightFixed(const int &height);

signals:
    void fixedHeightChanged();

private:
    int oldHeight;
    int oldWidth;

    bool isToggle = false;
    bool m_isAnimating = false;
    bool canToggle;
    bool haveCloseButton;
    QString titleContent;

    int m_fixedHeight;
    QColor m_fontColor;

    QLabel *title;
    myIconButton *toggleButton;
    QGraphicsDropShadowEffect *shadowEffect;
    QPropertyAnimation *colorAnimation;
    QGridLayout *contentLayout;
};

class myMessageBox : public QWidget {
    Q_OBJECT

public:
    myMessageBox(QWidget *parent,int w = 500,int h = 300);
    ~myMessageBox();

    void initControl();
    void setControlStyle(controlType type);
    void addChildWidget(QWidget *widget);
    controlType getControlStyle() { return type_; }

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
    QLabel *background;
    QGridLayout *mainPane;
};

#endif //CMCL_MYCONTROLS_H
