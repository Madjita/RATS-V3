#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    b = new OnixB3();
    b->index =1;
    b1 = new OnixB3();
    b1->index = 2;

    sss = new Sss3();


    auto valid = new QRegExpValidator(QRegExp("^[0-9]*|0"));

    ui->lineEdit->setValidator(valid);
    ui->lineEdit_2->setValidator(valid);
    ui->lineEdit_3->setValidator(valid);
    ui->lineEdit_4->setValidator(valid);

    ui->lineEdit_5->setValidator(valid);

    qRegisterMetaType<QList<quint16>>("QList<quint16>");

    connect(this,&MainWindow::startSimulation,sss,&Sss3::startSimulation);
    connect(this,&MainWindow::stopSimulation,sss,&Sss3::stopSimulation);

    connect(this,&MainWindow::clearAll,sss,&Sss3::clearAll);
    connect(this,&MainWindow::clearAll,b,&OnixB3::clearAll);
    connect(this,&MainWindow::clearAll,b1,&OnixB3::clearAll);

    connect(sss,&Sss3::writeSSS, b,&OnixB3::readSSS, Qt::DirectConnection);
    connect(b,&OnixB3::writeSSS_ISS3,sss,&Sss3::readSSS, Qt::DirectConnection);


    connect(b1,&OnixB3::writeSSS_ISS3,sss,&Sss3::readSSS2, Qt::DirectConnection);
    connect(sss,&Sss3::writeSSS2, b1,&OnixB3::readSSS, Qt::DirectConnection);

    connect(b,&OnixB3::log,this,&MainWindow::logOnix);
    connect(b1,&OnixB3::log,this,&MainWindow::logOnix);


    connect(this,&MainWindow::iss3_setSeitings,b,&OnixB3::iss3_setSeitings);
    connect(this,&MainWindow::iss3_setSeitings_2,b1,&OnixB3::iss3_setSeitings);

    connect(sss,&Sss3::log1,this,&MainWindow::log1);
    connect(sss,&Sss3::log2,this,&MainWindow::log2);

    connect(this,&MainWindow::Reg,b,&OnixB3::Reg);
    connect(this,&MainWindow::Reg2,b1,&OnixB3::Reg);

    connect(sss,&Sss3::set_regMod1, this,&MainWindow::slot_regMod1);
    connect(sss,&Sss3::set_regMod2, this,&MainWindow::slot_regMod2);

    connect(sss,&Sss3::set_otvetZS, this,&MainWindow::slot_otvetZs);
    connect(sss,&Sss3::set_otvetZS2, this,&MainWindow::slot_otvetZs2);

    connect(this,&MainWindow::call,b,&OnixB3::Call);
    connect(this,&MainWindow::call2,b1,&OnixB3::Call);

    connect(this,&MainWindow::callEnter,b,&OnixB3::CallEnter);
    connect(this,&MainWindow::callEnter2,b1,&OnixB3::CallEnter);

    connect(this,&MainWindow::RegCansell,b,&OnixB3::RegCansell);
    connect(this,&MainWindow::Reg2Cansell,b1,&OnixB3::RegCansell);

    connect(this,&MainWindow::cansel,b,&OnixB3::cansel);
    connect(this,&MainWindow::cansel2,b1,&OnixB3::cansel);

    connect(this,&MainWindow::canselPrioritet,sss,&Sss3::canselPrioritet);



    kod_npb = 100;
    kod_speed = 100;

    kod_npb2 = 100;
    kod_speed2 = 100;
    kod_priori2 = 100;
    kod_priori = 100;

    myNumber_number_LSB = 0;
    myNumber_number_MSB = 0;

    myNumber2_number_LSB = 0;
    myNumber2_number_MSB = 0;

    myNumberCall_number_LSB = 0;
    myNumberCall_number_MSB = 0;

    myNumberCall2_number_LSB = 0;
    myNumberCall2_number_MSB = 0;


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::log1(QString data)
{
    ui->textEdit->append(data);

    if(ui->checkBox->checkState() == Qt::Checked)
        ui->textEdit->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
}

