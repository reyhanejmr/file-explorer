#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QtWidgets"
#include "QDir"
#include "QHeaderView"
#include "QModelIndex"
#include "QDebug"
#include "QSettings"
#include "detail.h"
#include "QThread"
#include <thread>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
//connect(this,&MainWindow::setData,this,&MainWindow::setvalue);
    ui->setupUi(this);
    currentindex=0;
    settings=new QSettings(QSettings::NativeFormat,QSettings::UserScope,"mine","FileManger");

    model=new QFileSystemModel(this);
    QString path=QDir::rootPath();
    model->setRootPath(path);
    ListModel=new QFileSystemModel(this);
    ListModel->setRootPath(path);
    QModelIndex rootindex;
    if(settings->value("Check").toString()=="No")
    {
        rootindex= ListModel->setRootPath("");
        ui->AddresslineEdit->setText("Welcome To File Manager ;)");
        addresses.append(path);
        currentindex=addresses.size()-1;
        ui->action_Restore_last_view->setChecked(false);
    }
    else
    {
        rootindex= ListModel->setRootPath(settings->value("LastFolder").toString());
        ui->AddresslineEdit->setText(settings->value("LastFolder").toString());
        addresses.append(settings->value("LastFolder").toString());
        currentindex=addresses.size()-1;
         ui->action_Restore_last_view->setChecked(true);
    }
    ui->listView->setModel(ListModel);
    ui->listView->setRootIndex(rootindex);

    model->setFilter(QDir::AllDirs|QDir::NoDotAndDotDot);
    ui->FilestreeView->setModel(model);
    ui->FilestreeView->setColumnHidden(1,true);
    ui->FilestreeView->setColumnHidden(2,true);
    ui->FilestreeView->setColumnHidden(3,true);
    //    ui->FilestreeView->header()->setSectionResizeMode(QHeaderView::Interactive);
    //    ui->FilestreeView->header()->setDefaultSectionSize(100);
    //    ui->FilestreeView->header()->setSectionsMovable(true);
    connect(ui->FilestreeView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onItemClicked(const QModelIndex&)));
    connect(ui->FilestreeView, SIGNAL(pressed(const QModelIndex&)), this, SLOT(onItemClicked(const QModelIndex&)));
    connect(ui->actionTitle_view, &QAction::triggered, this, &MainWindow::title_View);
    connect(ui->actionList_view, &QAction::triggered, this, &MainWindow::list_View);
    connect(ui->action_Restore_last_view, &QAction::triggered, this, &MainWindow::Restoreview);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onExitClicked);
    QPushButton *Titleb=new QPushButton("",this);
    QPixmap pixmap1 = QPixmap(":/new/prefix1/images/t2.png");
    Titleb->setIcon(QIcon(pixmap1));
    QPushButton *listb=new QPushButton("",this);
    QPixmap pixmap2 = QPixmap(":/new/prefix1/images/t1.png");
    listb->setIcon(QIcon(pixmap2));
    ui->statusBar->addPermanentWidget(Titleb);
    ui->statusBar->addPermanentWidget(listb);
    connect(Titleb,&QPushButton::clicked, this, &MainWindow::title_View);
    connect(listb,&QPushButton::clicked, this, &MainWindow::list_View);
    connect(ui->NextButton,&QPushButton::clicked, this, &MainWindow::onNextClicked);
    connect(ui->PreviousButton,&QPushButton::clicked, this, &MainWindow::onPreviousClicked);
    connect(ui->actionCopy, &QAction::triggered, this, &MainWindow::CopyAction);
    connect(ui->actionCut, &QAction::triggered, this, &MainWindow::CutAction);
    connect(ui->actionPaste, &QAction::triggered, this, &MainWindow::PasteAction);

}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::title_View()
{
    ui->listView->setViewMode(QListView::IconMode);
    ui->actionList_view->setChecked(false);
    ui->listView->setFlow(QListView::Flow::LeftToRight);
    ui->listView->setWrapping(true);


}
void MainWindow::list_View()
{
    ui->listView->setViewMode(QListView::ListMode);
    ui->actionTitle_view->setChecked(false);
   ui->listView->setFlow(QListView::Flow::TopToBottom);
 ui->listView->setWrapping(false);

}
void MainWindow::Restoreview(){
    if(ui->action_Restore_last_view->isChecked()==false)
    {
        settings->setValue("Check","No");
    }
    else
    {
        settings->setValue("Check","Yes");
    }
}
void MainWindow::onItemClicked(const QModelIndex &index)
{

    QString filepath= model->filePath(index);
    settings->setValue("LastFolder",filepath);
    ui->AddresslineEdit->setText(filepath);
    addresses.erase(addresses.begin()+currentindex+1,addresses.end());
    addresses.append(ui->AddresslineEdit->text());
    currentindex=addresses.size()-1;
    QModelIndex rootindex= ListModel->setRootPath(filepath);
    ui->listView->setModel(ListModel);
    ui->listView->setRootIndex(rootindex);
    QDir directory(filepath);
    QStringList fileList = directory.entryList(QDir::Files);
    QStringList folderList = directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    Mylbl->setText("   "+QString::number(folderList.count())+" Folder | "+QString::number(fileList.count())+" File ");
    ui->statusBar->addWidget(Mylbl);
}
void MainWindow::onNextClicked()
{
    if(currentindex<addresses.size()-1)
    {
        //         addresses.append(ui->AddresslineEdit->text());
        currentindex++;
        ui->AddresslineEdit->setText(addresses.at(currentindex));
        QModelIndex rootindex= ListModel->setRootPath(addresses.at(currentindex));
        ui->listView->setRootIndex(rootindex);


    }
}
void MainWindow::onPreviousClicked()
{
    if(currentindex>0)
    {
        currentindex--;

        ui->AddresslineEdit->setText(addresses.at(currentindex));
        QModelIndex rootindex= ListModel->setRootPath(addresses.at(currentindex));
        ui->listView->setRootIndex(rootindex);
        //    currentindex=addresses.size()-1;
        //    currentindex++;
        //    ui->AddresslineEdit->setText(addresses.at(currentindex));
        //    QModelIndex rootindex= ListModel->setRootPath(addresses.at(currentindex));
        //    ui->listView->setRootIndex(rootindex);
        //    qDebug()<<currentindex;

    }
}
void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    QString filepath= ListModel->filePath(index);
    ui->AddresslineEdit->setText(filepath);
    addresses.erase(addresses.begin()+currentindex+1,addresses.end());
    addresses.append(ui->AddresslineEdit->text());
    currentindex=addresses.size()-1;
    if(model->isDir(index))
    {
        ui->listView->setRootIndex(index);
        settings->setValue("LastFolder",filepath);
    }
    else
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(filepath));

    }
    QDir directory(filepath);
    QStringList fileList = directory.entryList(QDir::Files);
    QStringList folderList = directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    Mylbl->setText("   "+QString::number(folderList.count())+" Folder | "+QString::number(fileList.count())+" File ");
    ui->statusBar->addWidget(Mylbl);
}
void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    QString Folderpath;
    int CountOfFolder=0;
    int CountOfFile=0;
    qint64 Filesize=0;
    QModelIndexList selectedindexes=ui->listView->selectionModel()->selectedIndexes();
    if(selectedindexes.count()==0)
    {
        ui->actionCopy->setEnabled(false);
        ui->actionCut->setEnabled(false);
    }
    else
    {

        ui->actionCopy->setEnabled(true);
        ui->actionCut->setEnabled(true);
    }
    if(selectedindexes.count()>1)
    {

        for (int i = 0; i < selectedindexes.size(); ++i) {
            if(ListModel->isDir(selectedindexes.at(i)))
            {
                CountOfFolder++;
            }
            else
            {
                CountOfFile++;
                Folderpath= ListModel->fileInfo(selectedindexes.at(i)).absoluteFilePath();
                QFileInfo fileinfo1(Folderpath);
                Filesize+=fileinfo1.size();
            }
        }
        if(CountOfFolder>0)
        {
            Mylbl->setText("   "+QString::number(CountOfFolder)+" Folder | "+QString::number(CountOfFile)+" File ");
            ui->statusBar->addWidget(Mylbl);
        }
        else
        {
            Mylbl->setText("  " +QString::number(static_cast<double>(Filesize)/(1024*1024))+"  MB ");
            ui->statusBar->addWidget(Mylbl);

        }
    }
    else  if(selectedindexes.count()==1)
    {
        if(ListModel->isDir(selectedindexes.at(0)))
        {
            Folderpath=ListModel->filePath(selectedindexes.at(0));
            QDir directory(Folderpath);
            QStringList fileList = directory.entryList(QDir::Files);
            QStringList folderList = directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
            Mylbl->setText("   "+QString::number(folderList.count())+" Folder | "+QString::number(fileList.count())+" File ");
        ui->statusBar->addWidget(Mylbl);
        }
        else
        {
            Folderpath= ListModel->fileInfo(selectedindexes.at(0)).absoluteFilePath();
            QFileInfo fileinfo1(Folderpath);
            Filesize=fileinfo1.size();
            Mylbl->setText("  " +QString::number(static_cast<float>(Filesize)/1024)+"  KB ");
            ui->statusBar->addWidget(Mylbl);
        }
    }
    else
    {

    }
}
void MainWindow::on_serchButton_clicked()
{
    QString filepath= ui->AddresslineEdit->text();
    QModelIndex rootindex= ListModel->setRootPath(filepath);
    QFileInfo fileinfo(filepath);
    // ui->AddresslineEdit->setText(filepath);
    if(fileinfo.isDir())
    {
        ui->listView->setRootIndex(rootindex);
        settings->setValue("LastFolder",filepath);
    }
    else if(fileinfo.isFile())
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(filepath));
        //qDebug()<<filepath;
    }
    else
    {

    }
    QDir directory(filepath);
    QStringList fileList = directory.entryList(QDir::Files);
    QStringList folderList = directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    Mylbl->setText("   "+QString::number(folderList.count())+" Folder | "+QString::number(fileList.count())+" File ");
    addresses.erase(addresses.begin()+currentindex+1,addresses.end());
    addresses.append(ui->AddresslineEdit->text());
    currentindex=addresses.size()-1;

}
void MainWindow::onExitClicked()
{
    this->close();
}
void MainWindow::on_listView_customContextMenuRequested(const QPoint &pos)
{

    QModelIndexList selectedindexes=ui->listView->selectionModel()->selectedIndexes();
    QMenu myMenu;
    myMenu.addAction("Open");
    myMenu.addAction("Copy");
    myMenu.addAction("Cut");
    myMenu.addAction("Paste");
    if(selectedindexes.size()==0)
    {
        myMenu.actions().at(0)->setEnabled(false);
         myMenu.actions().at(1)->setEnabled(false);
          myMenu.actions().at(2)->setEnabled(false);
          ui->actionCopy->setEnabled(false);
          ui->actionCut->setEnabled(false);

    }

    QAction* selectedItem = myMenu.exec(ui->listView->mapToGlobal(pos));
    if(selectedItem == nullptr)
    {
        return;
    }


    if(selectedItem->text()=="Copy")
    {

        for(int i=0;i<selectedindexes.size();i++)
        {
            CopyAddresses.append(ListModel->filePath(selectedindexes.at(i)));
        }

        FlagCopyorCut=true;

    }
    if(selectedItem->text()=="Cut")
    {

        for(int i=0;i<selectedindexes.size();i++)
        {
            CopyAddresses.append(ListModel->filePath(selectedindexes.at(i)));
        }
        FlagCopyorCut=false;

    }



    if(selectedItem->text()=="Open")
    {
         if(selectedindexes.count()!=0)
         {
              on_listView_doubleClicked(selectedindexes.at(0));
         }
    }


    if(selectedItem->text()=="Paste")
    {
        if(CopyAddresses.size()!=0)
        {

            for(int i=0;i<CopyAddresses.size();i++)
            {
                QFileInfo f(CopyAddresses.at(i));
                if(f.isDir())
                {
                    countFiles(CopyAddresses.at(i));
                    countSize(CopyAddresses.at(i));
                }
                else
                {
                    totalSize1 +=f.size();
                    fileCount++;
                }
            }
            Detail *D=new Detail();
            D->setTotalVlues(fileCount,totalSize1);
            D->show();
            QString destination=ui->AddresslineEdit->text();
            D->CopyAndCut(CopyAddresses,destination,FlagCopyorCut,fileCount,totalSize1);
            CopyAddresses.clear();
            fileCount = 0;
            totalSize1 = 0;;
        }
    }

}
void MainWindow::countFiles(const QString& folderPath)
{
    QDir dir(folderPath);


    foreach (const QFileInfo& fileInfo, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs))
    {
        if (fileInfo.isFile())
        {

            fileCount++;
        }
        else if (fileInfo.isDir())
        {
             countFiles(fileInfo.filePath());
               fileCount++;
        }
    }

}
void MainWindow:: countSize(const QString& folderPath)
{
    QDir dir(folderPath);



    foreach (const QFileInfo& fileInfo, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs))
    {
        if (fileInfo.isFile())
        {
              totalSize1 += fileInfo.size();

        }
        else if (fileInfo.isDir())
        {
            countSize(fileInfo.filePath());
        }
    }


}
void MainWindow::CopyAction()
{
     QModelIndexList selectedindexes=ui->listView->selectionModel()->selectedIndexes();
     for(int i=0;i<selectedindexes.size();i++)
     {
         CopyAddresses.append(ListModel->filePath(selectedindexes.at(i)));
     }

     FlagCopyorCut=true;


}
void MainWindow::CutAction()
{
     QModelIndexList selectedindexes=ui->listView->selectionModel()->selectedIndexes();
     for(int i=0;i<selectedindexes.size();i++)
     {
         CopyAddresses.append(ListModel->filePath(selectedindexes.at(i)));
     }
     FlagCopyorCut=false;
}
void MainWindow::PasteAction()
{
   if(CopyAddresses.size()!=0)
   {
       for(int i=0;i<CopyAddresses.size();i++)
       {
           QFileInfo f(CopyAddresses.at(i));
           if(f.isDir())
           {
               countFiles(CopyAddresses.at(i));
               countSize(CopyAddresses.at(i));
           }
           else
           {
               totalSize1 +=f.size();
               fileCount++;
           }
       }
       Detail *D=new Detail();
       D->setTotalVlues(fileCount,totalSize1);
       D->show();
       QString destination=ui->AddresslineEdit->text();
       D->CopyAndCut(CopyAddresses,destination,FlagCopyorCut,fileCount,totalSize1);
       CopyAddresses.clear();
       fileCount = 0;
       totalSize1 = 0;
   }
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        onItemClicked(ui->FilestreeView->currentIndex());

    }

}
