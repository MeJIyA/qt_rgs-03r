#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QThread"
#include "QMouseEvent"
#include "QApplication"
#include <QCoreApplication>
//#include "QKeyEvent"
//#include "QKeySequence"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
     ui->setupUi(this);
     tmr = new QTimer(this);
     tmr->setInterval(1000); // Задаємо інтервал таймера

     tmr2 = new QTimer(this);
     tmr2->setInterval(200);
   //
    connect(tmr, SIGNAL(timeout()), this, SLOT(test_sost())); // Подключаем сигнал таймера к нашему слоту
    connect(tmr2, SIGNAL(timeout()), this, SLOT(blink())); // Подключаем сигнал таймера к нашему слоту
    //    tmr->start(); // Запускаем таймер
    setup();
}
bool ftOpen =0,test=0;
FT_HANDLE ftHandle;
FT_STATUS ftStatus;

int line_nmber = 0;

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::mousePressEvent(QMouseEvent *event)  // Знаходження місця кліку мишки для того щоб збільшити попадання
{
  if (event->localPos().x()>45 && event->localPos().x()<130 && event->localPos().y()>135 && event->localPos().y()< 195)
  {
      ui->lineEdit_21->setFocus();
  }
  if (event->localPos().x()>45 && event->localPos().x()<130 && event->localPos().y()>205 && event->localPos().y()< 260)
  {
      ui->lineEdit_22->setFocus();
  }
  if (event->localPos().x()>45 && event->localPos().x()<130 && event->localPos().y()>270 && event->localPos().y()< 335)
  {
      ui->lineEdit_23->setFocus();
  }


qDebug() << "x" << event->localPos().x();

qDebug() << event;
}

void MainWindow::setup()
{
 ui->pushButton_90->setEnabled(0);
 ui->radioButton->setVisible(0);

// ui->label_3->setStyleSheet("background-color: #00BF00");
 ui->label_3->setStyleSheet("background-color: orange");
 ui->label_25->setStyleSheet("background-color: #00BF00");
 ui->label_26->setStyleSheet("background-color: #00BF00");
 ui->label_28->setStyleSheet("background-color: #00BF00");
 ui->label_27->setStyleSheet("background-color: orange");
 ui->label_39->setStyleSheet("background-color: #00BF00");
 ui->label_49->setStyleSheet("background-color: #00BF00");
 ui->label_45->setStyleSheet("background-color: red");
 ui->label_4->setStyleSheet("background-color: orange");
 ui->label_5->setStyleSheet("background-color: orange");
 ui->label_20->setStyleSheet("background-color: #00BF00");
 ui->label_85->setStyleSheet("background-color: orange");
 ui->label_86->setStyleSheet("background-color: orange");
 ui->label_87->setStyleSheet("background-color: orange");
 ui->label_88->setStyleSheet("background-color: orange");
 ui->label_89->setStyleSheet("background-color: orange");
 ui->label_90->setStyleSheet("background-color: orange");
 // Window 1
 ui->label_29->setStyleSheet("background-color: orange");
 ui->label_30->setStyleSheet("background-color: orange");
 ui->label_31->setStyleSheet("background-color: orange");
 ui->label_34->setStyleSheet("background-color: orange");
 ui->label_37->setStyleSheet("background-color: orange");
 ui->label_43->setStyleSheet("background-color: orange");

 ui->label_55->setStyleSheet("background-color: #00BF00");
// Info Zone
 ui->label_50->setStyleSheet("background-color: orange");
 ui->label_51->setStyleSheet("background-color: orange");
 ui->label_105->setStyleSheet("background-color: orange");
 ui->label_107->setStyleSheet("background-color: orange");
 ui->label_108->setStyleSheet("background-color: red");
 ui->label_106->setStyleSheet("background-color: #00BF00");

ui->pushButton_key_clear->setStyleSheet("background-color: red");
ui->pushButton_key_enter->setStyleSheet("background-color: #00BF00");
ui->pushButton_key_0->setStyleSheet("background-color: silver");
ui->pushButton_key_1->setStyleSheet("background-color: silver");
ui->pushButton_key_2->setStyleSheet("background-color: silver");
ui->pushButton_key_3->setStyleSheet("background-color: silver");
ui->pushButton_key_4->setStyleSheet("background-color: silver");
ui->pushButton_key_5->setStyleSheet("background-color: silver");
ui->pushButton_key_6->setStyleSheet("background-color: silver");
ui->pushButton_key_7->setStyleSheet("background-color: silver");
ui->pushButton_key_8->setStyleSheet("background-color: silver");
ui->pushButton_key_9->setStyleSheet("background-color: silver");
ui->groupBox_keyboard->setStyleSheet("font-size: 20px");

 //QPixmap
 //ui->label_50->setPixmap();
}

