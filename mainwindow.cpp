#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_addnew.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->InitUI();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
    delete myView;
    delete addFunc;
    delete graph;
}

void MainWindow::InitUI() //初始化主屏幕
{
    //设置背景
    QPixmap background(BackgroundPath);
    QPalette palette;
    palette.setBrush(QPalette::Background,background);
    this->setPalette(palette);

    this->myView = new View_zoom(ui->graphicsView);
    myView->set_modifiers(Qt::NoModifier);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing); //设置反走样

    this->scene = new QGraphicsScene;
    scene->setSceneRect(0,0,SCENE_WIDTH,SCENE_HEIGHT);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag); //设置场景

    this->graph = new Graph(); //初始化绘图管理对象
    this->addFunc = new AddNew(); //初始化增加功能对象
    this->helpfile = new HelpFile(); //初始化帮助文档

    //初始化状态栏
    statuslabel = new QLabel;
    statuslabel->setFrameShape(QFrame::Box);
    statuslabel->setFrameShadow(QFrame::Sunken);
    statuslabel->setMinimumSize(200,15);
    ui->statusbar->addWidget(statuslabel);

    bool flag = graph->readData(DATA_PATH); //读取线路数据
    if (!flag) //读取失败
    {
        //显示提示对话框
        QMessageBox box;
        box.setWindowTitle(tr("ERROR"));
        box.setIcon(QMessageBox::Warning);
        box.setText("读取数据错误!");
        box.addButton("确定", QMessageBox::AcceptRole);
        if (box.exec() == QMessageBox::Accepted)
        {
            box.close();
        }
    }

    //信号和槽函数的连接
    connect(addFunc->ui->comboBox_linesIn, SIGNAL(currentIndexChanged(QString)), this, SLOT(UpdateCombo_S(QString)));
    connect(addFunc->ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(UpdateTabWidget(int)));
    connect(this->ui->comboBox_Sline, SIGNAL(currentIndexChanged(QString)), this, SLOT(UpdateCombo_S_start(QString)));
    connect(this->ui->comboBox_Eline, SIGNAL(currentIndexChanged(QString)), this, SLOT(UpdateCombo_S_end(QString)));
    connect(addFunc->ui->pushButton_addLine, SIGNAL(clicked()), this, SLOT(addLine()));
    connect(addFunc->ui->pushButton_addStation, SIGNAL(clicked()), this, SLOT(addStation()));
    connect(addFunc->ui->pushButton_addEdge, SIGNAL(clicked()), this, SLOT(addEdge()));

    QTimer *timer = new QTimer(this);//新建定时器
    connect(timer,SIGNAL(timeout()),this,SLOT(UpdateTimer()));//关联定时器计满信号和相应的槽函数
    timer->start(1000);//定时器开始计时，其中1000表示1000ms即1秒

    this->UpdateCombo_Main(); //更新主屏幕下拉框
    this->on_pushButton_DisplayAll_clicked(); //显示地铁图
}

//线路选择后更新连接站点的下拉框
void MainWindow::AddEdge_line_selected(QString l_name, QVector<Line> lines, QVector<Station> stations, QHash<QString, int> linesHash)
{
    int l = linesHash[l_name];
    this->addFunc->ui->comboBox_S1->clear();
    this->addFunc->ui->comboBox_S2->clear(); //清空下拉框选项
    for(auto &s: lines[l].stations)
    {
        this->addFunc->ui->comboBox_S1->addItem(stations[s].name);
        this->addFunc->ui->comboBox_S2->addItem(stations[s].name);
    }
}

void MainWindow::UpdateCombo_S(QString l_name) //更新连接站点的下拉框
{
    this->AddEdge_line_selected(l_name, graph->lines, graph->stations, graph->linesHash);
}

void MainWindow::UpdateTabWidget(int i) //更新TabWidget
{
    Q_UNUSED(i);
    this->addFunc->UpdateLinesList(this->graph->lines);
    this->addFunc->UpdateComboBox(this->graph->lines, this->graph->stations);
}