void MainWindow::log2(QString data)
{
    ui->textEdit_2->append(data);
    if(ui->checkBox_2->checkState() == Qt::Checked)
        ui->textEdit_2->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
}

void MainWindow::logOnix(QString data)
{
    OnixB3* obj = (OnixB3*)sender(); //Объект который вызвал сигнал

    switch (obj->index)
    {
    case 1:
    {
        ui->textEdit->append(data);
        if(ui->checkBox->checkState() == Qt::Checked)
        {
            ui->textEdit->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
        }

        break;

    }
    case 2:
    {
        ui->textEdit_2->append(data);
        if(ui->checkBox_2->checkState() == Qt::Checked)
        {
            ui->textEdit_2->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
        }
        break;
    }
    }

}

// Настройка скорости передачи от ОНИСК 1
void MainWindow::on_comboBox_activated(int index)
{
    switch (index) {
    case 1 :{
        //emit iss3_setSpeed(0x00);

        ui->comboBox_2->clear();
        QStringList list ;
        list   << "Установить значение" << "6" << "12";
        ui->comboBox_2->addItems(list);
        ui->comboBox_2->setEnabled(true);

        break;
    }
    case 2 :{
        // emit iss3_setSpeed(0x01);

        ui->comboBox_2->clear();
        QStringList list ;
        list   << "Установить значение" << "6" << "12";
        ui->comboBox_2->addItems(list);
        ui->comboBox_2->setEnabled(true);
        break;

    }
    case 3 :{
        //emit iss3_setSpeed(0x02);

        ui->comboBox_2->clear();
        QStringList list ;
        list   << "Установить значение" << "6" << "12";
        ui->comboBox_2->addItems(list);
        ui->comboBox_2->setEnabled(true);

        break;
    }
    case 4 :{
        //emit iss3_setSpeed(0x03);

        ui->comboBox_2->clear();
        QStringList list ;
        list   << "Установить значение" << "12";
        ui->comboBox_2->addItems(list);
        ui->comboBox_2->setEnabled(true);

        break;

    }
    default:
    {
        //emit iss3_setSpeed(NULL);

        ui->comboBox_2->setEnabled(false);
        ui->comboBox_2->clear();
        QStringList list ;
        list << "Установить значение";
        ui->comboBox_2->addItems(list);

        ui->comboBox_3->clear();
        ui->comboBox_3->addItems(list);
        ui->comboBox_3->setEnabled(false);
        kod_npb = 100;
        kod_speed = 100;

        break;
    }


    }

    ui->comboBox_3->clear();
    QStringList list ;
    list   << "Установить значение";
    ui->comboBox_3->addItems(list);
    ui->comboBox_3->setEnabled(false);
    kod_npb = 100;
    kod_speed = 100;

}

void MainWindow::on_comboBox_2_activated(const QString &arg1)
{
    if(ui->comboBox->currentText() == "1.2")
    {
        if(arg1 == "6" || arg1 == "12")
        {
            ui->comboBox_3->clear();
            QStringList list ;
            list   << "Установить значение" << "6" << "12";
            ui->comboBox_3->addItems(list);
            ui->comboBox_3->setEnabled(true);

            return;
        }
    }
    else
    {
        if(ui->comboBox->currentText() == "2.4")
        {
            if(arg1 == "6" || arg1 == "12")
            {
                ui->comboBox_3->clear();
                QStringList list ;
                list   << "Установить значение" << "6" << "12";
                ui->comboBox_3->addItems(list);
                ui->comboBox_3->setEnabled(true);

                return;
            }
        }
        else
        {
            if(ui->comboBox->currentText() == "4.8")
            {
                if(arg1 == "6")
                {
                    ui->comboBox_3->clear();
                    QStringList list ;
                    list   << "Установить значение" << "1" << "2";
                    ui->comboBox_3->addItems(list);
                    ui->comboBox_3->setEnabled(true);

                    return;
                }

                if(arg1 == "12")
                {
                    ui->comboBox_3->clear();
                    QStringList list ;
                    list   << "Установить значение" << "1.3" << "2.4";
                    ui->comboBox_3->addItems(list);
                    ui->comboBox_3->setEnabled(true);

                    return;
                }
            }
            else
            {
                if(ui->comboBox->currentText() == "9.6")
                {

                    if(arg1 == "12")
                    {
                        ui->comboBox_3->clear();
                        QStringList list ;
                        list   << "Установить значение" << "1" << "2" <<"3"<<"4";
                        ui->comboBox_3->addItems(list);
                        ui->comboBox_3->setEnabled(true);

                        return;
                    }
                }
            }
        }
    }

    if(ui->comboBox_2->currentText() == "Установить значение")
    {
        ui->comboBox_3->clear();
        QStringList list ;
        list   << "Установить значение";
        ui->comboBox_3->addItems(list);
        ui->comboBox_3->setEnabled(false);
        kod_npb = 100;
        kod_speed = 100;

    }
}

