#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <math.h>
#include <string>
#include <fstream>
#include <iostream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    //tu je napevno nastavena ip. treba zmenit na to co ste si zadali do text boxu alebo nejaku inu pevnu. co bude spravna
    ipaddress="192.168.1.14";
  //  cap.open("http://192.168.1.11:8000/stream.mjpg");
    ui->setupUi(this);
    datacounter=0;
  //  timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(getNewFrame()));
    actIndex=-1;
    useCamera1=false;




    datacounter=0;


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(Qt::black);
    QPen pero;
    pero.setStyle(Qt::SolidLine);
    pero.setWidth(3);
    pero.setColor(Qt::green);
    QRect rect(20,120,700,500);
    rect= ui->frame->geometry();
    rect.translate(0,15);
    painter.drawRect(rect);

    if(useCamera1==true && actIndex>-1)
    {
        std::cout<<actIndex<<std::endl;
        QImage image = QImage((uchar*)frame[actIndex].data, frame[actIndex].cols, frame[actIndex].rows, frame[actIndex].step, QImage::Format_RGB888  );
        painter.drawImage(rect,image.rgbSwapped());
    }
    else
    {
        if(updateLaserPicture==1)
        {
            updateLaserPicture=0;

            painter.setPen(pero);
            //teraz tu kreslime random udaje... vykreslite to co treba... t.j. data z lidaru
         //   std::cout<<copyOfLaserData.numberOfScans<<std::endl;
            for(int k=0;k<copyOfLaserData.numberOfScans/*360*/;k++)
            {
                /*  int dist=rand()%500;
            int xp=rect.width()-(rect.width()/2+dist*2*sin((360.0-k)*3.14159/180.0))+rect.topLeft().x();
            int yp=rect.height()-(rect.height()/2+dist*2*cos((360.0-k)*3.14159/180.0))+rect.topLeft().y();*/
                int dist=copyOfLaserData.Data[k].scanDistance/20;
                int xp=rect.width()-(rect.width()/2+dist*2*sin((360.0-copyOfLaserData.Data[k].scanAngle)*3.14159/180.0))+rect.topLeft().x();
                int yp=rect.height()-(rect.height()/2+dist*2*cos((360.0-copyOfLaserData.Data[k].scanAngle)*3.14159/180.0))+rect.topLeft().y();
                if(rect.contains(xp,yp))
                    painter.drawEllipse(QPoint(xp, yp),2,2);
            }
        }
    }
}

void  MainWindow::setUiValues(double robotX,double robotY,double robotFi)
{
     ui->lineEdit_2->setText(QString::number(robotX));
     ui->lineEdit_3->setText(QString::number(robotY));
     ui->lineEdit_4->setText(QString::number(robotFi));
}