void MainWindow::addLine() //添加新线路
{
    QMessageBox box;
    box.setWindowTitle(tr("添加新线路"));
    if(addFunc->line_name.isEmpty()) //线路名称为空
    {
        box.setIcon(QMessageBox::Warning);
        box.setText("请输入线路名称！");
    }
    else if(graph->linesHash.contains(addFunc->line_name)) //线路已存在
    {
        box.setIcon(QMessageBox::Warning);
        box.setText("该线路已存在！");
    }
    else if(addFunc->Sstation_name.isEmpty()) //未输入起始站名称
    {
        box.setIcon(QMessageBox::Warning);
        box.setText("请输入起始站名称！");
    }
    else if(addFunc->Estation_name.isEmpty()) //未输入终点站名称
    {
        box.setIcon(QMessageBox::Warning);
        box.setText("请输入终点站名称！");
    }
    else //正确输入
    {
        box.setIcon(QMessageBox::Information);
        box.setText("线路：" + addFunc->line_name + " 添加成功！");
        Station s1, s2;
        s1.name = addFunc->Sstation_name;
        s1.longitude = addFunc->SS_longitude;
        s1.latitude = addFunc->SS_latitude;
        s2.name = addFunc->Estation_name;
        s2.longitude = addFunc->ES_longitude;
        s2.latitude = addFunc->ES_latitude; //初始化s1和s2
        graph->addnew_line(addFunc->line_name, addFunc->line_color, s1, s2);
        this->UpdateCombo_Main();
    }

    box.addButton("确定",QMessageBox::AcceptRole);
    if(box.exec()==QMessageBox::Accepted)
    {
        box.close();
    }
    //this->UpdateCombo_Main();
    this->on_pushButton_DisplayAll_clicked();
}

void MainWindow::UpdateCombo_S_start(QString l_name) //更新起始站下拉框
{
    int l = this->graph->linesHash[l_name];
    if(l == -1)
        return;
    ui->comboBox_Sstation->clear();
    for(auto &s: graph->lines[l].stations)
    {
        ui->comboBox_Sstation->addItem(graph->stations[s].name);
    }
}

void MainWindow::UpdateCombo_S_end(QString l_name) //更新终点站下拉框
{
    int l = this->graph->linesHash[l_name];
    if(l == -1)
        return;
    ui->comboBox_Estation->clear();
    for(auto &s: graph->lines[l].stations)
    {
        ui->comboBox_Estation->addItem(graph->stations[s].name);
    }
}

void MainWindow::UpdateCombo_Main() //更新主屏幕下拉框
{
    ui->comboBox_Sline->clear();
    ui->comboBox_Eline->clear();
    ui->comboBox_DisplayOne->clear();
    for(auto &l: graph->lines) //更新线路下拉框
    {
        ui->comboBox_Sline->addItem(l.name);
        ui->comboBox_Eline->addItem(l.name);
        ui->comboBox_DisplayOne->addItem(l.name);
    }
    this->UpdateCombo_S_start(ui->comboBox_Sline->itemText(0)); //更新起始站下拉框
    this->UpdateCombo_S_end(ui->comboBox_Eline->itemText(0)); //更新终点站下拉框
}

void MainWindow::addStation() //新建站点
{
    QMessageBox box;
    box.setWindowTitle(tr("添加站点"));

    if(addFunc->Nstation_name.isEmpty()) //站点名称为空
    {
        box.setIcon(QMessageBox::Warning);
        box.setText("请输入站点名称！");
    }
    else if(graph->stationsHash.contains(addFunc->Nstation_name)) //站点名称已存在
    {
        box.setIcon(QMessageBox::Warning);
        box.setText("该站点已存在！");
    }
    else if(addFunc->lines_selected.isEmpty()) //未选择站点所属线路
    {
        box.setIcon(QMessageBox::Warning);
        box.setText("请选择站点所属线路！");
    }
    else //成功输入站点信息
    {
        box.setIcon(QMessageBox::Information);
        box.setText("站点：" + addFunc->Nstation_name + " 添加成功！");
        QList<int> LinesIn;
        for(auto &l: addFunc->lines_selected)
        {
            LinesIn.push_back(graph->linesHash[l]);
        }
        Station s(addFunc->Nstation_name, addFunc->NS_longitude, addFunc->NS_latitude, LinesIn);
        graph->addnew_station(s); //添加新站点
        this->UpdateCombo_Main(); //更新下拉框
    }
    box.addButton("确定",QMessageBox::AcceptRole);
    if(box.exec()==QMessageBox::Accepted)
    {
        box.close();
    }
    //this->UpdateCombo_Main();
    this->on_pushButton_DisplayAll_clicked();
}

