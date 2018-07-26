#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>


#include <onixb3.h>
#include <sss3.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    quint8 kod_npb;
    quint8 kod_speed;
    quint8 kod_priori;
    quint8 kod_rchvk;

    quint8 kod_npb2;
    quint8 kod_speed2;
    quint8 kod_priori2;
    quint8 kod_rchvk2;


    quint8 myNumber_number_LSB;
    quint8 myNumber_number_MSB;

    quint8 myNumber2_number_LSB;
    quint8 myNumber2_number_MSB;


    quint8 myNumberCall_number_LSB;
    quint8 myNumberCall_number_MSB;

    quint8 myNumberCall2_number_LSB;
    quint8 myNumberCall2_number_MSB;


    OnixB3* b;
    OnixB3* b1;

    Sss3* sss;


    quint8 getNumber(QCharRef c);



signals:
    void iss3_setSeitings(quint8,quint8,quint8,quint8,quint8,quint8);
    void iss3_setSeitings_2(quint8,quint8,quint8,quint8,quint8,quint8);

    void startSimulation(int);
    void stopSimulation();
    void clearAll();

    void Reg(quint8,quint8,quint8,quint8,quint8,quint8);
    void Reg2(quint8,quint8,quint8,quint8,quint8,quint8);

    void RegCansell();
    void Reg2Cansell();

    void call(quint8,quint8,quint8,quint8);
    void call2(quint8,quint8,quint8,quint8);

    void callEnter();
    void callEnter2();

    void cansel();
    void cansel2();

    void canselPrioritet();


private slots:
    void log1(QString data);
    void log2(QString data);
    void logOnix(QString data);

    void on_comboBox_activated(int index);

    void on_comboBox_2_activated(const QString &arg1);

    void on_comboBox_3_activated(const QString &arg1);


    void on_comboBox_5_activated(int index);

    void on_comboBox_6_activated(const QString &arg1);

    void on_comboBox_7_activated(const QString &arg1);

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();
    void on_pushButton_9_clicked();

    void on_comboBox_4_activated(int index);

    void on_comboBox_8_activated(int index);

    void on_radioButton_2_clicked(bool checked);

    void on_radioButton_clicked(bool checked);

    void on_radioButton_3_clicked(bool checked);

    void on_radioButton_4_clicked(bool checked);



    void slot_regMod1(bool);
    void slot_regMod2(bool);

    void slot_otvetZs(bool);
    void slot_otvetZs2(bool);


    void on_pushButton_12_clicked();

    void on_pushButton_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_pushButton_3_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_13_clicked();

    void on_radioButton_8_clicked(bool checked);

    void on_radioButton_9_clicked(bool checked);

    void on_radioButton_7_clicked(bool checked);

    void on_pushButton_11_clicked();

    void on_pushButton_10_clicked();

    void on_radioButton_10_clicked(bool checked);

    void on_radioButton_11_clicked(bool checked);


    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_8_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
