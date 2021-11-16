# Shanghai_Subway_System
Shanghai_Subway_System(QT)
1题目
★★★上海的地铁交通网路已基本成型，建成的地铁线十多条，站点上百个，现需建立一个换乘指南打印系统，通过输入起点站和终点站，打印出地铁换乘指南，指南内容包括起点站、换乘站、终点站。
（1）图形化显示地铁网络结构，能动态添加地铁线路和地铁站点。
（2）根据输入起点站和终点站，显示地铁换乘指南。
（3）通过图形界面显示乘车路径。
2软件功能
软件功能：
1）查看当前地铁线路
查看上海地铁线路图，可通过鼠标进行拖拽，并提供操纵滚轮进行定点放大缩小功能以及点击放大缩小按钮进行视图中心放大缩小。
2）添加新线路、站点和连接
可以通过添加窗口添加新线路，为任意线路添加新站点，并且可以为任意两个站点添加连接的边。
3）根据输入起点站和终点站，显示地铁换乘指南
可以根据输入的起始站和终点站，搜索地铁换乘指南。其中换乘的方式有两种，一种是最短时间换乘，一种是最少换乘次数换乘。可以根据选择的方式进行最优线路搜索，并将搜索到的乘车路径通过图形界面显示出来。
4）帮助文档
可以显示帮助文档，帮助用户正确地使用该软件
实现方式：
通过QT和C++编程完成。
3设计思想
实现思路：
将整个程序分为三个大模块，分别为前端显示模块、后端核心模块以及连接前后端的控制模块，整体采用MVC模式系统架构。MVC模式即为模型-视图-控制器结构模式，其前后端实现分离，前端主要考虑如何显示地铁线路以及各站点信息、如何通过操纵鼠标对视图进行调整等；后端主要考虑如何设计算法生成换乘指南，如何保存线路和站点的信息等。首先将前后端两个模块设计完成，最后设计控制器连接前后端接口，实现前后端的实时互联，以达到及时响应用户操作请求的效果。
数据结构的选用和数据结构的设计思想：
本软件选用了多种数据结构来储存数据，其中包括：图结构（邻接表，用于存储地铁线路图）、线性表结构（数组，用于存储站点序列、线路序列、换乘站点序列等）、队列结构（优先队列和普通队列，分别用于迪杰斯特拉算法和宽度优先搜索）、哈希表结构（用于站点名称和站点序号之间以及线路名称和线路序号之间的哈希变换）。
数据结构的设计思想是将地铁线路图看成一个普通的图，其中将站点看作结点，将相邻站点之间的连线看作是两个结点的边，这样就可以将地铁线路图抽象为一个图。在此基础上可以进而将这些站点当作结点存储在线性表中，并用哈希表进行站点序列和名称之间的互相变换。
算法设计基本流程：
本程序主要涉及两个较为关键的算法，即迪杰斯特拉算法和宽度优先搜索算法。迪杰斯特拉算法用于搜索最短时间的换乘路径；宽度优先搜索用于搜索最少换乘次数的换乘路径（其中宽度指换乘次数）。
1）迪杰斯特拉算法：
迪杰斯特拉算法是典型的单源最短路径算法，用于计算一个节点到其他节点的最短路径。它的主要特点是以起始点为中心向外层层扩展(广度优先搜索思想)，直到扩展到目标点为止。
通过迪杰斯特拉算法计算图G中的最短路径时，需要指定起点s(即从顶点s开始计算)。此外，引进两个集合S和U。S的作用是记录已求出最短路径的顶点(以及相应的最短路径长度)，而U则是记录还未求出最短路径的顶点(以及该顶点到起点s的距离)。初始时，S中只有起点s；U中是除s之外的顶点，并且U中顶点的路径是"起点s到该顶点的路径"。然后，从U中找出路径最短的顶点，并将其加入到S中；接着，更新U中的顶点和顶点对应的路径。 然后，再从U中找出路径最短的顶点，并将其加入到S中；接着，更新U中的顶点和顶点对应的路径。……重复该操作，直到遍历完所有顶点。这样就得到了源点到所有其他点的最短路径。
在本题中，源点为起始站，目标点为终点站。起初设置集合S仅包含起始站，然后依次按照迪杰斯特拉算法的步骤向S中添加结点，直到发现目标点，即可得到起始站到终点站的最短路径，也是从起点到终点的最短时间路径。具体算法流程如下流程图：
 
2）宽度优先搜索算法
宽度优先搜索是经典的搜索算法，又称广度优先搜索或横向优先搜索。该算法是一种图形搜索算法，从起点开始搜索然后一层一层的向下搜索，如果找到目标或者搜索完了全部的节点则算法结束。Dijkstra单源最短路径算法和Prim最小生成树算法都采用了和宽度优先搜索类似的思想。宽度优先搜索系统地展开并检查图中的所有节点，以找寻结果。换句话说，它并不考虑结果的可能位置，彻底地搜索整张图，直到找到结果为止。从根节点开始，沿着树(图)的宽度遍历树(图)的节点。如果所有节点均被访问，则算法中止。一般用队列数据结构来辅助实现宽度优先搜索算法。
在本题中，将起始站设置为搜索的起点，将换乘次数作为宽度优先搜索中的宽度，那么在这样的宽度优先搜索下搜索到目标结点，即终点站时，搜索过程中保存的路径就是最少换乘次数路径。因此本题可利用宽度优先搜索对最少换乘次数的换乘路径进行搜索。具体算法流程如下流程图：
 
4逻辑结构与物理结构
逻辑结构：整体采用网络结构，部分类包含线性结构和集合结构。
物理结构：包括存储网状线路的图结构、存储站点和线路的数组结构、以及存储站点所属线路集合的集合结构。
1）站点类
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

2）线路类
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

3）图的邻接点结构
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

4）图管理类
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

5开发平台
开发平台： 
开发语言：C++
开发框架：QT
集成开发环境：Qt Version 5.14.1
编辑器：Qt Creator Manual 4.11.1
编译器：MinGW 32bit
运行环境：
计算机型号：DELL G3
操作系统：Win 10
在上述集成环境下运行