void MainWindow::addEdge() //添加新边
{
    int l = graph->linesHash[addFunc->ui->comboBox_linesIn->currentText()];
    int s1 = graph->stationsHash[addFunc->ui->comboBox_S1->currentText()];
    int s2 = graph->stationsHash[addFunc->ui->comboBox_S2->currentText()];
    QMessageBox box;
    box.setWindowTitle(tr("添加新边"));
    if(s1 == s2) //输入的两个站点相同
    {
        box.setIcon(QMessageBox::Warning);
        box.setText("连接的站点不能相同！");
    }
    else if(!graph->lines[l].stations.contains(s1)) //站点1与线路不匹配
    {
        box.setIcon(QMessageBox::Warning);
        box.setText("选择的线路不包含站点1！");
    }
    else if(!graph->lines[l].stations.contains(s2)) //站点2与线路不匹配
    {
        box.setIcon(QMessageBox::Warning);
        box.setText("选择的线路不包含站点2！");
    }
    else //输入信息正确
    {
        box.setIcon(QMessageBox::Information);
        box.setText(tr("新边添加成功！"));
        graph->addnew_edge(s1, s2, l);
        this->UpdateCombo_Main();
    }
    box.addButton("确定",QMessageBox::AcceptRole);
    if(box.exec()==QMessageBox::Accepted)
    {
        box.close();
    }
    //this->UpdateCombo_Main();
    this->on_pushButton_DisplayAll_clicked();
}

void MainWindow::UpdateTimer() //更新计时器
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
    this->statuslabel->setText(str);
}

QColor MainWindow::getLinesColor(const QList<int>& linesList) //获取线路颜色
{
    QColor color1 = QColor(255,255,255); //初始白色
    QColor color2;
    for (int i = 0; i < linesList.size(); i++) //添加其他线路的颜色
    {
        color2 = graph->lines[linesList[i]].color;
        color1.setRed(color1.red()*color2.red()/255);
        color1.setGreen(color1.green()*color2.green()/255);
        color1.setBlue(color1.blue()*color2.blue()/255);
    }
    return color1;
}

QString MainWindow::getLinesName(const QList<int>& linesList) //获取线路名称序列
{
    QString str;
    str += "\t";
    for (int i = 0; i < linesList.size(); i++)
    {
        str += " ";
        str += graph->lines[linesList[i]].name;
    }
    return str;
}

QPointF MainWindow::transferCoord(QPointF coord) //坐标转换，由经纬度转换为scene中的坐标
{
    QPointF minCoord = QPointF(Station::minLongitude, Station::minLatitude);
    QPointF maxCoord = QPointF(Station::maxLongitude, Station::maxLatitude);
    double x = (coord.x()-minCoord.x())/(maxCoord.x()-minCoord.x())*NET_WIDTH + MARGIN;
    double y = (maxCoord.y()-coord.y())/(maxCoord.y()-minCoord.y())*NET_HEIGHT + MARGIN;
    return QPointF(x,y);
}