void MainWindow::on_comboBox_3_activated(const QString &arg1)
{
    if(ui->comboBox->currentText() == "1.2")
    {
        if(arg1 == "6" || arg1 == "12")
        {
            kod_npb = 0x00;
            kod_speed = 0x00;
        }
    }
    else
    {
        if(ui->comboBox->currentText() == "2.4")
        {
            if(arg1 == "6" || arg1 == "12")
            {
                kod_npb = 0x00;
                kod_speed = 0x01;
            }
        }
        else
        {
            if(ui->comboBox->currentText() == "4.8")
            {
                if(arg1 == "1" || arg1 == "1.3")
                {
                    kod_npb = 0x00;
                    kod_speed = 0x02;
                }

                if(arg1 == "2" || arg1 == "2.4")
                {
                    kod_npb = 0x01;
                    kod_speed = 0x02;
                }
            }
            else
            {
                if(ui->comboBox->currentText() == "9.6")
                {
                    kod_speed = 0x03;

                    if(arg1 == "1")
                    {
                        kod_npb = 0x00;
                    }
                    if(arg1 == "2")
                    {
                        kod_npb = 0x01;
                    }
                    if(arg1 == "3")
                    {
                        kod_npb = 0x02;
                    }
                    if(arg1 == "4")
                    {
                        kod_npb = 0x03;
                    }
                }
            }
        }
    }

    //    qDebug() << "==========";
    //    qDebug() << kod_npb;
    //    qDebug() << kod_speed;


}

void MainWindow::on_comboBox_5_activated(int index)
{
    switch (index) {
    case 1 :{
        //emit iss3_setSpeed(0x00);

        ui->comboBox_6->clear();
        QStringList list ;
        list   << "Установить значение" << "6" << "12";
        ui->comboBox_6->addItems(list);
        ui->comboBox_6->setEnabled(true);

        break;
    }
    case 2 :{
        // emit iss3_setSpeed(0x01);

        ui->comboBox_6->clear();
        QStringList list ;
        list   << "Установить значение" << "6" << "12";
        ui->comboBox_6->addItems(list);
        ui->comboBox_6->setEnabled(true);
        break;

    }
    case 3 :{
        //emit iss3_setSpeed(0x02);

        ui->comboBox_6->clear();
        QStringList list ;
        list   << "Установить значение" << "6" << "12";
        ui->comboBox_6->addItems(list);
        ui->comboBox_6->setEnabled(true);

        break;
    }
    case 4 :{
        //emit iss3_setSpeed(0x03);

        ui->comboBox_6->clear();
        QStringList list ;
        list   << "Установить значение" << "12";
        ui->comboBox_6->addItems(list);
        ui->comboBox_6->setEnabled(true);

        break;

    }
    default:
    {
        //emit iss3_setSpeed(NULL);

        ui->comboBox_6->setEnabled(false);
        ui->comboBox_6->clear();
        QStringList list ;
        list << "Установить значение";
        ui->comboBox_6->addItems(list);
        kod_npb2 = 100;
        kod_speed2 = 100;

        break;
    }

    }

    ui->comboBox_7->clear();
    QStringList list ;
    list   << "Установить значение";
    ui->comboBox_7->addItems(list);
    ui->comboBox_7->setEnabled(false);
    kod_npb2 = 100;
    kod_speed2 = 100;
}