int MainWindow::processThisRobot(TKobukiData robotdata)
{


    if(forwardspeed==0 && rotationspeed!=0)
        robot.setRotationSpeed(rotationspeed);
    else if(forwardspeed!=0 && rotationspeed==0)
        robot.setTranslationSpeed(forwardspeed);
    else if((forwardspeed!=0 && rotationspeed!=0))
        robot.setArcSpeed(forwardspeed,forwardspeed/rotationspeed);
    else
        robot.setTranslationSpeed(0);



    //if(datacounter%5)
    //{

      //  emit uiValuesChanged(robotdata.EncoderLeft,11,12);
    //}
    //datacounter++;

    if(datacounter == 0){
        encoderLeftValue = robotdata.EncoderLeft;
        encoderRightValue =  robotdata.EncoderRight;
        iniPhi = robotdata.GyroAngle/100.0*PI*2.0/360.0;
    }
    datacounter++;
    if(robotdata.EncoderLeft<encoderLeftValue){
        if((encoderLeftValue - robotdata.EncoderLeft)>60000){
            encoderLeftOverflow++;
        }
    }
    if(robotdata.EncoderLeft>encoderLeftValue){
        if((robotdata.EncoderLeft-encoderLeftValue)>60000){
            encoderLeftOverflow--;
        }
    }

    if(robotdata.EncoderRight<encoderRightValue){
        if((encoderRightValue - robotdata.EncoderRight)>60000){
            encoderRightOverflow++;
        }
    }
    if(robotdata.EncoderRight>encoderRightValue){
        if((robotdata.EncoderRight-encoderRightValue)>60000){
            encoderRightOverflow--;
        }
    }


    lr = tickToMeter*(robotdata.EncoderRight-encoderRightValue+encoderRightOverflow*65536);
    ll = tickToMeter*(robotdata.EncoderLeft-encoderLeftValue+encoderLeftOverflow*65536);
    celkova = celkova + lr;
    encoderRightValue = robotdata.EncoderRight;
    encoderLeftValue = robotdata.EncoderLeft;

    encoderRightOverflow = 0;
    encoderLeftOverflow = 0;

    l = (lr+ll)/2;

    phi = (robotdata.GyroAngle/100.0*PI*2.0/360.0)-iniPhi;

    //if((lr-ll)==0){
    x = x + l*cos(phi)*1000;
    y = y + l*sin(phi)*1000;

    //int highX=x+1000, lowX=x-1000, highY=y+1000, lowY=y-1000;
    //cout<<highX<<endl<<lowX<<endl<<highY<<endl<<lowY<<endl;
    //if(/*x!=xOld || y!=yOld*/(datacounter%100==0 || datacounter<1)&&l>lOld+10){


            /*for(i1 = 0; i1 < 12000; i1 = i1+100){
                for(j1 = 0; j1 < 12000; j1 = j1+100){
                    mapX=i1/100;
                    mapY=j1/100;
                    //if(xo>lowX && xo<highX && yo>lowY && yo<highY){
                        if(xo>=i1 && xo<i1+100 && yo>=j1 && yo<j1+100){
                            map[mapX][mapY]=1;
                        }else{
                            map[mapX][mapY]=0;
                        }
                    //}
                }

            }
            for(int i2 = 0; i2 < 120; i2++){
                for(int j2 = 0; j2 < 120; j2++){
                    if(map[i2][j2]==1 && mapSafe[i2][j2]==1){
                        mapSafe[i2][j2]=2;
                    }else if(map[i2][j2]==0 && mapSafe[i2][j2]==2){
                        mapSafe[i2][j2]=1;
                    }else if(map[i2][j2]==0 && mapSafe[i2][j2]==1){
                        mapSafe[i2][j2]=0;
                    }else if(map[i2][j2]==0 && mapSafe[i2][j2]==0){
                        mapSafe[i2][j2]=0;
                    }else if(map[i2][j2]==1 && mapSafe[i2][j2]==0){
                        mapSafe[i2][j2]=1;
                    }else if(mapSafe[i2][j2]==1){
                        mapSafe[i2][j2]=1;
                    }else{
                        mapSafe[i2][j2]=0;
                    }
                    //map[i2][j2]=0;
                }
            }*/

    //}



// cize velkost mojho pola je 12000 a to rozdelim na 120 dielikov cize jeden dielik moze obsahovat 12000/ 120 suradnic
// cize for ciklus pojde od 0 po 12000 pre oba smary s krokom 12000/120
// hodnotu ktoru budem dostavat po iteracii budem prehanta *120/12000 aby som sa vedel posuvat hodnotami v poli



    //cout << datacounter << "\n";
    xOld=x;
    yOld=y;
    lOld=l;
    return 0;

}

int MainWindow::processThisLidar(LaserMeasurement laserData)
{


    memcpy( &copyOfLaserData,&laserData,sizeof(LaserMeasurement));
    //tu mozete robit s datami z lidaru.. napriklad najst prekazky, zapisat do mapy. naplanovat ako sa prekazke vyhnut.
    // ale nic vypoctovo narocne - to iste vlakno ktore cita data z lidaru
    updateLaserPicture=1;
    update();//tento prikaz prinuti prekreslit obrazovku.. zavola sa paintEvent funkcia

    for(int k = 0; k<copyOfLaserData.numberOfScans; k++){
        //printf("\nvosiel som do lidar procesu");
        alfa = (360-copyOfLaserData.Data[k].scanAngle)*PI*2.0/360;
        dist = copyOfLaserData.Data[k].scanDistance;
        xo = x + dist*cos(phi+alfa);
        yo = y + dist*sin(phi+alfa);

        //printf("\nphi=%f  x=%f  y=%f  alfa=%f  xo=%f  yo=%f",phi,x,y,alfa,xo,yo);
        // skusit zmenit rozsah na 240 kvoli lepsej presnosti pripadne nejako doriesit falosnu detekciu.
        if ((copyOfLaserData.Data[k].scanDistance > 150 && copyOfLaserData.Data[k].scanDistance < 640) || (copyOfLaserData.Data[k].scanDistance > 700 && copyOfLaserData.Data[k].scanDistance < 3000)){
            mapX=xo/100;
            mapY=yo/100;
            if(mapX>120||mapX<0){

            }
            if(mapY>120||mapY<0){

            }
            map[mapX][mapY]=1;
        }
    }

    return 0;

}