void MainWindow::drawStations(const QList<int>& stationsList) //绘制站点
{
    for (int i = 0; i < stationsList.size(); i++)
    {
        int s = stationsList[i];
        QString name = graph->stations[s].name; //获取站点名称
        QList<int> linesList = graph->stations[s].linesIn.values(); //获取站点所属线路
        QColor color = getLinesColor(linesList); //获取站点颜色
        QPointF point = QPointF(graph->stations[s].longitude, graph->stations[s].latitude); //获取经纬度坐标
        QPointF coord = transferCoord(point); //转换为视图坐标
        QString tip = ("站名：  " + name + "\n" +
                       "经度：  " + QString::number(point.x(),'f',7) + "\n" +
                       "纬度：  " + QString::number(point.y(),'f',7) + "\n" +
                       "线路：  " + getLinesName(linesList)); //输入tip

        //画圆
        QGraphicsEllipseItem* stationItem = new QGraphicsEllipseItem;
        stationItem->setRect(-NODE_HALF_WIDTH, -NODE_HALF_WIDTH, NODE_HALF_WIDTH<<1, NODE_HALF_WIDTH<<1);
        stationItem->setPos(coord);
        stationItem->setPen(color);
        stationItem->setCursor(Qt::PointingHandCursor);
        stationItem->setToolTip(tip);

        if(linesList.size()<=1) //若仅有一个所属线路则设置透明背景
        {
            stationItem->setBrush(QColor(QRgb(0xffffff)));
        }

        scene->addItem(stationItem); //视图中增加站点元件

        //设置站点名称
        QGraphicsTextItem* textItem = new QGraphicsTextItem;
        textItem->setPlainText(name);
        textItem->setFont(QFont("consolas", 4, 1));
        textItem->setPos(coord.x(), coord.y()-NODE_HALF_WIDTH*2);
        //textItem->setPos(coord.x(),coord.y());
        scene->addItem(textItem);
    }
}

void MainWindow::drawEdges(const QList<Edge>& edgesList) //绘制边
{
    for(int i = 0; i < edgesList.size(); i++)
    {
        int s1 = edgesList[i].first; //边出
        int s2 = edgesList[i].second; //边入

        QList<int> linesList = graph->getCommon_lines(s1, s2); //获取公共线路
        QColor color = getLinesColor(linesList); //获取线路颜色
        QString tip = "线路：" + getLinesName(linesList);
        QPointF s1Pos = transferCoord(QPointF(graph->stations[s1].longitude, graph->stations[s1].latitude));
        QPointF s2Pos = transferCoord(QPointF(graph->stations[s2].longitude, graph->stations[s2].latitude));

        QGraphicsLineItem* edgeItem = new QGraphicsLineItem; //绘制线路
        edgeItem->setPen(QPen(color, EDGE_PEN_WIDTH));
        edgeItem->setCursor(Qt::PointingHandCursor);
        edgeItem->setToolTip(tip);
        edgeItem->setPos(s1Pos);
        edgeItem->setLine(0, 0, s2Pos.x()-s1Pos.x(), s2Pos.y()-s1Pos.y());
        scene->addItem(edgeItem); //添加元件
    }
}

