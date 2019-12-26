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
//#include <QDBusInterface>
//#include <QDBusReply>

extern "C"{
//    #include "qrencode/qrencode.h"
}
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

//获取上下文菜单paste点击
/*KeyDLineEdit::KeyDLineEdit(QWidget *parent):QLineEdit(parent)
{
    const int editWidth = 60;
    const int editHeight = 36;
    setAlignment(Qt::AlignCenter);
    setMaxLength(4);
    //setValidator(v);
    setFixedSize(editWidth, editHeight);
    setStyleSheet("background:rgba(0,0,0,0.08); border-radius:8px;");
}

void KeyDLineEdit::contextMenuEvent(QContextMenuEvent *event)
{
    if (QMenu *menu = createStandardContextMenu()) {
        menu->setAttribute(Qt::WA_DeleteOnClose);
        menu->popup(event->globalPos());
        menu->connect(menu, &QMenu::triggered,
        this, [ = ](QAction *action)
        {
            //上下文菜单 点击粘贴
            if(action->text().contains("&P"))
            {
                qDebug()<<"ContextMenu click paste....";
                Q_EMIT sigPasteClipboard();
            }
        });
    }
}*/

//void QRCodeLabel::generateQRCode(QString url)
//{
//    //生成二维码
//    QRcode *qrcode; //二维码数据,二维码图像是25x25
//    //QR_ECLEVEL_Q 容错等级
//    qrcode = QRcode_encodeString(url.toStdString().c_str(), 2, QR_ECLEVEL_Q, QR_MODE_8, 1);
//    qint32 qrcodeWidth = qrcode->width > 0 ? qrcode->width : 1;
//    qDebug()<<"qrcodeWidth"<<qrcodeWidth;
//    //二维码图片的缩放比例
//    double scaleX = static_cast<double>(width())  / static_cast<double>(qrcodeWidth);
//    double scaleY = static_cast<double>(height()) / static_cast<double>(qrcodeWidth);
//    QPixmap pixmap=QPixmap(width(),height());
//    QPainter painter(&pixmap);
//    QColor background(Qt::white);
//    painter.setBrush(background);
//    painter.setPen(Qt::NoPen);
//    painter.drawRect(0, 0, width() + 2, height() + 2);
//    QColor foreground(Qt::black);
//    painter.setBrush(foreground);
//    for( qint32 y = 0; y < qrcodeWidth; y ++)
//    {
//        for(qint32 x = 0; x < qrcodeWidth; x++)
//        {
//            unsigned char data = qrcode->data[y * qrcodeWidth + x];
//            if(data & 0x01)
//            {
//                QRectF r(x * scaleX, y * scaleY, scaleX, scaleY);
//                painter.drawRects(&r, 1);
//            }
//        }
//    }
//    this->setPixmap(pixmap);
//}

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
//    if(event->type() == QEvent::MouseButtonPress){

//        QDBusInterface manual("com.deepin.Manual.Open",
//                                 "/com/deepin/Manual/Open",
//                                 "com.deepin.Manual.Open",
//                                 QDBusConnection::sessionBus());
//        QDBusReply<void> reply = manual.call("Open","");
//            if (reply.isValid())  {
//                qDebug() << "call com.deepin.Manual.Open success";
//                return;
//            }
//            qDebug() << "call com.deepin.Manual.Open failed" << reply.error();
//    }
}
