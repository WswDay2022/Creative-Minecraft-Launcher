//
// Created by WswDay2022 on 2024/12/6.
//

#include "myControls.h"

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
    colorAnimation->setTargetObject(shadowEffect);
    colorAnimation->setParent(shadowEffect);
    colorAnimation->setDuration(150);
}

myContentCard::~myContentCard() = default;

void myContentCard::setHeight(int h) {
    setFixedHeight(h);
    // setMaximumHeight(h);

    oldHeight = h;
    oldContentHeight = h-60;
}

void myContentCard::setWidth(int w) {
    setFixedWidth(w);
    // setMaximumWidth(w);
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
                // deleteLater();
                delete animation;
            });
        });
    }

    QWidget* contentContainer = new QWidget();
    contentLayout = new QGridLayout(contentContainer);  // 将内容布局绑定到容器
    contentLayout->setContentsMargins(5,5,5,5);
    contentLayout->setAlignment(Qt::AlignTop);
    oldContentHeight = contentContainer->height();

    panel->addLayout(bar);
    panel->addWidget(contentContainer);
    setLayout(panel);
}

void myContentCard::toggleCard() {
    /*
    myAnimator animator(150);
    auto *animation = animator.fixedHeightAnimation(this,this->height(),50,false);
    auto *returnAnimation = animator.fixedHeightAnimation(this,animation->endValue().toInt(),oldHeight,false);

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
     -----------------------
    if (m_isAnimating || !canToggle) return; // 防止重复触发和无效操作
    m_isAnimating = true;

    const int animationDuration = 150;
    const QEasingCurve curve = QEasingCurve::OutCubic;
    myAnimator animator(1000);

    // 根据当前状态决定动画方向
    if (!isToggle) {
        rotatedToggleButton->setAngle(14);
        // 折叠动画
        oldHeight = height(); // 保存展开时的高度
        auto* collapseAnim = animator.fixedHeightAnimation(this, oldHeight, 50, false);
        collapseAnim->setEasingCurve(curve);

        // 子部件渐隐
        for (int i = 0; i < contentLayout->count(); ++i) {
            if (auto widget = contentLayout->itemAt(i)->widget()) {
                connect(animator.opacityAnimation(widget, 1, 0, true),&QAbstractAnimation::finished,[=]() {
                    widget->hide();
                });
            }
        }

        connect(collapseAnim, &QPropertyAnimation::finished, this, [=] {
            toggleButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarUnshadeButton));
            isToggle = true;
            m_isAnimating = false;
            collapseAnim->deleteLater();
        });
        collapseAnim->start();
    } else {
        // 展开动画
        auto* expandAnim = animator.fixedHeightAnimation(this, 50, oldHeight, false);
        expandAnim->setEasingCurve(curve);

        // 子部件渐显
        for (int i = 0; i < contentLayout->count(); ++i) {
            if (auto widget = contentLayout->itemAt(i)->widget()) {
                widget->show();
                animator.opacityAnimation(widget, 0, 1, true)->start();
            }
        }

        connect(expandAnim, &QPropertyAnimation::finished, this, [=] {
            toggleButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_TitleBarShadeButton));
            isToggle = false;
            m_isAnimating = false;
            expandAnim->deleteLater();
        });
        expandAnim->start();
    }
     */
    if (m_isAnimating || !canToggle) return;
    m_isAnimating = true;
    myAnimator animator(150);
    QWidget* contentContainer = contentLayout->parentWidget();

    const int contentHeight = contentContainer->height();
    auto* collapseAnimation = animator.fixedHeightAnimation(contentContainer, contentHeight, 0, false);
    auto* collapseAnimation1 = animator.fixedHeightAnimation(this, this->height(), 50, false);
    auto* expandAnimation = animator.fixedHeightAnimation(contentContainer, 0, oldContentHeight, false);
    auto* expandAnimation1 = animator.fixedHeightAnimation(this, 50, oldHeight, false);

    if (!isToggle) {
        // 折叠内容区域（保留标题高度）
        QEasingCurve curve(QEasingCurve::OutElastic);
        //curve.setAmplitude(1.5); // 设置振幅
        curve.setPeriod(2.1);    // 设置周期

        collapseAnimation->setEasingCurve(curve);
        collapseAnimation1->setEasingCurve(curve);

        auto *group = new QParallelAnimationGroup(this);
        group->addAnimation(collapseAnimation);
        group->addAnimation(collapseAnimation1);
        connect(group, &QPropertyAnimation::finished, [=] {
            isToggle = true;
            m_isAnimating = false;
        });
        group->start(QAbstractAnimation::DeleteWhenStopped);
    } else {
        // 展开内容区域到原始高度
        QEasingCurve curve(QEasingCurve::OutElastic);
        //curve.setAmplitude(1.5); // 设置振幅
        curve.setPeriod(2.1);    // 设置周期

        expandAnimation->setEasingCurve(curve);
        expandAnimation1->setEasingCurve(curve);

        auto *group = new QParallelAnimationGroup(this);
        group->addAnimation(expandAnimation);
        group->addAnimation(expandAnimation1);

        connect(group, &QPropertyAnimation::finished, [=] {
            isToggle = false;
            m_isAnimating = false;
        });
        group->start(QAbstractAnimation::DeleteWhenStopped);
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