/*******************************************************************************************
                    ВИЧТКА ВСТАНОВЛЕНИХ РЕЖИМІВ
********************************************************************************************/
void MainWindow::read_mode()
{
    if (ftOpen)
    {
        QByteArray RxBuffer;
        BYTE TxBuffer[20];
        BYTE sost,mode,addr;
        TxBuffer[0] = 0x00;
        TxBuffer[1] = 0x20;
        TxBuffer[2] = 0x0b; // адрес слейва(синхронизатор)
        TxBuffer[3] = 0x0a; // ADDR младший байт
        TxBuffer[4] = 0x00; // ADDR старший байт
        TxBuffer[5] = 0x01; // кількість данних які відправляємо
        write(TxBuffer,6);  // записуємо буфер (буфер, к-сть даних (байт))
    //    if (flag()) qDebug() << "flag OK";
        QThread::msleep(10);
        TxBuffer[0] = 0x92; // задання секції таблиці
        write(TxBuffer,1);
       // if (flag()) qDebug() << "flag OK";
       QThread::msleep(10);
        TxBuffer[0] = 0xa0; // команда на виконання
        write(TxBuffer,1);
      //  if (flag()) qDebug() << "flag OK";
        QThread::msleep(10);
        TxBuffer[0] = 0x40;
        TxBuffer[1] = 0x20;
        TxBuffer[2] = 0x08;
        write(TxBuffer,3);
        QThread::msleep(50);
        RxBuffer = read();
        sost = RxBuffer[0]>>2;
        addr = RxBuffer[4];
        mode = RxBuffer[7];
        qDebug() << "MODE is" << mode;
        sost&=1;
        qDebug()<< sost;
        if (sost == 1 && addr == 0x0a) // статус байт ОК
        {
            switch (mode)
            {
            case 0:
              //  ui->comboBox_2->setCurrentIndex(0);
                ui->listWidget->setCurrentRow(0);
                ui->label_101->setText("Режим установлен");
                ui->label_101->setStyleSheet("background-color: #00BF00");
                break;
            case 1:
                //ui->comboBox_2->setCurrentIndex(1);
                ui->listWidget->setCurrentRow(1);
                break;
            case 2:
                //ui->comboBox_2->setCurrentIndex(2);
                ui->listWidget->setCurrentRow(2);
                break;
            case 3:
                //ui->comboBox_2->setCurrentIndex(3);
                ui->listWidget->setCurrentRow(3);
                break;

            }
         }
        else
        {
            ui->label_101->setText("ОШИБКА!");
            ui->label_101->setStyleSheet("background-color: red");
        }
    }
}

/*******************************************************************************************
*******************************************************************************************/



void MainWindow::on_pushButton_89_clicked()
{
    if (ui->pushButton_90->isEnabled()==0)
    {
        ui->pushButton_90->setEnabled(1);
        ui->pushButton_89->setEnabled(0);
    }

}

void MainWindow::on_pushButton_90_clicked()
{
    if (ui->pushButton_89->isEnabled()==0)
    {
        ui->pushButton_90->setEnabled(0);
        ui->pushButton_89->setEnabled(1);
    }
}

void MainWindow::conect_disconect()
{
    // FT_HANDLE ftHandle;
    //  FT_STATUS ftStatus;
      //UCHAR BitMode;
      UCHAR Mask = 0xff;
      UCHAR Mode = 0x40;  // FIFO mode

      if (!ftOpen)
      {
      ftStatus = FT_Open(0, &ftHandle);
        if (ftStatus==FT_OK){
            //ftOpen=1;
            ftStatus = FT_SetBitMode(ftHandle, Mask, Mode);
            if (ftStatus == FT_OK) {
            ftOpen=1;
            qDebug() << "set bit mode FIFO" ;

            ui->label_5->setStyleSheet("background-color: #00FF00");
            ui->label_85->setStyleSheet("background-color: #00FF00");
            ui->radioButton->setVisible(1);

            ui->pushButton->setText("Disconect");

            test_sost(); //. !!!!!!!!!!!!!!!!
            read_mode();


            tmr->start();
            }
            return;
           }
        else
        {
            ftOpen=0;
            ui->pushButton->setText("Connect");
        }
      }
      if (ftOpen)
      {
          FT_Close(ftHandle);
       //   tmr2->stop();
          ui->radioButton->setChecked(0);
          ftOpen =0;
          ui->pushButton->setText("Conect");
          qDebug()<< "disconected";
          setup();

          tmr->stop();
          tmr2->stop();
      }
}

QByteArray MainWindow::read() // Ф-ція вичитки даних з ФІФО
{
    //  bool ok;
      //FT_HANDLE ftHandle;
    //  FT_STATUS ftStatus;
      DWORD EventDWord;
      DWORD TxBytes;
      DWORD RxBytes;
      DWORD BytesReceived;
      QByteArray data;

      FT_GetStatus(ftHandle,&RxBytes,&TxBytes,&EventDWord);
    //  while (RxBytes < 1) {FT_GetStatus(ftHandle,&RxBytes,&TxBytes,&EventDWord);}

      if (RxBytes > 0)
      {
            char RxBuffer[RxBytes];
      //    QByteArray data[RxBytes];
          qDebug() << "RxBytes" << RxBytes;
          qDebug() << " RxBytes is availabel";
      ftStatus = FT_Read(ftHandle,RxBuffer,RxBytes,&BytesReceived);
      if (ftStatus == FT_OK) {
       qDebug() << "FT_Read OK" ;
       //ui->textEdit->insertPlainText("FT_read OK \n");
      data.clear();
      qDebug() << data.length();
      for (uint i= 0; i<RxBytes; i++)
       {
           data[i] = RxBuffer[i];
       }
      }
      else
      {
       qDebug() << "FT_Read Failed";
      }
      }

  return data;
}

