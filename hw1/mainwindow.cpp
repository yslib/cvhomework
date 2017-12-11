#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_videoOpenBtn_clicked()
{
   m_videoName = QFileDialog::getOpenFileName(this,QStringLiteral("Video Selection"),"",tr("Video Files (*.avi *.rmvb)"));
   if(m_videoName == QString())
       return;
   ui->videoListCBox->clear();
   ui->videoListCBox->addItem(m_videoName);
}

void MainWindow::on_imagesOpenBtn_clicked()
{
     m_imageNames = QFileDialog::getOpenFileNames(this,QStringLiteral("Images Selection"),"",tr("Image Files (*.png *.jpg *.bmp)"));
     if(m_imageNames.empty() == true)
         return;
     ui->imageListCBox->clear();
     ui->imageListCBox->addItems(m_imageNames);
}

void MainWindow::on_processBtn_clicked()
{
    cv::VideoCapture cap(m_videoName.toStdString());
    if(cap.isOpened() == false){
        QMessageBox::critical(this,QStringLiteral("Error"),QStringLiteral("Video file does not exsit\n"),QMessageBox::Ok,QMessageBox::Ok);
        return;
    }

    //Get Video Info
    int frameCount = cap.get(CV_CAP_PROP_FRAME_COUNT);
    int frameWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    int frameHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    double fps = cap.get(CV_CAP_PROP_FPS);

    QString vinfo = tr("\tFrame Count:%1\n").arg(frameCount)
            +tr("\tFrame Width:%1").arg(frameWidth)
            +tr("\tFrame Height:%1").arg(frameHeight)
            +tr("\tFps:%1\n").arg(fps);

    ui->videoInfoTextEdit->clear();
    ui->videoInfoTextEdit->setText(vinfo);


    QVector<cv::Mat> images;
    foreach(const QString & name,m_imageNames){
//        cv::Mat image = cv::imread(name.toStdString(),-1);
//        if(image.empty() == true){
//            QMessageBox::critical(this,QStringLiteral("Error"),tr("%1 can not be read").arg(name),QMessageBox::Ok,QMessageBox::Ok);
//        }
//        images.push_back(image);
    }



    //Resize Images
    QVector<cv::Mat> resizeImages;

}

void MainWindow::on_outputPathBtn_clicked()
{
    m_outputName = QFileDialog::getSaveFileName(this,QStringLiteral("Save"),"",tr("Video (*.avi *.rmvb)"));
    ui->outputPathEdit->clear();
    ui->outputPathEdit->setText(m_outputName);
}


void MainWindow::on_closeBtn_clicked()
{

}
