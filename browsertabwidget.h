#ifndef BROWSERTABWIDGET_H
#define BROWSERTABWIDGET_H
#include <QWidget>

class BrowserTabWidget : public QWidget
{
    Q_OBJECT

    signals:

    void TabLoadingFinished(QString tabTitle);
};
#endif // BROWSERTABWIDGET_H
