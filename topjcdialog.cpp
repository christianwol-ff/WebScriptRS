#include "topjcdialog.h"
#include "ui_topjcdialog.h"
#include "webtabcontents.h"

#include <QtGui>
/** Constructor
LinksDialog::LinksDialog(RsPeers *peers, RsFiles *files, QWidget *parent)
: MainPage(parent), mPeers(peers), mFiles(files)
{

}**/

WebScriptDialog::WebScriptDialog(QWidget *parent) :
    MainPage(parent),
    ui(new Ui::WebScriptDialog)
{
    ui->setupUi(this);
    bridge = new WebBridgeRS();
    connect(ui->newTabBtn, SIGNAL(clicked()), this, SLOT(addTab()));
    connect(ui->closeTabBtn, SIGNAL(clicked()), this, SLOT(removeTab()));
    connect( bridge, SIGNAL(newTabUrl(QString)),    this,   SLOT(onNewTabUrl(QString)) );
    /*webview = new WebViewRS();
    this->ui->verticalLayout->addWidget(webview,1);
    webview->show();
    QString loadfirst = "";//QDir::homePath();

    loadfirst.append("html/index.html");
    webview->setUrl(QUrl(loadfirst));


    //jslog = new QTextBrowser();
    //this->ui->verticalLayout->addWidget(jslog,1);
    //jslog->show();
    QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);

    //connect(ui->testButton, SIGNAL(clicked()), this->webview, SLOT(reload()));
    connect(ui->testButton, SIGNAL(clicked()), this, SLOT(changeLocation()));


    connect(ui->lineEdit, SIGNAL(returnPressed()), SLOT(changeLocation()));
    connect(webview, SIGNAL(loadFinished(bool)), SLOT(adjustLocation()));*/

}

WebScriptDialog::~WebScriptDialog()
{
    delete ui;
}

void WebScriptDialog::changeLocation()
{
    //QUrl url = QUrl(ui->lineEdit->text());
    //webview->load(url);
    //webview->setFocus();
}
void WebScriptDialog::adjustLocation()
{
    //ui->lineEdit->setText(webview->url().toString());
}

void WebScriptDialog::setP3service(p3JsonRS *p3servicein)
{
    //webview->setP3service(p3servicein);
    p3service = p3servicein;
    p3service->bridge = bridge;
    bridge->p3service = p3service;
    std::cerr << "bridge on set: " << p3service->bridge << std::endl;
    addTab();
}
void WebScriptDialog::addTab(){
    onNewTabUrl("html/index.html");
}

void WebScriptDialog::onNewTabUrl(QString url)
{
    WebTabContents * wtc = new WebTabContents(this);
    wtc->getWebView()->setUrl(QUrl(url));
    wtc->setP3service(p3service);
    ui->webTabs->addTab(wtc,QString("hi"));
    connect( wtc->getWebView(), SIGNAL(titleChanged(QString)),    this,   SLOT(onTitleChanged(QString)) );
    connect( wtc->getWebView(), SIGNAL(requestTabForHash(QString)),    this,   SLOT(onNewRsTab(QString)) );
}
void WebScriptDialog::onNewRsTab(QString hash){
    onNewTabUrl("html/loading.html?hash="+hash);
    WebTabContents * wtc = qobject_cast<WebTabContents *>(ui->webTabs->widget(ui->webTabs->count()-1));
    wtc->getWebView()->setHashQue(hash);
    //ui->webTabs->currentIndex();
    //ui->webTabs->currentWidget();
}
void WebScriptDialog::onTitleChanged(QString title){
    WebTabContents * wtc = qobject_cast<WebTabContents *>(QObject::sender()->parent());
    int index = ui->webTabs->indexOf(wtc);
    ui->webTabs->setTabText(index, title);
}

void WebScriptDialog::removeTab()
{
    WebTabContents * wtc = qobject_cast<WebTabContents *>(ui->webTabs->currentWidget());
    ui->webTabs->removeTab(ui->webTabs->currentIndex());
    delete(wtc);
}
