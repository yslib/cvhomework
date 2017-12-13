#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGlobal>
#include <cassert>
#include <QTime>
#include <QDebug>
#include <chrono>
#include <random>

/*
 *Effect Style
*/

enum class SlideStyle{GaussianBlur,Gradient,Scan,Shrink,Mix,Prologue};

/*
 *  Parameters in this function depend on what the style is.
 *  If parameter style = SlideStyle::Scan or style == SlideStyle::Shrink ,
 *  this function requires image1 and image2. style == Prologue,no requirment.
 *  style == GaussianBlur or style == SlideStyle::Gradient,only image1 is required.
*/
void effect(cv::VideoWriter & writer,
            const cv::Mat & image1,
            const cv::Mat & image2,
            int frameWidth,
            int frameHeight,
            int frameCount,SlideStyle style){

    if(writer.isOpened() == false)
        return;
    assert(frameCount >0);

    cv::Mat image;
    int width=image1.cols;
    int height =image1.rows;
    if(style == SlideStyle::GaussianBlur){
        for(int i=0;i<frameCount;i++){
                cv::GaussianBlur(image1,image,cv::Size(25,25),0.1*i,0.1*i);
                writer<<image;
        }
    }else if(style == SlideStyle::Gradient){
        for(int i=0;i<frameCount;i++){
            double factor = static_cast<double>(frameCount-i)/static_cast<double>(frameCount);
            image = image1*factor;
            writer<<image;
        }
    }else if(style == SlideStyle::Scan){
        assert(image1.size() == image2.size());
        cv::Rect rect1(0,0,0,height);
        cv::Rect rect2(0,0,width,height);
        image1.copyTo(image);//Initialize a image by other image
        for(int i=0;i<frameCount;i++){
            double factor = static_cast<double>(i)/static_cast<double>(frameCount);
            rect1.width = width*factor;
            rect2.x = rect1.width+1;
            rect2.width = width-rect2.x;
            cv::Mat leftROIofImage = image(rect1);      //Only a pointer point to related data,not a deep copy
            cv::Mat rightROIofImage = image(rect2);
            cv::Mat leftImage1 = image1(rect1);
            cv::Mat rightImage2 = image2(rect2);
            leftImage1.copyTo(leftROIofImage);          //deep copy to the pointer points to, i.e. image
            rightImage2.copyTo(rightROIofImage);

            writer<<image;
        }
    }else if(style == SlideStyle::Shrink){
        assert(image1.size() == image2.size());

        for(int i=0;i<frameCount;i++){
            double factor =1- static_cast<double>(i)/static_cast<double>(frameCount);
            int widthROI = factor*width;
            int heightROI = factor*height;
            image2.copyTo(image);
            cv::Rect roi(width/2-widthROI/2,height/2-heightROI/2,widthROI,heightROI);
            cv::Mat imageROI = image(roi);
            cv::Mat image1ROI = image1(roi);
            image1ROI.copyTo(imageROI);
            writer<<image;
        }
    }else if(style == SlideStyle::Mix){
        image1.copyTo(image);
        for(int i=0;i<frameCount;i++){
            double alpha = static_cast<double>((frameCount-i))/static_cast<double>(frameCount);
            cv::addWeighted(image1,
                            alpha,
                            image2,
                            1.0-alpha,
                            0.0,
                            image);
            writer<<image;
        }
    }else if(style == SlideStyle::Prologue){
        QVector<std::string> captions;
        captions.push_back("DIRECTOR&EDITOR");
        captions.push_back("YSL");
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

        cv::Mat image(frameHeight,frameWidth,CV_8UC3,cv::Scalar(255,255,255));
        cv::Mat captionImage(frameHeight,frameWidth,CV_8UC3,cv::Scalar(255,255,255));

        //Font Settings
        int fontFace = cv::FONT_HERSHEY_DUPLEX;
        int thickness = 10;
        cv::Scalar fontColor = cv::Scalar(0,0,0);

        for(const std::string & caption:captions){
            for(int i=0;i<frameCount;i++){
                double fontScale = 5+0.1*std::exp(i*0.1);
                cv::Size textSize = cv::getTextSize(caption,fontFace,fontScale,thickness,nullptr);
                captionImage = cv::Scalar(255,255,255);
                //image = cv::Scalar(255,255,255);
                cv::putText(captionImage,caption,cv::Point(frameWidth/2-textSize.width/2,frameHeight/2+textSize.height/2),fontFace,fontScale,fontColor,thickness,CV_AA);
                double alpha = static_cast<double>((frameCount-i))/static_cast<double>(frameCount);
                cv::addWeighted(captionImage,
                                alpha,
                                image,
                                1.0-alpha,
                                0.0,
                                image);
                writer<<image;
            }
        }


//        for(int i=0;i<frameCount;i++){

//            cv::line(image,
//                     cv::Point(qrand()%width,qrand()%height),
//                     cv::Point(qrand()%width,qrand()%height),
//                     cv::Scalar(qrand()%256,qrand()%256,qrand()%256),
//                     qrand()%5
//                     );
//            cv::rectangle(image,
//                          cv::Rect(qrand()%width,qrand()%height,qrand()%width,qrand()%height),
//                          cv::Scalar(qrand()%256,qrand()%256,qrand()%256),
//                          qrand()%5
//                          );
//            cv::ellipse(image,
//                        cv::RotatedRect(cv::Point2f(qrand()%width,qrand()%height),cv::Size2f(qrand()%width,qrand()%height),qrand()),
//                        cv::Scalar(qrand()%256,qrand()%256,qrand()%256)
//                        );
//            writer<<image;
//        }

    }
}
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
   m_videoName = QFileDialog::getOpenFileName(this,
                                              QStringLiteral("Video Selection"),
                                              "/Users/Ysl/Downloads",
                                              tr("Video Files (*.avi *mp4)"));
   if(m_videoName == QString())
       return;
   ui->videoListCBox->clear();
   ui->videoListCBox->addItem(m_videoName);
}

