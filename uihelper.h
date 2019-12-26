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


