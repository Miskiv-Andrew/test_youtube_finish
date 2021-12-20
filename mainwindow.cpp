#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_channell = new connectYt(this);

    m_timer = new QTimer(this);

    setWindowIcon(QIcon(":/images/youtube.png"));

    connect(ui->queryWin, &QLineEdit::textChanged, this, &MainWindow::textProc);
    connect(ui->queryWin, &QLineEdit::textChanged, this, &MainWindow::setLinkName);
    connect(this, &MainWindow::sendRequest, m_channell, &connectYt::fetchData);    
    connect(m_channell, &connectYt::sendData, this, &MainWindow::showResult);    
    connect(m_timer, &QTimer::timeout, this, &MainWindow::timerReq);

    setLinkName();

    startFunc();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getLinkName() const
{
    return linkName;
}

void MainWindow::setLinkName(QString _linkName)
{
    linkName = _linkName;
}

unsigned long MainWindow::getCountReq() const
{
    return countReq;
}

void MainWindow::setCountReq(unsigned long _countReq)
{
    countReq = _countReq;
}

void MainWindow::showResult(bool flagReq, unsigned long num)
{
    if(flagReq)
        successConn(num);

    else
        errorConn(num);
}

void MainWindow::startFunc()
{
    ui->label_error->setText("Insert YouTube video link");
    ui->label_count->setText("0");

    QImage imgLikePng(":/images/question.png");
    ui->label_pic->setPixmap(QPixmap::fromImage(imgLikePng));

    setCountReq();
    ui->numReq->setText(QString::number(getCountReq()));

     m_timer->stop();
}

void MainWindow::successConn(unsigned long num)
{
    ui->label_error->setText("All right. Communication is correct");
    ui->label_count->setText(QString::number(num));

    QImage imgLikePng(":/images/thumb-up.png");
    ui->label_pic->setPixmap(QPixmap::fromImage(imgLikePng));

    m_timer->start(INTERVAL);
}

void MainWindow::errorConn(unsigned long num)
{
    ui->label_error->setText("Ooops... something went wrong");
    ui->label_count->setText(QString::number(num));

    QImage imgLikePng(":/images/close.png");
    ui->label_pic->setPixmap(QPixmap::fromImage(imgLikePng));
}

void MainWindow::timerReq()
{
    m_timer->stop();

    unsigned long num = getCountReq() + 1;
    setCountReq(num);
    ui->numReq->setText(QString::number(num));

    QString tmp_str(strProc(getLinkName()));
    emit sendRequest(tmp_str);
}

void MainWindow::textProc(const QString &str)
{
    int size = str.length();

    if(!size)  {       
        startFunc();
        return;
    }

    else if(size <= SIZE_PREAM)
        return;

    QString tmp_str(strProc(str));

    emit sendRequest(tmp_str);
}

QString MainWindow::strProc(const QString &str)
{
    QString tmp_str;

    int posit = str.indexOf('&'),
        size  = str.size();

    if(posit == -1)
        tmp_str = str.trimmed().mid(SIZE_PREAM, size - SIZE_PREAM);

    else
        tmp_str = str.trimmed().mid(SIZE_PREAM, posit - SIZE_PREAM);

    return tmp_str;
}


void MainWindow::on_pushButton_clear_clicked()
{
    m_timer->stop();
    ui->queryWin->clear();

}