void MainWindow::on_imagesOpenBtn_clicked()
{
     m_imageNames = QFileDialog::getOpenFileNames(this,
                                                  QStringLiteral("Images Selection"),
                                                  "/Users/Ysl/Downloads",
                                                  tr("Image Files (*.png *.jpg *.bmp)"));
     if(m_imageNames.empty() == true)
         return;
     ui->imageListCBox->clear();
     ui->imageListCBox->addItems(m_imageNames);
}

void MainWindow::on_processBtn_clicked()
{

    //Read a video
    cv::VideoCapture cap(m_videoName.toStdString());
    if(cap.isOpened() == false){
        QMessageBox::critical(this,QStringLiteral("Error"),
                              QStringLiteral("Video file does not exsit\n"),
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }
    //Get Video Info
    int frameCount = cap.get(CV_CAP_PROP_FRAME_COUNT);
    int frameWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    int frameHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    double fps = cap.get(CV_CAP_PROP_FPS);
    QString vinfo = tr("\tFrame Count:%1\n").arg(frameCount)
            +tr("\tFrame Width:%1\n").arg(frameWidth)
            +tr("\tFrame Height:%1\n").arg(frameHeight)
            +tr("\tFps:%1\n").arg(fps);

    ui->videoInfoTextEdit->clear();
    ui->videoInfoTextEdit->setText(vinfo);

    //
    //Read images
    QVector<cv::Mat> images;
    for(const QString & name:m_imageNames){
        cv::Mat image = cv::imread(name.toStdString(),-1);
        if(image.empty() == true){
            QMessageBox::warning(this,QStringLiteral("Warning"),
                                  tr("%1 can not be read.").arg(name),
                                  QMessageBox::Ok,
                                  QMessageBox::Ok);
        }
        images.push_back(image);
    }
    //Resize images
    for(cv::Mat & image:images){
        cv::resize(image,image,cv::Size(frameWidth,frameHeight));        //LINEAR INTERPOLATION
    }

    //Initialize somo effect styles
    QVector<SlideStyle> styles;
    styles.push_back(SlideStyle::GaussianBlur);
    styles.push_back(SlideStyle::Gradient);
    styles.push_back(SlideStyle::Mix);
    styles.push_back(SlideStyle::Scan);
    styles.push_back(SlideStyle::Shrink);

    QVector<SlideStyle> singleImageStyles;
    singleImageStyles.push_back(SlideStyle::GaussianBlur);
    singleImageStyles.push_back(SlideStyle::Gradient);

    int frames = 60;

    QString outputPath = ui->outputPathEdit->text();
    cv::VideoWriter writer(outputPath.toStdString(),
                           cv::VideoWriter::fourcc('D','I','V','X'),
                           fps,
                           cv::Size(frameWidth,frameHeight),
                           true);
    if(writer.isOpened() == false){
        QMessageBox::critical(this,QStringLiteral("Error"),
                              QStringLiteral("Can not save this video. Check the output path."),
                              QMessageBox::Ok,
                              QMessageBox::Ok);
        return;
    }

    //Add a prologue
    effect(writer,cv::Mat(),cv::Mat(),frameWidth,frameHeight,frames,SlideStyle::Prologue);


    assert(styles.empty() == false);
    assert(singleImageStyles.empty() == false);

    //Shuffle the styles and gurantee a different effect every time run the program
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(styles.begin(),styles.end(),std::default_random_engine(seed));
    std::shuffle(singleImageStyles.begin(),singleImageStyles.end(),std::default_random_engine(seed));

    //If there is only one image,single style is employed.
    if(images.size() <= 1){
        int styleCount = singleImageStyles.size();
        for(int i=0;i<images.size();i++){
            effect(writer,
                   images[i],
                   images[i],
                   0,
                   0,
                   frames,
                   singleImageStyles[i%styleCount]);
        }
    }else{
        int styleCount = styles.size();
        for(int i=0;i<images.size()-1;i++){
            effect(writer,
                   images[i],
                   images[i+1],
                    frameWidth,
                    frameHeight,
                    frames,
                    styles[i%styleCount]);
        }
        //There could be lost a image if the style is not a two-image transform
    }
    std::string caption = ui->captionEdit->text().toStdString();
    //Caption Properties Settings
    int fontFace = cv::FONT_HERSHEY_COMPLEX;
    double fontScale = 2;
    int thickness = 2;
    int baseLine;
    cv::Scalar fontColor = cv::Scalar(255,255,255);
    cv::Size textSize = cv::getTextSize(caption,fontFace,fontScale,thickness,&baseLine);
    //fast play 2x, just drop a frame
    while(true){
        cv::Mat frame1,frame2;
        cap>>frame1;
        if(frame1.empty()==true)
            break;
        cv::putText(frame1,caption,cv::Point(frameWidth-textSize.width,frameHeight),fontFace,fontScale,fontColor,thickness);
        writer<<frame1;
        cap>>frame2;
        if(frame2.empty() == true)
            break;
    }
    writer.release();
    cap.release();
}

void MainWindow::on_outputPathBtn_clicked()
{
    m_outputName = QFileDialog::getSaveFileName(this,QStringLiteral("Save"),"/Users/Ysl/Downloads",tr("Video (*.avi *.mp4)"));
    ui->outputPathEdit->clear();
    ui->outputPathEdit->setText(m_outputName);
}


void MainWindow::on_closeBtn_clicked()
{
    this->close();
}
