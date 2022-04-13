#include <QApplication>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTabWidget>
#include <QPushButton>
#include "browsertabwidget.h"


BrowserTabWidget* CreateNewBrowserWindow(){
    BrowserTabWidget* w = new BrowserTabWidget;
    auto* vboxLayout = new QVBoxLayout(w);
    auto* urlLineEdit = new QLineEdit;
    auto* webView = new QWebEngineView;
    vboxLayout->addWidget(urlLineEdit);
    vboxLayout->addWidget(webView);
    QObject::connect(urlLineEdit, &QLineEdit::returnPressed,[urlLineEdit, webView]()
    {
        QString urlText = urlLineEdit->text();
        if(!urlText.contains("http://"))
        {
            urlText = "http://" + urlText;
            urlLineEdit->setText(urlText);
        }
        webView->load(urlText);
    });

    QObject::connect(webView, &QWebEngineView::loadFinished, [webView, w](bool loadFinished)
    {
        if(loadFinished)
        {
            emit w->TabLoadingFinished(webView->title());
        }
    });
    return w;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTabWidget tabs;
    QPushButton newTabButton("+");
    tabs.setTabsClosable(true);
    tabs.setCornerWidget(&newTabButton,Qt::BottomLeftCorner);

    QObject::connect(&newTabButton, &QPushButton::clicked,[&tabs]()
    {
        auto* newTab = CreateNewBrowserWindow();
        QObject::connect(newTab, &BrowserTabWidget::TabLoadingFinished, [&tabs](QString title)
        {
            tabs.setTabText(tabs.currentIndex(), title);
            tabs.setWindowTitle(title);
        });
        tabs.addTab(newTab, "tab " + QString::number(tabs.count() + 1));
    });
    QObject::connect(&tabs, &QTabWidget::tabCloseRequested,[&tabs](int index)
    {
        if(tabs.count() > 1)
        {
            auto* widgetToDelete = tabs.widget(index);
            delete widgetToDelete;
        }
    });
    QObject::connect(&tabs,&QTabWidget::currentChanged,[&tabs](int index)
    {


        tabs.setWindowTitle(tabs.tabText(index));
    });
    newTabButton.click();
    tabs.show();
    return a.exec();
}

