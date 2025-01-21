//
// Created by WswDay2022 on 2024/11/30.
//

#include "baseWindow.h"
#include "pages/pageBase.h"
#include <QCursor>

baseWindow::baseWindow(QWidget *parent)
    : QWidget(parent) {
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    setAttribute(Qt::WA_TranslucentBackground);
    loadStyleSheet();
    initPanel();
    initTitleBar();
    initMenuBar();
    initPageChanger();

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    core core_;core_.globalInit();
    Json::Value value = core_.getSettingJson();
    int width,height;
    width = value["width"].asInt();
    height = value["height"].asInt();
    setStyleSheet(core_.getQCssFile());

    QScreen *screen = QGuiApplication::primaryScreen();
    setWindowTitle(QString::fromStdString(core_.getSettingJson()["launcherLongName"].asString()));
    setGeometry((screen->geometry().width() - width) / 2,
                (screen->geometry().height() - height) / 2,
                width, height);

    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
}

baseWindow::~baseWindow() = default;

void baseWindow::initPanel() {
    m_panel = new QHBoxLayout(this);
    m_panel->setContentsMargins(10,50,10,10);
    m_panel->setSpacing(0);
}

void baseWindow::initPageChanger() {
    m_pageChange = new pageChange(this);
    m_pageChange->move(260,50);

    m_pageChange->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // 宽度固定，高度自适应
    auto *base = new pageBase();
    base->setBaseSize(100,100);
    base->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_pageChange->addWidget(base);
    m_pageChange->setCurrentIndex(0);
    m_pageChange->setContentsMargins(0,0,0,0);
    m_panel->addWidget(m_pageChange);
}

void baseWindow::initTitleBar() {
    m_titleBar = new myTitleBar(this);
    m_titleBar->move(0, 0);
    core core_;core_.globalInit();
    m_titleBar->setTitleContent(QString::fromStdString(core_.getSettingJson()["launcherLongName"].asString()),12);

    connect(m_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
    connect(m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));
}

void baseWindow::initMenuBar() {
    m_menuBar = new myMainSideBar();
    m_menuBar->move(10,50);
    m_menuBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // 宽度固定，高度自适应
    m_panel->addWidget(m_menuBar);
}

void baseWindow::paintEvent(QPaintEvent* event) {
    return QWidget::paintEvent(event);
}

void baseWindow::loadStyleSheet() {
    // 加载样式表
    core core_; core_.globalInit();
    QString styleSheet = this->styleSheet();
    styleSheet += QLatin1String(core_.getCssFile());
    this->setStyleSheet(styleSheet);
}

void baseWindow::onButtonMinClicked() {
    showMinimized();
}

void baseWindow::onButtonCloseClicked() {
    close();
}

void baseWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) { // 左键按下
        isResizing = event->pos().x() >= width() - 10 && event->pos().y() >= height() - 10;
        if (isResizing) { // 如果处于调整边框范围内
            mousePressPos = event->globalPosition().toPoint();
            windowSize = size();
            event->accept();
        }
    }
    return QWidget::mousePressEvent(event);
}

void baseWindow::mouseMoveEvent(QMouseEvent *event) {
    if (isResizing) {
        // resize the window
        int dx = event->globalPosition().toPoint().x() - mousePressPos.x();
        int dy = event->globalPosition().toPoint().y() - mousePressPos.y();
        setGeometry(x(), y(), windowSize.width() + dx, windowSize.height() + dy);
    } else {
        const int resizeAreaSize = 10;
        if (event->pos().x() >= width() - resizeAreaSize && event->pos().y() >= height() - resizeAreaSize) {
            // 右下角
            setCursor(Qt::SizeBDiagCursor);
        } else if (event->pos().x() < resizeAreaSize && event->pos().y() < resizeAreaSize) {
            // 左上角
            setCursor(Qt::SizeFDiagCursor);
        } else if (event->pos().x() < resizeAreaSize && event->pos().y() >= height() - resizeAreaSize) {
            // 左下角
            setCursor(Qt::SizeBDiagCursor);
        } else if (event->pos().x() >= width() - resizeAreaSize && event->pos().y() < resizeAreaSize) {
            // 右上角
            setCursor(Qt::SizeFDiagCursor);
        } else if (event->pos().x() < resizeAreaSize) {
            // 左边缘
            setCursor(Qt::SizeHorCursor);
        } else if (event->pos().x() >= width() - resizeAreaSize) {
            // 右边缘
            setCursor(Qt::SizeHorCursor);
        } else if (event->pos().y() < resizeAreaSize) {
            // 上边缘
            setCursor(Qt::SizeVerCursor);
        } else if (event->pos().y() >= height() - resizeAreaSize) {
            // 下边缘
            setCursor(Qt::SizeVerCursor);
        } else {
            // 默认光标
            setCursor(Qt::ArrowCursor);
        }
    }
    return QWidget::mouseMoveEvent(event);
}

void baseWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isResizing = false; // 结束调整大小
    }
    return QWidget::mouseReleaseEvent(event);
}