bool MainWindow::write(BYTE TxBuffer[], int len)  // ф-ЦІЯ ЗАПИСУ В ПОРТ
{
   DWORD BytesWritten;

    if(ftOpen != 1) // FT_Open failed
    {
        qDebug() << "device NOT opening";
    }
    if (ftOpen == 1) {
    ftStatus = FT_Write(ftHandle, TxBuffer, len, &BytesWritten);
    if (ftStatus == FT_OK) // FT_Write OK
    {
      return true;
    }
    else {
    qDebug() << "FT_Write Failed";
    return false;
    }
}
   return true;
}

bool MainWindow::flag() // перевірка статусу готованості
{
    int count;
    QByteArray flag;
    BYTE TxBuffer[3],point;
    TxBuffer[0] = 0x41;
    TxBuffer[1] = 0x00;
    TxBuffer[2] = 0x01;
    while (point!= 1)
    {
    write(TxBuffer,3);
    QThread::msleep(10);
    flag = read();
    point = flag[0];
    count+=1;
    }
  //  flag = read();
    qDebug() << "count:" << count;
    return true;
}

bool MainWindow::test(BYTE address)
{
   // if (ui->radioButton->isChecked()) {ui->radioButton->setChecked(0);}
 //   else { ui->radioButton->setChecked(1);}

    BYTE sost;
    QByteArray RxBuffer;
    BYTE TxBuffer[255];
    TxBuffer[0] = 0x05;
    TxBuffer[1] = 0x50;
    TxBuffer[2] = address; // адрес слейва(устройства)
    TxBuffer[3] = 0x00;
    TxBuffer[4] = 0x00;
    TxBuffer[5] = 0x01; // кількість данних які відправляємо

    TxBuffer[6] = 0x00;  // дані для тесту
//    TxBuffer[7] = 0xbb;
//    TxBuffer[8] = 0xcc;
//    TxBuffer[9] = 0xaa;
    write(TxBuffer,7);
//    if (flag()) qDebug() << "flag OK";
    QThread::msleep(10);
    TxBuffer[0] = 0x95; // задання секції таблиці
    write(TxBuffer,1);
   // if (flag()) qDebug() << "flag OK";
   QThread::msleep(10);
    TxBuffer[0] = 0xa0; // команда на виконання
    write(TxBuffer,1);
  //  if (flag()) qDebug() << "flag OK";
    QThread::msleep(10);
    TxBuffer[0] = 0x40;
    TxBuffer[1] = 0x50;
    TxBuffer[2] = 0x01;
    write(TxBuffer,3);
    QThread::msleep(50);
    RxBuffer = read();
    sost = RxBuffer[0]>>2;
    sost&=1;
    qDebug()<< sost;
    if (sost == 1) // статус байт ОК
    {
     //  ui->label_89->setStyleSheet("background-color: #00FF00");
      return true;
    }
    else
    {
     //ui->label_87->setStyleSheet("background-color: orange");
       return false;
    }

// return false;

 }
bool MainWindow::test_can() // Перевірка чи є підключення по CAN
{
    BYTE sost;
    QByteArray RxBuffer;
    BYTE TxBuffer[255];
    TxBuffer[0] = 0xd1;  // переключення в режим CAN
    write(TxBuffer,1);
    QThread::msleep(10);

    TxBuffer[0] = 0x00;
    TxBuffer[1] = 0xa0;
    TxBuffer[2] = 0x01;
    TxBuffer[3] = 0x01;
    TxBuffer[4] = 0x30;
    write(TxBuffer,5);
    QThread::msleep(10);
    TxBuffer[0] = 0x9a; // задання секції таблиці
    write(TxBuffer,1);
    QThread::msleep(10);
    TxBuffer[0] = 0xa0; // команда на виконання
    write(TxBuffer,1);
  //  if (flag()) qDebug() << "flag OK";
    QThread::msleep(10);
    TxBuffer[0] = 0x40;
    TxBuffer[1] = 0xa0;
    TxBuffer[2] = 0x03;
    write(TxBuffer,3);
    QThread::msleep(50);
    RxBuffer = read();
    sost = RxBuffer[2];
    qDebug()<< sost;

    TxBuffer[0] = 0xd0;
    write(TxBuffer,1);
    QThread::msleep(10);
    TxBuffer[1] = 0xa0;
    write(TxBuffer,1);
    QThread::msleep(10);

    if (sost == 0x30) // возвращенная CMD = 30
    {
         return true;
    }
    else
    {
         return false;
    }
}