int MainWindow::processThisCamera(cv::Mat cameraData)
{
    cameraData.copyTo(frame[(actIndex+1)%3]);
    actIndex=(actIndex+1)%3;
    updateLaserPicture=1;
    return 0;
}
void MainWindow::on_pushButton_9_clicked() //start button
{

    forwardspeed=0;
    rotationspeed=0;
    //tu sa nastartuju vlakna ktore citaju data z lidaru a robota
      /*  laserthreadID=pthread_create(&laserthreadHandle,NULL,&laserUDPVlakno,(void *)this);
      robotthreadID=pthread_create(&robotthreadHandle,NULL,&robotUDPVlakno,(void *)this);*/
    connect(this,SIGNAL(uiValuesChanged(double,double,double)),this,SLOT(setUiValues(double,double,double)));

    robot.setLaserParameters("192.168.1.14",52999,5299,/*[](LaserMeasurement dat)->int{std::cout<<"som z lambdy callback"<<std::endl;return 0;}*/std::bind(&MainWindow::processThisLidar,this,std::placeholders::_1));
    robot.setRobotParameters("192.168.1.14",53000,5300,std::bind(&MainWindow::processThisRobot,this,std::placeholders::_1));
    robot.setCameraParameters("http://127.0.0.1:8000/stream.mjpg",std::bind(&MainWindow::processThisCamera,this,std::placeholders::_1));
    robot.robotStart();



    instance = QJoysticks::getInstance();

    /* Enable the virtual joystick */
  /*  instance->setVirtualJoystickRange(1);
    instance->setVirtualJoystickEnabled(true);
    instance->setVirtualJoystickAxisSensibility(0.7);*/
    //instance->
    connect(
        instance, &QJoysticks::axisChanged,
        [this]( const int js, const int axis, const qreal value) { if(/*js==0 &&*/ axis==1){forwardspeed=-value*300;}
            if(/*js==0 &&*/ axis==0){rotationspeed=-value*(3.14159/2.0);}}
    );
}

void MainWindow::on_pushButton_2_clicked() //forward
{
    /*printf("\nklikol som\n");
    for(int x1 = 0; x1 < 120; x1++){
        for(int y1 = 0; y1 < 120; y1++){
            //printf("som vo fore");
            if(map[x1][y1]==1){
                cout << "X";
            }else{
                cout << "0";
            }

        }
        cout << "\n";
    }*/



    //pohyb dopredu
    robot.setTranslationSpeed(500);
    /*std::vector<unsigned char> mess=robot.setTranslationSpeed(500);
    if (sendto(rob_s, (char*)mess.data(), sizeof(char)*mess.size(), 0, (struct sockaddr*) &rob_si_posli, rob_slen) == -1)
    {

    }*/
}

void MainWindow::on_pushButton_3_clicked() //back
{
    robot.setTranslationSpeed(-250);
  /*  std::vector<unsigned char> mess=robot.setTranslationSpeed(-250);
    if (sendto(rob_s, (char*)mess.data(), sizeof(char)*mess.size(), 0, (struct sockaddr*) &rob_si_posli, rob_slen) == -1)
    {

    }*/
}

void MainWindow::on_pushButton_6_clicked() //left
{
robot.setRotationSpeed(3.14159/2);
  /*  std::vector<unsigned char> mess=robot.setRotationSpeed(3.14159/2);
    if (sendto(rob_s, (char*)mess.data(), sizeof(char)*mess.size(), 0, (struct sockaddr*) &rob_si_posli, rob_slen) == -1)
    {

    }*/
}

void MainWindow::on_pushButton_5_clicked()//right
{
robot.setRotationSpeed(-3.14159/2);
   /* std::vector<unsigned char> mess=robot.setRotationSpeed(-3.14159/2);
    if (sendto(rob_s, (char*)mess.data(), sizeof(char)*mess.size(), 0, (struct sockaddr*) &rob_si_posli, rob_slen) == -1)
    {

    }*/
}

void MainWindow::on_pushButton_4_clicked() //stop
{
    robot.setTranslationSpeed(0);
  /*  std::vector<unsigned char> mess=robot.setTranslationSpeed(0);
    if (sendto(rob_s, (char*)mess.data(), sizeof(char)*mess.size(), 0, (struct sockaddr*) &rob_si_posli, rob_slen) == -1)
    {

    }*/
      //exception handling
      try {
        cout << "\nWriting  array contents to file...";
        //open file for writing
        ofstream fw("C:\\Users\\Patrik\\Desktop\\Files\\fei\\Ing\\1st class\\LS\\RMR\\Zadanie 3\\map.txt", std::ofstream::out);
        //check if file was successfully opened for writing
        fw << "mapa:\n";
        if (fw.is_open())
        {
          //store array contents to text file
            for(int i = 0; i < 120; i++){
                for(int j = 0; j < 120; j++){
                    if(map[i][j]==1){
                        fw << "x ";
                    }else{
                        fw << "  ";
                    }

                }
                fw << "\n";
            }


          fw.close();
        }
        else cout << "Problem with opening file";
      }
      catch (const char* msg) {
        cerr << msg << endl;
      }
      cout << "\nDone!";
}




void MainWindow::on_pushButton_clicked()
{
    if(useCamera1==true)
    {
        useCamera1=false;

        ui->pushButton->setText("use camera");
    }
    else
    {
        useCamera1=true;

        ui->pushButton->setText("use laser");
    }
}

void MainWindow::getNewFrame()
{

}
