#include "helpfile.h"
#include "ui_helpfile.h"

HelpFile::HelpFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HelpFile)
{
    ui->setupUi(this);
    this->InitUI();
}

HelpFile::~HelpFile()
{
    delete ui;
}

void HelpFile::InitUI()
{
    this->setWindowTitle("帮助文档"); //设置窗口标题
    //设置背景
    QPixmap background(BackgroundPath2);
    QPalette palette;
    palette.setBrush(QPalette::Background,background);
    this->setPalette(palette);
    QFile file_show(FilePath);
    file_show.open(QIODevice::ReadOnly);
    this->ui->textBrowser->setHtml(file_show.readAll());
    file_show.close();
}