void MainWindow::test_sost()
{
tmr2->start();
    if (ftOpen)
    {


       if (test(0x03))
       {
         ui->label_89->setStyleSheet("background-color: #00FF00");
       }
       else
       {
         ui->label_89->setStyleSheet("background-color: orange");
       }
       if (test(0x16))
       {
         ui->label_86->setStyleSheet("background-color: #00FF00");
       }
       else
       {
         ui->label_86->setStyleSheet("background-color: orange");
       }
       if (test(0x0b))
       {
         ui->label_87->setStyleSheet("background-color: #00FF00");
       }
       else
       {
         ui->label_87->setStyleSheet("background-color: orange");
       }
       if (test(0x09))
       {
         ui->label_88->setStyleSheet("background-color: #00FF00");
       }
       else
       {
         ui->label_88->setStyleSheet("background-color: orange");
       }

       if (test_can())  //проверка CAN
       {
          ui->label_4->setStyleSheet("background-color: #00FF00");
       }
       else
       {
         ui->label_4->setStyleSheet("background-color: orange");
       }

  }
 //    ui->radioButton->setVisible(0);
  //  tmr2->stop();


}

void MainWindow::on_pushButton_clicked() // COnect/Disconect
{
    conect_disconect();
 //   test_sost();
//    read_mode();
}

void MainWindow::blink()
{
    if (ui->radioButton->isChecked()) {ui->radioButton->setChecked(0);}
    else { ui->radioButton->setChecked(1);}
}



// ******* ПЕРЕКЛЮЧЕННЯ РЕЖИМІВ РОБОТИ СИНХРОНІЗАТОРА*********

