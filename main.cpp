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
        pixmap.load(":/new/ui/ui/1.jpg");

        auto systemTray = new QSystemTrayIcon(pixmap, &app);
        systemTray->setToolTip(QSystemTrayIcon::tr("Activator"));
        systemTray->setVisible(true);
        //QMenu *trayIconMenu = new QMenu(&assistance);

//        trayIconMenu.addAction(QAcm_minimizeAction);
//        trayIconMenu->addAction(m_maximizeAction);
//        trayIconMenu->addAction(m_restoreAction);
//        trayIconMenu->addSeparator();
//        trayIconMenu->addAction(m_quitAction);
//        trayIcon->setContextMenu(m_trayIconMenu);
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


    assistance.show();//assistance
    //QWidget *u;
    Dtk::Widget::moveToCenter(&assistance);


    return app.exec();
}
