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
#include <QDebug>
#include <QStack>
#include <QDate>
#include <QPainter>
#include <QLabel>
#include <QFileDialog>
#include <QDesktopServices>
#include <QMouseEvent>
#include <DLabel>
#include <ddialog.h>
#include <DThemeManager>
#include <DPushButton>
#include <DWidgetUtil>
#include <DStandardPaths>
#include <DHiDPIHelper>
#include <DTitlebar>
#include <DApplication>
#include <DFrame>
#include <QClipboard>
#include <dtoast.h>
#include <QVBoxLayout>
#include <DSuggestButton>
#include <dfilechooseredit.h>
#include "uihelper.h"
#include <QIcon>
#include <DSpinner>
#include <QTimer>
#include <QMap>
#include <QMovie>
#include <QSize>
#include <QObject>
#include <QNetworkConfigurationManager>

DWIDGET_USE_NAMESPACE

#define LAYOUT_DEBUG

#ifdef LAYOUT_DEBUG
#define DBG(widget) widget->setStyleSheet(QString("%1background:%2;border: 1px solid %3").arg(widget->styleSheet()).arg(randomColor()).arg(randomColor()))
#else
#define DBG(widget) while(0){}
#endif

qint32 g_nTimerTip = 0;
//qint64 g_nValidateCode = 0;

class AssistanceWindowPrivate
{
public:
    AssistanceWindowPrivate(AssistanceWindow *parent) : q_ptr(parent) {}

    void initUI();
    void setBackground(const QPixmap &pixmap);

    QWidget *wrapLayout(QLayout *layout);

//    void pasteInputSerial(QLineEdit *key);
    QWidget *createActions(QList<QWidget *> buttons,bool isVertical=true);

    QWidget *createMainView();
    QWidget *createNeedHelpView();
    QWidget *createNeedHelpCopyView();
    QWidget *createNeedHelpCopyOverView();


    QWidget *createHelpOtherView();
    QWidget *createHelpOtherToDesktopView();
    QWidget *createHelpOtherSucceedView();
    QWidget *createHelpOtherFailureView();

    QWidget *m_mainView           = Q_NULLPTR;
    //QMap<QString, QObject*> m_mainViewUiMap;
    QWidget *m_needHelpView       = Q_NULLPTR;
    //QMap<QString, QObject*> m_needHelpViewUiMap;
    QTimer  *m_waitTimer          = Q_NULLPTR;

    QWidget *m_needHelpCopyView       = Q_NULLPTR;
    QMap<QString, QObject*> m_needHelpCopyViewUiMap;

    QWidget *m_needHelpCopyOverView       = Q_NULLPTR;

    QWidget *m_helpOtherView      = Q_NULLPTR;
    //QMap<QString, QObject*> m_helpOtherViewUiMap;
    QWidget *m_helpOtherToDesktopView      = Q_NULLPTR;

    QWidget *m_helpOtherSucceedView      = Q_NULLPTR;
    QWidget *m_helpOtherFailureView      = Q_NULLPTR;

    //DToast *toast;
    QNetworkConfigurationManager   *m_Mgr;
    AssistanceWindow *q_ptr;
    QClipboard *m_clipboard;
    //QString m_activeCode;


private:
    Q_DECLARE_PUBLIC(AssistanceWindow)
};

void AssistanceWindowPrivate::initUI()
{
    Q_Q(AssistanceWindow);
    m_Mgr = new QNetworkConfigurationManager;
    m_clipboard = DApplication::clipboard();
    //toast = new DToast(q);
    m_mainView = createMainView();
    m_needHelpView  = createNeedHelpView();
    m_waitTimer = new QTimer();
    m_waitTimer->connect(m_waitTimer, SIGNAL(timeout()), q, SLOT(updateProgress()));
    m_needHelpCopyView = createNeedHelpCopyView();
    m_needHelpCopyOverView = createNeedHelpCopyOverView();
    m_helpOtherView = createHelpOtherView();
    m_helpOtherToDesktopView = createHelpOtherToDesktopView();
    m_helpOtherSucceedView = createHelpOtherSucceedView();
    m_helpOtherFailureView = createHelpOtherFailureView();

    q->stackMainWindows->addWidget(m_mainView);
    q->stackMainWindows->addWidget(m_needHelpView);
    q->stackMainWindows->addWidget(m_needHelpCopyView);
    q->stackMainWindows->addWidget(m_needHelpCopyOverView);
    q->stackMainWindows->addWidget(m_helpOtherView);
    q->stackMainWindows->addWidget(m_helpOtherToDesktopView);

    q->stackMainWindows->addWidget(m_helpOtherSucceedView);
    q->stackMainWindows->addWidget(m_helpOtherFailureView);

    //   q->stackMainWindows->setCurrentWidget(needHelpView);
}

