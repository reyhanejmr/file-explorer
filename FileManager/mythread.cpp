#include "mythread.h"
#include"QThread"
#include "QDebug"
#include "QElapsedTimer"
#include "detail.h"
MyThread::MyThread()
{

}


MyThread::~MyThread()
{

}

void MyThread::run()
{

    timer->start();
    for (int i=0;i<source.size();i++)
    {
        QFile fileee2(source.at(i));
        QFileInfo fileInfo(fileee2.fileName());
        QString filename2(fileInfo.fileName());
        if(QFile::exists(destination+"/"+filename2)==false)
        {
            if(isSubfolder(source.at(i),destination+"/"+filename2)==false)
            {
            if (flag)
            {


                QFileInfo sourceDir(source.at(i));
                if(sourceDir.isDir())
                {
                    QFile fileee(source.at(i));
                    QFileInfo fileInfo(fileee.fileName());
                    QString filename1(fileInfo.fileName());
                    QDir().mkpath(destination+"/"+filename1);
                    CopyFolder(source.at(i),(destination+"/"+filename1));

                }
                else
                {
                    CopyFile(source.at(i),destination);
                }


            }
            else
            {
                QFileInfo sourceDir(source.at(i));
                if(sourceDir.isDir())
                {
                    QFile fileee(source.at(i));
                    QFileInfo fileInfo(fileee.fileName());
                    QString filename1(fileInfo.fileName());
                    QDir().mkpath(destination+"/"+filename1);
                    CopyFolder(source.at(i),(destination+"/"+filename1));
                    QDir dir(source.at(i));
                    dir.removeRecursively();

                }
                else
                {
                    CopyFile(source.at(i),destination);
                    QFile file(source.at(i));
                    file.remove();
                }


            }
        }
            else
            {
                emit MSG2();
            }
        }
        else
        {

            if((destination+"/"+filename2)==source.at(i))
            {

               if(flag)
               {
                   QFileInfo sourceDir(source.at(i));
                   if(sourceDir.isDir())
                   {
//
//
                       QFile fileee(source.at(i));
                       QFileInfo fileInfo(fileee.fileName());
                      // QString filename1(fileInfo.fileName());
                        QString newFilePath= fileInfo.absolutePath() + "/" + fileInfo.fileName() + "-Copy" + fileInfo.completeSuffix();
                       int copyIndex = 1;
                       while (QFile::exists(newFilePath))
                       {
                           newFilePath = fileInfo.absolutePath() + "/" + fileInfo.fileName() + "-Copy(" + QString::number(copyIndex) + ")" ;
                           copyIndex++;
                       }
                       QDir().mkpath(newFilePath);
                       CopyFolder(source.at(i),newFilePath);

                   }
                   else
                   {
                       CopyFileexist(source.at(i),destination);
                   }
               }
               else
               {
                    emit sendData(100,0,QString::number(0),0,0);
               }

            }
            else
            {
                emit MSG();

                while (!valid)
                {

                    sleep(1);


                }
                valid=false;
                if(TF)
                {
                    if(flag)
                    {
                        QFileInfo sourceDir(source.at(i));
                        if(sourceDir.isDir())
                        {
                            QFile fileee(source.at(i));
                            QFileInfo fileInfo(fileee.fileName());
                            QString filename1(fileInfo.fileName());
                            QDir().mkpath(destination+"/"+filename1);
                            CopyFolder(source.at(i),(destination+"/"+filename1));

                        }
                        else
                        {
                            CopyFile(source.at(i),destination);
                        }
                    }
                    else
                    {
                        QFileInfo sourceDir(source.at(i));
                        if(sourceDir.isDir())
                        {
                            QFile fileee(source.at(i));
                            QFileInfo fileInfo(fileee.fileName());
                            QString filename1(fileInfo.fileName());
                            QDir().mkpath(destination+"/"+filename1);
                            CopyFolder(source.at(i),(destination+"/"+filename1));
                            QDir dir(source.at(i));
                            dir.removeRecursively();

                        }
                        else
                        {
                            CopyFile(source.at(i),destination);
                            QFile file(source.at(i));
                            file.remove();
                        }
                    }
                }




            }
        }
    }
emit cls();
}






















