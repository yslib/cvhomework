#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QVector>
#include <QStringList>
#include <QString>
#include <QFileInfo>
#include <QMessageBox>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_videoOpenBtn_clicked();

    void on_imagesOpenBtn_clicked();

    void on_processBtn_clicked();

    void on_outputPathBtn_clicked();

    void on_closeBtn_clicked();

private:
    Ui::MainWindow *ui;
    QStringList m_imageNames;
    QString m_videoName;
    QString m_outputName;
};

#endif // MAINWINDOW_H