QWidget *AssistanceWindowPrivate::createMainView()
{
    Q_Q(AssistanceWindow);
    auto centralLayout = new DThinVBoxLayout();

    auto mainFrame = new QWidget(q);
    mainFrame->setFixedSize(320,240);
    mainFrame->setStyleSheet("DFrame#mainFrame{border-width:1px;border-color:#767676;border-style:dotted;}");

    auto mainFrameLayout = new DThinVBoxLayout();
    auto titleLable = new DLabel();
    titleLable->setFixedSize(50,50);
    QPixmap pixmap = QPixmap::fromImage(QImage(":/new/ui/ui/1.png"));
    pixmap = pixmap.scaled(50, 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充

    titleLable->setPixmap(pixmap);
    auto helpLable = new DLabel();
    helpLable->setWordWrap(true);
    helpLable->setLineWidth(100);
    helpLable->setText(AssistanceWindow::tr("这是由deepin团队开发的远程协助应用，如果您在使用过程中遇到困难，或者有人求助于你，请点击下方的我要求助或帮助别人。"));

    auto helpBt = new DPushButton(AssistanceWindow::tr("我要求助"));
    QIcon heIco(QPixmap(":/new/ui/ui/2.png"));
    helpBt->setIcon(heIco);
    helpBt->connect(helpBt, &DPushButton::clicked, q, [=]()
    {
        q->adjustSize();
        //qDebug() << q->size();
        q->titlebar()->setTitle(AssistanceWindow::tr("我要求助"));
        q->stackMainWindows->setCurrentWidget(m_needHelpView);
        g_nTimerTip = 1;
        m_waitTimer->start(TIMERTIME2s);//&QTimer::timeout
    });

    auto helpOtherBt = new DPushButton(AssistanceWindow::tr("帮助别人"));
    helpOtherBt->setIcon(QIcon(QPixmap(":/new/ui/ui/3.png")));
    helpOtherBt->connect(helpOtherBt, &DPushButton::clicked, q, [=]()
    {
        q->adjustSize();
        //qDebug() << q->size();
        q->titlebar()->setTitle(AssistanceWindow::tr("帮助别人"));
        q->stackMainWindows->setCurrentWidget(m_helpOtherView);

    });

    mainFrameLayout->addWidget(titleLable, 0, Qt::AlignCenter );
    mainFrameLayout->addWidget(helpLable, 0, Qt::AlignCenter );

    mainFrameLayout->addWidget(createActions({helpBt, helpOtherBt}), 0, Qt::AlignCenter);

    mainFrame->setLayout(mainFrameLayout);
    centralLayout->addWidget(mainFrame, 0, Qt::AlignCenter | Qt::AlignTop);

    return  wrapLayout(centralLayout);
}

QWidget *AssistanceWindowPrivate::createNeedHelpView()
{
    Q_Q(AssistanceWindow);
    auto centralLayout = new DThinVBoxLayout();
    auto mainFrame = new QWidget(q);
    mainFrame->setFixedSize(320,240);
    mainFrame->setStyleSheet("DFrame#mainFrame{border-width:1px;border-color:#767676;border-style:dotted;}");

    auto mainFrameLayout = new DThinVBoxLayout();
    auto spin =new DSpinner();
    spin->start();
    spin->setFixedSize(50,50);

    auto inforLable = new DLabel();
    inforLable->setText(AssistanceWindow::tr("正在生成请求验证码，请稍候..."));
    inforLable->setWordWrap(true);
    inforLable->setLineWidth(100);

    auto cancelBt = new DSuggestButton(AssistanceWindow::tr("取消"));
    cancelBt->connect(cancelBt, &DSuggestButton::clicked, q, [=]()
    {
        q->adjustSize();
        //qDebug() << q->size();
        q->titlebar()->setTitle(AssistanceWindow::tr("远程协助"));
        q->stackMainWindows->setCurrentWidget(m_mainView);
        //m_waitTimer->stop();
        m_waitTimer->stop();
        g_nTimerTip = 0;
    });
    mainFrameLayout->addWidget(spin, 0, Qt::AlignCenter );
    //mainFrameLayout->addWidget(movieLable, 0, Qt::AlignCenter );
    mainFrameLayout->addWidget(inforLable, 0, Qt::AlignCenter );
    mainFrameLayout->addWidget(createActions({cancelBt}), 0, Qt::AlignCenter);

    mainFrame->setLayout(mainFrameLayout);
    centralLayout->addWidget(mainFrame, 0, Qt::AlignCenter | Qt::AlignTop);
    return  wrapLayout(centralLayout);
}

QWidget *AssistanceWindowPrivate::createNeedHelpCopyView()
{
    Q_Q(AssistanceWindow);
    auto centralLayout = new DThinVBoxLayout();
    auto mainFrame = new QWidget(q);
    mainFrame->setFixedSize(320,240);
    mainFrame->setStyleSheet("DFrame#mainFrame{border-width:1px;border-color:#767676;border-style:dotted;}");

    auto mainFrameLayout = new DThinVBoxLayout();
    auto validateCodeLable = new DLabel();
    validateCodeLable->setText("000000");
    validateCodeLable->setStyleSheet("font-size:48px;letter-spacing：10px");

    validateCodeLable->setFixedSize(160,100);

    m_needHelpCopyViewUiMap.insert("validateCodeLable",validateCodeLable);

    auto inforLable = new DLabel();
    inforLable->setWordWrap(true);
    inforLable->setText(AssistanceWindow::tr("要开始共享您的桌面，请将上面的验证码提供给协助您的人员，他们输入验证码后，您的共享会话会立即开始"));

    auto copyBt = new DSuggestButton(AssistanceWindow::tr("复制"));
    copyBt->connect(copyBt, &DSuggestButton::clicked, q, [=]()
    {
        q->adjustSize();
        //qDebug() << q->size();
        QString strNum = validateCodeLable->text();
        qint64 nValidateCode = strNum.toInt();

        strNum = QString::number(nValidateCode);
        m_clipboard->setText(strNum);
        q->titlebar()->setTitle(AssistanceWindow::tr("我要求助"));
        q->stackMainWindows->setCurrentWidget(m_needHelpCopyOverView);

    });

    mainFrameLayout->addWidget(validateCodeLable, 0, Qt::AlignCenter);
    mainFrameLayout->addWidget(inforLable, 0, Qt::AlignCenter);
    mainFrameLayout->addWidget(createActions({copyBt}), 0, Qt::AlignCenter);
    //mainFrameLayout->addWidget(copyBt, 0, Qt::AlignCenter );
    mainFrame->setLayout(mainFrameLayout);
    centralLayout->addWidget(mainFrame, 0, Qt::AlignCenter | Qt::AlignTop);
    return  wrapLayout(centralLayout);
}

QWidget *AssistanceWindowPrivate::createNeedHelpCopyOverView()
{
    Q_Q(AssistanceWindow);
    auto centralLayout = new DThinVBoxLayout();
    auto mainFrame = new QWidget(q);
    mainFrame->setFixedSize(320,240);
    mainFrame->setStyleSheet("DFrame#mainFrame{border-width:1px;border-color:#767676;border-style:dotted;}");

    auto mainFrameLayout = new DThinVBoxLayout();

    auto validateCodeLable = new DLabel();
    validateCodeLable->setText("成功复制到粘贴板");
    validateCodeLable->setStyleSheet("font-size:36px;letter-spacing：17px");

    //m_needHelpCopyViewUiMap.insert("validateCodeLable",validateCodeLable);

    auto inforLable = new DLabel();
    inforLable->setWordWrap(true);
    inforLable->setText(AssistanceWindow::tr("正在等待连接，请稍后..."));

    auto inforLable2 = new DLabel();
    inforLable2->setWordWrap(true);
    inforLable2->setText(AssistanceWindow::tr("连接成功后此界面会自动隐藏到系统栏"));

    auto backBt = new DSuggestButton(AssistanceWindow::tr("返回"));
    backBt->connect(backBt, &DSuggestButton::clicked, q, [=]()
    {
        q->adjustSize();
        q->titlebar()->setTitle(AssistanceWindow::tr("远程协助"));
        q->stackMainWindows->setCurrentWidget(m_mainView);
    });

    //mainFrameLayout->setSpacing(-40);
    mainFrameLayout->addWidget(validateCodeLable, 0, Qt::AlignCenter);
    mainFrameLayout->addWidget(inforLable, 0, Qt::AlignCenter);
    mainFrameLayout->addWidget(inforLable2, 0, Qt::AlignCenter);
    mainFrameLayout->addWidget(createActions({backBt}), 0, Qt::AlignCenter);
    //mainFrameLayout->addWidget(backBt, 0, Qt::AlignCenter );

    mainFrame->setLayout(mainFrameLayout);
    centralLayout->addWidget(mainFrame, 0, Qt::AlignCenter | Qt::AlignTop);
    return  wrapLayout(centralLayout);
}

QWidget *AssistanceWindowPrivate::createHelpOtherView()
{
    Q_Q(AssistanceWindow);
    auto centralLayout = new DThinVBoxLayout();

    auto mainFrame = new QWidget(q);
    mainFrame->setFixedSize(320,240);

    mainFrame->setStyleSheet("DFrame#mainFrame{border-width:1px;border-color:#767676;border-style:dotted;}");

    auto mainFrameLayout = new DThinVBoxLayout();
    auto validateEdit = new DLineEdit();
    validateEdit->setFixedSize(250,80);
    validateEdit->setStyleSheet("font-size:48px;letter-spacing：17px");
    //validateEdit;

    auto inforLabel = new DLabel(AssistanceWindow::tr("请在上方输入你想要帮助的人提供给你的验证码，完成后点击连接开始协助对方"));
    inforLabel->setWordWrap(true);

    //validateEdit->setMaximumSize(6,1);

        auto cancelBt = new DSuggestButton(AssistanceWindow::tr("取消"));
    cancelBt->connect(cancelBt, &DSuggestButton::clicked, q, [=]()
    {
        QString strCaBt = cancelBt->text();
        if(strCaBt ==QString::fromLocal8Bit("取消"))
        {
            q->adjustSize();
            //qDebug() << q->size();
            q->titlebar()->setTitle(AssistanceWindow::tr("远程协助"));
            q->stackMainWindows->setCurrentWidget(m_mainView);
        }
        else if(strCaBt ==QString::fromLocal8Bit("连接"))
        {
            q->adjustSize();
            QString strNum = validateEdit->text();
            qint64 nValidateCode = strNum.toInt();
            if(q->m_nValidateCode == nValidateCode)
            {
                //qDebug() << q->size();
                DDialog popDlg;
                popDlg.setIcon(QIcon(":/new/ui/ui/uos-win.svg"));
                DSuggestButton popDlgOKbt(AssistanceWindow::tr("确定"));
                DSuggestButton popDlgcancelbt(AssistanceWindow::tr("返回"));


                popDlg.insertButton(0, &popDlgOKbt, true);
                popDlg.insertButton(0, &popDlgcancelbt, true);
                popDlg.setTitle(AssistanceWindow::tr("确定进行远程连接吗？"));
                popDlg.setFixedSize(200,160);

                popDlgOKbt.connect(&popDlgOKbt, &DSuggestButton::clicked, q, [=]()
                {
                    q->titlebar()->setTitle(AssistanceWindow::tr("帮助别人"));
                    q->stackMainWindows->setCurrentWidget(m_helpOtherToDesktopView);
                    g_nTimerTip = 2;
                    m_waitTimer->start(TIMERTIME5s);//&QTimer::timeout
                });
                popDlg.exec();
            }
            else
            {
                validateEdit->showAlertMessage(AssistanceWindow::tr("请输入正确的验证码"),1000);
            }
        }
    });

    validateEdit->connect(validateEdit, &DLineEdit::textChanged,
                    validateEdit, [ = ](const QString & keyText)
    {
        int pos = 0;
        QString str = validateEdit->text();
        if(str.length()>6)
        {
            str = str.mid(0,6);
            validateEdit->setText(str);
        }

        if(str.length() == 6)
        {
            QRegExpValidator keyValidator(QRegExp("[0-9]{1,6}"));
            bool keyValid = !((keyValidator.validate(str, pos) != QValidator::Acceptable));
            if(!keyValid)
            {
                validateEdit->showAlertMessage(AssistanceWindow::tr("请输入0-9之间的6位数字"),1000);
            }
            else
            {
                cancelBt->setText(AssistanceWindow::tr("连接"));
            }
        }
        else
        {
            cancelBt->setText(AssistanceWindow::tr("取消"));
        }
        qDebug()<<" createHelpOtherView validateEdit:"<<keyText;
    });
    mainFrameLayout->addSpacing(20);
    mainFrameLayout->addWidget(validateEdit, 0, Qt::AlignCenter );
    mainFrameLayout->addWidget(inforLabel, 0, Qt::AlignCenter );
    mainFrameLayout->addWidget(createActions({cancelBt}), 0, Qt::AlignCenter);
    //mainFrameLayout->addWidget(cancelBt, 0, Qt::AlignCenter );

    mainFrame->setLayout(mainFrameLayout);

    centralLayout->addWidget(mainFrame, 0, Qt::AlignCenter | Qt::AlignTop);

    return  wrapLayout(centralLayout);
}

QWidget *AssistanceWindowPrivate::createHelpOtherToDesktopView()
{
    Q_Q(AssistanceWindow);
    auto centralLayout = new DThinVBoxLayout();

    auto mainFrame = new QWidget(q);
    mainFrame->setFixedSize(320,240);

    mainFrame->setStyleSheet("DFrame#mainFrame{border-width:1px;border-color:#767676;border-style:dotted;}");


    auto mainFrameLayout = new DThinVBoxLayout();
    auto spin =new DSpinner();
    spin->start();
    spin->setFixedSize(50,50);

    auto inforLabel = new DLabel(AssistanceWindow::tr("正在建立连接，请稍侯..."));
    inforLabel->setWordWrap(true);

    auto cancelBt = new DSuggestButton(AssistanceWindow::tr("取消"));
    cancelBt->connect(cancelBt, &DSuggestButton::clicked, q, [=]()
    {
        q->adjustSize();
        //qDebug() << q->size();
        q->titlebar()->setTitle(AssistanceWindow::tr("远程协助"));
        q->stackMainWindows->setCurrentWidget(m_mainView);
        m_waitTimer->stop();
        g_nTimerTip = 0;
    });

    mainFrameLayout->addWidget(spin, 0, Qt::AlignCenter );
    mainFrameLayout->addWidget(inforLabel, 0, Qt::AlignCenter );
    mainFrameLayout->addWidget(createActions({cancelBt}), 0, Qt::AlignCenter);
    //mainFrameLayout->addWidget(cancelBt, 0, Qt::AlignCenter );

    mainFrame->setLayout(mainFrameLayout);

    centralLayout->addWidget(mainFrame, 0, Qt::AlignCenter | Qt::AlignTop);

    return  wrapLayout(centralLayout);
}

QWidget *AssistanceWindowPrivate::createHelpOtherSucceedView()
{
    Q_Q(AssistanceWindow);
    auto centralLayout = new DThinVBoxLayout();

    auto mainFrame = new QWidget(q);
    mainFrame->setFixedSize(320, 240);

    mainFrame->setStyleSheet("DFrame#mainFrame{border-width:1px;border-color:#767676;border-style:dotted;}");

    auto mainFrameLayout = new DThinVBoxLayout();

    auto inforLabel = new DLabel(AssistanceWindow::tr("您正在进行远程协助"));
    inforLabel->setWordWrap(true);
    inforLabel->setStyleSheet("font-size:28px;letter-spacing：17px");

    auto inforLabel2 = new DLabel(AssistanceWindow::tr("当前已经有一个连接会话，如果您想建立新的连接，请先断开"));
    inforLabel2->setWordWrap(true);

    auto disconnectBt = new DSuggestButton(AssistanceWindow::tr("断开"));
    disconnectBt->connect(disconnectBt, &DSuggestButton::clicked, q, [=]()
    {
        q->adjustSize();
        //qDebug() << q->size();
        q->titlebar()->setTitle(AssistanceWindow::tr("远程协助"));
        q->stackMainWindows->setCurrentWidget(m_mainView);
        g_nTimerTip = 0;
        m_waitTimer->stop();//&QTimer::timeout
    });

    mainFrameLayout->addWidget(inforLabel, 0, Qt::AlignCenter );
    mainFrameLayout->addWidget(inforLabel2, 0, Qt::AlignCenter );
    mainFrameLayout->addWidget(createActions({disconnectBt}), 0, Qt::AlignCenter);
    //mainFrameLayout->addWidget(disconnectBt, 0, Qt::AlignCenter );

    mainFrame->setLayout(mainFrameLayout);

    centralLayout->addWidget(mainFrame, 0, Qt::AlignCenter | Qt::AlignTop);

    return  wrapLayout(centralLayout);
}

QWidget *AssistanceWindowPrivate::createHelpOtherFailureView()
{
    Q_Q(AssistanceWindow);
    auto centralLayout = new DThinVBoxLayout();

    auto mainFrame = new QWidget(q);
    mainFrame->setFixedSize(320,240);

    mainFrame->setStyleSheet("DFrame#mainFrame{border-width:1px;border-color:#767676;border-style:dotted;}");


    auto mainFrameLayout = new DThinVBoxLayout();


    auto inforLabel = new DLabel(AssistanceWindow::tr("连接失败"));
    inforLabel->setWordWrap(true);
    auto inforLabel2 = new DLabel(AssistanceWindow::tr("网络连接状态不是很稳定，点击重试按钮再次尝试连接"));
    inforLabel2->setWordWrap(true);

    auto retryBt = new DSuggestButton(AssistanceWindow::tr("重试"));
    retryBt->connect(retryBt, &DSuggestButton::clicked, q, [=]()
    {
        q->adjustSize();
        //qDebug() << q->size();
        q->titlebar()->setTitle(AssistanceWindow::tr("帮助别人"));
        q->stackMainWindows->setCurrentWidget(m_helpOtherToDesktopView);
        g_nTimerTip = 2;
        m_waitTimer->start(TIMERTIME2s);//&QTimer::timeout
    });

    mainFrameLayout->addWidget(inforLabel, 0, Qt::AlignCenter );
    mainFrameLayout->addWidget(inforLabel2, 0, Qt::AlignCenter );
    mainFrameLayout->addWidget(createActions({retryBt}), 0, Qt::AlignCenter);
    //mainFrameLayout->addWidget(retryBt, 0, Qt::AlignCenter );

    mainFrame->setLayout(mainFrameLayout);

    centralLayout->addWidget(mainFrame, 0, Qt::AlignCenter | Qt::AlignTop);

    return  wrapLayout(centralLayout);
}

QWidget *AssistanceWindowPrivate::wrapLayout(QLayout *layout)
{
    auto wrapWidget = new QWidget;

    layout->setAlignment(Qt::AlignVCenter);
    layout->setAlignment(Qt::AlignHCenter);

    wrapWidget->setLayout(layout);
    return wrapWidget;
}
//默认按钮垂直布局
QWidget *AssistanceWindowPrivate::createActions(QList<QWidget *> buttons,bool isVertical)
{
    auto actionWidget = new QWidget;
//    actionWidget->setFixedHeight(90);
    QBoxLayout *actionLayout = nullptr;
    if(isVertical)
    {
        actionLayout = new DThinVBoxLayout(actionWidget);
    }
    else
    {
        actionLayout = new DThinHBoxLayout(actionWidget);
    }

    //actionLayout->addSpacing(40);
    for (auto button : buttons)
    {
        button->setFixedSize(140, 36);
        actionLayout->addWidget(button);
        actionLayout->setAlignment(button, Qt::AlignHCenter | Qt::AlignTop);
        actionLayout->setSpacing(10);
    }
    //actionLayout->addStretch();

    actionWidget->setTabOrder(buttons.last(), buttons.first());
    return actionWidget;
}
//---------------------------------------------------------------------------------------------
AssistanceWindow::AssistanceWindow(QWidget *parent) :
    DMainWindow(parent), dd_ptr(new AssistanceWindowPrivate(this))
{
    Q_D(AssistanceWindow);
    //setWindowTitle(tr("远程协助"));
    //DThemeManager::registerWidget(this);
    setProperty("_d_useDxcb", true);

    DThemeManager::registerWidget(this);
    stackMainWindows = new QStackedWidget(this);
    setCentralWidget(stackMainWindows);
    //生成二维码图片
    /*QRCodeLabel *qrCode = new QRCodeLabel;
    qrCode->resize(130,130);
    qrCode->generateQRCode(QString("www.baidu.com"));*/

    if (titlebar())
    {
           //initTitlebar();
           QPixmap iconPixmap = DHiDPIHelper::loadNxPixmap(":/new/ui/ui/uos-win.svg");
           QIcon fallback(iconPixmap);
           titlebar()->setIcon(QIcon(fallback));
           titlebar()->setSeparatorVisible(true);
           titlebar()->setFixedHeight(40);
           titlebar()->setTitle(AssistanceWindow::tr("远程协助"));
     }
    //setAttribute(Qt::WA_DeleteOnClose);
    qApp->installEventFilter(this);
    setFixedSize(360, 320);
    d->initUI();
}

AssistanceWindow::~AssistanceWindow()
{

}

void AssistanceWindow::closeEvent(QCloseEvent *event)
{
    //TODO: 在退出窗口之前，实现希望做的操作
    //hide();
}

void AssistanceWindow::updateProgress()
{
    Q_D(AssistanceWindow);
    if(g_nTimerTip == 1)
    {
        qDebug() <<"updateProgress";
        DLabel * la = dynamic_cast<DLabel*>(d->m_needHelpCopyViewUiMap["validateCodeLable"]);
        qint64 nValidateCode = createValidateCode();
        QString strValidate = QString::number(nValidateCode);
        la->setText(strValidate);
        stackMainWindows->setCurrentWidget(d->m_needHelpCopyView);
        g_nTimerTip = 0;
        d->m_waitTimer->stop();
    }
    else if(g_nTimerTip == 2)
    {
        bool isOnline = d->m_Mgr->isOnline();
        if(isOnline)
        {
            adjustSize();
            //qDebug() << q->size();
            titlebar()->setTitle(AssistanceWindow::tr("我要求助"));
            stackMainWindows->setCurrentWidget(d->m_helpOtherSucceedView);
            d->m_waitTimer->stop();
            g_nTimerTip = 3;
            d->m_waitTimer->start(TIMERTIME2s);
        }
        else
        {
            adjustSize();
            //qDebug() << q->size();
            titlebar()->setTitle(AssistanceWindow::tr("帮助别人"));
            stackMainWindows->setCurrentWidget(d->m_helpOtherFailureView);
            g_nTimerTip = 0;
            d->m_waitTimer->stop();
        }
    }
    else if(g_nTimerTip == 3)
    {
        this->close();//hide()
        g_nTimerTip = 0;
        d->m_waitTimer->stop();
    }

}

qint64 AssistanceWindow::createValidateCode()
{
    m_nValidateCode = rand()%(1000000);
    return m_nValidateCode;
}

qint64 AssistanceWindow::ValidateCode()
{
    return m_nValidateCode;
}
