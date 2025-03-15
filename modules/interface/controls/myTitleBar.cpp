//
// Created by WswDay2022 on 2024/11/30.
//

#include "myTitlebar.h"
#include "myControls.h"

#define BUTTON_HEIGHT 50
#define BUTTON_WIDTH 30
#define TITLE_HEIGHT 50

myTitleBar::myTitleBar(QWidget *parent)
        : QWidget(parent)
        , m_isPressed(false)
        , m_windowBorderWidth(0)
        , m_isTransparent(false) {
    setBackgroundColor();
    initControl();
    initConnections();
    loadStyleSheet();
}

myTitleBar::~myTitleBar() {}

void myTitleBar::initControl() {
    m_pIcon = new QLabel;
    m_pTitleContent = new QLabel;

    QIcon min = QApplication::style()->standardIcon(QStyle::SP_TitleBarMinButton);
    QIcon close = QApplication::style()->standardIcon(QStyle::SP_TitleBarCloseButton);

    m_pButtonMin = new myIconButton(min);
    m_pButtonClose = new myIconButton(close);
    m_pButtonMin->setControlStyle(CONTROL_WARING);
    m_pButtonClose->setControlStyle(CONTROL_ERROR);

    m_pTitleContent->setFixedHeight(BUTTON_HEIGHT);
    m_pButtonMin->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
    m_pButtonClose->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));

    m_pTitleContent->setObjectName("titleContent");
    m_pButtonMin->setObjectName("hideButton");
    m_pButtonClose->setObjectName("closeButton");

    static core core_;
    core_.globalInit(); // rgb("+core_.themeColor.name()+")
    const QString colorStyle = "{background-color:rgb("+core_.themeColor.name()+");border:none;}";

    m_pTitleContent->setStyleSheet("#titleContent "+colorStyle);
    // m_pButtonMin->setStyleSheet("#hideButton "+colorStyle);
    // m_pButtonClose->setStyleSheet("#closeButton "+colorStyle);

    /*
    QPalette palette = m_pButtonMin->palette();
    QPalette palette1 = m_pButtonClose->palette();
    // m_pTitleContent->setStyleSheet("#titleContent {color:rgb("+core_.fontColor.name()+")}");
    palette.setColor(QPalette::ButtonText,core_.fontColor);
    palette1.setColor(QPalette::ButtonText,core_.fontColor);
    m_pButtonMin->setPalette(palette);
    m_pButtonClose->setPalette(palette1);
     */

    myControls controlTools;
    min = controlTools.setIconColor(min,core_.fontColor);
    close = controlTools.setIconColor(close,core_.fontColor);

    m_pButtonClose->setIcon(close);
    m_pButtonMin->setIcon(min);
    m_pButtonClose->setIconSize(QSize(14,14));
    m_pButtonMin->setIconSize(QSize(14,14));

    QHBoxLayout* titleBar = new QHBoxLayout(this);
    titleBar->addWidget(m_pIcon);
    titleBar->addWidget(m_pTitleContent);
    titleBar->addWidget(m_pButtonMin);
    titleBar->addWidget(m_pButtonClose);
    titleBar->setContentsMargins(20, 10, 20, 0);
    titleBar->setSpacing(0);
    titleBar->setStretchFactor(m_pTitleContent,0);
    m_pTitleContent->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    this->setFixedHeight(TITLE_HEIGHT);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

void myTitleBar::initConnections() {
    connect(m_pButtonMin, &QPushButton::clicked, this, &myTitleBar::onButtonMinClicked);
    connect(m_pButtonClose, &QPushButton::clicked, this, &myTitleBar::onButtonCloseClicked);
}

void myTitleBar::setBackgroundColor(bool isTransparent) {
    core core_;core_.globalInit();
    m_colorR = core_.getSettingJson()["themeColor"][0].asInt();
    m_colorG = core_.getSettingJson()["themeColor"][1].asInt();
    m_colorB = core_.getSettingJson()["themeColor"][2].asInt();
    m_isTransparent = isTransparent;
    update();
}

void myTitleBar::setTitleIcon(QString filePath, QSize IconSize) {
    QPixmap titleIcon(filePath);
    m_pIcon->setPixmap(titleIcon.scaled(IconSize));
}

void myTitleBar::setTitleContent(QString titleContent, int titleFontSize) {
    static core core_;
    core_.globalInit();
    m_pTitleContent->setFont(core::getFont());
    m_pTitleContent->setText("<font color="+core_.fontColor.name()+">"+titleContent+"</font>");
    m_titleContent = titleContent;
}

void myTitleBar::setTitleWidth(int width) {
    this->setFixedWidth(width);
}

void myTitleBar::setTitleRoll() {
    connect(&m_titleRollTimer, &QTimer::timeout, this, &myTitleBar::onRollTitle);
    m_titleRollTimer.start(200);
}

void myTitleBar::setWindowBorderWidth(int borderWidth) {
    m_windowBorderWidth = borderWidth;
}

void myTitleBar::saveRestoreInfo(const QPoint point, const QSize size) {
    m_restorePos = point;
    m_restoreSize = size;
}

void myTitleBar::getRestoreInfo(QPoint& point, QSize& size) {
    point = m_restorePos;
    size = m_restoreSize;
}

void myTitleBar::paintEvent(QPaintEvent *event) {
    if (!m_isTransparent) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(QColor(m_colorR, m_colorG, m_colorB));

        QPainterPath pathBack;
        pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()+10).adjusted(10,10,-10,10), 10, 10);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.fillPath(pathBack, QBrush(QColor(m_colorR, m_colorG, m_colorB)));
    }

    if (this->width() != (this->parentWidget()->width() - m_windowBorderWidth)) {
        this->setFixedWidth(this->parentWidget()->width() - m_windowBorderWidth);
    }
    QWidget::paintEvent(event);
}

void myTitleBar::mouseDoubleClickEvent(QMouseEvent *event) {
    return QWidget::mouseDoubleClickEvent(event);
}

void myTitleBar::mousePressEvent(QMouseEvent *event) {
    m_isPressed = true;
    m_startMovePos = event->globalPosition().toPoint();
    return QWidget::mousePressEvent(event);
}

void myTitleBar::mouseMoveEvent(QMouseEvent *event) {
    if (m_isPressed) {
        QPoint movePoint = event->globalPosition().toPoint() - m_startMovePos;
        QPoint widgetPos = this->parentWidget()->pos();
        m_startMovePos = event->globalPosition().toPoint();
        this->parentWidget()->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
    }
    return QWidget::mouseMoveEvent(event);
}

void myTitleBar::mouseReleaseEvent(QMouseEvent *event) {
    m_isPressed = false;
    return QWidget::mouseReleaseEvent(event);
}

void myTitleBar::loadStyleSheet() {
    core core_; core_.globalInit();
    QString styleSheet = this->styleSheet();
    styleSheet += QLatin1String(core_.getCssFile());
    this->setStyleSheet(styleSheet);
}

void myTitleBar::onButtonMinClicked() {
    emit signalButtonMinClicked();
}

void myTitleBar::onButtonCloseClicked() {
    emit signalButtonCloseClicked();
}

void myTitleBar::onRollTitle() {
    static int nPos = 0;
    QString titleContent = m_titleContent;
    if (nPos > titleContent.length()) nPos = 0;
    m_pTitleContent->setText(titleContent.mid(nPos));
    nPos++;
}