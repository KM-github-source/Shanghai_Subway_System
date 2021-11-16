#ifndef GRAPH_H
#define GRAPH_H

#include <QString>
#include <QSet>
#include <QList>
#include <QColor>
#include <QPointF>
#include <QFile>
#include <QTextStream>
#include <queue>

#define Infinity 10000000000
#define BackgroundPath ":/Data/background.jpg"
#define BackgroundPath2 ":/Data/background2.jpg"
#define FilePath ":/Data/help.html"

class AddNew;
class MainWindow;

//定义边类型
typedef QPair<int,int> Edge;

class Station
{
private:
    int id;                     //站点ID
    QString name;               //站点名称
    double longitude;           //站点经度
    double latitude;            //站点纬度
    QSet<int> linesIn;          //站点所属线路

    //所有站点的边界位置
    static double minLongitude, minLatitude, maxLongitude, maxLatitude;

public:
    //构造函数
    Station();
    Station(QString name, double longitude, double latitude, QList<int> linesList);

    //求取站点间实地直线距离
    int distance(Station other);

    //声明友元
    friend class Graph;
    friend class AddNew;
    friend class QTextStream;
    friend class MainWindow;
};

//线路类
class Line
{
private:
    int id;                     //线路ID
    QString name;               //线路名称
    QColor color;               //线路颜色
    QVector <QString> fromTo;   //线路起始站点
    QSet<int> stations;         //线路站点集合
    QSet<Edge> edges;           //线路站点连接关系

public:
    //构造函数
    Line(){};
    Line(QString lineName, QColor lineColor):name(lineName), color(lineColor)
    {};

    //声明友元
    friend class Graph;
    friend class AddNew;
    friend class QTextStream;
    friend class MainWindow;
};

//图的邻接点结构
class Node{
public:
    int next_id;        //邻接点ID
    double distance;    //两点距离

    //构造函数
    Node(){};
    Node(int id, double dist) :next_id(id), distance(dist)
    {};

    //">"运算重载，用于小顶堆
    bool operator > (const Node& n) const
    {
        return this->distance>n.distance;
    }
};

class Graph
{
private:
    QVector<Station> stations;          //存储所有站点
    QVector<Line> lines;                //存储所有线路
    QHash<QString, int> stationsHash;   //站点名到存储位置的hash
    QHash<QString, int> linesHash;      //线路名到存储位置的hash
    QSet<Edge> edges;                   //所有边的集合
    QVector<QVector<Node>> graph;       //地铁线路网络图邻接表

    //清空数据
    void dataClear();
    //插入一条边
    bool insertEdge(int s1, int s2);
    //更新边界经纬度
    void updateMinMaxLongiLati();
    //生成图结构
    void makeGraph();

public:
    //构造函数
    Graph();

    bool readData(QString fileName); //从文件读取数据
    void UpdateBorder(); //更新边界范围
    int getHash_line(QString line_name); //得到线路哈希值
    int getHash_station(QString station_name); //得到站点哈希值
    QList<int> getCommon_lines(int s1, int s2); //获得相同线路集

    void addnew_station(Station s); //添加新站点
    void addnew_edge(int s1, int s2, int l); //添加新边
    void addnew_line(QString line_name, QColor line_color, Station s1, Station s2); //添加新线路

    bool Min_cost_path(int s1, int s2, QList<int>&PathList);
    bool Min_transfer_path(int s1, int s2, QList<int>&PathList, int& transfer_num);

    //声明友元
    friend class MainWindow;
};

#endif // GRAPH_H
