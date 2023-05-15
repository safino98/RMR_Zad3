#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include<windows.h>
#include<iostream>
//#include<arpa/inet.h>
//#include<unistd.h>
//#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>
//#include "ckobuki.h"
//#include "rplidar.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgcodecs.hpp"
#include "robot.h"

#include <QJoysticks.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    bool useCamera1;
  //  cv::VideoCapture cap;

    int actIndex;
    //    cv::Mat frame[3];

    cv::Mat frame[3];
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int processThisLidar(LaserMeasurement laserData);

    int processThisRobot(TKobukiData robotdata);
    double xOld, yOld, lOld;
    uint8_t map[120][120];
    uint8_t mapSafe[120][120];
    int mapX, mapY, i1, j1;
    double xo, yo;
    double dist;
    double alfa;
    long double celkova = 0;
    long double d = 0.23;
    long double tickToMeter = 0.000085292090497737556558;
    double lr = 0.0;
    double ll = 0.0;
    double encoderLeftValue = 0.0;
    double encoderRightValue = 0.0;
    double x = 6000.0;
    double y = 6000.0;
    double phiOldValue = 0.0;
    double d_alfa = 0.0;
    double l = 0.0;
    double phi = 0.0;
    int encoderLeftOverflow = 0;
    int encoderRightOverflow = 0;
    double targetX = 0.0;
    double targetY = 0.0;
    boolean go = false;
    double iniEncRight = 0;
    double iniEncLeft = 0;
    double iniPhi = 0;

int processThisCamera(cv::Mat cameraData);

private slots:
    void on_pushButton_9_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();
    void getNewFrame();

private:
     JOYINFO joystickInfo;
    Ui::MainWindow *ui;
     void paintEvent(QPaintEvent *event);// Q_DECL_OVERRIDE;
     int updateLaserPicture;
     LaserMeasurement copyOfLaserData;
     std::string ipaddress;
     Robot robot;
     TKobukiData robotdata;
     int datacounter;
     QTimer *timer;

     QJoysticks *instance;

     double forwardspeed;//mm/s
     double rotationspeed;//omega/s
public slots:
     void setUiValues(double robotX,double robotY,double robotFi);
signals:
     void uiValuesChanged(double newrobotX,double newrobotY,double newrobotFi); ///toto nema telo


};

#endif // MAINWINDOW_H
