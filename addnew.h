#ifndef ADDNEW_H
#define ADDNEW_H

#include <QWidget>
#include <QPixmap>
#include <QColorDialog>
#include <QDialog>
#include <QVector>
#include <QString>
#include <QListWidget>
#include <QListWidgetItem>
#include <QComboBox>
#include <QColor>
#include "graph.h"

class MainWindow;

namespace Ui {
class AddNew;
}

class AddNew : public QWidget
{
    Q_OBJECT

public:
    explicit AddNew(QWidget *parent = nullptr);
    ~AddNew();

    void InitUI(); //初始化添加界面
    void UpdateLinesList(QVector<Line> lines); //更新添加新站点中的可选线路
    void UpdateComboBox(QVector<Line> lines, QVector<Station> stations); //更新下拉框选项
    friend class MainWindow;
private slots:
    void on_lineEdit_inputLineName_textChanged(const QString &arg1);
    void on_pushButton_selectColor_clicked();
    void on_lineEdit_inputSSname_textChanged(const QString &arg1);
    void on_doubleSpinBox_SSlongitude_valueChanged(double arg1);
    void on_doubleSpinBox_SSlatitude_valueChanged(double arg1);
    void on_lineEdit_inputESname_textChanged(const QString &arg1);
    void on_doubleSpinBox_ESlongitude_valueChanged(double arg1);
    void on_doubleSpinBox_ESlatitude_valueChanged(double arg1);

    void on_lineEdit_Sname_textChanged(const QString &arg1);
    void on_doubleSpinBox_Slongitude_valueChanged(double arg1);
    void on_doubleSpinBox_Slatitude_valueChanged(double arg1);
    void on_listWidget_LinesIn_itemClicked(QListWidgetItem *item);

private:
    Ui::AddNew *ui;
    QString line_name; //线路名称
    QColor line_color; //线路颜色
    QString Sstation_name; //起始站名称
    QString Estation_name; //终点站名称
    double SS_longitude; //起始站经度
    double SS_latitude; //起始站纬度
    double ES_longitude; //终点站经度
    double ES_latitude; //终点站纬度
    QString Nstation_name; //新建站点名称
    double NS_longitude; //新建站点经度
    double NS_latitude; //新建站点纬度
    QList<QString> lines_selected; //新建站点所属线路
};

#endif // ADDNEW_H
