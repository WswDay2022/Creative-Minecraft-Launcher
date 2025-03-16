//
// Created by WswDay2022 on 2024/12/14.
//

#include "pageChange.h"

pageChange::pageChange(QWidget *parent)
    : QStackedWidget(parent) {
    setObjectName("pageChange");
    initControl();
    update();
}

void pageChange::initControl() {
}

void pageChange::changePage(pageBase *page) {
    addWidget(page);
}

void pageChange::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    myControls controlTools;
    static core core_;
    core_.globalInit();
    painter.setBrush(controlTools.lighterColor(core_.themeColor,160));

    QPainterPath pathBack;
    QRect rect(0, 0, this->width(), this->height());
    pathBack.moveTo(rect.topLeft());
    pathBack.lineTo(rect.right() - 12, rect.top());
    QRectF cornerArcRect(
            rect.right() - 12,
            rect.bottom() - 12,
            12,12
    );
    pathBack.arcTo(cornerArcRect, 0, 90);
    pathBack.lineTo(rect.left(), rect.bottom());
    pathBack.closeSubpath();

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(pathBack, QBrush(controlTools.lighterColor(core_.themeColor,160)));
    return QStackedWidget::paintEvent(event);
}

pageChange::~pageChange() = default;
