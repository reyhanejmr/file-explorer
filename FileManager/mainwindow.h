#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "QtWidgets"
#include <QMainWindow>
#include "QSettings"
#include "detail.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots :
    void onItemClicked(const QModelIndex &index);
    void title_View();
    void list_View();
    void Restoreview();
    void on_listView_doubleClicked(const QModelIndex &index);
    void on_listView_clicked(const QModelIndex &index);
    void on_serchButton_clicked();
    void onNextClicked();
    void onPreviousClicked();
    void onExitClicked();
    void on_listView_customContextMenuRequested(const QPoint &pos);
    void countFiles(const QString& folderPath);
    void countSize(const QString& folderPath);
    void CopyAction();
    void CutAction() ;
    void PasteAction();
    void keyPressEvent(QKeyEvent *event);



private:
    Ui::MainWindow *ui;
    QFileSystemModel *model;
    QLabel *Mylbl=new QLabel();
    QFileSystemModel* ListModel;
    QSettings *settings;
    QList<QString> addresses;
    int currentindex;
    QList<QString> CopyAddresses;
    bool FlagCopyorCut;
    int fileCount = 0;
    qint64 totalSize1 = 0;
};

#endif // MAINWINDOW_H