void MainWindow::on_comboBox_6_activated(const QString &arg1)
{
    if(ui->comboBox_5->currentText() == "1.2")
    {
        if(arg1 == "6" || arg1 == "12")
        {
            ui->comboBox_7->clear();
            QStringList list ;
            list   << "Установить значение" << "6" << "12";
            ui->comboBox_7->addItems(list);
            ui->comboBox_7->setEnabled(true);

            return;
        }
    }
    else
    {
        if(ui->comboBox_5->currentText() == "2.4")
        {
            if(arg1 == "6" || arg1 == "12")
            {
                ui->comboBox_7->clear();
                QStringList list ;
                list   << "Установить значение" << "6" << "12";
                ui->comboBox_7->addItems(list);
                ui->comboBox_7->setEnabled(true);

                return;
            }
        }
        else
        {
            if(ui->comboBox_5->currentText() == "4.8")
            {
                if(arg1 == "6")
                {
                    ui->comboBox_7->clear();
                    QStringList list ;
                    list   << "Установить значение" << "1" << "2";
                    ui->comboBox_7->addItems(list);
                    ui->comboBox_7->setEnabled(true);

                    return;
                }

                if(arg1 == "12")
                {
                    ui->comboBox_7->clear();
                    QStringList list ;
                    list   << "Установить значение" << "1.3" << "2.4";
                    ui->comboBox_7->addItems(list);
                    ui->comboBox_7->setEnabled(true);

                    return;
                }
            }
            else
            {
                if(ui->comboBox_5->currentText() == "9.6")
                {

                    if(arg1 == "12")
                    {
                        ui->comboBox_7->clear();
                        QStringList list ;
                        list   << "Установить значение" << "1" << "2" <<"3"<<"4";
                        ui->comboBox_7->addItems(list);
                        ui->comboBox_7->setEnabled(true);

                        return;
                    }
                }
            }
        }
    }

    if(ui->comboBox_5->currentText() == "Установить значение")
    {
        ui->comboBox_6->clear();
        QStringList list ;
        list   << "Установить значение";
        ui->comboBox_6->addItems(list);
        ui->comboBox_6->setEnabled(false);
        kod_npb2 = 100;
        kod_speed2 = 100;

    }

    if(ui->comboBox_6->currentText() == "Установить значение")
    {
        ui->comboBox_7->clear();
        QStringList list ;
        list   << "Установить значение";
        ui->comboBox_7->addItems(list);
        ui->comboBox_7->setEnabled(false);
        kod_npb2 = 100;
        kod_speed2 = 100;

    }
}

void MainWindow::on_comboBox_7_activated(const QString &arg1)
{
    if(ui->comboBox_5->currentText() == "1.2")
    {
        if(arg1 == "6" || arg1 == "12")
        {
            kod_npb2 = 0x00;
            kod_speed2 = 0x00;
        }
    }
    else
    {
        if(ui->comboBox_5->currentText() == "2.4")
        {
            if(arg1 == "6" || arg1 == "12")
            {
                kod_npb2 = 0x00;
                kod_speed2 = 0x01;
            }
        }
        else
        {
            if(ui->comboBox_5->currentText() == "4.8")
            {
                if(arg1 == "1" || arg1 == "1.3")
                {
                    kod_npb2 = 0x00;
                    kod_speed2 = 0x02;
                }

                if(arg1 == "2" || arg1 == "2.4")
                {
                    kod_npb2 = 0x01;
                    kod_speed2 = 0x02;
                }
            }
            else
            {
                if(ui->comboBox_5->currentText() == "9.6")
                {
                    kod_speed2 = 0x03;

                    if(arg1 == "1")
                    {
                        kod_npb2 = 0x00;
                    }
                    if(arg1 == "2")
                    {
                        kod_npb2 = 0x01;
                    }
                    if(arg1 == "3")
                    {
                        kod_npb2 = 0x02;
                    }
                    if(arg1 == "4")
                    {
                        kod_npb2 = 0x03;
                    }
                }
            }
        }
    }

    //    qDebug() << "==========";
    //    qDebug() << kod_npb2;
    //    qDebug() << kod_speed2;

}

