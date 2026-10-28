#ifndef DETAIL_H
#define DETAIL_H
#include "thread"
#include <QWidget>
#include "QElapsedTimer"
namespace Ui {
class Detail;
}

class Detail : public QWidget
{
    Q_OBJECT

public:
    explicit Detail(QWidget *parent = 0);
    ~Detail();
void setValue(int p,qint64 size,QString reitem,qint64 elapsedTime,qint64 remainingTime);
  void MSG();
  void cls();
    void MSG2();
void CopyAndCut(QList<QString> SourcePath,QString DestinationPath,bool flagCopyOrCut,int Totalitem,qint64 TotalSize);
void setTotalVlues(int item, qint64 size);

signals:
void ret(bool b);

//void Copyfile(QString sourceFilePath,QString destinationFilePath);
//void Movefile(QString SourcePath,QString DestinationPath);
//void CopyFolder(QString SourcePath,QString DestinationPath);
//void CopyMorefile(QString sourceFilePath,QString destinationFilePath);
//void MoveFolder(QString sourceFolderPath,QString destinationFolderPath);
private:
    Ui::Detail *ui;

    int Filecount=0;
    int Foldercount=0;
    int REFilecount=0;
    int REFoldercount=0;
    int Sizecount=0;
    qint64 TotalbytesCopied = 0;
    int progresstotal;
    QElapsedTimer* timerFolder = new QElapsedTimer();


};

#endif // DETAIL_H
