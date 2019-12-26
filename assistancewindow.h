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
