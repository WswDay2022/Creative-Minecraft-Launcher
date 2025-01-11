//
// Created by WswDay2022 on 2024/11/30.
//

#include "myControls.h"

QIcon myControls::setIconColor(QIcon icon, QColor color) {
    QPixmap pixmap = icon.pixmap(QSize(64,64));
    QPainter painter(&pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(pixmap.rect(), color);
    QIcon colorIcon = QIcon(pixmap);
    return colorIcon;
}

QColor myControls::lighterColor(QColor color,double f) {
    return color.lighter(int(f));
}