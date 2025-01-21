//
// Created by WswDay2022 on 2024/12/7.
//

#include "myControls.h"

myMessageBox::myMessageBox(QWidget *parent,int w,int h)
    : QWidget(parent), width(w), height(h) {
    setControlStyle(CONTROL_INFO);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    setGeometry(0,0,this->parentWidget()->width(),this->parentWidget()->height());
    this->setWindowFlags(Qt::FramelessWindowHint);
    initControl();

    setLayout(mainPane);
}

myMessageBox::~myMessageBox() = default;

void myMessageBox::setControlStyle(controlType type) {
    type_ = type;
    update();
}

void myMessageBox::addChildWidget(QWidget *widget) {
    mainPane->addWidget(widget);
}

void myMessageBox::initControl() {
    mainPane = new QGridLayout();
    mainPane->setContentsMargins(5,5,5,5);
    mainPane->setObjectName("myMessageBox");
    background = new QLabel(this); // 遮蔽
    background->setLayout(mainPane);
    background->setGeometry((this->size().width()-width)/2,
                       (this->size().height()-height)/2,
                       width,height);
    background->setStyleSheet("background-color:white;border:none;border-radius:6px;");

    QIcon icon = QApplication::style()->standardIcon(QStyle::SP_TitleBarCloseButton);
    auto *close = new myIconButton(icon);
    close->setSize(20,20);
    close->setControlStyle(type_);
    connect(close, &QPushButton::clicked,this, &myMessageBox::onCloseButtonClicked);
    mainPane->addWidget(close,1,2);

    auto *shadowEffect = new QGraphicsDropShadowEffect(mainPane);
    shadowEffect->setBlurRadius(50);
    shadowEffect->setXOffset(0);
    shadowEffect->setYOffset(10);
    core core_;core_.globalInit();
    switch (type_) {
        case CONTROL_INFO:
            shadowEffect->setColor(QColor(0, 0, 0, 130));
            break;
        case CONTROL_ERROR:
            core_.errColor.setAlpha(130);
            shadowEffect->setColor(core_.errColor);
            core_.warnColor.setAlpha(255);
            break;
        case CONTROL_WARING:
            core_.warnColor.setAlpha(130);
            shadowEffect->setColor(core_.warnColor);
            core_.warnColor.setAlpha(255);
            break;
    }
    background->setGraphicsEffect(shadowEffect);
}

void myMessageBox::onCloseButtonClicked() {
    auto *opacityEffect = new QGraphicsOpacityEffect();
    //opacityEffect->setOpacity(1);
    //this->setGraphicsEffect(opacityEffect);
    auto *fadeOutAnimation = new QPropertyAnimation(opacityEffect, "opacity");
    fadeOutAnimation->setDuration(250); // 设置消失动画持续时间
    fadeOutAnimation->setStartValue(1);  // 从透明度 1 开始
    fadeOutAnimation->setEndValue(0);    // 到透明度 0 结束
    auto *fadeOutAnimation1 = new QPropertyAnimation(background, "geometry");
    fadeOutAnimation1->setDuration(100); // 设置消失动画持续时间
    fadeOutAnimation1->setStartValue(geometry());  // 从透明度 1 开始
    fadeOutAnimation1->setEndValue(geometry().adjusted(5,5,-10,-10));    // 到透明度 0 结束
    //fadeOutAnimation1->start(QAbstractAnimation::DeleteWhenStopped);
    this->close();
    deleteLater();

    //fadeOutAnimation->start();
    connect(fadeOutAnimation, &QPropertyAnimation::finished,this,[this]() {
        this->close();
        deleteLater();
    });
}

void myMessageBox::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    core core_;core_.globalInit();
    switch (type_) {
        case CONTROL_INFO:
            painter.setBrush(QColor(0,0,0,100));
            break;
        case CONTROL_ERROR:
            core_.errColor.setAlpha(100);
            painter.setBrush(core_.errColor);
            core_.errColor.setAlpha(255);
            break;
        case CONTROL_WARING:
            core_.warnColor.setAlpha(100);
            painter.setBrush(core_.warnColor);
            core_.warnColor.setAlpha(255);
            break;
    }
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRoundedRect(rect().adjusted(10,10,-10,-10), 9.1, 9.1);
}

void myMessageBox::mousePressEvent(QMouseEvent *event) {
    m_isPressed = true;
    m_startMovePos = event->globalPosition().toPoint();
    return QWidget::mousePressEvent(event);
}

void myMessageBox::mouseMoveEvent(QMouseEvent *event) {
    if (m_isPressed) {
        QPoint movePoint = event->globalPosition().toPoint() - m_startMovePos;
        QPoint widgetPos = this->parentWidget()->pos();
        m_startMovePos = event->globalPosition().toPoint();
        this->parentWidget()->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
    }
    return QWidget::mouseMoveEvent(event);
}

void myMessageBox::mouseReleaseEvent(QMouseEvent *event) {
    m_isPressed = false;
    return QWidget::mouseReleaseEvent(event);
}