quint8 MainWindow::getNumber(QCharRef c)
{
    quint8 x;

    switch (c.unicode()) {

    case '0':x = 0x00;break;
    case '1':x = 0x01;break;
    case '2':x = 0x02;break;
    case '3':x = 0x03;break;
    case '4':x = 0x04;break;
    case '5':x = 0x05;break;
    case '6':x = 0x06;break;
    case '7':x = 0x07;break;
    case '8':x = 0x08;break;
    case '9':x = 0x09;break;
    }

    return x;
}

void MainWindow::on_pushButton_6_clicked()
{

    if(kod_npb == 100 || kod_npb2 == 100 || kod_priori == 100 || kod_priori2 == 100 || kod_speed == 100 || kod_speed2 == 100 )
    {

        int ret = QMessageBox::warning(this, tr("Ошибка"),
                                       tr("Установите значения в полях <b>СТАТУСА</b> в ОНИКС №1 и ОНИКС №2 !"),
                                       QMessageBox::Ok);
        return;
    }


    ui->pushButton_7->setDefault(false);
    ui->pushButton_8->setDefault(false);

    ui->pushButton_6->setDefault(true);


    if(ui->lineEdit_5->text().toInt() != 0)
    {
        emit startSimulation(ui->lineEdit_5->text().toInt()*1000);
    }
    else
    {
        ui->lineEdit_5->setText("1");

        emit startSimulation(1000);
    }
}

void MainWindow::on_pushButton_7_clicked()
{
    ui->pushButton_6->setDefault(false);
    ui->pushButton_8->setDefault(false);

    ui->pushButton_7->setDefault(true);

    emit stopSimulation();

    emit clearAll();

    ui->textEdit->clear();
    ui->textEdit_2->clear();

}

void MainWindow::on_pushButton_9_clicked()
{
    ui->pushButton_9->setEnabled(false);
    ui->pushButton_10->setEnabled(true);
    myNumber_number_LSB = 0x00;
    myNumber_number_MSB = 0x00;

    //запись младшего байта
    myNumber_number_LSB += getNumber(ui->lineEdit_3->text()[0]);
    myNumber_number_LSB += getNumber(ui->lineEdit_3->text()[1]) << 4;

    //запись старшего байта
    myNumber_number_MSB += getNumber(ui->lineEdit_3->text()[2]);
    myNumber_number_MSB += getNumber(ui->lineEdit_3->text()[3]) << 4;

    //emit iss3_setSeitings();


    qDebug () << kod_npb << endl << kod_speed << endl << kod_priori << endl << kod_rchvk << endl << "+=+=+=+=";

    emit Reg(kod_npb , kod_speed, kod_priori,kod_rchvk, myNumber_number_LSB, myNumber_number_MSB);
}

void MainWindow::on_comboBox_4_activated(int index)
{

    switch (index) {
    case 1: kod_priori = 0x01 ; break;
    case 2: kod_priori = 0x02 ; break;
    case 3: kod_priori = 0x03 ; break;
    case 4: kod_priori = 0x04 ; break;
    case 5: kod_priori = 0x05 ; break;
    case 6: kod_priori = 0x06 ; break;
    case 7: kod_priori = 0x07 ; break;
    case 8: kod_priori = 0x08 ; break;
    default: kod_priori = 100;

    }
}