/*void MainWindow::on_comboBox_2_currentIndexChanged(int index)  // режим роботи синхронізатора
{
    QByteArray RxBuffer;
    BYTE TxBuffer[255];
    BYTE sost;
    if (ftOpen)
    {
    if (index == 0)
    {
        ui->lineEdit_185->setText("51.2");
        ui->lineEdit_186->setText("0.4");
        ui->lineEdit_187->setText("19.5");

        TxBuffer[0] = 0x05;
        TxBuffer[1] = 0x50;
        TxBuffer[2] = 0x0b; // адрес слейва(синхронизатор)
        TxBuffer[3] = 0x0a; // ADDR младший байт
        TxBuffer[4] = 0x00; // ADDR старший байт
        TxBuffer[5] = 0x01; // кількість данних які відправляємо
        TxBuffer[6] = 0x00;  // дані для режиму роботи

        write(TxBuffer,7);  // записуємо буфер (буфер, к-сть даних (байт))
    //    if (flag()) qDebug() << "flag OK";
        QThread::msleep(10);
        TxBuffer[0] = 0x95; // задання секції таблиці
        write(TxBuffer,1);
       // if (flag()) qDebug() << "flag OK";
       QThread::msleep(10);
        TxBuffer[0] = 0xa0; // команда на виконання
        write(TxBuffer,1);
      //  if (flag()) qDebug() << "flag OK";
        QThread::msleep(10);
        TxBuffer[0] = 0x40;
        TxBuffer[1] = 0x50;
        TxBuffer[2] = 0x01;
        write(TxBuffer,3);
        QThread::msleep(50);
        RxBuffer = read();
        sost = RxBuffer[0]>>2;
        sost&=1;
        qDebug()<< sost;
        if (sost == 1) // статус байт ОК
        {
         ui->label_101->setStyleSheet("background-color: #00BF00");

        }
        else
        {
            ui->label_101->setText("ОШИБКА!");
            ui->label_101->setStyleSheet("background-color: red");
            ui->lineEdit_185->setText("-");
            ui->lineEdit_186->setText("-");
            ui->lineEdit_187->setText("-");
        }
    }
    if (index == 1)
    {
        ui->lineEdit_185->setText("40.96");
        ui->lineEdit_186->setText("0.5");
        ui->lineEdit_187->setText("24.4");

        TxBuffer[0] = 0x05;
        TxBuffer[1] = 0x50;
        TxBuffer[2] = 0x0b; // адрес слейва(синхронизатор)
        TxBuffer[3] = 0x0a; // ADDR младший байт
        TxBuffer[4] = 0x00; // ADDR старший байт
        TxBuffer[5] = 0x01; // кількість данних які відправляємо
        TxBuffer[6] = 0x01;  // дані для режиму роботи

        write(TxBuffer,7);  // записуємо буфер (буфер, к-сть даних (байт))
    //    if (flag()) qDebug() << "flag OK";
        QThread::msleep(10);
        TxBuffer[0] = 0x95; // задання секції таблиці
        write(TxBuffer,1);
       // if (flag()) qDebug() << "flag OK";
       QThread::msleep(10);
        TxBuffer[0] = 0xa0; // команда на виконання
        write(TxBuffer,1);
      //  if (flag()) qDebug() << "flag OK";
        QThread::msleep(10);
        TxBuffer[0] = 0x40;
        TxBuffer[1] = 0x50;
        TxBuffer[2] = 0x01;
        write(TxBuffer,3);
        QThread::msleep(50);
        RxBuffer = read();
        sost = RxBuffer[0]>>2;
        sost&=1;
        qDebug()<< sost;
        if (sost == 1) // статус байт ОК
        {
         ui->label_101->setStyleSheet("background-color: #00BF00");
        }
        else
        {
            ui->label_101->setText("ОШИБКА!");
            ui->label_101->setStyleSheet("background-color: red");
            ui->lineEdit_185->setText("-");
            ui->lineEdit_186->setText("-");
            ui->lineEdit_187->setText("-");
        }
    }
    if (index == 2)
    {
        ui->lineEdit_185->setText("20.48");
        ui->lineEdit_186->setText("0.5");
        ui->lineEdit_187->setText("48.8");

        TxBuffer[0] = 0x05;
        TxBuffer[1] = 0x50;
        TxBuffer[2] = 0x0b; // адрес слейва(синхронизатор)
        TxBuffer[3] = 0x0a; // ADDR младший байт
        TxBuffer[4] = 0x00; // ADDR старший байт
        TxBuffer[5] = 0x01; // кількість данних які відправляємо
        TxBuffer[6] = 0x02;  // дані для режиму роботи

        write(TxBuffer,7);  // записуємо буфер (буфер, к-сть даних (байт))
    //    if (flag()) qDebug() << "flag OK";
        QThread::msleep(10);
        TxBuffer[0] = 0x95; // задання секції таблиці
        write(TxBuffer,1);
       // if (flag()) qDebug() << "flag OK";
       QThread::msleep(10);
        TxBuffer[0] = 0xa0; // команда на виконання
        write(TxBuffer,1);
      //  if (flag()) qDebug() << "flag OK";
        QThread::msleep(10);
        TxBuffer[0] = 0x40;
        TxBuffer[1] = 0x50;
        TxBuffer[2] = 0x01;
        write(TxBuffer,3);
        QThread::msleep(50);
        RxBuffer = read();
        sost = RxBuffer[0]>>2;
        sost&=1;
        qDebug()<< sost;
        if (sost == 1) // статус байт ОК
        {
         ui->label_101->setStyleSheet("background-color: #00BF00");
        }
        else
        {
            ui->label_101->setText("ОШИБКА!");
            ui->label_101->setStyleSheet("background-color: red");
            ui->lineEdit_185->setText("-");
            ui->lineEdit_186->setText("-");
            ui->lineEdit_187->setText("-");
        }
    }
    if (index == 3)
    {
        ui->lineEdit_185->setText("10.24");
        ui->lineEdit_186->setText("0.5");
        ui->lineEdit_187->setText("97.7");

        TxBuffer[0] = 0x05;
        TxBuffer[1] = 0x50;
        TxBuffer[2] = 0x0b; // адрес слейва(синхронизатор)
        TxBuffer[3] = 0x0a; // ADDR младший байт
        TxBuffer[4] = 0x00; // ADDR старший байт
        TxBuffer[5] = 0x01; // кількість данних які відправляємо
        TxBuffer[6] = 0x03;  // дані для режиму роботи

        write(TxBuffer,7);  // записуємо буфер (буфер, к-сть даних (байт))
    //    if (flag()) qDebug() << "flag OK";
        QThread::msleep(10);
        TxBuffer[0] = 0x95; // задання секції таблиці
        write(TxBuffer,1);
       // if (flag()) qDebug() << "flag OK";
       QThread::msleep(10);
        TxBuffer[0] = 0xa0; // команда на виконання
        write(TxBuffer,1);
      //  if (flag()) qDebug() << "flag OK";
        QThread::msleep(10);
        TxBuffer[0] = 0x40;
        TxBuffer[1] = 0x50;
        TxBuffer[2] = 0x01;
        write(TxBuffer,3);
        QThread::msleep(50);
        RxBuffer = read();
        sost = RxBuffer[0]>>2;
        sost&=1;
        qDebug()<< sost;
        if (sost == 1) // статус байт ОК
        {
           ui->label_101->setStyleSheet("background-color: #00BF00");
        }
        else
        {
            ui->label_101->setText("ОШИБКА!");
            ui->label_101->setStyleSheet("background-color: red");
        }
    }
    }
    ui->comboBox_2->setCurrentIndex(index);

} */

