#ifndef ONIXB3_H
#define ONIXB3_H

#include <QObject>

#include <QDebug>
#include <QThread>

#include <QTimer>
#include <QTime>


struct bytes8
{
   quint8 bit0  : 1;
   quint8 bit1  : 1;
   quint8 bit2  : 1;
   quint8 bit3  : 1;
   quint8 bit4  : 1;
   quint8 bit5  : 1;
   quint8 bit6  : 1;
   quint8 bit7  : 1;
};

struct bytes6
{
   quint8 bit0  : 1;
   quint8 bit1  : 1;
   quint8 bit2  : 1;
   quint8 bit3  : 1;
   quint8 bit4  : 1;
   quint8 bit5  : 1;
};

struct bytes4
{
   quint8 bit0  : 1;
   quint8 bit1  : 1;
   quint8 bit2  : 1;
   quint8 bit3  : 1;
};

struct bytes2
{
   quint8 bit0  : 1;
   quint8 bit1  : 1;
};






//ISS 3
//«ВЫЗОВ», «ОТВЕТ РТР», «ОТВЕТ ЗС», «ОТБОЙ», «ОТБОЙ ПО ПРИОРИТЕТУ», «ЗАНЯТА ЗС» , «РАБОТА», «ИС все 1111»
#pragma pack(push, 1)
struct ISS3_Call
{
    quint8 comand : 4 ; //(4 бита) Код команды
    quint8 nbp : 2; // (2 бита) Номер базовой последователности
    quint8 speed : 2; // (2 бита) Информационная скорость абонентского сигнала.

    // (16 бит) Собственный номер абонента в двоичном коде
    quint8 numberSendLSB;
    quint8 numberSendMSB;


    // (16 бит) Номер вызываемого абонента в двоичном коде
    quint8 numberReceiverMSB;
    quint8 numberReceiverLSB;


};
#pragma pack(pop)


//«РЕГИСТРАЦИЯ», «ПОДТВЕРЖДЕНИЕ РЕГИСТРАЦИИ», «ОТКАЗ ОТ РЕГИСТРАЦИИ», «ПОДТВЕРЖДЕНИЕ ОТКАЗА ОТ РЕГИСТРАЦИИ»
#pragma pack(push, 1)
struct ISS3_Receipt
{
    quint8 comand : 4 ; //(4 бита) Код команды
    quint8 nbp : 2; // (2 бита) Номер базовой последователности
    quint8 speed : 2; // (2 бита) Информационная скорость абонентского сигнала.

    // (16 бит) Собственный номер абонента в двоичном коде
    quint8 numberSendLSB;
    quint8 numberSendMSB;


    quint8 RChVK : 2; //(2 бита) Тип рабочего ЧВК
    quint8 Rezerv1 : 6; //(6 бита) Заполняется нулями
    quint8 Priopy : 4; //(4 бита) Код уровня приоритета
    quint8 Rezerv2 : 4; //(4 бита) Заполняется нулями


};
#pragma pack(pop)

//Форамт команды «ОТКАЗ РТР»
#pragma pack(push, 1)
struct ISS3_Refusal
{
    quint8 comand : 4 ; //(4 бита) Код команды
    quint8 why : 4; // (2 бита) Код причины отказа

    // (16 бит) Собственный номер абонента в двоичном коде
    quint8 numberSendLSB;
    quint8 numberSendMSB;



    // (16 бит) Номер вызываемого абонента в двоичном коде
    quint8 numberReceiverLSB;
    quint8 numberReceiverMSB;



};
#pragma pack(pop)

//Форамт команды «СВОБОДЕН»
#pragma pack(push, 1)
struct ISS3_Free
{
    quint8 comand : 4 ; //(4 бита) Код команды
    quint8 nbp : 2; // (2 бита) Номер базовой последователности
    quint8 speed : 2; // (2 бита) Информационная скорость абонентского сигнала.

    // (16 бит) Собственный номер абонента в двоичном коде
    quint8 numberSendLSB;
    quint8 numberSendMSB;



    // (16 бит) Заполняется нулями
    quint8 Rezerv1;
    quint8 Rezerv2;


};
#pragma pack(pop)

//Форамт команды «СВОБОДНА ССС», «ВЫЗОВ ПРИОРИТЕТНЫЙ», «ГОТОВ»
#pragma pack(push, 1)
struct ISS3_FreeSSS
{
    quint8 comand : 4 ; //(4 бита) Код команды
    quint8 nbp : 2; // (2 бита) Номер базовой последователности
    quint8 speed : 2; // (2 бита) Информационная скорость абонентского сигнала.

    // (16 бит) Собственный номер абонента в двоичном коде
    quint8 numberSendLSB;
    quint8 numberSendMSB;


