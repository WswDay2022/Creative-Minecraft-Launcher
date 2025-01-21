//
// Created by WswDay2022 on 2024/12/6.
//

#include "myControls.h"
#include "../animation/myAnimator.h"

myContentCard::myContentCard(QWidget *parent,bool canToggle,bool canClose)
    : QWidget(parent), canToggle(canToggle), haveCloseButton(canClose) {
    setObjectName("myContentCard");
    setStyleSheet("background-color:white");
    setAutoFillBackground(true);
    setMinimumHeight(50);
    initControl();

    shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(10);
    shadowEffect->setXOffset(0);
    shadowEffect->setYOffset(1);
    shadowEffect->setColor(QColor(0, 0, 0, 150));

    setGraphicsEffect(shadowEffect);
    colorAnimation = new QPropertyAnimation(shadowEffect, "color");
    colorAnimation->setDuration(150);
}

myContentCard::~myContentCard() = default;

void myContentCard::setHeight(int h) {
    setFixedHeight(h);
    setMaximumHeight(h);
    oldHeight = h;
}

void myContentCard::setWidth(int w) {
    setFixedWidth(w);
    setMaximumWidth(w);
    oldWidth = w;
}

void myContentCard::setCanToggle(bool b) {
    canToggle = b;
    update();
}

void myContentCard::setHaveCloseButton(bool b) {
    haveCloseButton = b;
    update();
}

void myContentCard::addChildWidget(QWidget *widget) {
    contentLayout->addWidget(widget);
}

void myContentCard::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawRoundedRect(rect(),6,6);
    return QWidget::paintEvent(event);
}

void myContentCard::setTitle(const QString &content) {
    titleContent = content;
    title->setText(titleContent);
}

void myContentCard::initControl() {
    QVBoxLayout *panel = new QVBoxLayout();
    panel->setContentsMargins(10,10,10,10);
    panel->setSpacing(5);
    QHBoxLayout *bar = new QHBoxLayout();
    bar->setContentsMargins(0,0,0,0);
    bar->setSpacing(0);
    panel->setAlignment(Qt::AlignmentFlag::AlignTop);

    title = new QLabel(titleContent);
    title->setFixedHeight(30);
    title->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    core core_;core_.globalInit();
    QPalette palette1 = title->palette();
    palette1.setColor(QPalette::Text,core_.fontColor);
    title->setPalette(palette1);

    QIcon closeIcon = QApplication::style()->standardIcon(QStyle::SP_TitleBarCloseButton);
    QIcon toggleIcon = QApplication::style()->standardIcon(QStyle::SP_TitleBarShadeButton);
    myIconButton *closeButton = new myIconButton(closeIcon);
    closeButton->setControlStyle(controlType::CONTROL_ERROR);
    closeButton->setFixedSize(30,30);
    closeButton->setIconSize(QSize(16,16));
    toggleButton = new myIconButton(toggleIcon);
    toggleButton->setFixedSize(30,30);
    toggleButton->setIconSize(QSize(16,16));

    bar->addWidget(title);
    if (canToggle) {
        toggleButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        bar->addWidget(toggleButton);
        connect(toggleButton,&QPushButton::clicked,this,&myContentCard::toggleCard);
    }
    if (haveCloseButton) {
        closeButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        bar->addWidget(closeButton);
        connect(closeButton,&QPushButton::clicked, this,[=]() {
            auto *animation = new QPropertyAnimation(this,"geometry");
            animation->setDuration(100);
            animation->setStartValue(geometry());
            animation->setEndValue(geometry().adjusted(5,5,-10,-10));
            animation->start(QAbstractAnimation::DeleteWhenStopped);
            connect(animation,&QPropertyAnimation::finished,this,[=]() {
                this->close();
                deleteLater();
            });
        });
    }

    contentLayout = new QGridLayout();
    contentLayout->setContentsMargins(0,0,0,0);
    panel->addLayout(bar);
    panel->addLayout(contentLayout);
    setLayout(panel);
}

void myContentCard::toggleCard() {
    auto *animation = new QPropertyAnimation(this,"fixedHeight");
    animation->setDuration(150);
    animation->setStartValue(this->height());
    animation->setEndValue(50);

    auto *returnAnimation = new QPropertyAnimation(this,"fixedHeight");
    returnAnimation->setDuration(150);
    returnAnimation->setStartValue(animation->endValue());
    returnAnimation->setEndValue(oldHeight);

    animation->setEasingCurve(QEasingCurve::OutCubic);
    returnAnimation->setEasingCurve(QEasingCurve::OutCubic);

    if (canToggle) {
        if (!m_isAnimating) {
            m_isAnimating = true;
            if (this->height() > 50) {
                myAnimator animator(150);
                animation->start(QAbstractAnimation::DeleteWhenStopped);
                for (int i = 0; i < contentLayout->count(); ++i) {
                    QLayoutItem* item = contentLayout->itemAt(i);
                    if (item->widget()) {
                        auto a = animator.opacityAnimation(item->widget(),1,0,true);
                        connect(a,&QPropertyAnimation::finished,this,[=]() {
                            item->widget()->hide();
                            a->deleteLater();
                        });
                    }
                }
                connect(animation, &QPropertyAnimation::finished, this, [=]() {
                    animation->deleteLater();
                    isToggle = true;
                    QIcon toggleIcon = QApplication::style()->standardIcon(QStyle::SP_TitleBarUnshadeButton);
                    toggleButton->setIcon(toggleIcon);
                    m_isAnimating = false;
                });
            } else if (this->height() == 50) {
                returnAnimation->start(QAbstractAnimation::DeleteWhenStopped);
                connect(returnAnimation, &QPropertyAnimation::finished, this, [=]() {
                    for (int i = 0; i < contentLayout->count(); ++i) {
                        QLayoutItem* item = contentLayout->itemAt(i);
                        if (item->widget()) {
                            item->widget()->show();
                            myAnimator animator(150);
                            auto a = animator.opacityAnimation(item->widget(),0,1,true);
                            connect(a,&QPropertyAnimation::finished,this,[=]() {
                                a->deleteLater();
                            });
                        }
                    }
                    QIcon toggleIcon = QApplication::style()->standardIcon(QStyle::SP_TitleBarShadeButton);
                    toggleButton->setIcon(toggleIcon);
                    m_isAnimating = false;
                    isToggle = false;
                    returnAnimation->deleteLater();
                });
            }
        }
    }
}

void myContentCard::enterEvent(QEnterEvent *event) {
    core core_;core_.globalInit();
    colorAnimation->setStartValue(QColor(0, 0, 0,150));
    colorAnimation->setEndValue(core_.themeColor);
    colorAnimation->start();
    myAnimator animator(150);
    animator.fontColorAnimation(title,core_.fontColor,core_.themeColor,true);
    return QWidget::enterEvent(event);
}

void myContentCard::leaveEvent(QEvent *event) {
    core core_;core_.globalInit();
    colorAnimation->setStartValue(core_.themeColor);
    colorAnimation->setEndValue(QColor(0, 0, 0,150));
    colorAnimation->start();
    myAnimator animator(150);
    animator.fontColorAnimation(title,core_.themeColor,core_.fontColor,true);
    return QWidget::leaveEvent(event);
}

int myContentCard::fixedHeight() const {
    return m_fixedHeight;
}

void myContentCard::setHeightFixed(const int &height) {
    if (m_fixedHeight != height) {
        m_fixedHeight = height;
        setFixedHeight(height);
        emit fixedHeightChanged();
    }
}