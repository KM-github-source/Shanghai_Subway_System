#include "addnew.h"
#include "ui_addnew.h"
#include "graph.h"

AddNew::AddNew(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddNew)
{
    ui->setupUi(this);
    this->InitUI();
}

AddNew::~AddNew()
{
    delete ui;
}

void AddNew::InitUI() //初始化添加界面
{
    this->setWindowTitle("添加"); //设置窗口标题
    //设置背景
    QPixmap background(BackgroundPath2);
    QPalette palette;
    palette.setBrush(QPalette::Background,background);
    this->setPalette(palette);
    //this->ui->tabWidget->setPalette(palette);

    this->line_color = QColor(); //初始化颜色
}

void AddNew::UpdateLinesList(QVector<Line> lines) //更新添加新站点中的可选线路
{
    for(int i=0; i<ui->listWidget_LinesIn->count(); i++) //动态空间释放
    {
        QListWidgetItem* item = ui->listWidget_LinesIn->takeItem(i); //取列表项
        delete item; //释放列表项
    }
    ui->listWidget_LinesIn->clear();
    ui->textBrowser_addStation->clear();

    for(auto &l: lines) //添加线路
    {
        QListWidgetItem *item=new QListWidgetItem(l.name);
        item->setFlags(item->flags()|Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        ui->listWidget_LinesIn->addItem(item); //添加列表项
    }
}

void AddNew::UpdateComboBox(QVector<Line> lines, QVector<Station> stations) //更新下拉框选项
{
    ui->comboBox_linesIn->clear();
    ui->comboBox_S1->clear();
    ui->comboBox_S2->clear(); //清空下拉框选项

    for(auto &l: lines) //线路下拉框
    {
        ui->comboBox_linesIn->addItem(l.name);
    }

    for(auto &s: stations) //起始站和终点站下拉框
    {
        ui->comboBox_S1->addItem(s.name);
        ui->comboBox_S2->addItem(s.name);
    }
}

void AddNew::on_lineEdit_inputLineName_textChanged(const QString &arg1) //输入线路名称
{
    this->line_name = arg1; //保存输入线路名称
    ui->textBrowser_addLine->setText("线路：" + line_name + "\n" + "颜色：" + QString::number(line_color.rgb()-0xff000000,16));
    //QString::number(line_color.rgb()-0xff000000,16)
}

void AddNew::on_pushButton_selectColor_clicked() //选择线路颜色
{
    QColor color = QColorDialog::getColor(QColor(255, 0, 0)); //颜色框，默认红色
    this->line_color = color; //color接收选择的颜色
    ui->textBrowser_addLine->setText("线路：" + line_name + "\n" + "颜色：" + QString::number(line_color.rgb()-0xff000000,16));
}

void AddNew::on_lineEdit_inputSSname_textChanged(const QString &arg1) //输入起始站
{
    this->Sstation_name = arg1; //保存起始站名称
    this->SS_longitude = ui->doubleSpinBox_SSlongitude->text().toDouble();
    this->SS_latitude = ui->doubleSpinBox_SSlatitude->text().toDouble();
    ui->textBrowser_addLine->setText("线路：" + line_name + "\n" +
                                     "颜色：" + QString::number(line_color.rgb()-0xff000000,16) + "\n" +
                                     "起始站名：" + Sstation_name + "\n" +
                                     "起始站经度：" + QString::number(SS_longitude, 'f', 6) + "\n" +
                                     "起始站纬度：" + QString::number(SS_latitude, 'f', 6));
}

void AddNew::on_doubleSpinBox_SSlongitude_valueChanged(double arg1) //输入起始站经度
{
    this->SS_longitude = arg1;
    this->SS_latitude = ui->doubleSpinBox_SSlatitude->text().toDouble();
    ui->textBrowser_addLine->setText("线路：" + line_name + "\n" +
                                     "颜色：" + QString::number(line_color.rgb()-0xff000000,16) + "\n" +
                                     "起始站名：" + Sstation_name + "\n" +
                                     "起始站经度：" + QString::number(SS_longitude, 'f', 6) + "\n" +
                                     "起始站纬度：" + QString::number(SS_latitude, 'f', 6));
}

void AddNew::on_doubleSpinBox_SSlatitude_valueChanged(double arg1) //输入起始站纬度
{
    this->SS_latitude = arg1;
    this->SS_longitude = ui->doubleSpinBox_SSlongitude->text().toDouble();
    ui->textBrowser_addLine->setText("线路：" + line_name + "\n" +
                                     "颜色：" + QString::number(line_color.rgb()-0xff000000,16) + "\n" +
                                     "起始站名：" + Sstation_name + "\n" +
                                     "起始站经度：" + QString::number(SS_longitude, 'f', 6) + "\n" +
                                     "起始站纬度：" + QString::number(SS_latitude, 'f', 6));
}

void AddNew::on_lineEdit_inputESname_textChanged(const QString &arg1) //输入终点站
{
    this->Estation_name = arg1;
    this->ES_longitude = ui->doubleSpinBox_ESlongitude->text().toDouble();
    this->ES_latitude = ui->doubleSpinBox_ESlatitude->text().toDouble();
    ui->textBrowser_addLine->setText("线路：" + line_name + "\n" +
                                     "颜色：" + QString::number(line_color.rgb()-0xff000000,16) + "\n" +
                                     "起始站名：" + Sstation_name + "\n" +
                                     "起始站经度：" + QString::number(SS_longitude, 'f', 6) + "\n" +
                                     "起始站纬度：" + QString::number(SS_latitude, 'f', 6) + "\n" +
                                     "终点站名：" + Estation_name + "\n" +
                                     "终点站经度：" + QString::number(ES_longitude, 'f', 6) + "\n" +
                                     "终点站纬度：" + QString::number(ES_latitude, 'f', 6));
}

void AddNew::on_doubleSpinBox_ESlongitude_valueChanged(double arg1) //输入终点站经度
{
    this->ES_longitude = arg1;
    this->ES_latitude = ui->doubleSpinBox_ESlatitude->text().toDouble();
    ui->textBrowser_addLine->setText("线路：" + line_name + "\n" +
                                     "颜色：" + QString::number(line_color.rgb()-0xff000000,16) + "\n" +
                                     "起始站名：" + Sstation_name + "\n" +
                                     "起始站经度：" + QString::number(SS_longitude, 'f', 6) + "\n" +
                                     "起始站纬度：" + QString::number(SS_latitude, 'f', 6) + "\n" +
                                     "终点站名：" + Estation_name + "\n" +
                                     "终点站经度：" + QString::number(ES_longitude, 'f', 6) + "\n" +
                                     "终点站纬度：" + QString::number(ES_latitude, 'f', 6));
}

void AddNew::on_doubleSpinBox_ESlatitude_valueChanged(double arg1) //输入终点站纬度
{
    this->ES_latitude = arg1;
    this->ES_longitude = ui->doubleSpinBox_ESlongitude->text().toDouble();
    ui->textBrowser_addLine->setText("线路：" + line_name + "\n" +
                                     "颜色：" + QString::number(line_color.rgb()-0xff000000,16) + "\n" +
                                     "起始站名：" + Sstation_name + "\n" +
                                     "起始站经度：" + QString::number(SS_longitude, 'f', 6) + "\n" +
                                     "起始站纬度：" + QString::number(SS_latitude, 'f', 6) + "\n" +
                                     "终点站名：" + Estation_name + "\n" +
                                     "终点站经度：" + QString::number(ES_longitude, 'f', 6) + "\n" +
                                     "终点站纬度：" + QString::number(ES_latitude, 'f', 6));
}

void AddNew::on_lineEdit_Sname_textChanged(const QString &arg1) //输入新建站点名称
{
    this->Nstation_name = arg1;
    this->NS_longitude = ui->doubleSpinBox_Slongitude->text().toDouble();
    this->NS_latitude = ui->doubleSpinBox_Slatitude->text().toDouble();
    ui->textBrowser_addStation->setText("新建站点名称：" + Nstation_name + "\n" +
                                        "新建站点经度：" + QString::number(NS_longitude, 'f', 6) + "\n" +
                                        "新建站点纬度：" + QString::number(NS_latitude, 'f', 6));
}

void AddNew::on_doubleSpinBox_Slongitude_valueChanged(double arg1) //输入新建站点经度
{
    this->NS_longitude = arg1;
    this->NS_latitude = ui->doubleSpinBox_Slatitude->text().toDouble();
    ui->textBrowser_addStation->setText("新建站点名称：" + Nstation_name + "\n" +
                                        "新建站点经度：" + QString::number(NS_longitude, 'f', 6) + "\n" +
                                        "新建站点纬度：" + QString::number(NS_latitude, 'f', 6));
}

void AddNew::on_doubleSpinBox_Slatitude_valueChanged(double arg1) //输入新建站点纬度
{
    this->NS_latitude = arg1;
    this->NS_longitude = ui->doubleSpinBox_Slongitude->text().toDouble();
    ui->textBrowser_addStation->setText("新建站点名称：" + Nstation_name + "\n" +
                                        "新建站点经度：" + QString::number(NS_longitude, 'f', 6) + "\n" +
                                        "新建站点纬度：" + QString::number(NS_latitude, 'f', 6));
}

void AddNew::on_listWidget_LinesIn_itemClicked(QListWidgetItem *item) //所属线路列表有选项被选中或取消选中，更新显示信息
{
    Q_UNUSED(item);
    QString str;
    this->lines_selected.clear();
    str += ("新建站点名称：" + Nstation_name + "\n" +
            "新建站点经度：" + QString::number(NS_longitude, 'f', 6) + "\n" +
            "新建站点纬度：" + QString::number(NS_latitude, 'f', 6) + "\n" +
            "所属线路：" + "\n");

    for (int i = 0; i < ui->listWidget_LinesIn->count(); i++)
    {
        QListWidgetItem* item = ui->listWidget_LinesIn->item(i);
        if(item->checkState() == Qt::Checked)
        {
            this->lines_selected.push_back(item->text());
            str += item->text();
            str += "\n";
        }
    }
    ui->textBrowser_addStation->setText(str);
}