void MainWindow::on_comboBox_8_activated(int index)
{
    switch (index) {
    case 1: kod_priori2 = 0x01 ; break;
    case 2: kod_priori2 = 0x02 ; break;
    case 3: kod_priori2 = 0x03 ; break;
    case 4: kod_priori2 = 0x04 ; break;
    case 5: kod_priori2 = 0x05 ; break;
    case 6: kod_priori2 = 0x06 ; break;
    case 7: kod_priori2 = 0x07 ; break;
    case 8: kod_priori2 = 0x08 ; break;
    default: kod_priori2 = 100;

    }
}

void MainWindow::on_radioButton_2_clicked(bool checked)
{
    if(checked == true)
    {
        kod_rchvk = 0x01;
    }
}

void MainWindow::on_radioButton_clicked(bool checked)
{
    if(checked == true)
    {
        kod_rchvk = 0x02;
    }
}

void MainWindow::on_radioButton_3_clicked(bool checked)
{
    if(checked == true)
    {
        kod_rchvk2 = 0x01;
    }
}

void MainWindow::on_radioButton_4_clicked(bool checked)
{
    if(checked == true)
    {
        kod_rchvk2 = 0x02;
    }
}

void MainWindow::slot_regMod1(bool mod)
{
    if(mod)
    {
        ui->label_regMod1->setText("ЕСТЬ");
        ui->label_regMod1->setStyleSheet("color:#fff; background-color: green; padding-left: 10px; ");
    }
    else
    {
        ui->label_regMod1->setText("НЕТ");
        ui->label_regMod1->setStyleSheet("color:#fff; background-color: rgb(170, 0, 0); padding-left: 10px; ");
    }
}

void MainWindow::slot_regMod2(bool mod)
{
    if(mod)
    {
        ui->label_regMod2->setText("ЕСТЬ");
        ui->label_regMod2->setStyleSheet("color:#fff; background-color: green; padding-left: 10px; ");
    }
    else
    {
        ui->label_regMod2->setText("НЕТ");
        ui->label_regMod2->setStyleSheet("color:#fff; background-color: rgb(170, 0, 0); padding-left: 10px; ");
    }
}

void MainWindow::slot_otvetZs(bool mod)
{
    if(mod)
    {
        ui->label_otvMod1->setText("ЕСТЬ");
        ui->label_otvMod1->setStyleSheet("color:#fff; background-color: green; padding-left: 10px; ");
    }
    else
    {
        ui->label_otvMod1->setText("НЕТ");
        ui->label_otvMod1->setStyleSheet("color:#fff; background-color: rgb(170, 0, 0); padding-left: 10px; ");
    }
}

void MainWindow::slot_otvetZs2(bool mod)
{
    if(mod)
    {
        ui->label_otvMod2->setText("ЕСТЬ");
        ui->label_otvMod2->setStyleSheet("color:#fff; background-color: green; padding-left: 10px; ");
    }
    else
    {
        ui->label_otvMod2->setText("НЕТ");
        ui->label_otvMod2->setStyleSheet("color:#fff; background-color: rgb(170, 0, 0); padding-left: 10px; ");
    }
}

void MainWindow::on_pushButton_12_clicked()
{
    ui->pushButton_12->setEnabled(false);
    ui->pushButton_11->setEnabled(true);
    //запись младшего байта
    myNumber2_number_LSB += getNumber(ui->lineEdit_4->text()[0]);
    myNumber2_number_LSB += getNumber(ui->lineEdit_4->text()[1]) << 4;

    //запись старшего байта
    myNumber2_number_MSB += getNumber(ui->lineEdit_4->text()[2]);
    myNumber2_number_MSB += getNumber(ui->lineEdit_4->text()[3]) << 4;

    //emit iss3_setSeitings();

    emit Reg2(kod_npb2 , kod_speed2, kod_priori2,kod_rchvk2, myNumber2_number_LSB, myNumber2_number_MSB);
}

