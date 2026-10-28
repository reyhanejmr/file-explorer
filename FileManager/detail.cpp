#include "detail.h"
#include "ui_detail.h"
#include "QDebug"
#include "QThread"
#include "QFile"
#include "QMessageBox"
#include "QTimer"
#include "QElapsedTimer"
#include "QDir"
#include "QThread"
#include "mythread.h"
Detail::Detail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Detail)
{
    ui->setupUi(this);
}

Detail::~Detail()
{
    delete ui;
}


void Detail::CopyAndCut(QList<QString> SourcePath,QString DestinationPath,bool flagCopyOrCut,int Totalitem,qint64 TotalSize)
{
    MyThread *thread = new MyThread(SourcePath,DestinationPath,flagCopyOrCut, Totalitem, TotalSize);
    connect(thread,&MyThread::MSG,this,&Detail::MSG);
    connect(thread,&MyThread::cls,this,&Detail::cls);
    connect(thread,&MyThread::MSG2,this,&Detail::MSG2);
    connect(this,&Detail::ret,thread,&MyThread::ret);
    connect(thread,&MyThread::sendData,this,&Detail::setValue);
    thread->start();

}

void Detail::setValue(int p, qint64 size,QString reitem,qint64 elapsedTime,qint64 remainingTime)
{
    ui->progressBar->setValue(p);

    ui->ItemRmind->setText(reitem+" items");
    ui->remainsize->setText(QString::number(static_cast<double>(size)/(1024*1024))+"  MB ");
    ui->Timetotal->setText(QString::number(elapsedTime/1000) +" s");
    if((remainingTime/1000)<60)
    {
    ui->RemainTime->setText("About "+QString::number(remainingTime/1000) +" second");
    }
    else
    {
        int a,b;
        a=(remainingTime/1000)/60;
        b=(remainingTime/1000)%60;
       ui->RemainTime->setText("About "+QString::number(a) +" minute "+QString::number(b))+" second ";

    }
    if(p==100)
    {
        this->close();
    }
}
void Detail::setTotalVlues(int item, qint64 size)
{
     ui->TotalSize->setText(QString::number(static_cast<double>(size)/(1024*1024))+"  MB ");
     ui->TotalItem->setText(QString::number(item)+" items");
}

void Detail::MSG()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Remove Item");
    msgBox.setText("This file is exist, do you want to replace it?");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    QPixmap pixmap = QPixmap(":/new/prefix1/images/mmss.png");
    msgBox.setWindowIcon(QIcon(pixmap));
    if(msgBox.exec() == QMessageBox::Yes)
    {
        emit ret(true);
    }
    else
    {
        emit ret(false);
        this->close();
    }
}

void Detail::cls()
{
    this->close();
}

void Detail::MSG2()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Remove Item");
    msgBox.setText("The destination folder is a subfolder of the source folder");
    msgBox.setStandardButtons(QMessageBox::Ok);
    QPixmap pixmap = QPixmap(":/new/prefix1/images/mmss.png");
      msgBox.setWindowIcon(QIcon(pixmap));
    if(msgBox.exec() ==QMessageBox::Ok )
    {
        this->close();
     }
}



































//void Detail::Copyfile(QString sourceFilePath,QString destinationFilePath)
//{

//    QFile sourceFile(sourceFilePath);
//    QFile destinationFile(destinationFilePath);
//    if(QFile::exists(destinationFilePath))
//    {
//        QMessageBox msgBox;
//        msgBox.setWindowTitle("Remove Item");
//        msgBox.setText("This file is exist, do you want to replace it?");
//        msgBox.setStandardButtons(QMessageBox::Yes);
//        msgBox.addButton(QMessageBox::No);
//        QPixmap pixmap = QPixmap(":/new/prefix1/images/t2.png");
//        msgBox.setWindowIcon(QIcon(pixmap));
//        if(msgBox.exec() == QMessageBox::Yes)
//        {

//            QElapsedTimer* timer = new QElapsedTimer();

