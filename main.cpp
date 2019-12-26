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
#include "assistancewindow.h"
#include <DApplication>
#include <QApplication>
#include <DGuiApplicationHelper>
#include <QSystemTrayIcon>

#include <DWidgetUtil>
#include <DHiDPIHelper>

using namespace Dtk::Widget;
using namespace Dtk::Core;

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication app(argc, argv);
    DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::LightType);
    app.setOrganizationName("deepin");
    app.setApplicationName("assistance");
    app.setApplicationLicense("Peng Guo.");
    app.setApplicationHomePage(QString("https://www.chinauos.com"));
    app.setQuitOnLastWindowClosed(false);

    app.loadTranslator();

    AssistanceWindow assistance;
    //if (isUseSystemTry())
    {
        QPixmap pixmap;
        pixmap.load(":/new/ui/ui/uos-win.svg");

        auto systemTray = new QSystemTrayIcon(pixmap, &app);
        systemTray->setToolTip(QSystemTrayIcon::tr("Activator"));
        systemTray->setVisible(true);

        if (systemTray)
        {
            systemTray->connect(systemTray, &QSystemTrayIcon::activated,
                      &app, [ & ](QSystemTrayIcon::ActivationReason )
            {
                assistance.show();
            });
            systemTray->show();
        }
    }

    assistance.show();
    Dtk::Widget::moveToCenter(&assistance);
    return app.exec();
}
