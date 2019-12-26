/*
 * Copyright (C) 2011 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     guopeng <guopeng_cm@deepin.com>
 *
 * Maintainer: guopeng <guopeng_cm@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "uihelper.h"
#include <QMenu>
#include <QContextMenuEvent>
#include <QDebug>
#include <QPainter>

Label::Label(QWidget *parent, Qt::WindowFlags f): QLabel(parent, f)
{
    setFixedHeight(16);
    setContentsMargins(0, 0, 0, 0);
}

Label::Label(const QString &text, QWidget *parent, Qt::WindowFlags f): QLabel(text, parent, f)
{
    setFixedHeight(16);
    setContentsMargins(0, 0, 0, 0);
}

void Label::mousePressEvent(QMouseEvent *ev)
{
    Q_EMIT clicked();
    QLabel::mousePressEvent(ev);
}

DThinVBoxLayout::DThinVBoxLayout(QWidget *parent): QVBoxLayout(parent)
{
    setMargin(0);
    setSpacing(0);
}

DThinHBoxLayout::DThinHBoxLayout(QWidget *parent): QHBoxLayout(parent)
{
    setMargin(0);
    setSpacing(0);
}

TitleLabel::TitleLabel(QWidget *parent, QPixmap pixmap): QLabel(parent),m_pixmap(pixmap)
{
    setPixmap(m_pixmap);
    resize(156,46);
    setAutoFillBackground(true);
}

HelpLabel::HelpLabel(QWidget *parent, QPixmap pixmap): QLabel(parent),m_pixmap(pixmap)
{
    setPixmap(m_pixmap);
    resize(15,15);
    setAutoFillBackground(true);
    setToolTip(tr("Click for help."));
}

void HelpLabel::mousePressEvent(QMouseEvent *event)
{

}