//            if (!sourceFile.open(QIODevice::ReadOnly))
//            {
//                return;
//            }
//            if (!destinationFile.open(QIODevice::WriteOnly))
//            {
//                return;
//            }
//            qint64 fileSize = sourceFile.size();
//            qint64 bytesCopied = 0;
//            char buffer[250000];
//            timer->start();
//            while (!sourceFile.atEnd())
//            {
//                qint64 bytesRead = sourceFile.read(buffer, sizeof(buffer));
//                destinationFile.write(buffer, bytesRead);
//                bytesCopied += bytesRead;
//                int progress = static_cast<int>((bytesCopied * 100) / fileSize);
//                qint64 elapsedTime = timer->elapsed();
//                qint64 remainingTime = (elapsedTime * fileSize / bytesCopied) - elapsedTime;
//                //this->setValue(progress,sourceFile.size(),"1 Item","1 Item",elapsedTime,remainingTime);
//                QApplication::processEvents();

//            }
//            sourceFile.close();
//            destinationFile.close();
//            this->close();


//        }
//    }
//    else
//    {
//        QElapsedTimer* timer = new QElapsedTimer();

//        if (!sourceFile.open(QIODevice::ReadOnly))
//        {
//            return;
//        }
//        if (!destinationFile.open(QIODevice::WriteOnly))
//        {
//            return;
//        }
//        qint64 fileSize = sourceFile.size();
//        qint64 bytesCopied = 0;
//        char buffer[4096];

//        timer->start();
//        while (!sourceFile.atEnd())
//        {
//            qint64 bytesRead = sourceFile.read(buffer, sizeof(buffer));
//            destinationFile.write(buffer, bytesRead);
//            bytesCopied += bytesRead;
//            int progress = static_cast<int>((bytesCopied * 100) / fileSize);
//            qint64 elapsedTime = timer->elapsed();
//            qint64 remainingTime = (elapsedTime * fileSize / bytesCopied) - elapsedTime;
//            //this->setValue(progress,sourceFile.size(),"1 File","1 File",elapsedTime,remainingTime);
//            QApplication::processEvents();

//        }
//        sourceFile.close();
//        destinationFile.close();
//        this->close();
//    }


//}
//void Detail::Movefile(QString SourcePath,QString DestinationPath)
//{
//    if(QFile::exists(DestinationPath))
//    {

//            Copyfile(SourcePath, DestinationPath);
//            QFile file(SourcePath);
//            file.remove();

//    }
//    else
//    {
//          QFile::rename(SourcePath, DestinationPath);
//    }


//}
//void Detail::CopyMorefile(QString sourceFilePath,QString destinationFilePath)
//{


//    QFile sourceFile(sourceFilePath);
//    QFile destinationFile(destinationFilePath);

//    if (!sourceFile.open(QIODevice::ReadOnly))
//    {
//        return;
//    }
//    if (!destinationFile.open(QIODevice::WriteOnly))
//    {
//        return;
//    }

//    char buffer[4096];

//    while (!sourceFile.atEnd())
//    {
//        qint64 bytesRead = sourceFile.read(buffer, sizeof(buffer));
//        destinationFile.write(buffer, bytesRead);
//        TotalbytesCopied += bytesRead;
//        qint64 elapsedTime = timerFolder->elapsed();
//        qint64 remainingTime = (elapsedTime * Sizecount / TotalbytesCopied) - elapsedTime;
//        //this->setValue((static_cast<int>((TotalbytesCopied * 100) / Sizecount)),Sizecount,((" "+QString::number(Foldercount))+" Folder  "+(QString::number(Filecount))+" File"),((" "+QString::number(Foldercount-REFoldercount))+" Folder  "+(QString::number(Filecount-REFilecount))+" File"),elapsedTime,remainingTime);
//        QApplication::processEvents();
//    }
//    sourceFile.close();
//    destinationFile.close();

//}

//void Detail::MoveFolder(QString sourceFolderPath,QString destinationFolderPath)
//{
// CopyFolder(sourceFolderPath, destinationFolderPath);
//  QDir dir(sourceFolderPath);
//  dir.removeRecursively();
//}
