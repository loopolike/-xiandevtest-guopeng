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
#ifndef ASSISTANCEWINDOW_H
#define ASSISTANCEWINDOW_H
#include <QScopedPointer>
#include <DMainWindow>
DWIDGET_USE_NAMESPACE
#include <QStackedWidget>
#include <QObject>
#include <QCloseEvent>

class AssistanceWindowPrivate;
class AssistanceWindow : public DMainWindow
{
    Q_OBJECT

public:
    explicit AssistanceWindow(QWidget *parent = Q_NULLPTR);
    ~AssistanceWindow() Q_DECL_OVERRIDE;
    qint64 ValidateCode();
private:
    qint64 createValidateCode();

protected:
     void closeEvent(QCloseEvent *event);

public slots:
    void updateProgress();
private:
    qint64 m_nValidateCode;
    QStackedWidget *stackMainWindows;
    QScopedPointer<AssistanceWindowPrivate> dd_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(dd_ptr), AssistanceWindow)
};

#endif // ASSISTANCEWINDOW_H