    quint8 RChVK : 2; //(2 бита) Тип рабочего ЧВК
    quint8 Rezerv1 : 6; //(6 бита) Заполняется нулями
    quint8 Priopy : 4; //(4 бита) Код уровня приоритета (занулять)
    quint8 Rezerv2 : 4; //(4 бита) Заполняется нулями


};
#pragma pack(pop)








class OnixB3 : public QObject
{
    Q_OBJECT
public:
    explicit OnixB3(QObject *parent = nullptr);


    enum STATE
    {
        S3_IS = 0xF,

        S3_FREE = quint8(0xC),    //Команда "Свободен" 0xC = 1100
        S3_FREE_SSS = 0x0D,    //Команда "Свободен CCC" 0xC = 1101

        S3_WORK = 0x00,    // Команда "Работа" 0x00 = 0000
        S3_REG =  0x01,    // Команда "Регистрация"  0x01 = 0001

        S3_REG_Cansel = 0x03, //"Отказ от Регистрации" 0x03 = 0011

        S3_Ok_REG = 0x02,  // Команда "Подтверждение Регистрации" 0x02 = 0010
        S3_Ok_Cansel_REG = 0x04,  // Команда "Подтверждение отказа от Регистрации" 0x04 = 0100

        S3_CALL = 0x05,    //Команда "Вызов" 0x05 = 0101
        S3_ZS = 0x07,    //Команда "Ответ ЗС" 0x07 = 0111
        S3_RTR = 0x09,     //Команда "Отказ РТР" 0x09 = 1001
        S3_RTR_Otv = 0x06, //Команда "Ответ РТР" 0x06 = 0110

        S3_HANGUP = 0x0A, // Команда "Отбой" 0x0A = 1010
        S3_PRIORITY_CALL_OFF = 0x0B, // Команда "Отбой по приоритету" 0x0B = 1011

        S3_CALL_PRIORITY = 0x0E, // Команда "Вызов приоритетный" 0x0E = 1110

        S3_READY = 0x0F,   // Команда "Готов" 0x0F = 1111

    } state;


    int index;
    int timerCount;

    int kol;

    ISS3_Free *iss3Free = new ISS3_Free();
    ISS3_Receipt *iss3Receipt = new ISS3_Receipt();
    ISS3_Call *iss3Call = new ISS3_Call();
    ISS3_Refusal* iss3Refusal = new ISS3_Refusal;


    QByteArray massData;



    int flag_messege;

    int systemSchet;


    QString get_Byte8(quint8 data);
    QString get_Byte6(quint8 data);
    QString get_Byte4(quint8 data);
    QString get_Byte2(quint8 data);


signals:
    void writeSSS(char data);

    void writeSSS_ISS3(char data);

    void log(QString data);

    void IS_Gui(bool);
    void Free_Gui(bool);


public slots:
    void process_start();

    void readSSS(char data);

    void Reg(quint8 npb, quint8 speed, quint8 priori, quint8 rchvk,quint8 number_LSB, quint8 number_MSB);

    void RegCansell();

    void clearAll();

    void comandCall(QList<quint16> number_1,QList<quint16> number_2);

    void cansel();

    void setNumber_1(QList<quint16> number_1, QList<quint16> number_2);

    void setMod_Onix(quint16 data);

    QString nomer(QList<quint16> number);

    //ISS3

    void ISS3_Receipt_fn(quint8 CNS, quint8 numberSendMSB,quint8 numberSendLSB,quint8 RchVK,quint8 Priorytet);
    void ISS3_Call_fn(quint8 CNS, quint8 numberSendMSB,quint8 numberSendLSB,quint8 numberReceiverMSB,quint8 numberReceiverLSB);

    void ISS3_Free_fn(quint8 CNS, quint8 numberSendMSB,quint8 numberSendLSB,quint8 Rezerv1,quint8 Rezerv2);

    void ISS3_ISS3_Refusal_fn(quint8 CNS, quint8 numberSendMSB,quint8 numberSendLSB,quint8 numberReceiverMSB, quint8 numberReceiverLSB);

    void iss3_setSeitings(quint8 npb, quint8 speed, quint8 priori, quint8 rchvk,quint8 number_LSB, quint8 number_MSB);

    void Call(quint8 number_LSB, quint8 number_MSB,quint8 numberCall_LSB, quint8 numberCall_MSB);

    void CallEnter();



private:

    quint8 Mod_Onix_ms;

    quint8 Mod_Onix;
    quint8 Mod_SSS;

    QList<quint16> DefaultNumber_1;
    QList<quint16> DefaultNumber_2;
    QList<quint16> Number_1;
    QList<quint16> Number_2;

    bool setNumber;
    bool flagNumber;

    int timerCall;

    int countEnterCall;


};

#endif // ONIXB_H