void MainWindow::on_pushButton_clicked()
{
    //запись младшего байта
    myNumberCall_number_LSB += getNumber(ui->lineEdit->text()[0]);
    myNumberCall_number_LSB += getNumber(ui->lineEdit->text()[1]) << 4;

    //запись старшего байта
    myNumberCall_number_MSB += getNumber(ui->lineEdit->text()[2]);
    myNumberCall_number_MSB += getNumber(ui->lineEdit->text()[3]) << 4;

    qDebug () << myNumber_number_LSB << endl <<  myNumber_number_MSB << endl << myNumberCall_number_LSB << endl << myNumberCall_number_MSB << endl <<  "======" << endl;

    emit call(myNumber_number_LSB, myNumber_number_MSB, myNumberCall_number_LSB, myNumberCall_number_MSB);
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    if(arg1.count() > 0)
    {
        ui->pushButton->setEnabled(true);
    }
    else
    {
        ui->pushButton->setEnabled(false);
    }
}

void MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
    if(arg1.count() > 0)
    {
        ui->pushButton_3->setEnabled(true);
    }
    else
    {
        ui->pushButton_3->setEnabled(false);
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    myNumberCall2_number_LSB = 0x00;
    myNumberCall2_number_MSB = 0x00;

    //запись младшего байта
    myNumberCall2_number_LSB += getNumber(ui->lineEdit_2->text()[0]);
    myNumberCall2_number_LSB += getNumber(ui->lineEdit_2->text()[1]) << 4;

    //запись старшего байта
    myNumberCall2_number_MSB += getNumber(ui->lineEdit_2->text()[2]);
    myNumberCall2_number_MSB += getNumber(ui->lineEdit_2->text()[3]) << 4;

    emit call2(myNumber2_number_LSB, myNumber2_number_MSB, myNumberCall2_number_LSB, myNumberCall2_number_MSB);
}

void MainWindow::on_pushButton_14_clicked()
{
    emit callEnter2();
}

void MainWindow::on_pushButton_13_clicked()
{
    emit callEnter();
}

void MainWindow::on_radioButton_8_clicked(bool checked)
{
    if(checked)
    {
        sss->flag_CallPr1 = sss->S3_RTR;
    }
    else
    {
        sss->flag_CallPr1 = sss->S3_FREE_SSS;
    }
}

void MainWindow::on_radioButton_9_clicked(bool checked)
{
    if(checked)
    {
        sss->flag_CallPr2 = sss->S3_RTR;
    }
    else
    {
        sss->flag_CallPr2 = sss->S3_FREE_SSS;
    }
}

void MainWindow::on_radioButton_7_clicked(bool checked)
{
    if(checked)
    {
        sss->flag_CallPr1 = sss->S3_FREE_SSS;
        sss->flag_CallPr2 = sss->S3_FREE_SSS;
    }

}

void MainWindow::on_pushButton_11_clicked()
{
    ui->pushButton_12->setEnabled(true);
    ui->pushButton_11->setEnabled(false);
    emit Reg2Cansell();
}

void MainWindow::on_pushButton_10_clicked()
{
    ui->pushButton_9->setEnabled(true);
    ui->pushButton_10->setEnabled(false);
    emit RegCansell();
}

void MainWindow::on_radioButton_10_clicked(bool checked)
{
    if(checked)
    {
        sss->flag_CallPr1 = sss->S3_REG_Cansel;
    }
    else
    {
        sss->flag_CallPr1 = sss->S3_FREE_SSS;
    }

}

void MainWindow::on_radioButton_11_clicked(bool checked)
{

    if(checked)
    {
        sss->flag_CallPr2 = sss->S3_REG_Cansel;
    }
    else
    {
        sss->flag_CallPr2 = sss->S3_FREE_SSS;
    }

}


void MainWindow::on_pushButton_2_clicked()
{
    emit cansel();
}

void MainWindow::on_pushButton_4_clicked()
{
    emit cansel2();
}

void MainWindow::on_pushButton_5_clicked()
{
    emit canselPrioritet();
}

void MainWindow::on_pushButton_8_clicked()
{
    ui->pushButton_6->setDefault(false);
    ui->pushButton_7->setDefault(false);

    ui->pushButton_8->setDefault(true);

    emit stopSimulation();
}
