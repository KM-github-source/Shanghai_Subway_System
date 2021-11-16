#include "graph.h"
#include <math.h>
#include <QDebug>

double Station::minLongitude = 200;
double Station::minLatitude = 200;
double Station::maxLongitude = 0;
double Station::maxLatitude = 0;

Graph::Graph()
{

}

double rad(double d) // 角度转弧度
{
    const double PI = 3.1415926535898;
    return d * PI / 180.0;
}

int calcuDistance(double fLati1, double fLong1, double fLati2, double fLong2) //传入两个经纬度，计算之间的大致直线距离
{
    const double EARTH_RADIUS = 6378.137;

    double radLat1 = rad(fLati1);
    double radLat2 = rad(fLati2);
    double a = radLat1 - radLat2;
    double b = rad(fLong1) - rad(fLong2);
    double s = 2 * asin(sqrt(pow(sin(a/2),2) + cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));
    s = s * EARTH_RADIUS;
    s = (int)(s * 10000000) / 10000;
    return s;
}

Station::Station()
{
    ;
}

//构造函数
Station::Station(QString name, double longitude, double latitude, QList<int> linesList):
    name(name), longitude(longitude), latitude(latitude)
{
    this->linesIn = QSet<int>(linesList.begin(), linesList.end());
}

int Station::distance(Station other) //求取站点间实地直线距离
{
    return calcuDistance(latitude, longitude, other.latitude, other.longitude);
}

bool Graph::readData(QString fileName) //从文件中读取上海地铁线路和站点数据
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return false; //文件打开失败返回false

    QTextStream in(&file);
    while(!in.atEnd())
    {
        Line line;
        QString id, name, colour, fromTo, totalStations; //忽略前缀字符
        QString color, froms, tos; //接收颜色、出发站和终点站
        bool ok;
        int total; //站点数
        Station station;
        int line_Hindex; //线路哈希值
        int station_preIndex;
        int station_nowIndex;

        in >> id >> line.id;
        in >> name >> line.name;
        in >> colour >> color;
        line.color.setRgba(color.remove(0,1).toUInt(&ok, 16));

        in >> fromTo >> froms >> tos;
        in >> totalStations >> total;

        if (linesHash.count(line.name)) //若lines中存在线路line.name
        {
            line_Hindex = linesHash[line.name];
            lines[line_Hindex].fromTo.push_back(froms);
            lines[line_Hindex].fromTo.push_back(tos);
        }
        else //若lines中不存在线路line.name
        {
            line_Hindex = linesHash[line.name] = lines.size();
            lines.push_back(line);
            line.fromTo.push_back(froms);
            line.fromTo.push_back(tos);
        }
        //输入站点信息
        QString longlat;
        QStringList strList;
        for (int i = 0; !in.atEnd() && i < total; i++)
        {
            in >> station.id >> station.name >> longlat;
            strList = longlat.split(QChar(','));
            station.longitude = strList.first().toDouble();
            station.latitude = strList.last().toDouble();

            if (stationsHash.count(station.name)) //若当前站点已出现过
            {
                station_nowIndex = stationsHash[station.name];
            }
            else //若当前站点未出现过
            {
                station_nowIndex = stationsHash[station.name] = stations.size();
                stations.push_back(station);
            }

            stations[station_nowIndex].linesIn.insert(line_Hindex);
            lines[line_Hindex].stations.insert(station_nowIndex);

            if (i) //若当前站点不是第一个站点
            {
                lines[line_Hindex].edges.insert(Edge(station_preIndex, station_nowIndex));
                lines[line_Hindex].edges.insert(Edge(station_nowIndex, station_preIndex));
                insertEdge(station_preIndex, station_nowIndex);
            }
            station_preIndex = station_nowIndex;
        }
        //flag表示文件是否符合规范
        bool flag = (id=="id:" && name=="name:" && colour=="colour:" && fromTo=="fromTo:" && totalStations=="totalStations:" && ok && !in.atEnd());

        if(flag == false) //文件不符合规范
        {
            file.close();
            dataClear();
            return false;
        }
        in.readLine();
    }
    file.close();

    this->UpdateBorder(); //更新站点经纬度范围
    return true;
}