void MainWindow::transfer() //搜索换乘路线
{
    int s1 = graph->stationsHash[ui->comboBox_Sstation->currentText()];
    int s2 = graph->stationsHash[ui->comboBox_Estation->currentText()];
    QString Trans_strategy = ui->comboBox_Transfer->currentText();
    int way = (Trans_strategy == "最短时间优先" ? 1 : 2);

    //输入站点为空
    if(!graph->stationsHash.contains(ui->comboBox_Sstation->currentText()) || !graph->stationsHash.contains(ui->comboBox_Estation->currentText()))
    {
        QMessageBox box;
        box.setWindowTitle(tr("换乘查询"));
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("站点为空！"));
        box.addButton(tr("确定"),QMessageBox::AcceptRole);
        if(box.exec()==QMessageBox::Accepted)
        {
            box.close();
        }
    }
    else
    {
        QList<int> PathList;
        QList<Edge> EdgeList;
        bool success = true;
        int transfer_num; //记录换乘次数
        if(way == 1) //最短时间策略
            success = graph->Min_cost_path(s1, s2, PathList);
        else //最少换乘策略
            success = graph->Min_transfer_path(s1, s2, PathList, transfer_num);

        if(!success) //换乘失败
        {
            QMessageBox box;
            box.setWindowTitle(tr("换乘查询"));
            box.setIcon(QMessageBox::Warning);
            box.setText(tr("所选起始站与终点站不连通！"));
            box.addButton(tr("确定"),QMessageBox::AcceptRole);
            if(box.exec()==QMessageBox::Accepted)
            {
                box.close();
            }
        }
        else //成功找到终点站
        {
            for(int i = 0; i < PathList.size() - 1; i++)
            {
                EdgeList.push_back(Edge(PathList[i], PathList[i+1]));
            }
            scene->clear();
            this->drawStations(PathList); //绘制站点
            this->drawEdges(EdgeList); //绘制线路
            QString str = (way == 1 ? ("以下线路时间最短，共换乘" + QString::number(PathList.size()-1) + "个站点\n\n"):
                                      ("以下线路换乘最少，共换乘" + QString::number(transfer_num) + "条线路\n\n"));
            for(int i = 0; i < PathList.size(); i++)
            {
                if(i)
                {
                    str += "\n  ↓\n";
                }
                str += graph->stations[PathList[i]].name;
                QString linesIn = getLinesName(graph->stations[PathList[i]].linesIn.values());
                str += linesIn;
            }
            ui->textBrowser->clear();
            ui->textBrowser->setText(str); //设置换乘流程
        }
    }
}

void MainWindow::on_actionZoomUp_triggered() //放大视图
{
    ui->graphicsView->scale(1.5,1.5);
}

void MainWindow::on_actionZoomDown_triggered() //缩小视图
{
    ui->graphicsView->scale(2.0/3,2.0/3);
}

void MainWindow::on_actionAddNewLine_triggered() //添加按钮-显示添加界面
{
    this->addFunc->ui->tabWidget->setCurrentIndex(0);
    this->addFunc->show();
}

void MainWindow::on_actionAddNewStation_triggered() //添加按钮-显示添加界面
{
    this->addFunc->ui->tabWidget->setCurrentIndex(1);
    this->addFunc->show();
}

void MainWindow::on_actionAddNewEdge_triggered() //添加按钮-显示添加界面
{
    this->addFunc->ui->tabWidget->setCurrentIndex(2);
    this->addFunc->show();
}

void MainWindow::on_actionAddNew_triggered() //添加按钮-显示添加界面
{
    this->addFunc->ui->tabWidget->setCurrentIndex(0);
    this->addFunc->show();
}

void MainWindow::on_pushButton_DisplayAll_clicked() //显示全部线路
{
    scene->clear();
    QList<int> stationList;
    QList<Edge> edgeList;
    for(int i=0; i<graph->stations.size(); i++)
        stationList.push_back(i);
    edgeList = graph->edges.values();
    drawEdges(edgeList);
    drawStations(stationList);
}

void MainWindow::on_actionQuit_triggered() //退出
{
    close();
}

void MainWindow::on_pushButton_Transfer_clicked() //搜索换乘线路
{
    transfer();
}

void MainWindow::on_pushButton_DisplayOne_clicked() //单独显示指定线路
{
    QString Sline_name = ui->comboBox_DisplayOne->currentText();
    if(Sline_name == "请选择") //未选择线路
    {
        QMessageBox box;
        box.setWindowTitle(tr("Warning!"));
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("请选择要单独显示的线路！"));
        box.addButton(tr("确定"),QMessageBox::AcceptRole);
        if(box.exec()==QMessageBox::Accepted)
        {
            box.close();
        }
    }
    else //成功选择线路
    {
        int line = graph->linesHash[Sline_name];
        QList<int> stationList;
        QList<Edge> edgeList;
        for(auto &s: graph->lines[line].stations)
            stationList.push_back(s);
        edgeList = graph->lines[line].edges.values();
        this->scene->clear();
        this->drawStations(stationList);
        this->drawEdges(edgeList);
    }
}

void MainWindow::on_actionHelp_triggered() //显示帮助文档
{
    this->helpfile->show();
}
