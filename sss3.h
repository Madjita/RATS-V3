#ifndef SSS3_H
#define SSS3_H

#include <QObject>

#include <QThread>
#include <QDebug>

#include <QTimer>
#include <QTime>



class Sss3 : public QObject
{
    Q_OBJECT

public:
    explicit Sss3(QObject *parent = nullptr);


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

    //ISS 3
    //«ВЫЗОВ», «ОТВЕТ РТР», «ОТВЕТ ЗС», «ОТБОЙ», «ОТБОЙ ПО ПРИОРИТЕТУ», «ЗАНЯТА ЗС»
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
        quint8 numberReceiverLSB;
        quint8 numberReceiverMSB;



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


    enum STATE
    {
        S3_IS = 0xF,
        S3_FREE = 0x0C,    //Команда "Свободен" 0xC = 1100
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

    int kol;
    int kol2;

    int flag_messege;
    int flag_messege2;
    QByteArray massData;
    QByteArray massData2;

    ISS3_FreeSSS * iss3Free = new ISS3_FreeSSS;
    ISS3_Receipt * iss3Receipt = new ISS3_Receipt;
    ISS3_Call * iss3Call = new ISS3_Call;
    ISS3_Refusal* iss3Refusal = new ISS3_Refusal;



    ISS3_FreeSSS * iss3Free_2 = new ISS3_FreeSSS;
    ISS3_Receipt * iss3Receipt_2 = new ISS3_Receipt;
    ISS3_Call * iss3Call_2 = new ISS3_Call;
    ISS3_Refusal* iss3Refusal_2 = new ISS3_Refusal;

    int systemSchet = 2;

    void shapka();
    void shapka2();


    quint8 flag_CallPr1;
    quint8 flag_CallPr2;

signals:
    void writeSSS(char data);
    void writeSSS2(char data);

    void log1(QString data);
    void log2(QString data);

    void set_regMod1(bool);
    void set_regMod2(bool);

    void set_otvetZS(bool);
    void set_otvetZS2(bool);





public slots:
    void process_start();

    void readSSS(char data);
    void readSSS2(char data);

    void startSimulation(int);
    void stopSimulation();

    void clearAll();

    void free();


    QString get_Byte8(quint8 data);
    QString get_Byte6(quint8 data);
    QString get_Byte4(quint8 data);
    QString get_Byte2(quint8 data);

    void canselPrioritet();

private:
    quint8 Mod_Onix_1;
    quint8 Mod_SSS_1;

    quint8 Mod_Onix_2;
    quint8 Mod_SSS_2;


    QTimer* timer;

    int timerCall;
    int timerCall2;

    bool flagReg;
    bool flagReg2;
};

#endif // SSS3_H