void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    QByteArray RxBuffer;
    BYTE TxBuffer[255];
    BYTE sost;
    if (ftOpen)
    {
    if (currentRow == 0)
    {
        ui->lineEdit_185->setText("51.2");
        ui->lineEdit_186->setText("0.4");
        ui->lineEdit_187->setText("19.5");

        TxBuffer[0] = 0x05;
        TxBuffer[1] = 0x50;
        TxBuffer[2] = 0x0b; // адрес слейва(синхронизатор)
        TxBuffer[3] = 0x0a; // ADDR младший байт
        TxBuffer[4] = 0x00; // ADDR старший байт
        TxBuffer[5] = 0x01; // кількість данних які відправляємо
        TxBuffer[6] = 0x00;  // дані для режиму роботи

        write(TxBuffer,7);  // записуємо буфер (буфер, к-сть даних (байт))
    //    if (flag()) qDebug() << "flag OK";
        QThread::msleep(10);
        TxBuffer[0] = 0x95; // задання секції таблиці
        write(TxBuffer,1);
       // if (flag()) qDebug() << "flag OK";
       QThread::msleep(10);
        TxBuffer[0] = 0xa0; // команда на виконання
        write(TxBuffer,1);
      //  if (flag()) qDebug() << "flag OK";
        QThread::msleep(10);
        TxBuffer[0] = 0x40;
        TxBuffer[1] = 0x50;
        TxBuffer[2] = 0x01;
        write(TxBuffer,3);
        QThread::msleep(50);
        RxBuffer = read();
        sost = RxBuffer[0]>>2;
        sost&=1;
        qDebug()<< sost;
        if (sost == 1) // статус байт ОК
        {
         ui->label_101->setStyleSheet("background-color: #00BF00");

        }
        else
        {
            ui->label_101->setText("ОШИБКА!");
            ui->label_101->setStyleSheet("background-color: red");
            ui->lineEdit_185->setText("-");
            ui->lineEdit_186->setText("-");
            ui->lineEdit_187->setText("-");
        }
    }
    if (currentRow == 1)
    {
        ui->lineEdit_185->setText("40.96");
        ui->lineEdit_186->setText("0.5");
        ui->lineEdit_187->setText("24.4");

        TxBuffer[0] = 0x05;
        TxBuffer[1] = 0x50;
        TxBuffer[2] = 0x0b; // адрес слейва(синхронизатор)
        TxBuffer[3] = 0x0a; // ADDR младший байт
        TxBuffer[4] = 0x00; // ADDR старший байт
        TxBuffer[5] = 0x01; // кількість данних які відправляємо
        TxBuffer[6] = 0x01;  // дані для режиму роботи

        write(TxBuffer,7);  // записуємо буфер (буфер, к-сть даних (байт))
    //    if (flag()) qDebug() << "flag OK";
        QThread::msleep(10);
        TxBuffer[0] = 0x95; // задання секції таблиці
        write(TxBuffer,1);
       // if (flag()) qDebug() << "flag OK";
       QThread::msleep(10);
        TxBuffer[0] = 0xa0; // команда на виконання
        write(TxBuffer,1);
      //  if (flag()) qDebug() << "flag OK";
        QThread::msleep(10);
        TxBuffer[0] = 0x40;
        TxBuffer[1] = 0x50;
        TxBuffer[2] = 0x01;
        write(TxBuffer,3);
        QThread::msleep(50);
        RxBuffer = read();
        sost = RxBuffer[0]>>2;
        sost&=1;
        qDebug()<< sost;
        if (sost == 1) // статус байт ОК
        {
         ui->label_101->setStyleSheet("background-color: #00BF00");
        }
        else
        {
            ui->label_101->setText("ОШИБКА!");
            ui->label_101->setStyleSheet("background-color: red");
            ui->lineEdit_185->setText("-");
            ui->lineEdit_186->setText("-");
            ui->lineEdit_187->setText("-");
        }
    }
    if (currentRow == 2)
    {
        ui->lineEdit_185->setText("20.48");
        ui->lineEdit_186->setText("0.5");
        ui->lineEdit_187->setText("48.8");

        TxBuffer[0] = 0x05;
        TxBuffer[1] = 0x50;
        TxBuffer[2] = 0x0b; // адрес слейва(синхронизатор)
        TxBuffer[3] = 0x0a; // ADDR младший байт
        TxBuffer[4] = 0x00; // ADDR старший байт
        TxBuffer[5] = 0x01; // кількість данних які відправляємо
        TxBuffer[6] = 0x02;  // дані для режиму роботи

        write(TxBuffer,7);  // записуємо буфер (буфер, к-сть даних (байт))
    //    if (flag()) qDebug() << "flag OK";
        QThread::msleep(10);
        TxBuffer[0] = 0x95; // задання секції таблиці
        write(TxBuffer,1);
       // if (flag()) qDebug() << "flag OK";
       QThread::msleep(10);
        TxBuffer[0] = 0xa0; // команда на виконання
        write(TxBuffer,1);
      //  if (flag()) qDebug() << "flag OK";
        QThread::msleep(10);
        TxBuffer[0] = 0x40;
        TxBuffer[1] = 0x50;
        TxBuffer[2] = 0x01;
        write(TxBuffer,3);
        QThread::msleep(50);
        RxBuffer = read();
        sost = RxBuffer[0]>>2;
        sost&=1;
        qDebug()<< sost;
        if (sost == 1) // статус байт ОК
        {
         ui->label_101->setStyleSheet("background-color: #00BF00");
        }
        else
        {
            ui->label_101->setText("ОШИБКА!");
            ui->label_101->setStyleSheet("background-color: red");
            ui->lineEdit_185->setText("-");
            ui->lineEdit_186->setText("-");
            ui->lineEdit_187->setText("-");
        }
    }
    if (currentRow == 3)
    {
        ui->lineEdit_185->setText("10.24");
        ui->lineEdit_186->setText("0.5");
        ui->lineEdit_187->setText("97.7");

        TxBuffer[0] = 0x05;
        TxBuffer[1] = 0x50;
        TxBuffer[2] = 0x0b; // адрес слейва(синхронизатор)
        TxBuffer[3] = 0x0a; // ADDR младший байт
        TxBuffer[4] = 0x00; // ADDR старший байт
        TxBuffer[5] = 0x01; // кількість данних які відправляємо
        TxBuffer[6] = 0x03;  // дані для режиму роботи

        write(TxBuffer,7);  // записуємо буфер (буфер, к-сть даних (байт))
    //    if (flag()) qDebug() << "flag OK";
        QThread::msleep(10);
        TxBuffer[0] = 0x95; // задання секції таблиці
        write(TxBuffer,1);
       // if (flag()) qDebug() << "flag OK";
       QThread::msleep(10);
        TxBuffer[0] = 0xa0; // команда на виконання
        write(TxBuffer,1);
      //  if (flag()) qDebug() << "flag OK";
        QThread::msleep(10);
        TxBuffer[0] = 0x40;
        TxBuffer[1] = 0x50;
        TxBuffer[2] = 0x01;
        write(TxBuffer,3);
        QThread::msleep(50);
        RxBuffer = read();
        sost = RxBuffer[0]>>2;
        sost&=1;
        qDebug()<< sost;
        if (sost == 1) // статус байт ОК
        {
           ui->label_101->setStyleSheet("background-color: #00BF00");
        }
        else
        {
            ui->label_101->setText("ОШИБКА!");
            ui->label_101->setStyleSheet("background-color: red");
        }
    }
    }

}

