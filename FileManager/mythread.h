#ifndef MYTHREAD_H
#define MYTHREAD_H
#include "QThread"
#include "QFileInfo"
#include "QDir"
#include "QFile"
#include "QElapsedTimer"
class MyThread : public QThread
{
private:
public:
    QList<QString> source;
    bool flag;
    QString destination;
    int Totalitem;
    qint64 TotalSize;
    MyThread(QList<QString> sou,QString des,bool fl,int Titem,qint64 TSize):source(sou),destination(des),flag(fl),Totalitem(Titem),TotalSize(TSize){}
    void CopyFolder(QString sourceFolderPath,QString destinationFolderPath);
    void CopyFile(QString source,QString destination );
     void CopyFileexist(QString source,QString destination );
     bool isSubfolder(const QString& sourceFolder, const QString& destinationFolder);
    explicit MyThread();
    bool TF;
    bool valid=false;
    Q_OBJECT
    void run();
    QElapsedTimer* timer = new QElapsedTimer();
    qint64 bytesCopied = 0;
    ~MyThread();
signals:
    void sendData(int p, qint64 size,QString reitem,qint64 elapsedTime,qint64 remainingTime);
    void MSG();
    void cls();
    void MSG2();
public slots:
    void ret(bool b);
};


#endif // MYTHREAD_H
