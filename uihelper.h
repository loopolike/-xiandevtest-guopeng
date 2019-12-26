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
#pragma once


//#include <dlineedit.h>

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QValidator>
#include <QMouseEvent>

const unsigned int TIMERTIME2s = 2000;
const unsigned int TIMERTIME5s = 5000;

class DThinVBoxLayout: public QVBoxLayout
{
    Q_OBJECT
public:
    DThinVBoxLayout(QWidget *parent = nullptr);
};

class DThinHBoxLayout: public QHBoxLayout
{
    Q_OBJECT
public:
    DThinHBoxLayout(QWidget *parent = nullptr);
};

class Label: public QLabel
{
    Q_OBJECT
public:
    Label(QWidget *parent = 0, Qt::WindowFlags f = nullptr);
    Label(const QString &text, QWidget *parent = nullptr, Qt::WindowFlags f = nullptr);

protected:
    void mousePressEvent(QMouseEvent *ev);

Q_SIGNALS:
    void clicked();
};

class TitleLabel: public QLabel
{
    Q_OBJECT
public:
    TitleLabel(QWidget *parent,QPixmap pixmap);
private:
    QPixmap m_pixmap;
};

class HelpLabel: public QLabel
{
    Q_OBJECT
public:
    HelpLabel(QWidget *parent,QPixmap pixmap);
    void mousePressEvent(QMouseEvent *event);

private:
    QPixmap m_pixmap;
};

class QRCodeLabel: public QLabel
{
    Q_OBJECT

public:
    void generateQRCode(QString url);
};

class UpcaseValidator: public QValidator
{
    Q_OBJECT
public:
    UpcaseValidator(QWidget *parent = Q_NULLPTR): QValidator(parent) {}

    virtual State validate(QString &text, int &) const Q_DECL_OVERRIDE
    {
        text = text.toUpper();
        return Acceptable;
    }
};