//************************************************************



void MainWindow::on_pushButton_key_1_clicked()
{
    QCoreApplication::sendEvent(QApplication::focusWidget(), new QKeyEvent(QEvent::KeyPress, Qt::Key_1, Qt::NoModifier, "1", true, 1));
    QCoreApplication::sendEvent(QApplication::focusWidget(), new QKeyEvent(QEvent::KeyRelease, Qt::Key_1, Qt::NoModifier, "1", true, 1));
}
void MainWindow::on_pushButton_key_2_clicked()
{
    QCoreApplication::sendEvent(QApplication::focusWidget(), new QKeyEvent(QEvent::KeyPress, Qt::Key_2, Qt::NoModifier, "2", true, 1));
    QCoreApplication::sendEvent(QApplication::focusWidget(), new QKeyEvent(QEvent::KeyRelease, Qt::Key_2, Qt::NoModifier, "2", true, 1));
}
void MainWindow::on_pushButton_key_3_clicked()
{
    QCoreApplication::sendEvent(QApplication::focusWidget(), new QKeyEvent(QEvent::KeyPress, Qt::Key_3, Qt::NoModifier, "3", true, 1));
    QCoreApplication::sendEvent(QApplication::focusWidget(), new QKeyEvent(QEvent::KeyRelease, Qt::Key_3, Qt::NoModifier, "3", true, 1));
}
void MainWindow::on_pushButton_key_4_clicked()
{
    QCoreApplication::sendEvent(QApplication::focusWidget(), new QKeyEvent(QEvent::KeyPress, Qt::Key_4, Qt::NoModifier, "4", true, 1));
    QCoreApplication::sendEvent(QApplication::focusWidget(), new QKeyEvent(QEvent::KeyRelease, Qt::Key_4, Qt::NoModifier, "4", true, 1));
}
void MainWindow::on_pushButton_key_5_clicked()
{
    QCoreApplication::sendEvent(QApplication::focusWidget(), new QKeyEvent(QEvent::KeyPress, Qt::Key_5, Qt::NoModifier, "5", true, 1));
    QCoreApplication::sendEvent(QApplication::focusWidget(), new QKeyEvent(QEvent::KeyRelease, Qt::Key_5, Qt::NoModifier, "5", true, 1));
}
void MainWindow::on_pushButton_key_6_clicked()
{
    QCoreApplication::sendEvent(QApplication::focusWidget(), new QKeyEvent(QEvent::KeyPress, Qt::Key_6, Qt::NoModifier, "6", true, 1));
    QCoreApplication::sendEvent(QApplication::focusWidget(), new QKeyEvent(QEvent::KeyRelease, Qt::Key_6, Qt::NoModifier, "6", true, 1));
}
void MainWindow::on_pushButton_key_7_clicked()
{
    QCoreApplication::sendEvent(QApplication::focusWidget(), new QKeyEvent(QEvent::KeyPress, Qt::Key_7, Qt::NoModifier, "7", true, 1));
    QCoreApplication::sendEvent(QApplication::focusWidget(), new QKeyEvent(QEvent::KeyRelease, Qt::Key_7, Qt::NoModifier, "7", true, 1));
}
void MainWindow::on_pushButton_key_8_clicked()
{
    QCoreApplication::sendEvent(QApplication::focusWidget(), new QKeyEvent(QEvent::KeyPress, Qt::Key_8, Qt::NoModifier, "8", true, 1));
    QCoreApplication::sendEvent(QApplication::focusWidget(), new QKeyEvent(QEvent::KeyRelease, Qt::Key_8, Qt::NoModifier, "8", true, 1));
}
void MainWindow::on_pushButton_key_9_clicked()
{
    QCoreApplication::sendEvent(QApplication::focusWidget(), new QKeyEvent(QEvent::KeyPress, Qt::Key_9, Qt::NoModifier, "9", true, 1));
    QCoreApplication::sendEvent(QApplication::focusWidget(), new QKeyEvent(QEvent::KeyRelease, Qt::Key_9, Qt::NoModifier, "9", true, 1));
}
void MainWindow::on_pushButton_key_clear_clicked()
{
    QCoreApplication::sendEvent(QApplication::focusWidget(), new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier, "", true, 1));
    QCoreApplication::sendEvent(QApplication::focusWidget(), new QKeyEvent(QEvent::KeyRelease, Qt::Key_Backspace, Qt::NoModifier, "", true, 1));
}
void MainWindow::on_pushButton_key_0_clicked()
{
    QCoreApplication::sendEvent(QApplication::focusWidget(), new QKeyEvent(QEvent::KeyPress, Qt::Key_0, Qt::NoModifier, "0", true, 1));
    QCoreApplication::sendEvent(QApplication::focusWidget(), new QKeyEvent(QEvent::KeyRelease, Qt::Key_0, Qt::NoModifier, "0", true, 1));
}
void MainWindow::on_pushButton_key_enter_clicked()
{
   // setFocus(4);
    //QCoreApplication::sendEvent(QApplication::focusWidget(), new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier, "", true, 1));
   // QCoreApplication::sendEvent(QApplication::focusWidget(), new QKeyEvent(QEvent::KeyRelease, Qt::Key_Enter, Qt::NoModifier, "", true, 1));
}


