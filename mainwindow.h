#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QDateTime>
#include <QLabel>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include "view_zoom.h"
#include "graph.h"
#include "addnew.h"
#include "helpfile.h"

#define NET_WIDTH 2000
#define NET_HEIGHT 2000
#define MARGIN 30
#define SCENE_WIDTH (MARGIN*2 + NET_WIDTH)
#define SCENE_HEIGHT (MARGIN*2 + NET_HEIGHT)
#define EDGE_PEN_WIDTH 2    //线路边宽
#define NODE_HALF_WIDTH 3   //节点大小
#define DATA_PATH ":/Data/data.txt"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void InitUI();

private:
    Ui::MainWindow *ui;
    View_zoom *myView;     //自定义视图，用于鼠标缩放
    QGraphicsScene *scene; //场景
    Graph* graph;          //图管理
    AddNew* addFunc;       //添加功能管理类
    QLabel* statuslabel;   //状态栏
    HelpFile* helpfile;    //帮助文档


public slots:
    void AddEdge_line_selected(QString l_name, QVector<Line> lines, QVector<Station> stations, QHash<QString, int> linesHash); //线路选择下拉框槽函数
    void UpdateTabWidget(int i); //更新TabWidget的信息
    void UpdateCombo_S(QString l_name); //更新连接的站点下拉框选项
    void UpdateCombo_S_start(QString l_name); //更新起始站下拉框
    void UpdateCombo_S_end(QString l_name); //更新终点站下拉框
    void UpdateCombo_Main(); //更新MainWindow的下拉框
    void UpdateTimer(); //更新计时器
    void addLine(); //添加新线路
    void addStation(); //添加新站点
    void addEdge(); //添加新边
    QColor getLinesColor(const QList<int>& linesList); //获取线路颜色
    QString getLinesName(const QList<int>& linesList); //获取线路名称序列
    QPointF transferCoord(QPointF coord); //转换坐标，获得scene的坐标
    void drawStations(const QList<int>& stationsList); //绘制站点
    void drawEdges(const QList<Edge>& edgesList); //绘制边
    void transfer(); //搜索换乘策略

    void on_pushButton_DisplayAll_clicked();
    void on_actionZoomUp_triggered();
    void on_actionZoomDown_triggered();
    void on_actionAddNewLine_triggered();
    void on_actionAddNewStation_triggered();
    void on_actionAddNewEdge_triggered();
    void on_actionAddNew_triggered();
    void on_actionQuit_triggered();
    void on_actionHelp_triggered();
    void on_pushButton_Transfer_clicked();
    void on_pushButton_DisplayOne_clicked();
};
#endif // MAINWINDOW_H