void Graph::dataClear() //清除存储的地铁信息
{
    stations.clear();
    lines.clear();
    stationsHash.clear();
    linesHash.clear();
    edges.clear();
    graph.clear();
}

bool Graph::insertEdge(int s1, int s2) //插入边
{
    if(this->edges.contains(Edge(s1, s2)) || this->edges.contains(Edge(s2, s1)))
        return false;
    this->edges.insert(Edge(s1, s2));
    this->makeGraph(); //重新生成邻接表
    return true;
}

void Graph::makeGraph() //生成邻接表
{
    this->graph.clear();
    this->graph = QVector<QVector<Node>>(this->stations.size(), QVector<Node>());
    for(auto &e : this->edges)
    {
        double distance = this->stations[e.first].distance(this->stations[e.second]); //计算距离
        this->graph[e.first].push_back(Node(e.second, distance));
        this->graph[e.second].push_back(Node(e.first, distance));
    }
}

void Graph::UpdateBorder() //更新边界范围
{
    double minLongitude = 130;
    double minLatitude = 130;
    double maxLongitude = 0;
    double maxLatitude = 0;

    for (auto &s : this->stations)
    {
        minLongitude = qMin(minLongitude, s.longitude);
        minLatitude = qMin(minLatitude, s.latitude);
        maxLongitude = qMax(maxLongitude, s.longitude);
        maxLatitude = qMax(maxLatitude, s.latitude);
    }
    Station::minLongitude = minLongitude;
    Station::minLatitude = minLatitude;
    Station::maxLongitude = maxLongitude;
    Station::maxLatitude = maxLatitude;
}

int Graph::getHash_line(QString line_name) //获得线路哈希值
{
    if(this->linesHash.contains(line_name))
        return this->linesHash[line_name];
    else
        return -1;
}

int Graph::getHash_station(QString station_name) //获得站点哈希值
{
    if(this->stationsHash.contains(station_name))
        return this->stationsHash[station_name];
    else
        return -1;
}

QList<int> Graph::getCommon_lines(int s1, int s2) //获取两个站点的公共所属线路
{
    QList<int> linesList;
    for (auto &s : stations[s1].linesIn)
    {
        if(stations[s2].linesIn.contains(s))
            linesList.push_back(s);
    }
    return linesList;
}

void Graph::addnew_station(Station s) //添加新站点
{
    int s_hash = this->stations.size();
    this->stationsHash[s.name] = s_hash;
    s.id = s_hash;
    this->stations.push_back(s);
    for(auto &l: s.linesIn)
    {
        this->lines[l].stations.insert(s_hash);
    }
    this->UpdateBorder(); //更新站点经纬度范围
}

void Graph::addnew_edge(int s1, int s2, int l) //添加新边
{
    this->lines[l].edges.insert(Edge(s1, s2));
    this->lines[l].edges.insert(Edge(s2, s1));
    this->insertEdge(s1, s2);
}

void Graph::addnew_line(QString line_name, QColor line_color, Station s1, Station s2) //添加新线路
{
    int l_hash = this->lines.size();
    this->linesHash[line_name] = l_hash;
    this->lines.push_back(Line(line_name, line_color));
    QSet<int> List;
    List.insert(l_hash);
    s1.linesIn = List;
    s2.linesIn = List;
    this->addnew_station(s1); //添加起始站
    this->addnew_station(s2); //添加终点站
}

