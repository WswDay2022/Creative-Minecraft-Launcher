//
// Created by WswDay2022 on 2024/12/14.
//

#include "pageBase.h"

pageBase::pageBase(QWidget *parent)
    : QWidget(parent) {
    setObjectName("pageBase");
    initControls();
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

void pageBase::initControls() {
    mainLayout_ = new QHBoxLayout();
    mainLayout_->setContentsMargins(0,0,0,0);
    mainLayout_->setSpacing(0);
    list = new myContentList();
    list->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    mainLayout_->addWidget(list);
    setLayout(mainLayout_);
    list->loadWidget();
}

void pageBase::startPage() {
    myAnimator animator(150);
    for (int i = 0; i < mainLayout_->count(); ++i) {
        QLayoutItem* item = mainLayout_->itemAt(i);
        if (item->widget()) {
            QWidget *widget = item->widget(); // 获取小部件指针
            auto opacityAnimation = animator.opacityAnimation(widget,0,1,false);
            auto geometryAnimation = animator.geometryAnimation(widget,widget->geometry().adjusted(2,2,-4,-4),widget->geometry().adjusted(-2,-2,4,4),false);

            opacityAnimation->setDuration(150 + i);
            geometryAnimation->setDuration(150 + i);

            auto *group = new QParallelAnimationGroup(widget);
            group->addAnimation(opacityAnimation);
            group->addAnimation(geometryAnimation);
            connect(group,&QPropertyAnimation::finished,this,[=]() {
                widget->show();
            });
            group->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }
}

void pageBase::closePage() {
    myAnimator animator(150);
    for (int i = mainLayout_->count()-1; i >= 0; --i) {
        QLayoutItem* item = mainLayout_->itemAt(i);
        if (item->widget()) {
            QWidget *widget = item->widget(); // 获取小部件指针
            auto opacityAnimation = animator.opacityAnimation(widget,1,0,false);
            auto geometryAnimation = animator.geometryAnimation(widget,widget->geometry(),widget->geometry().adjusted(2,2,-4,-4),false);

            opacityAnimation->setDuration(150 + i);
            geometryAnimation->setDuration(150 + i);

            auto *group = new QParallelAnimationGroup(widget);
            group->addAnimation(opacityAnimation);
            group->addAnimation(geometryAnimation);
            connect(group,&QPropertyAnimation::finished,this,[=]() {
                widget->deleteLater();
            });
            group->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }
}

void pageBase::paintEvent(QPaintEvent *event) {
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
    return QWidget::paintEvent(event);
}

pageBase::~pageBase() = default;