void MainWindow::on_lineEdit_10_editingFinished()
{
    line_nmber = 10;
}
void MainWindow::on_lineEdit_21_editingFinished()
{
    line_nmber = 21;
}
void MainWindow::on_lineEdit_22_editingFinished()
{
    line_nmber = 22;
}
void MainWindow::on_lineEdit_23_editingFinished()
{
    line_nmber = 23;
}
void MainWindow::on_lineEdit_11_editingFinished()
{
    line_nmber = 11;
}
void MainWindow::on_lineEdit_14_editingFinished()
{
    line_nmber = 14;
}
void MainWindow::on_lineEdit_9_editingFinished()
{
    line_nmber = 9;
}
void MainWindow::on_lineEdit_12_editingFinished()
{
    line_nmber = 12;
}
void MainWindow::on_lineEdit_16_editingFinished()
{
    line_nmber = 16;
}
void MainWindow::on_lineEdit_13_editingFinished()
{
    line_nmber = 13;
}
void MainWindow::on_lineEdit_15_editingFinished()
{
    line_nmber = 15;
}
void MainWindow::on_lineEdit_18_editingFinished()
{
    line_nmber = 18;
}
void MainWindow::on_lineEdit_20_editingFinished()
{
    line_nmber = 20;
}
void MainWindow::on_lineEdit_17_editingFinished()
{
    line_nmber = 17;
}
void MainWindow::on_lineEdit_19_editingFinished()
{
    line_nmber = 19;
}
void MainWindow::on_lineEdit_editingFinished()
{
     line_nmber = 1;
}
void MainWindow::on_lineEdit_2_editingFinished()
{
     line_nmber = 2;
}
void MainWindow::on_lineEdit_3_editingFinished()
{
     line_nmber = 3;
}
void MainWindow::on_lineEdit_4_editingFinished()
{
     line_nmber = 4;
}
void MainWindow::on_lineEdit_24_editingFinished()
{
    line_nmber = 24;
}
void MainWindow::on_lineEdit_58_editingFinished()
{
     line_nmber = 58;
}
void MainWindow::on_lineEdit_59_editingFinished()
{
    line_nmber = 59;
}
void MainWindow::on_lineEdit_5_editingFinished()
{
    line_nmber = 5;
}


void MainWindow::on_pushButton_81_clicked()
{
    bool ok;
    int num;
    QString str;
    str = ui->lineEdit_57->text();
    num = str.toInt(&ok);
    if (num < 6){ num +=1;}
    ui->lineEdit_57->setText(QString::number(num));
}
void MainWindow::on_pushButton_80_clicked()
{
    bool ok;
    int num;
    QString str;
    str = ui->lineEdit_57->text();
    num = str.toInt(&ok);
    if (num > 0){  num -=1;}
    ui->lineEdit_57->setText(QString::number(num));
}
void MainWindow::on_pushButton_84_clicked()
{
    bool ok;
    int num;
    QString str;
    str = ui->lineEdit_188->text();
    num = str.toInt(&ok);
    if (num < 6){ num +=1;}
    ui->lineEdit_188->setText(QString::number(num));
}
void MainWindow::on_pushButton_85_clicked()
{
    bool ok;
    int num;
    QString str;
    str = ui->lineEdit_188->text();
    num = str.toInt(&ok);
    if (num > 0){  num -=1;}
    ui->lineEdit_188->setText(QString::number(num));
}






void MainWindow::on_lineEdit_39_cursorPositionChanged(int arg1, int arg2)
{

}

void MainWindow::on_lineEdit_31_cursorPositionChanged(int arg1, int arg2)
{

}
