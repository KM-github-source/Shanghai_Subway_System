#ifndef HELPFILE_H
#define HELPFILE_H

#include <QWidget>
#include "graph.h"
//#include "mainwindow.h"

class MainWindow;

namespace Ui {
class HelpFile;
}

class HelpFile : public QWidget
{
    Q_OBJECT

public:
    explicit HelpFile(QWidget *parent = nullptr);
    ~HelpFile();
    void InitUI();

    friend class MainWindow;

private:
    Ui::HelpFile *ui;
};

#endif // HELPFILE_H
