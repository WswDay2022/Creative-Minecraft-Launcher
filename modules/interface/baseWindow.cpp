//
// Created by WswDay2022 on 2024/11/30.
//

#include "baseWindow.h"

namespace {
    constexpr int RESIZE_AREA_SIZE = 12;  // 边缘检测区域大小
    const Qt::CursorShape DEFAULT_CURSOR = Qt::ArrowCursor;
}

baseWindow::baseWindow(QWidget *parent)
    : QWidget(parent) {
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    setAttribute(Qt::WA_TranslucentBackground);
    loadStyleSheet();
    initPanel();
    initTitleBar();
    initMenuBar();
    initPageChanger();
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    core core_;core_.globalInit();
    Json::Value value = core_.getSettingJson();
    int width,height;
    width = value["width"].asInt();
    height = value["height"].asInt();
    setStyleSheet(core_.getQCssFile());

    const QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    setGeometry(
        (screenGeometry.width() - width) / 2,
        (screenGeometry.height() - height) / 2,
        width, height
    );

    setMinimumSize(700,400);

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
    auto *base = new mainPage();
    //base->setBaseSize(100,100);
    // base->startPage();

    m_pageChange->addWidget(base);
    m_pageChange->setCurrentIndex(0);
    m_pageChange->setContentsMargins(0,0,0,0);
    m_panel->addWidget(m_pageChange);
}

void baseWindow::initTitleBar() {
    m_titleBar = new myTitleBar(this);
    m_titleBar->move(10, 10);
    m_titleBar->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_titleBar->setTitleWidth(this->width());
    static core core_;
    core_.globalInit();
    m_titleBar->setTitleContent(QString::fromStdString(core_.getSettingJson()["launcherLongName"].asString()),12);

    connect(m_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
    connect(m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));
}

void baseWindow::changePage(pageBase *page) {
    m_pageChange->changePage(page);
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
    myAnimator animator(100);
    auto *fadeAnimation = animator.windowOpacityAnimation(this,1,0,true);
    connect(fadeAnimation,&QPropertyAnimation::finished,[=](){
        showMinimized();
        setWindowOpacity(1);
    });

}

void baseWindow::onButtonCloseClicked() {
    myAnimator animator(100);
    auto *fadeAnimation = animator.windowOpacityAnimation(this,1,0,true);
    connect(fadeAnimation,&QPropertyAnimation::finished,[=](){close();});
}

void baseWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_resizeEdge = calculateResizeEdge(event->pos());
        if (m_resizeEdge != ResizeEdge::None) {
            m_mousePressPos = event->globalPosition().toPoint();
            m_windowSize = size();
            event->accept();
            return;
        }
    }
    return QWidget::mousePressEvent(event);
}

void baseWindow::mouseMoveEvent(QMouseEvent *event) {
    if (m_resizeEdge != ResizeEdge::None) {
        handleResize(event->globalPosition().toPoint());
    } else {
        updateCursorShape(event->pos());
    }
    return QWidget::mouseMoveEvent(event);
}

void baseWindow::enterEvent(QEnterEvent *event) {
    if (m_resizeEdge != ResizeEdge::None) {
        handleResize(event->globalPosition().toPoint());
    } else {
        updateCursorShape(event->pos());
    }
    return QWidget::enterEvent(event);
}

void baseWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_resizeEdge = ResizeEdge::None;
        setCursor(DEFAULT_CURSOR);
    }
    return QWidget::mouseReleaseEvent(event);
}

baseWindow::ResizeEdge baseWindow::calculateResizeEdge(const QPoint &pos) const {
    const int x = pos.x();
    const int y = pos.y();
    const int w = width();
    const int h = height();

    // 检测边缘和角落
    const bool left = (x <= RESIZE_AREA_SIZE);
    const bool right = (x >= w - RESIZE_AREA_SIZE);
    const bool top = (y <= RESIZE_AREA_SIZE);
    const bool bottom = (y >= h - RESIZE_AREA_SIZE);

    if (left && top)       return ResizeEdge::TopLeft;
    if (right && top)      return ResizeEdge::TopRight;
    if (left && bottom)    return ResizeEdge::BottomLeft;
    if (right && bottom)   return ResizeEdge::BottomRight;
    if (left)              return ResizeEdge::Left;
    if (right)             return ResizeEdge::Right;
    if (top)               return ResizeEdge::Top;
    if (bottom)            return ResizeEdge::Bottom;

    return ResizeEdge::None;
}

void baseWindow::updateCursorShape(const QPoint &pos) {
    switch (calculateResizeEdge(pos)) {
        case ResizeEdge::TopLeft:
        case ResizeEdge::BottomRight:
            setCursor(Qt::SizeFDiagCursor);
            break;
        case ResizeEdge::TopRight:
        case ResizeEdge::BottomLeft:
            setCursor(Qt::SizeBDiagCursor);
            break;
        case ResizeEdge::Left:
        case ResizeEdge::Right:
            setCursor(Qt::SizeHorCursor);
            break;
        case ResizeEdge::Top:
        case ResizeEdge::Bottom:
            setCursor(Qt::SizeVerCursor);
            break;
        default:
            setCursor(DEFAULT_CURSOR);
    }
}

void baseWindow::handleResize(const QPoint &globalMousePos) {
    const QPoint delta = globalMousePos - m_mousePressPos;
    QRect newGeometry = geometry();

    switch (m_resizeEdge) {
        case ResizeEdge::Left:
            newGeometry.setLeft(newGeometry.left() + delta.x());
            break;
        case ResizeEdge::Right:
            newGeometry.setRight(newGeometry.right() + delta.x());
            break;
        case ResizeEdge::Top:
            newGeometry.setTop(newGeometry.top() + delta.y());
            break;
        case ResizeEdge::Bottom:
            newGeometry.setBottom(newGeometry.bottom() + delta.y());
            break;
        case ResizeEdge::TopLeft:
            newGeometry.setTopLeft(newGeometry.topLeft() + delta);
            break;
        case ResizeEdge::TopRight:
            newGeometry.setTopRight(newGeometry.topRight() + QPoint(delta.x(), delta.y()));
            break;
        case ResizeEdge::BottomLeft:
            newGeometry.setBottomLeft(newGeometry.bottomLeft() + QPoint(delta.x(), delta.y()));
            break;
        case ResizeEdge::BottomRight:
            newGeometry.setBottomRight(newGeometry.bottomRight() + delta);
            break;
        default: return;
    }

    // 限制最小尺寸
    if (newGeometry.width() < minimumWidth()) {
        newGeometry.setWidth(minimumWidth());
    }
    if (newGeometry.height() < minimumHeight()) {
        newGeometry.setHeight(minimumHeight());
    }

    setGeometry(newGeometry);
    m_mousePressPos = globalMousePos; // 更新基准点
}