bool Graph::Min_cost_path(int s1, int s2, QList<int> &PathList) //最短时间换乘路径搜索
{
    PathList.clear();
    if(s1 == s2) //起始站和终点站相同
    {
        PathList.push_back(s1);
        return true;
    }

    QVector<int> Father(this->stations.size(), -1); //记录路径
    QVector<double> Distance(this->stations.size(), Infinity); //记录距源点的距离
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> Q; //小顶堆

    Q.push(Node(s1, 0));
    Distance[s1] = 0;
    bool found = false;
    while(!Q.empty())
    {
        Node n = Q.top();
        Q.pop();
        if(n.next_id == s2) //若堆顶元素为终点站，则找到终点站，退出循环
        {
            found = true;
            break;
        }

        if(n.distance > Distance[n.next_id]) //若源点到该点的距离大于最小距离，则直接跳过
            continue;
        else //否则该点进入源点集，并更新所有与该点邻接的站点距离信息
        {
            for(auto &i: this->graph[n.next_id])
            {
                if((i.distance + n.distance) < Distance[i.next_id])
                {
                    Distance[i.next_id] = (i.distance + n.distance);
                    Q.push(Node(i.next_id, Distance[i.next_id]));
                    Father[i.next_id] = n.next_id;
                }
            }
        }
    }

    if(!found) //未找到目标站点
        return false;

    int s = s2;
    while(s != -1)
    {
        PathList.push_front(s); //路径信息输入到PathList中
        s = Father[s];
        //qDebug() << s;
    }
    return true;
}

bool Graph::Min_transfer_path(int s1, int s2, QList<int> &PathList, int& transfer_num) //最少换乘次数的路径搜索
{
    PathList.clear();
    if(s1 == s2) //起始站和终点站相同
    {
        PathList.push_back(s1);
        return true;
    }

    QVector<bool> Visted(this->lines.size(), false); //记录已访问线路
    QVector<int> Father(this->stations.size(), -1); //记录路径
    QVector<int> FatherInLine(this->stations.size(), -1); //记录Father对应的线路
    Father[s1] = -2;
    std::queue<int> Q;
    Q.push(s1);
    bool found = false;
    //基于宽度优先搜索的思想，换乘次数少的优先搜索
    while(!Q.empty())
    {
        int s = Q.front();
        Q.pop();
        if(s == s2) //若找到终点站则退出循环
        {
            found = true;
            break;
        }
        for(auto &l: this->stations[s].linesIn)
        {
            if(!Visted[l]) //若没有访问过该线路则开始访问
            {
                Visted[l] = true;

                int sNow = s; //记录当前访问站点
                std::queue<int> Q_l; //保存线路上的待扩展站点
                Q_l.push(s);

                while(!Q_l.empty())
                {
                    sNow = Q_l.front();
                    Q_l.pop();
                    if(sNow == s2)
                    {
                        found = true;
                        break;
                    }
                    for(auto &station: this->graph[sNow])
                    {    //若该站点在线路l上且该站点未被访问过则继续扩展
                        if(this->lines[l].stations.contains(station.next_id) && (Father[station.next_id] == -1 || FatherInLine[station.next_id] != l) && Father[station.next_id] != -2 && (Father[sNow] != station.next_id))
                        {
                            Q.push(station.next_id);
                            Q_l.push(station.next_id);
                            Father[station.next_id] = sNow;
                            FatherInLine[station.next_id] = l;
                        }
                    }
                }
            }
            if(found == true) //若找到则退出循环
                break;
        }
        if(found == true) //若找到则退出循环
            break;
    }

    if(!found) //若未找到目标站点则返回false
        return false;

    int s = s2;
    int count_lines = 0; //记录换乘次数
    while(s != -1 && s != -2)
    {
        PathList.push_front(s);
        if(Father[s] != -1 && Father[s] != -2 && (FatherInLine[s] != FatherInLine[Father[s]]))
            count_lines++;
        s = Father[s];
        //qDebug() << s;
    }

    transfer_num = count_lines - 1; //终点站的FatherInLine[s]是-1，因此需要减一
    return true;
}