void MyThread::CopyFolder(QString sourceFolderPath,QString destinationFolderPath)
{

    QDir sourceDir(sourceFolderPath);
    QDir destinationDir(destinationFolderPath);
    if (!destinationDir.exists())
    {
        QDir().mkpath(destinationFolderPath);
    }
    QFileInfoList fileInfoList = sourceDir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    foreach (const QFileInfo& fileInfo, fileInfoList)
    {
        QString filePath = fileInfo.filePath();
        QString newFilePath = destinationDir.filePath(fileInfo.fileName());

        if (fileInfo.isDir())
        {
            CopyFolder(filePath, newFilePath);
             Totalitem--;
        }
        else
        {
             CopyFile(filePath,destinationFolderPath);

            }

       }


    }

void MyThread::CopyFile(QString source,QString destination )
{
    QFile sourceFile(source);

        QFile fileee(source);
        QFileInfo fileInfo(fileee.fileName());
        QString filename1(fileInfo.fileName());
        QFile destinationFile(destination+"/"+filename1);

        if (!sourceFile.open(QIODevice::ReadOnly))
        {
            return;
        }
        if (!destinationFile.open(QIODevice::WriteOnly))
        {
            return;
        }



        char buffer[4096];
        while (!sourceFile.atEnd())
        {

            qint64 bytesRead = sourceFile.read(buffer, sizeof(buffer));
            destinationFile.write(buffer, bytesRead);
            bytesCopied += bytesRead;
            int progress = static_cast<int>((bytesCopied * 100) / TotalSize);
            qint64 elapsedTime = timer->elapsed();
            qint64 remainingTime = (elapsedTime * TotalSize / bytesCopied) - elapsedTime;

            //QApplication::processEvents();
            emit sendData(progress,TotalSize-bytesCopied,QString::number(Totalitem),elapsedTime,remainingTime);

        }

        sourceFile.close();
        destinationFile.close();
        Totalitem--;
}
void MyThread::CopyFileexist(QString source,QString destination )
{
    QFile sourceFile(source);

    QFile fileee(source);
    QFileInfo fileInfo(fileee.fileName());
   // QString filename1(fileInfo.fileName());
     QString newFilePath= fileInfo.absolutePath() + "/" + fileInfo.baseName() + "-Copy." + fileInfo.completeSuffix();

    int copyIndex = 1;
    while (QFile::exists(newFilePath))
    {
        newFilePath = fileInfo.absolutePath() + "/" + fileInfo.baseName() + "-Copy(" + QString::number(copyIndex) + ")." + fileInfo.completeSuffix();
        copyIndex++;
    }
    QFile destinationFile(newFilePath);
        if (!sourceFile.open(QIODevice::ReadOnly))
        {
            return;
        }
        if (!destinationFile.open(QIODevice::WriteOnly))
        {
            return;
        }



        char buffer[4096];
        while (!sourceFile.atEnd())
        {

            qint64 bytesRead = sourceFile.read(buffer, sizeof(buffer));
            destinationFile.write(buffer, bytesRead);
            bytesCopied += bytesRead;
            int progress= static_cast<int>((bytesCopied * 100) / TotalSize);
            qint64 elapsedTime = timer->elapsed();
            qint64 remainingTime = (elapsedTime * TotalSize / bytesCopied) - elapsedTime;

            //QApplication::processEvents();
            emit sendData(progress,TotalSize-bytesCopied,QString::number(Totalitem),elapsedTime,remainingTime);

        }


        Totalitem--;
        sourceFile.close();
        destinationFile.close();
}
void MyThread::ret(bool b)
{
TF= b;
valid=true;
}

bool MyThread::isSubfolder(const QString& sourceFolder, const QString& destinationFolder)
{
    QDir sourceDir(sourceFolder);
    QDir destDir(destinationFolder);

    QString sourcePath = sourceDir.absolutePath();
    QString destPath = destDir.absolutePath();

    return destPath.contains(sourcePath);
}
