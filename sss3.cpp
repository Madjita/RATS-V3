#include "sss3.h"

Sss3::Sss3(QObject *parent) : QObject(parent)
{
    this->moveToThread(new QThread());
    connect(this->thread(),&QThread::started,this,&Sss3::process_start);
    this->thread()->start();
}

QString Sss3::get_Byte8(quint8 data)
{
    QString str = nullptr;

    bytes8* b = (bytes8*)&data;

    str += QString::number(b->bit7,2);
    str += QString::number(b->bit6,2);
    str += QString::number(b->bit5,2);
    str += QString::number(b->bit4,2);
    str += QString::number(b->bit3,2);
    str += QString::number(b->bit2,2);
    str += QString::number(b->bit1,2);
    str += QString::number(b->bit0,2);

    return str;
}

QString Sss3::get_Byte6(quint8 data)
{
    QString str = nullptr;

    bytes6* b = (bytes6*)&data;

    str += QString::number(b->bit5,2);
    str += QString::number(b->bit4,2);
    str += QString::number(b->bit3,2);
    str += QString::number(b->bit2,2);
    str += QString::number(b->bit1,2);
    str += QString::number(b->bit0,2);

    return str;
}

QString Sss3::get_Byte4(quint8 data)
{
    QString str = nullptr;

    bytes4* b = (bytes4*)&data;


    str += QString::number(b->bit3,2);
    str += QString::number(b->bit2,2);
    str += QString::number(b->bit1,2);
    str += QString::number(b->bit0,2);

    return str;
}

QString Sss3::get_Byte2(quint8 data)
{
    QString str = nullptr;

    bytes2* b = (bytes2*)&data;

    str += QString::number(b->bit1,2);
    str += QString::number(b->bit0,2);

    return str;
}

void Sss3::canselPrioritet()
{
    Mod_Onix_1 = S3_IS;
    Mod_Onix_2 = S3_IS;
    Mod_SSS_1 = S3_PRIORITY_CALL_OFF;
    Mod_SSS_2 = S3_PRIORITY_CALL_OFF;

}

void Sss3::process_start()
{
    //Инициализация таймеров
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    timer = new QTimer();
    connect(timer,&QTimer::timeout,this,&Sss3::free);


    Mod_Onix_1 = S3_FREE;
    Mod_SSS_1  = S3_FREE_SSS;

    Mod_SSS_2 = S3_FREE_SSS;
    Mod_Onix_2 = S3_FREE;



    clearAll();

//    Number_1.clear();
//    Number_2.clear();


//    setNumber = false;
//    setNumber_1= false;
//    count = 0;

}

void Sss3::shapka()
{
     iss3Free->nbp = iss3Receipt->nbp;
     iss3Free->speed = iss3Receipt->speed;
     iss3Free->numberSendLSB = 0x00;//iss3Receipt->numberSendLSB;
     iss3Free->numberSendMSB = 0x00;//iss3Receipt->numberSendMSB;
     iss3Free->RChVK = iss3Receipt->RChVK;
     iss3Free->Priopy = 0x00;

}

void Sss3::shapka2()
{
    iss3Free_2->nbp = iss3Receipt_2->nbp;
    iss3Free_2->speed = iss3Receipt_2->speed;
    iss3Free_2->numberSendLSB = 0x00;//iss3Receipt_2->numberSendLSB;
    iss3Free_2->numberSendMSB = 0x00;//iss3Receipt_2->numberSendMSB;
    iss3Free_2->RChVK = iss3Receipt_2->RChVK;
    iss3Free_2->Priopy = 0x00;
}

void Sss3::readSSS(char data)
{
    massData.append(data);

    if(massData.count() == 1)
    {
        quint16 komand = massData.at(0) & 0xF;

        switch (komand) {



            case S3_FREE:
            {

                if(Mod_SSS_1 == S3_HANGUP)
                {
                    flag_messege = 0; //принял команду СВОБОДЕН на фоне  ОТБОЙ
                    return;
                }

                if(Mod_SSS_1 == S3_PRIORITY_CALL_OFF)
                {
                    flag_messege = 0; //принял команду СВОБОДЕН на фоне  ОТБОЙ ПО ПРИОРИТЕТУ
                    return;
                }

                if(Mod_Onix_1 == S3_FREE && Mod_SSS_1 == S3_FREE_SSS)
                {
                    timerCall = 0;
                    flag_messege = 0; //принял команду свободен ССС
                    return;
                }

                if(Mod_Onix_1 == S3_REG && Mod_SSS_1 == S3_Ok_REG)
                {
                    flag_messege = 0; //принял команду свободен ССС

                    return;
                }

                if(Mod_Onix_1 == S3_CALL && Mod_SSS_1 == S3_RTR)
                {
                    flag_messege = 0; //принял команду свободен ССС

                    return;
                }

                if(Mod_Onix_1 == S3_CALL && Mod_SSS_1 == S3_ZS)
                {
                    flag_messege = 3; //принял команду свободен ССС
                    return;
                }

                if(Mod_SSS_1 == S3_Ok_Cansel_REG && Mod_Onix_1 == S3_REG_Cansel)
                {
                    flag_messege = 0; //принял команду свободен ССС
                    return;
                }


                qDebug () << "Mod_Onix_1 " << Mod_Onix_1 << " Mod_SSS_1 " << Mod_SSS_1;
                qDebug () << "FREE 1";
                flag_messege2 = -1;
                return;

            }

            case S3_REG:
            {
                 flag_messege = 1; //принял команду РЕГИСТРАЦИЯ
                 return;
            }

            case S3_CALL:
            {

                if(flag_CallPr1 == S3_RTR)
                {
                    flag_messege = 2;
                    timerCall = 6;
                    return;
                }

                if(flag_CallPr1 == S3_REG_Cansel)
                {
                    flag_messege = 2;
                    timerCall = 6;
                    return;
                }

                if(Mod_Onix_1 == S3_CALL && Mod_SSS_1 == S3_ZS )
                {
                     flag_messege = -1;
                     return;
                }

                if(Mod_Onix_1 == S3_CALL && Mod_SSS_1 == S3_RTR )
                {
                    flag_messege = -1;
                    return;
                }

                qDebug () << timerCall;
                flag_messege = 2; //принял команду ВЫЗОВ
                return;

            }

            case S3_ZS:
            {
                 if(Mod_Onix_1 == S3_ZS && Mod_SSS_1 == S3_FREE_SSS)
                 {
                     emit set_otvetZS(true);
                     flag_messege = 6; //принял команду ОТВЕТ ЗС
                     break;
                 }
                 if(Mod_SSS_1 == S3_IS && Mod_Onix_1 == S3_IS)
                 {
                     flag_messege = -1;
                     break;
                 }

                 if(Mod_SSS_1 == S3_HANGUP)
                 {
                     flag_messege = -2;
                     break;
                 }



                 flag_messege = 4; //принял команду ОТВЕТ ЗС
                 break;
            }

            case S3_HANGUP:
            {
               emit set_otvetZS(false);
               flag_messege = 5; //принял команду ОТБОЙ
               break;
            }

            case S3_WORK:
            {
                Mod_Onix_1 = S3_WORK;
                flag_messege = 7; //принял команду РАБОТА
                return;
            }

            case S3_IS:
            {
                flag_messege = 8;
                return;
            }

            case S3_REG_Cansel:
            {
                flag_messege = 9;
                return;
            }



        default: return;


        }
    }

    if(massData.count() == 5)
    {

        switch (flag_messege) {
            case 0:{

                Mod_Onix_1 = S3_FREE;
                Mod_SSS_1 = S3_FREE_SSS;
                //Mod_SSS_2 = S3_FREE_SSS;
                //Mod_Onix_2 = S3_FREE;


                ISS3_Free*  iss3Free_get = (ISS3_Free*)massData.data();

                emit log1("1) SSS принял от OnixB команду <b>CВОБОДЕН</b>\n");
                emit log1("\n");

                timerCall = 0;



                break;
            }

            case 1:{
                emit log1("1) SSS принял от OnixB команду <b>РЕГИСТРАЦИЯ</b>\n");
                emit log1("\n");

                Mod_Onix_1 = S3_REG;
                Mod_SSS_1 = S3_Ok_REG;
                Mod_SSS_2 = S3_FREE_SSS;
              //  Mod_Onix_2 = S3_FREE;

                ISS3_Receipt*  iss3Receipt_get = (ISS3_Receipt*)massData.data();

                iss3Receipt->Rezerv1 = iss3Receipt_get->Rezerv1;
                iss3Receipt->Rezerv2 = iss3Receipt_get->Rezerv2;
                iss3Receipt->nbp = iss3Receipt_get->nbp;
                iss3Receipt->speed = iss3Receipt_get->speed;
                iss3Receipt->numberSendLSB = iss3Receipt_get->numberSendLSB;
                iss3Receipt->numberSendMSB = iss3Receipt_get->numberSendMSB;
                iss3Receipt->RChVK = iss3Receipt_get->RChVK;
                iss3Receipt->Priopy = iss3Receipt_get->Priopy;



                shapka();



                break;
            }

            case 2:{
                emit log1("1) SSS принял от OnixB команду <b>ВЫЗОВ</b>\n");
                emit log1("\n");

                ISS3_Call*  iss3Call_get = (ISS3_Call*)massData.data();

                iss3Call->comand = S3_RTR_Otv;

                iss3Call->nbp = iss3Call_get->nbp;
                iss3Call->speed = iss3Call_get->speed;
                iss3Call->numberSendLSB = iss3Call_get->numberSendLSB;
                iss3Call->numberSendMSB = iss3Call_get->numberSendMSB;
                iss3Call->numberReceiverLSB = iss3Call_get->numberReceiverLSB;
                iss3Call->numberReceiverMSB = iss3Call_get->numberReceiverMSB;

                qDebug () << iss3Receipt_2->numberSendLSB << " = " << iss3Call_get->numberReceiverLSB;
                qDebug () << iss3Receipt_2->numberSendMSB << " = " << iss3Call_get->numberReceiverMSB;

                if(iss3Receipt_2->nbp == 0x00 && iss3Receipt_2->numberSendLSB == 0x00  && iss3Receipt_2->numberSendMSB == 0x00  && iss3Receipt_2->speed == 0x00 )
                {
                    qDebug () << "Отсутствует регистрация вызываемого абонента";

                    flag_CallPr2 = 0x02;
                    timerCall = 6;
                }
                else
                {


                    if(iss3Receipt_2->speed != iss3Receipt->speed && iss3Receipt_2->nbp != iss3Receipt->nbp)
                    {
                        qDebug () << "Не совпадение скокростей";

                        flag_CallPr2 = 0xA;
                        timerCall = 6;
                    }
                    else
                    {
                        if(iss3Receipt_2->numberSendLSB == iss3Call_get->numberReceiverLSB && iss3Receipt_2->numberSendMSB == iss3Call_get->numberReceiverMSB )
                        {
                            if(iss3Receipt->nbp == 0x00 && iss3Receipt->numberSendLSB == 0x00  && iss3Receipt->numberSendMSB == 0x00  && iss3Receipt->speed == 0x00 )
                            {
                                qDebug () << "Отсутствует регистрация вызывающего абонента";

                                flag_CallPr2 =0x01;
                                timerCall = 6;
                            }
                            else
                            {
                                qDebug () << "Номера совпали";
                            }

                        }
                        else
                        {
                            qDebug () << "Номера НЕ совпали";

                            flag_CallPr2 =S3_Ok_REG;
                            timerCall = 6;
                        }
                    }

                }





                if(timerCall < 5)
                {
                    Mod_Onix_1 = S3_CALL;
                    Mod_SSS_1 = S3_RTR_Otv;
                    Mod_SSS_2 = S3_CALL;
                  //  Mod_Onix_2 = S3_FREE;


                    iss3Call_2->comand = S3_CALL;

                    //Перевернули
                    iss3Call_2->nbp = iss3Call_get->nbp;
                    iss3Call_2->speed = iss3Call_get->speed;
                    iss3Call_2->numberSendLSB = iss3Call_get->numberReceiverLSB;
                    iss3Call_2->numberSendMSB = iss3Call_get->numberReceiverMSB;
                    iss3Call_2->numberReceiverLSB = iss3Call_get->numberSendLSB;
                    iss3Call_2->numberReceiverMSB = iss3Call_get->numberSendMSB;

                    timerCall++;
                }
                else
                {
                    Mod_Onix_1 = S3_CALL;
                    Mod_SSS_1 = S3_RTR;
                    Mod_SSS_2 = S3_FREE_SSS;
                    Mod_Onix_2 = S3_FREE;

                    iss3Refusal->comand = S3_RTR;


                    switch (flag_CallPr2) {
                    case S3_RTR:iss3Refusal->why = S3_Ok_Cansel_REG; break;
                    case S3_REG_Cansel: iss3Refusal->why = S3_REG_Cansel; break;
                    case S3_Ok_REG: iss3Refusal->why = S3_Ok_REG; flag_CallPr2 = S3_FREE_SSS;  break;
                    case 0x01: iss3Refusal->why = 0x01; flag_CallPr2 = S3_FREE_SSS;  break;
                    case 0xA: iss3Refusal->why = 0xA; flag_CallPr2 = S3_FREE_SSS;  break;
                    default: iss3Refusal->why = S3_RTR_Otv;break; //0x06 (0110)

                    }


                    iss3Refusal->numberReceiverLSB = iss3Call->numberSendLSB;
                    iss3Refusal->numberReceiverMSB = iss3Call->numberSendMSB;
                    iss3Refusal->numberSendLSB = iss3Call->numberReceiverLSB;
                    iss3Refusal->numberSendMSB = iss3Call->numberReceiverMSB;


                }



                break;
            }

            case 3:
            {

                emit log1("1) SSS принял от OnixB команду <b>СВОБОДЕН</b>\n");
                emit log1("\n");

                Mod_Onix_1 = S3_FREE;
                Mod_SSS_1 = S3_ZS;
                Mod_SSS_2 = S3_FREE_SSS;
               // Mod_Onix_2 = S3_ZS;

                ISS3_Free*  iss3Free_get = (ISS3_Free*)massData.data();



                break;
            }

            case 4:
            {
                emit log1("1) SSS принял от OnixB команду <b>ОВТЕТ ЗС</b>\n");
                emit log1("\n");

                Mod_Onix_1 = S3_ZS;
                Mod_SSS_1 = S3_CALL;
                Mod_SSS_2 = S3_ZS;
               // Mod_Onix_2 = S3_CALL;


                ISS3_Call*  iss3Call_get = (ISS3_Call*)massData2.data();

                iss3Call->comand = Mod_SSS_1;

                iss3Call->nbp = iss3Receipt_2->nbp;
                iss3Call->speed = iss3Receipt_2->speed;

//                iss3Call->numberSendLSB = iss3Call->numberSendLSB;
//                iss3Call->numberSendMSB = iss3Call->numberSendMSB;
//                iss3Call->numberReceiverLSB = iss3Call->numberReceiverLSB;
//                iss3Call->numberReceiverMSB = iss3Call->numberReceiverMSB;

                iss3Call_2->comand = Mod_SSS_2;

                iss3Call_2->nbp = iss3Receipt->nbp;
                iss3Call_2->speed = iss3Receipt->speed;

                iss3Call_2->numberSendLSB = iss3Call_get->numberReceiverLSB;
                iss3Call_2->numberSendMSB = iss3Call_get->numberReceiverMSB;
                iss3Call_2->numberReceiverLSB =  iss3Call_get->numberSendLSB;
                iss3Call_2->numberReceiverMSB =  iss3Call_get->numberSendMSB;



                shapka2();


                break;
            }

            case 5:
            {
                emit log1("1) SSS принял от OnixB команду <b>ОТБОЙ</b>\n");
                emit log1("\n");

                Mod_Onix_1 = S3_HANGUP;
                Mod_SSS_1 = S3_HANGUP;
                Mod_SSS_2 = S3_HANGUP;
                Mod_Onix_2 = S3_FREE;



                ISS3_Call*  iss3Call_get = (ISS3_Call*)massData2.data();

                iss3Call_2->comand = S3_HANGUP;

                iss3Call->comand = S3_HANGUP;

                shapka();
                shapka2();

                break;
            }

            case 6:
            {
                emit log1("1) SSS принял от OnixB команду <b>ОВТЕТ ЗС</b>\n");
                emit log1("\n");

                Mod_Onix_1 = S3_ZS;
                Mod_SSS_1 = S3_FREE_SSS;
                Mod_SSS_2 = S3_ZS;
                Mod_Onix_2 = S3_FREE;


                ISS3_Call*  iss3Call_get = (ISS3_Call*)massData2.data();

                iss3Call->comand = Mod_SSS_1;

                iss3Call->nbp = iss3Receipt_2->nbp;
                iss3Call->speed = iss3Receipt_2->speed;

//                iss3Call->numberSendLSB = iss3Call->numberSendLSB;
//                iss3Call->numberSendMSB = iss3Call->numberSendMSB;
//                iss3Call->numberReceiverLSB = iss3Call->numberReceiverLSB;
//                iss3Call->numberReceiverMSB = iss3Call->numberReceiverMSB;

                iss3Call_2->comand = Mod_SSS_2;

                iss3Call_2->nbp = iss3Receipt->nbp;
                iss3Call_2->speed = iss3Receipt->speed;

                iss3Call_2->numberSendLSB = iss3Call_get->numberReceiverLSB;
                iss3Call_2->numberSendMSB =  iss3Call_get->numberReceiverMSB;
                iss3Call_2->numberReceiverLSB =  iss3Call_get->numberSendLSB;
                iss3Call_2->numberReceiverMSB = iss3Call_get->numberSendMSB;



                shapka2();



                break;
            }

            case 7:
            {
                emit log1("1) SSS принял от OnixB команду <b>РАБОТА</b>\n");
                emit log1("\n");

                Mod_Onix_1 = S3_WORK;
                Mod_SSS_1 = S3_FREE_SSS;
                Mod_SSS_2 = S3_WORK;
                Mod_Onix_2 = S3_FREE;


                ISS3_Call*  iss3Call_get = (ISS3_Call*)massData2.data();

                iss3Call->comand = Mod_SSS_1;

                iss3Call->nbp = iss3Receipt_2->nbp;
                iss3Call->speed = iss3Receipt_2->speed;

//                iss3Call->numberSendLSB = iss3Call->numberSendLSB;
//                iss3Call->numberSendMSB = iss3Call->numberSendMSB;
//                iss3Call->numberReceiverLSB = iss3Call->numberReceiverLSB;
//                iss3Call->numberReceiverMSB = iss3Call->numberReceiverMSB;

                iss3Call_2->comand = Mod_SSS_2;

                iss3Call_2->nbp = iss3Receipt->nbp;
                iss3Call_2->speed = iss3Receipt->speed;

                iss3Call_2->numberSendLSB = iss3Call_get->numberReceiverLSB;
                iss3Call_2->numberSendMSB = iss3Call_get->numberReceiverMSB;
                iss3Call_2->numberReceiverLSB = iss3Call_get->numberSendLSB;
                iss3Call_2->numberReceiverMSB = iss3Call_get->numberSendMSB;



                shapka2();
                break;
            }

            case 8:
            {
                emit log1("1) SSS принял от OnixB команду <b>ИС</b>\n");
                emit log1("\n");

                Mod_Onix_1 = S3_IS;
                Mod_SSS_1 = S3_IS;
                Mod_SSS_2 = S3_IS;
                Mod_Onix_2 = S3_IS;

                break;
            }

            case 9:{
                emit log1("1) SSS принял от OnixB команду <b>ОТКАЗ ОТ РЕГИСТРАЦИИ</b>\n");
                emit log1("\n");


                Mod_Onix_1 = S3_REG_Cansel;
                Mod_SSS_1 = S3_Ok_Cansel_REG;
                Mod_SSS_2 =  S3_FREE_SSS;
                //Mod_Onix_2 = S3_FREE;



                ISS3_Receipt*  iss3Receipt_get = (ISS3_Receipt*)massData2.data();

                iss3Receipt->Rezerv1 = 0x00;
                iss3Receipt->Rezerv2 = 0x00;
                iss3Receipt->nbp = 0x00;
                iss3Receipt->speed = 0x00;
                iss3Receipt->numberSendLSB = 0x00;
                iss3Receipt->numberSendMSB = 0x00;
                iss3Receipt->RChVK = 0x00;
                iss3Receipt->Priopy = 0x00;

                shapka();

                break;
            }

            default: {

                ISS3_Free*  iss3Free_get = (ISS3_Free*)massData2.data();

                emit log1("1) SSS принял от OnixB команду <b>CВОБОДЕН</b>\n");
                emit log1("\n");

                break;
            }

        }

        massData.clear();

    }
}

void Sss3::readSSS2(char data)
{
    massData2.append(data);

    if(massData2.count() == 1)
    {
        quint16 komand = massData2.at(0) & 0xF;

        switch (komand) {

            case S3_FREE:
            {


                if(Mod_SSS_2 == S3_HANGUP && Mod_Onix_1 == S3_HANGUP)
                {
                    flag_messege2 = -1; //принял команду СВОБОДЕН на фоне  ОТБОЙ
                    return;
                }

                if(Mod_SSS_2 == S3_HANGUP)
                {
                    flag_messege2 = 0; //принял команду СВОБОДЕН на фоне  ОТБОЙ
                    return;
                }

                if(Mod_SSS_2 == S3_PRIORITY_CALL_OFF)
                {
                    flag_messege2 = 0; //принял команду СВОБОДЕН на фоне  ОТБОЙ ПО ПРИОРИТЕТУ
                    return;
                }

                if(Mod_Onix_2 == S3_FREE && Mod_SSS_2 == S3_FREE_SSS)
                {
                    timerCall2 = 0;
                    flag_messege2 = 0; //принял команду свободен ССС
                    return;
                }

                if(Mod_Onix_2 == S3_REG && Mod_SSS_2 == S3_Ok_REG)
                {
                    flag_messege2 = 0; //принял команду свободен ССС
                    return;
                }

                if(Mod_Onix_2 == S3_CALL && Mod_SSS_2 == S3_RTR)
                {
                    flag_messege2 = 0; //принял команду свободен ССС
                    return;
                }

                if(Mod_Onix_2 == S3_CALL && Mod_SSS_2 == S3_ZS)
                {
                    flag_messege2 = 3; //принял команду свободен ССС
                    return;
                }

                if(Mod_SSS_2 == S3_Ok_Cansel_REG && Mod_Onix_2 == S3_REG_Cansel)
                {
                    flag_messege2 = 0; //принял команду свободен ССС
                    return;
                }



                flag_messege2 = -1;
                qDebug () << "FREE 2";
                return;


            }

            case S3_REG:
            {
                 flag_messege2 = 1; //принял команду РЕГИСТРАЦИЯ
                 return;
            }

            case S3_CALL:
            {
                if(flag_CallPr2 == S3_RTR)
                {
                    flag_messege2 = 2;
                    timerCall2 = 6;
                    return;
                }

                if(flag_CallPr2 == S3_REG_Cansel)
                {
                    flag_messege2 = 2;
                    timerCall2 = 6;
                    return;
                }

                if(Mod_Onix_2 == S3_CALL && Mod_SSS_2 == S3_ZS )
                {
                     flag_messege2 = -1;
                     return;
                }

                if(Mod_Onix_2 == S3_CALL && Mod_SSS_2 == S3_RTR )
                {
                    flag_messege2 = -1;
                    return;
                }

                qDebug () << timerCall2;
                flag_messege2 = 2; //принял команду ВЫЗОВ
                return;
            }

            case S3_ZS:
            {
                 if(Mod_SSS_2 == S3_FREE_SSS && Mod_Onix_2 == S3_ZS)
                 {
                     emit set_otvetZS2(true);
                     flag_messege2 = 6; //принял команду ОТВЕТ ЗС
                     return;
                 }
                 if(Mod_SSS_2 == S3_IS && Mod_Onix_2 == S3_IS)
                 {
                     flag_messege2 = -1;
                     return;
                 }




                 flag_messege2 = 4; //принял команду ОТВЕТ ЗС
                 return;
            }

            case S3_HANGUP:
            {
               emit set_otvetZS2(false);
               flag_messege2 = 5; //принял команду ОТБОЙ
               return;
            }

            case S3_WORK:
            {
                Mod_Onix_2 = S3_WORK;
                flag_messege2 = 7; //принял команду РАБОТА
                return;
            }

            case S3_IS:
            {
                flag_messege2 = 8;
                return;
            }

            case S3_REG_Cansel:
            {
                flag_messege2 = 9;
                return;
            }

            default: return;

        }

    }

    if(massData2.count() == 5)
    {

        switch (flag_messege2) {
            case 0:{

//                Mod_Onix_1 = S3_FREE;
//                Mod_SSS_1 = S3_FREE_SSS;
                Mod_SSS_2 = S3_FREE_SSS;
                Mod_Onix_2 = S3_FREE;

                timerCall2 = 0;

                ISS3_Free*  iss3Free_get = (ISS3_Free*)massData2.data();

                emit log2("2) SSS принял от OnixB команду <b>CВОБОДЕН</b>\n");
                emit log2("\n");


                break;
            }

            case 1:{
                emit log2("2) SSS принял от OnixB команду <b>РЕГИСТРАЦИЯ</b>\n");
                emit log2("\n");


               // Mod_Onix_1 = S3_FREE;
                Mod_SSS_1 = S3_FREE_SSS;
                Mod_SSS_2 = S3_Ok_REG;
                Mod_Onix_2 = S3_REG;



                ISS3_Receipt*  iss3Receipt_get = (ISS3_Receipt*)massData2.data();

                iss3Receipt_2->Rezerv1 = iss3Receipt_get->Rezerv1;
                iss3Receipt_2->Rezerv2 = iss3Receipt_get->Rezerv2;
                iss3Receipt_2->nbp = iss3Receipt_get->nbp;
                iss3Receipt_2->speed = iss3Receipt_get->speed;
                iss3Receipt_2->numberSendLSB = iss3Receipt_get->numberSendLSB;
                iss3Receipt_2->numberSendMSB = iss3Receipt_get->numberSendMSB;
                iss3Receipt_2->RChVK = iss3Receipt_get->RChVK;
                iss3Receipt_2->Priopy = iss3Receipt_get->Priopy;


                shapka2();



                break;
            }

        case 2:{
            emit log2("2) SSS принял от OnixB команду <b>ВЫЗОВ</b>\n");
            emit log2("\n");

            ISS3_Call*  iss3Call_get = (ISS3_Call*)massData2.data();

            qDebug () << iss3Receipt->numberSendLSB << " = " << iss3Call_get->numberReceiverLSB;
            qDebug () << iss3Receipt->numberSendMSB << " = " << iss3Call_get->numberReceiverMSB;

            if(iss3Receipt->nbp == 0x00 && iss3Receipt->numberSendLSB == 0x00  && iss3Receipt->numberSendMSB == 0x00  && iss3Receipt->speed == 0x00 )
            {
                qDebug () << "Отсутствует регистрация вызываемого абонента";

                flag_CallPr1 = 0x02;
                timerCall2 = 6;
            }
            else
            {

                if(iss3Receipt_2->speed != iss3Receipt->speed && iss3Receipt_2->nbp != iss3Receipt->nbp)
                {
                    qDebug () << "Не совпадение скокростей";

                    flag_CallPr1 = 0xA;
                    timerCall2 = 6;
                }
                else
                {
                    if(iss3Receipt->numberSendLSB == iss3Call_get->numberReceiverLSB && iss3Receipt->numberSendMSB == iss3Call_get->numberReceiverMSB )
                    {
                        if(iss3Receipt_2->nbp == 0x00 && iss3Receipt_2->numberSendLSB == 0x00  && iss3Receipt_2->numberSendMSB == 0x00  && iss3Receipt_2->speed == 0x00 )
                        {
                            qDebug () << "Отсутствует регистрация вызывающего абонента";

                            flag_CallPr1 = 0x01;
                            timerCall2 = 6;
                        }
                        else
                        {
                            qDebug () << "Номера совпали";
                        }
                    }
                    else
                    {
                        flag_CallPr1 =S3_Ok_REG;
                        timerCall2 = 6;
                    }
                }

            }




            if(timerCall2 < 5)
            {
               // Mod_Onix_1 = S3_FREE;
                Mod_SSS_1 = S3_CALL;
                Mod_SSS_2 = S3_RTR_Otv;
                Mod_Onix_2 = S3_CALL;



                iss3Call_2->comand = iss3Call_get->comand;
                iss3Call_2->nbp = iss3Call_get->nbp;
                iss3Call_2->speed = iss3Call_get->speed;
                iss3Call_2->numberSendLSB = iss3Call_get->numberReceiverLSB;
                iss3Call_2->numberSendMSB = iss3Call_get->numberReceiverMSB;
                iss3Call_2->numberReceiverLSB = iss3Call_get->numberSendLSB;
                iss3Call_2->numberReceiverMSB = iss3Call_get->numberSendMSB;

                iss3Call->comand = Mod_SSS_2;

                iss3Call->nbp = iss3Call_get->nbp;
                iss3Call->speed = iss3Call_get->speed;
                iss3Call->numberSendLSB = iss3Call_get->numberReceiverLSB;
                iss3Call->numberSendMSB = iss3Call_get->numberReceiverMSB;
                iss3Call->numberReceiverLSB = iss3Call_get->numberSendLSB;
                iss3Call->numberReceiverMSB = iss3Call_get->numberSendMSB;

                timerCall2++;
            }
            else
            {
               // Mod_Onix_1 = S3_FREE;
                Mod_SSS_1 = S3_FREE_SSS;
                Mod_SSS_2 = S3_RTR;
                Mod_Onix_2 = S3_CALL;
                qDebug () << " flag_CallPr1 = " << flag_CallPr1;

                switch (flag_CallPr1) {
                case S3_RTR:{iss3Refusal_2->why = S3_Ok_Cansel_REG; break;}
                case S3_REG_Cansel: {iss3Refusal_2->why = S3_REG_Cansel; break;}
                case 0x02: {iss3Refusal_2->why = 0x02; flag_CallPr1 = S3_FREE_SSS;  break;}
                case 0x01: {iss3Refusal_2->why = 0x01; flag_CallPr1 = S3_FREE_SSS;  break;}
                case 0xA: {iss3Refusal_2->why = 0xA; flag_CallPr1 = S3_FREE_SSS;  break;}
                default: {iss3Refusal_2->why = S3_RTR_Otv;break;} //0x06 (0110)
                }

                qDebug () << iss3Refusal_2->why;


                iss3Refusal_2->numberReceiverLSB = iss3Call_2->numberSendLSB;
                iss3Refusal_2->numberReceiverMSB = iss3Call_2->numberSendMSB;
                iss3Refusal_2->numberSendLSB = iss3Call_2->numberReceiverLSB;
                iss3Refusal_2->numberSendMSB = iss3Call_2->numberReceiverMSB;


            }



            break;
        }

            case 3:
            {

                emit log2("2) SSS принял от OnixB команду <b>СВОБОДЕН</b>\n");
                emit log2("\n");

                //Mod_Onix_1 = S3_FREE;
                Mod_SSS_1 = S3_FREE_SSS;
                Mod_SSS_2 = S3_ZS;
                Mod_Onix_2 = S3_FREE;

                ISS3_Free*  iss3Free_get = (ISS3_Free*)massData.data();

                break;
            }

            case 4:
            {
                emit log2("2) SSS принял от OnixB команду <b>ОВТЕТ ЗС</b>\n");
                emit log2("\n");

               // Mod_Onix_1 = S3_CALL;
                Mod_SSS_1 = S3_ZS;
                Mod_SSS_2 = S3_CALL;
                Mod_Onix_2 = S3_ZS;


                ISS3_Call*  iss3Call_get = (ISS3_Call*)massData2.data();

                iss3Call_2->comand = Mod_SSS_1;

                iss3Call_2->nbp = iss3Receipt_2->nbp;
                iss3Call_2->speed = iss3Receipt_2->speed;

//                iss3Call_2->numberSendLSB = iss3Call->numberSendLSB;
//                iss3Call_2->numberSendMSB = iss3Call->numberSendMSB;
//                iss3Call_2->numberReceiverLSB = iss3Call->numberReceiverLSB;
//                iss3Call_2->numberReceiverMSB = iss3Call->numberReceiverMSB;

                iss3Call->comand = Mod_SSS_2;

                iss3Call->nbp = iss3Receipt->nbp;
                iss3Call->speed = iss3Receipt->speed;

                iss3Call->numberSendLSB = iss3Call_get->numberReceiverLSB;
                iss3Call->numberSendMSB = iss3Call_get->numberReceiverMSB;
                iss3Call->numberReceiverLSB = iss3Call_get->numberSendLSB;
                iss3Call->numberReceiverMSB = iss3Call_get->numberSendMSB;



                shapka2();



                break;
            }

            case 5:
            {
                emit log2("2) SSS принял от OnixB команду <b>ОТБОЙ</b>\n");
                emit log2("\n");

                Mod_Onix_1 = S3_FREE;
                Mod_SSS_1 = S3_HANGUP;
                Mod_SSS_2 = S3_HANGUP;
                Mod_Onix_2 = S3_HANGUP;



                ISS3_Call*  iss3Call_get = (ISS3_Call*)massData2.data();

                iss3Call_2->comand = S3_HANGUP;

                iss3Call->comand = S3_HANGUP;

                shapka();
                shapka2();

                break;
            }

            case 6:
            {
                emit log2("2) SSS принял от OnixB команду <b>ОВТЕТ ЗС</b>\n");
                emit log2("\n");

              //  Mod_Onix_1 = S3_FREE;
                Mod_SSS_1 = S3_ZS;
                Mod_SSS_2 = S3_FREE_SSS;
                Mod_Onix_2 = S3_ZS;


                ISS3_Call*  iss3Call_get = (ISS3_Call*)massData2.data();

                iss3Call_2->comand = Mod_SSS_1;

                iss3Call_2->nbp = iss3Receipt_2->nbp;
                iss3Call_2->speed = iss3Receipt_2->speed;

//                iss3Call_2->numberSendLSB = iss3Call->numberSendLSB;
//                iss3Call_2->numberSendMSB = iss3Call->numberSendMSB;
//                iss3Call_2->numberReceiverLSB = iss3Call->numberReceiverLSB;
//                iss3Call_2->numberReceiverMSB = iss3Call->numberReceiverMSB;

                iss3Call->comand = Mod_SSS_2;

                iss3Call->nbp = iss3Receipt->nbp;
                iss3Call->speed = iss3Receipt->speed;

                iss3Call->numberSendLSB = iss3Call_get->numberReceiverLSB;
                iss3Call->numberSendMSB = iss3Call_get->numberReceiverMSB;
                iss3Call->numberReceiverLSB = iss3Call_get->numberSendLSB;
                iss3Call->numberReceiverMSB = iss3Call_get->numberSendMSB;

                shapka2();

                break;
            }

            case 7:
            {
                emit log2("2) SSS принял от OnixB команду <b>РАБОТА</b>\n");
                emit log2("\n");

                Mod_Onix_1 = S3_FREE;
                Mod_SSS_1 = S3_WORK;
                Mod_SSS_2 = S3_FREE_SSS;
                Mod_Onix_2 = S3_WORK;


                ISS3_Call*  iss3Call_get = (ISS3_Call*)massData2.data();

                iss3Call_2->comand = Mod_SSS_1;

                iss3Call_2->nbp = iss3Receipt_2->nbp;
                iss3Call_2->speed = iss3Receipt_2->speed;

//                iss3Call_2->numberSendLSB = iss3Call->numberSendLSB;
//                iss3Call_2->numberSendMSB = iss3Call->numberSendMSB;
//                iss3Call_2->numberReceiverLSB = iss3Call->numberReceiverLSB;
//                iss3Call_2->numberReceiverMSB = iss3Call->numberReceiverMSB;

                iss3Call->comand = Mod_SSS_2;

                iss3Call->nbp = iss3Receipt->nbp;
                iss3Call->speed = iss3Receipt->speed;

                iss3Call->numberSendLSB = iss3Call_get->numberReceiverLSB;
                iss3Call->numberSendMSB = iss3Call_get->numberReceiverMSB;
                iss3Call->numberReceiverLSB = iss3Call_get->numberSendLSB;
                iss3Call->numberReceiverMSB = iss3Call_get->numberSendMSB;




                shapka2();
                break;
            }

        case 8:
        {
            emit log2("2) SSS принял от OnixB команду <b>ИС</b>\n");
            emit log2("\n");

            Mod_Onix_1 = S3_IS;
            Mod_SSS_1 = S3_IS;
            Mod_SSS_2 = S3_IS;
            Mod_Onix_2 = S3_IS;

            ISS3_Call*  iss3Call_get = (ISS3_Call*)massData2.data();

            iss3Call_2->comand = iss3Call_get->comand;

            iss3Call_2->nbp = iss3Call_get->nbp;
            iss3Call_2->speed = iss3Call_get->speed;

            iss3Call_2->numberSendLSB =  iss3Call_get->numberReceiverLSB;
            iss3Call_2->numberSendMSB = iss3Call_get->numberReceiverMSB;
            iss3Call_2->numberReceiverLSB = iss3Call_get->numberSendLSB;
            iss3Call_2->numberReceiverMSB = iss3Call_get->numberSendMSB;

            iss3Call->comand = iss3Call_get->comand;

            iss3Call->nbp = iss3Call_get->nbp;
            iss3Call->speed = iss3Call_get->speed;

            iss3Call->numberSendLSB = iss3Call_get->numberReceiverLSB;
            iss3Call->numberSendMSB = iss3Call_get->numberReceiverMSB;
            iss3Call->numberReceiverLSB = iss3Call_get->numberSendLSB;
            iss3Call->numberReceiverMSB = iss3Call_get->numberSendMSB;

            break;
        }

        case 9:{
            emit log2("2) SSS принял от OnixB команду <b>ОТКАЗ ОТ РЕГИСТРАЦИИ</b>\n");
            emit log2("\n");


           // Mod_Onix_1 = S3_FREE;
            Mod_SSS_1 = S3_FREE_SSS;
            Mod_SSS_2 = S3_Ok_Cansel_REG;
            Mod_Onix_2 = S3_REG_Cansel;



            ISS3_Receipt*  iss3Receipt_get = (ISS3_Receipt*)massData2.data();

            iss3Receipt_2->Rezerv1 = 0x00;
            iss3Receipt_2->Rezerv2 = 0x00;
            iss3Receipt_2->nbp = 0x00;
            iss3Receipt_2->speed = 0x00;
            iss3Receipt_2->numberSendLSB = 0x00;
            iss3Receipt_2->numberSendMSB = 0x00;
            iss3Receipt_2->RChVK = 0x00;
            iss3Receipt_2->Priopy = 0x00;


            shapka2();



            break;
        }


            default: {

                ISS3_Free*  iss3Free_get = (ISS3_Free*)massData2.data();

                emit log2("2) SSS принял от OnixB команду <b>CВОБОДЕН</b>\n");
                emit log2("\n");


                break;
            }

        }

        massData2.clear();

    }
}

void Sss3::startSimulation(int time)
{
    timer->start(time);
}

void Sss3::stopSimulation()
{
     timer->stop();
}

void Sss3::clearAll()
{

    kol = 0;
    kol2 = 0;

    iss3Free->comand = S3_FREE_SSS;
    iss3Free->Rezerv2 = 0x00;
    iss3Free->RChVK = 0x00;
    iss3Free->Rezerv1 = 0x00;
    iss3Free->nbp = 0x00;
    iss3Free->speed = 0x00;
    iss3Free->numberSendLSB = 0x00;
    iss3Free->numberSendMSB = 0x00;
    iss3Free->Priopy = 0x00;

    iss3Receipt->comand = S3_Ok_REG;
    iss3Receipt->Rezerv1 = 0x00;
    iss3Receipt->Rezerv2 = 0x00;
    iss3Receipt->nbp = 0x00;
    iss3Receipt->speed = 0x00;
    iss3Receipt->numberSendLSB = 0x00;
    iss3Receipt->numberSendMSB = 0x00;

    iss3Refusal->comand = S3_RTR;
    iss3Refusal->why = S3_RTR_Otv; //0x06 (0110)
    iss3Refusal->numberReceiverLSB = 0x00;
    iss3Refusal->numberReceiverMSB = 0x00;
    iss3Refusal->numberSendLSB = 0x00;
    iss3Refusal->numberSendMSB = 0x00;



    iss3Free_2->comand = S3_FREE_SSS;
    iss3Free_2->Rezerv2 = 0x00;
    iss3Free_2->RChVK = 0x00;
    iss3Free_2->Rezerv1 = 0x00;
    iss3Free_2->nbp = 0x00;
    iss3Free_2->speed = 0x00;
    iss3Free_2->Priopy = 0x00;
    iss3Free_2->numberSendLSB = 0x00;
    iss3Free_2->numberSendMSB = 0x00;

    iss3Receipt_2->comand = S3_Ok_REG;
    iss3Receipt_2->Rezerv1 = 0x00;
    iss3Receipt_2->Rezerv2 = 0x00;
    iss3Receipt_2->nbp = 0x00;
    iss3Receipt_2->speed = 0x00;
    iss3Receipt_2->numberSendLSB = 0x00;
    iss3Receipt_2->numberSendMSB = 0x00;



    flag_messege =0;
    flag_messege2 =0;

    massData.clear();
    massData2.clear();

    systemSchet =2 ;

    timerCall = 1;
    timerCall2 = 1;

    flagReg = false;
    flagReg2 = false;

}


void Sss3::free()
{
    //  "СВОБОДЕН" "СВОБОДНА ССС" "СВОБОДНА ССС" " СВОБОДЕН"
    if( Mod_Onix_1 == S3_FREE && Mod_SSS_1 == S3_FREE_SSS && Mod_SSS_2 == S3_FREE_SSS && Mod_Onix_2 == S3_FREE)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        ////////////////////////////////////////////////////////////

        emit log1("1) SSS отправил команду <b>CВОБОДНА ССС</b>\n");
        emit log1("1 Байт( Cкорость: <b>"+get_Byte2(iss3Free->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Free->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Free->comand)+"</b>)\n");
        emit log1("2 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendLSB)+" </b>)\n");
        emit log1("3 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendMSB)+" </b>)\n");
        emit log1("4 Байт( Резерв: <b>"+get_Byte6(iss3Free->Rezerv1)+"</b> РЧВК: <b>" +get_Byte2(iss3Free->RChVK)+"</b>)\n");
        emit log1("5 Байт( Резерв: <b>"+get_Byte4(iss3Free->Rezerv2)+"</b> Приоритет: <b>" +get_Byte4(iss3Free->Priopy)+"</b>)\n");
        emit log1("\n");

        /////////////////////////////////////////////////////////////////////////

        emit log2("2) SSS отправил команду <b>CВОБОДНА ССС</b>\n");
        emit log2("1 Байт( Cкорость: <b>"+get_Byte2(iss3Free_2->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Free_2->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Free_2->comand)+"</b>)\n");
        emit log2("2 Байт( байт номера: <b>" +get_Byte8(iss3Free_2->numberSendLSB)+" </b>)\n");
        emit log2("3 Байт( байт номера: <b>" +get_Byte8(iss3Free_2->numberSendMSB)+" </b>)\n");
        emit log2("4 Байт( Резерв: <b>"+get_Byte6(iss3Free_2->Rezerv1)+"</b> РЧВК: <b>" +get_Byte2(iss3Free_2->RChVK)+"</b>)\n");
        emit log2("5 Байт( Резерв: <b>"+get_Byte4(iss3Free_2->Rezerv2)+"</b> Приоритет: <b>" +get_Byte4(iss3Free_2->Priopy)+"</b>)\n");
        emit log2("\n");



        QByteArray *mass =new QByteArray();

        mass->append(static_cast<char>((iss3Free->speed << 6) +(iss3Free->nbp << 4) + iss3Free->comand));
        mass->append(static_cast<char>(iss3Free->numberSendLSB));
        mass->append(static_cast<char>(iss3Free->numberSendMSB));
        mass->append(static_cast<char>((iss3Free->Rezerv1 << 2) + iss3Free->RChVK));
        mass->append(static_cast<char>((iss3Free->Rezerv2 << 4) + iss3Free->Priopy));

        QByteArray *mass_2 =new QByteArray();

        mass_2->append(static_cast<char>((iss3Free_2->speed << 6) +(iss3Free_2->nbp << 4) + iss3Free_2->comand));
        mass_2->append(static_cast<char>(iss3Free_2->numberSendLSB));
        mass_2->append(static_cast<char>(iss3Free_2->numberSendMSB));
        mass_2->append(static_cast<char>((iss3Free_2->Rezerv1 << 2) + iss3Free_2->RChVK));
        mass_2->append(static_cast<char>((iss3Free_2->Rezerv2 << 4) + iss3Free_2->Priopy));


        for (int i=0; i < mass->count();i++) {

            emit writeSSS(mass->at(i));
            emit writeSSS2(mass_2->at(i));
        }



        return;
    }

    ////////////////////////////////////////////////////////////////
    //  "РЕГИСТРАЦИЯ" "ПОДТВЕРЖДЕНИЕ РЕГИСТРАЦИИ" "СВОБОДНА ССС" " СВОБОДЕН"
    if( Mod_Onix_1 == S3_REG && Mod_SSS_1 == S3_Ok_REG && Mod_SSS_2 == S3_FREE_SSS && Mod_Onix_2 == S3_FREE)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        ////////////////////////////////////////////////////////////

        emit log1("1) SSS отправил команду <b>ПОДТВЕРЖДЕНИЕ РЕГИСТРАЦИИ</b>\n");
        emit log1("1 Байт( Cкорость: <b>"+get_Byte2(iss3Receipt->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Receipt->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Receipt->comand)+"</b>)\n");
        emit log1("2 Байт( байт номера: <b>" +get_Byte8(iss3Receipt->numberSendLSB)+" </b>)\n");
        emit log1("3 Байт( байт номера: <b>" +get_Byte8(iss3Receipt->numberSendMSB)+" </b>)\n");
        emit log1("4 Байт( Резерв: <b>"+get_Byte6(iss3Receipt->Rezerv1)+"</b> РЧВК: <b>" +get_Byte2(iss3Receipt->RChVK)+"</b>)\n");
        emit log1("5 Байт( Резерв: <b>"+get_Byte4(iss3Receipt->Rezerv2)+"</b> Приоритет: <b>" +get_Byte4(iss3Receipt->Priopy)+"</b>)\n");
        emit log1("\n");

        /////////////////////////////////////////////////////////////////////////

        emit log2("2) SSS отправил команду <b>CВОБОДНА ССС</b>\n");
        emit log2("1 Байт( Cкорость: <b>"+get_Byte2(iss3Free_2->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Free_2->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Free_2->comand)+"</b>)\n");
        emit log2("2 Байт( байт номера: <b>" +get_Byte8(iss3Free_2->numberSendLSB)+" </b>)\n");
        emit log2("3 Байт( байт номера: <b>" +get_Byte8(iss3Free_2->numberSendMSB)+" </b>)\n");
        emit log2("4 Байт( Резерв: <b>"+get_Byte6(iss3Free_2->Rezerv1)+"</b> РЧВК: <b>" +get_Byte2(iss3Free_2->RChVK)+"</b>)\n");
        emit log2("5 Байт( Резерв: <b>"+get_Byte4(iss3Free_2->Rezerv2)+"</b> Приоритет: <b>" +get_Byte4(iss3Free_2->Priopy)+"</b>)\n");
        emit log2("\n");


        QByteArray *mass =new QByteArray();

        mass->append(static_cast<char>((iss3Receipt->speed << 6) +(iss3Receipt->nbp << 4) + iss3Receipt->comand));
        mass->append(static_cast<char>(iss3Receipt->numberSendLSB));
        mass->append(static_cast<char>(iss3Receipt->numberSendMSB));
        mass->append(static_cast<char>((iss3Receipt->Rezerv1 << 2) + iss3Receipt->RChVK));
        mass->append(static_cast<char>((iss3Receipt->Rezerv2 << 4) + iss3Receipt->Priopy));



        QByteArray *mass_2 =new QByteArray();

        mass_2->append(static_cast<char>((iss3Free_2->speed << 6) +(iss3Free_2->nbp << 4) + iss3Free_2->comand));
        mass_2->append(static_cast<char>(iss3Free_2->numberSendLSB));
        mass_2->append(static_cast<char>(iss3Free_2->numberSendMSB));
        mass_2->append(static_cast<char>((iss3Free_2->Rezerv1 << 2) + iss3Free_2->RChVK));
        mass_2->append(static_cast<char>((iss3Free_2->Rezerv2 << 4) + iss3Free_2->Priopy));



        for (int i=0; i < mass->count();i++) {

            emit writeSSS(mass->at(i));
            emit writeSSS2(mass_2->at(i));
        }

        flagReg = true;

        emit set_regMod1(flagReg);


        return;
    }

    ////////////////////////////////////////////////////////////////
    //  "СВОБОДЕН" "СВОБОДНА ССС" "ПОДТВЕРЖДЕНИЕ РЕГИСТРАЦИИ" "РЕГИСТРАЦИЯ"
    if( Mod_Onix_1 == S3_FREE  && Mod_SSS_1 == S3_FREE_SSS  && Mod_SSS_2 == S3_Ok_REG && Mod_Onix_2 == S3_REG)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        ////////////////////////////////////////////////////////////

        emit log1("1) SSS отправил команду <b>CВОБОДНА ССС</b>\n");
        emit log1("1 Байт( Cкорость: <b>"+get_Byte2(iss3Free->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Free->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Free->comand)+"</b>)\n");
        emit log1("2 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendLSB)+" </b>)\n");
        emit log1("3 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendMSB)+" </b>)\n");
        emit log1("4 Байт( Резерв: <b>"+get_Byte6(iss3Free->Rezerv1)+"</b> РЧВК: <b>" +get_Byte2(iss3Free->RChVK)+"</b>)\n");
        emit log1("5 Байт( Резерв: <b>"+get_Byte4(iss3Free->Rezerv2)+"</b> Приоритет: <b>" +get_Byte4(iss3Free->Priopy)+"</b>)\n");
        emit log1("\n");


        /////////////////////////////////////////////////////////////////////////

        emit log2("2) SSS отправил команду <b>ПОДТВЕРЖДЕНИЕ РЕГИСТРАЦИИ</b>\n");
        emit log2("1 Байт( Cкорость: <b>"+get_Byte2(iss3Receipt_2->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Receipt_2->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Receipt_2->comand)+"</b>)\n");
        emit log2("2 Байт( байт номера: <b>" +get_Byte8(iss3Receipt_2->numberSendLSB)+" </b>)\n");
        emit log2("3 Байт( байт номера: <b>" +get_Byte8(iss3Receipt_2->numberSendMSB)+" </b>)\n");
        emit log2("4 Байт( Резерв: <b>"+get_Byte6(iss3Receipt_2->Rezerv1)+"</b> РЧВК: <b>" +get_Byte2(iss3Receipt_2->RChVK)+"</b>)\n");
        emit log2("5 Байт( Резерв: <b>"+get_Byte4(iss3Receipt_2->Rezerv2)+"</b> Приоритет: <b>" +get_Byte4(iss3Receipt_2->Priopy)+"</b>)\n");
        emit log2("\n");




        QByteArray *mass =new QByteArray();

        mass->append(static_cast<char>((iss3Free->speed << 6) +(iss3Free->nbp << 4) + iss3Free->comand));
        mass->append(static_cast<char>(iss3Free->numberSendLSB));
        mass->append(static_cast<char>(iss3Free->numberSendMSB));
        mass->append(static_cast<char>((iss3Free->Rezerv1 << 2) + iss3Free->RChVK));
        mass->append(static_cast<char>((iss3Free->Rezerv2 << 4) + iss3Free->Priopy));

        QByteArray *mass_2 =new QByteArray();

        mass_2->append(static_cast<char>((iss3Receipt_2->speed << 6) +(iss3Receipt_2->nbp << 4) + iss3Receipt_2->comand));
        mass_2->append(static_cast<char>(iss3Receipt_2->numberSendLSB));
        mass_2->append(static_cast<char>(iss3Receipt_2->numberSendMSB));
        mass_2->append(static_cast<char>((iss3Receipt_2->Rezerv1 << 2) + iss3Receipt_2->RChVK));
        mass_2->append(static_cast<char>((iss3Receipt_2->Rezerv2 << 4) + iss3Receipt_2->Priopy));


        for (int i=0; i < mass->count();i++) {

            emit writeSSS(mass->at(i));

            emit writeSSS2(mass_2->at(i));
        }

        flagReg2 = true;

        emit set_regMod2(flagReg2);


        return;
    }

    ////////////////////////////////////////////////////////////////
    //  "СВОБОДЕН" "СВОБОДНА ССС" "ПОДТВЕРЖДЕНИЕ ОТКАЗА ОТ РЕГИСТРАЦИИ" "ОТКАЗ ОТ РЕГИСТРАЦИИ"
    if( Mod_Onix_1 == S3_FREE  && Mod_SSS_1 == S3_FREE_SSS  && Mod_SSS_2 == S3_Ok_Cansel_REG && Mod_Onix_2 == S3_REG_Cansel)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        ////////////////////////////////////////////////////////////

        emit log1("1) SSS отправил команду <b>CВОБОДНА ССС</b>\n");
        emit log1("1 Байт( Cкорость: <b>"+get_Byte2(iss3Free->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Free->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Free->comand)+"</b>)\n");
        emit log1("2 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendLSB)+" </b>)\n");
        emit log1("3 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendMSB)+" </b>)\n");
        emit log1("4 Байт( Резерв: <b>"+get_Byte6(iss3Free->Rezerv1)+"</b> РЧВК: <b>" +get_Byte2(iss3Free->RChVK)+"</b>)\n");
        emit log1("5 Байт( Резерв: <b>"+get_Byte4(iss3Free->Rezerv2)+"</b> Приоритет: <b>" +get_Byte4(iss3Free->Priopy)+"</b>)\n");
        emit log1("\n");


        /////////////////////////////////////////////////////////////////////////

        emit log2("2) SSS отправил команду <b>ПОДТВЕРЖДЕНИЕ ОТКАЗА ОТ РЕГИСТРАЦИИ</b>\n");
        emit log2("1 Байт( Cкорость: <b>"+get_Byte2(iss3Receipt_2->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Receipt_2->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Receipt_2->comand)+"</b>)\n");
        emit log2("2 Байт( байт номера: <b>" +get_Byte8(iss3Receipt_2->numberSendLSB)+" </b>)\n");
        emit log2("3 Байт( байт номера: <b>" +get_Byte8(iss3Receipt_2->numberSendMSB)+" </b>)\n");
        emit log2("4 Байт( Резерв: <b>"+get_Byte6(iss3Receipt_2->Rezerv1)+"</b> РЧВК: <b>" +get_Byte2(iss3Receipt_2->RChVK)+"</b>)\n");
        emit log2("5 Байт( Резерв: <b>"+get_Byte4(iss3Receipt_2->Rezerv2)+"</b> Приоритет: <b>" +get_Byte4(iss3Receipt_2->Priopy)+"</b>)\n");
        emit log2("\n");




        QByteArray *mass =new QByteArray();

        mass->append(static_cast<char>((iss3Free->speed << 6) +(iss3Free->nbp << 4) + iss3Free->comand));
        mass->append(static_cast<char>(iss3Free->numberSendLSB));
        mass->append(static_cast<char>(iss3Free->numberSendMSB));
        mass->append(static_cast<char>((iss3Free->Rezerv1 << 2) + iss3Free->RChVK));
        mass->append(static_cast<char>((iss3Free->Rezerv2 << 4) + iss3Free->Priopy));

        QByteArray *mass_2 =new QByteArray();

        mass_2->append(static_cast<char>((iss3Receipt_2->speed << 6) +(iss3Receipt_2->nbp << 4) + iss3Receipt_2->comand));
        mass_2->append(static_cast<char>(iss3Receipt_2->numberSendLSB));
        mass_2->append(static_cast<char>(iss3Receipt_2->numberSendMSB));
        mass_2->append(static_cast<char>((iss3Receipt_2->Rezerv1 << 2) + iss3Receipt_2->RChVK));
        mass_2->append(static_cast<char>((iss3Receipt_2->Rezerv2 << 4) + iss3Receipt_2->Priopy));


        for (int i=0; i < mass->count();i++) {

            emit writeSSS(mass->at(i));

            emit writeSSS2(mass_2->at(i));
        }

        flagReg2 = false;

        emit set_regMod2(flagReg2);


        return;
    }

    ////////////////////////////////////////////////////////////////
    // "ОТКАЗ ОТ РЕГИСТРАЦИИ" "ПОДТВЕРЖДЕНИЕ ОТКАЗА ОТ РЕГИСТРАЦИИ" "СВОБОДЕН" "СВОБОДНА ССС"
    if( Mod_Onix_1 == S3_REG_Cansel   && Mod_SSS_1 == S3_Ok_Cansel_REG   && Mod_SSS_2 == S3_FREE_SSS && Mod_Onix_2 == S3_FREE)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        ////////////////////////////////////////////////////////////


        emit log1("1) SSS отправил команду <b>ПОДТВЕРЖДЕНИЕ ОТКАЗА ОТ РЕГИСТРАЦИИ</b>\n");
        emit log1("1 Байт( Cкорость: <b>"+get_Byte2(iss3Receipt->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Receipt->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Receipt->comand)+"</b>)\n");
        emit log1("2 Байт( байт номера: <b>" +get_Byte8(iss3Receipt->numberSendLSB)+" </b>)\n");
        emit log1("3 Байт( байт номера: <b>" +get_Byte8(iss3Receipt->numberSendMSB)+" </b>)\n");
        emit log1("4 Байт( Резерв: <b>"+get_Byte6(iss3Receipt->Rezerv1)+"</b> РЧВК: <b>" +get_Byte2(iss3Receipt->RChVK)+"</b>)\n");
        emit log1("5 Байт( Резерв: <b>"+get_Byte4(iss3Receipt->Rezerv2)+"</b> Приоритет: <b>" +get_Byte4(iss3Receipt->Priopy)+"</b>)\n");
        emit log1("\n");


        /////////////////////////////////////////////////////////////////////////

        emit log2("2) SSS отправил команду <b>CВОБОДНА ССС</b>\n");
        emit log2("1 Байт( Cкорость: <b>"+get_Byte2(iss3Free_2->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Free_2->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Free_2->comand)+"</b>)\n");
        emit log2("2 Байт( байт номера: <b>" +get_Byte8(iss3Free_2->numberSendLSB)+" </b>)\n");
        emit log2("3 Байт( байт номера: <b>" +get_Byte8(iss3Free_2->numberSendMSB)+" </b>)\n");
        emit log2("4 Байт( Резерв: <b>"+get_Byte6(iss3Free_2->Rezerv1)+"</b> РЧВК: <b>" +get_Byte2(iss3Free_2->RChVK)+"</b>)\n");
        emit log2("5 Байт( Резерв: <b>"+get_Byte4(iss3Free_2->Rezerv2)+"</b> Приоритет: <b>" +get_Byte4(iss3Free_2->Priopy)+"</b>)\n");
        emit log2("\n");




        QByteArray *mass =new QByteArray();

        mass->append(static_cast<char>((iss3Receipt->speed << 6) +(iss3Receipt->nbp << 4) + iss3Receipt->comand));
        mass->append(static_cast<char>(iss3Receipt->numberSendLSB));
        mass->append(static_cast<char>(iss3Receipt->numberSendMSB));
        mass->append(static_cast<char>((iss3Receipt->Rezerv1 << 2) + iss3Receipt->RChVK));
        mass->append(static_cast<char>((iss3Receipt->Rezerv2 << 4) + iss3Receipt->Priopy));

        QByteArray *mass_2 =new QByteArray();

        mass_2->append(static_cast<char>((iss3Free_2->speed << 6) +(iss3Free_2->nbp << 4) + iss3Free_2->comand));
        mass_2->append(static_cast<char>(iss3Free_2->numberSendLSB));
        mass_2->append(static_cast<char>(iss3Free_2->numberSendMSB));
        mass_2->append(static_cast<char>((iss3Free_2->Rezerv1 << 2) + iss3Free_2->RChVK));
        mass_2->append(static_cast<char>((iss3Free_2->Rezerv2 << 4) + iss3Free_2->Priopy));


        for (int i=0; i < mass->count();i++) {

            emit writeSSS(mass->at(i));

            emit writeSSS2(mass_2->at(i));
        }

        flagReg = false;

        emit set_regMod1(flagReg);


        return;
    }



    ////////////////////////////////////////////////////////////////
    //  "ВЫЗОВ" "ОТВЕТ РТР" "ВЫЗОВ" "СВОБОДЕН"
    if( Mod_Onix_1 == S3_CALL && Mod_SSS_1 == S3_RTR_Otv && Mod_SSS_2 == S3_CALL && Mod_Onix_2 == S3_FREE)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        ////////////////////////////////////////////////////////////

        iss3Call->comand = S3_RTR_Otv;

        emit log1("1) SSS отправил команду <b>ОТВЕТ РТР</b>\n");
        emit log1("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call->comand)+"</b>)\n");
        emit log1("2 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberSendLSB)+" </b>)\n");
        emit log1("3 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberSendMSB)+" </b>)\n");
        emit log1("4 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberReceiverLSB)+" </b>)\n");
        emit log1("5 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberReceiverMSB)+" </b>)\n");
        emit log1("\n");

        /////////////////////////////////////////////////////////////////////////

        emit log2("2) SSS отправил команду <b>ВЫЗОВ</b>\n");
        emit log2("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call_2->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call_2->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call_2->comand)+"</b>)\n");
        emit log2("2 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberSendLSB)+" </b>)\n");
        emit log2("3 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberSendMSB)+" </b>)\n");
        emit log2("4 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberReceiverLSB)+" </b>)\n");
        emit log2("5 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberReceiverMSB)+" </b>)\n");
        emit log2("\n");



        QByteArray *mass =new QByteArray();

        mass->append(static_cast<char>((iss3Call->speed << 6) +(iss3Call->nbp << 4) + iss3Call->comand));
        mass->append(static_cast<char>(iss3Call->numberSendLSB));
        mass->append(static_cast<char>(iss3Call->numberSendMSB));
        mass->append(static_cast<char>(iss3Call->numberReceiverLSB));
        mass->append(static_cast<char>(iss3Call->numberReceiverMSB));

        QByteArray *mass_2 =new QByteArray();

        mass_2->append(static_cast<char>((iss3Call_2->speed << 6) +(iss3Call_2->nbp << 4) + iss3Call_2->comand));
        mass_2->append(static_cast<char>(iss3Call_2->numberSendLSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberSendMSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberReceiverLSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberReceiverMSB));


        for (int i=0; i < mass->count();i++) {

            emit writeSSS(mass->at(i));
            emit writeSSS2(mass_2->at(i));
        }



        return;
    }

    ////////////////////////////////////////////////////////////////

    // "ВЫЗОВ" "ОТКАЗ РТР" "СВОБОДНА ССС" "СВОБОДЕН"

    if( Mod_Onix_1 == S3_CALL && Mod_SSS_1 == S3_RTR && Mod_SSS_2 == S3_FREE_SSS && Mod_Onix_2 == S3_FREE)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        ////////////////////////////////////////////////////////////


        emit log1("1) SSS отправил команду <b>ОТКАЗ РТР</b>\n");
        emit log1("1 Байт( Причина: <b>"+get_Byte4(iss3Refusal->why)+"</b> Команда: <b>"+get_Byte4(iss3Refusal->comand)+"</b>)\n");
        emit log1("2 Байт( байт номера: <b>" +get_Byte8(iss3Refusal->numberSendLSB)+" </b>)\n");
        emit log1("3 Байт( байт номера: <b>" +get_Byte8(iss3Refusal->numberSendMSB)+" </b>)\n");
        emit log1("4 Байт( байт номера: <b>" +get_Byte8(iss3Refusal->numberReceiverLSB)+" </b>)\n");
        emit log1("5 Байт( байт номера: <b>" +get_Byte8(iss3Refusal->numberReceiverMSB)+" </b>)\n");
        emit log1("\n");

        /////////////////////////////////////////////////////////////////////////

        emit log2("2) SSS отправил команду <b>CВОБОДНА ССС</b>\n");
        emit log2("1 Байт( Cкорость: <b>"+get_Byte2(iss3Free_2->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Free_2->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Free_2->comand)+"</b>)\n");
        emit log2("2 Байт( байт номера: <b>" +get_Byte8(iss3Free_2->numberSendLSB)+" </b>)\n");
        emit log2("3 Байт( байт номера: <b>" +get_Byte8(iss3Free_2->numberSendMSB)+" </b>)\n");
        emit log2("4 Байт( Резерв: <b>"+get_Byte6(iss3Free_2->Rezerv1)+"</b> РЧВК: <b>" +get_Byte2(iss3Free_2->RChVK)+"</b>)\n");
        emit log2("5 Байт( Резерв: <b>"+get_Byte4(iss3Free_2->Rezerv2)+"</b> Приоритет: <b>" +get_Byte4(iss3Free_2->Priopy)+"</b>)\n");
        emit log2("\n");



        QByteArray *mass =new QByteArray();

        mass->append(static_cast<char>((iss3Refusal->why << 4) + iss3Refusal->comand));
        mass->append(static_cast<char>(iss3Refusal->numberSendLSB));
        mass->append(static_cast<char>(iss3Refusal->numberSendMSB));
        mass->append(static_cast<char>(iss3Refusal->numberReceiverLSB));
        mass->append(static_cast<char>(iss3Refusal->numberReceiverMSB));

        QByteArray *mass_2 =new QByteArray();

        mass_2->append(static_cast<char>((iss3Free_2->speed << 6) +(iss3Free_2->nbp << 4) + iss3Free_2->comand));
        mass_2->append(static_cast<char>(iss3Free_2->numberSendLSB));
        mass_2->append(static_cast<char>(iss3Free_2->numberSendMSB));
        mass_2->append(static_cast<char>((iss3Free_2->Rezerv1 << 2) + iss3Free_2->RChVK));
        mass_2->append(static_cast<char>((iss3Free_2->Rezerv2 << 4) + iss3Free_2->Priopy));



        for (int i=0; i < mass->count();i++) {

            emit writeSSS(mass->at(i));
            emit writeSSS2(mass_2->at(i));
        }



        return;
    }

    ////////////////////////////////////////////////////////////////
    // "СВОБОДЕН" "ВЫЗОВ" "ОТВЕТ РТР" "ВЫЗОВ"
    if( Mod_Onix_1 == S3_FREE  && Mod_SSS_1 == S3_CALL  && Mod_SSS_2 == S3_RTR_Otv && Mod_Onix_2 == S3_CALL)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        ////////////////////////////////////////////////////////////

        iss3Call->comand = S3_CALL;


        emit log1("1) SSS отправил команду <b>ВЫЗОВ</b>\n");
        emit log1("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call->comand)+"</b>)\n");
        emit log1("2 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberSendLSB)+" </b>)\n");
        emit log1("3 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberSendMSB)+" </b>)\n");
        emit log1("4 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberReceiverLSB)+" </b>)\n");
        emit log1("5 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberReceiverMSB)+" </b>)\n");
        emit log1("\n");


        /////////////////////////////////////////////////////////////////////////

        iss3Call_2->comand = S3_RTR_Otv;

        emit log2("2) SSS отправил команду <b>ОТВЕТ РТР</b>\n");
        emit log2("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call_2->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call_2->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call_2->comand)+"</b>)\n");
        emit log2("2 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberSendLSB)+" </b>)\n");
        emit log2("3 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberSendMSB)+" </b>)\n");
        emit log2("4 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberReceiverLSB)+" </b>)\n");
        emit log2("5 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberReceiverMSB)+" </b>)\n");
        emit log2("\n");



        QByteArray *mass =new QByteArray();

        mass->append(static_cast<char>((iss3Call->speed << 6) +(iss3Call->nbp << 4) + iss3Call->comand));
        mass->append(static_cast<char>(iss3Call->numberSendLSB));
        mass->append(static_cast<char>(iss3Call->numberSendMSB));
        mass->append(static_cast<char>(iss3Call->numberReceiverLSB));
        mass->append(static_cast<char>(iss3Call->numberReceiverMSB));

        QByteArray *mass_2 =new QByteArray();

        mass_2->append(static_cast<char>((iss3Call_2->speed << 6) +(iss3Call_2->nbp << 4) + iss3Call_2->comand));
        mass_2->append(static_cast<char>(iss3Call_2->numberSendLSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberSendMSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberReceiverLSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberReceiverMSB));


        for (int i=0; i < mass->count();i++) {

            emit writeSSS(mass->at(i));
            emit writeSSS2(mass_2->at(i));
        }



        return;
    }


    ////////////////////////////////////////////////////////////////
    // "СВОБОДЕН"  "СВОБОДНА ССС"  "ОТКАЗ РТР" "ВЫЗОВ"
    if( Mod_Onix_1 == S3_FREE  && Mod_SSS_1 == S3_FREE_SSS  && Mod_SSS_2 == S3_RTR && Mod_Onix_2 == S3_CALL)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        ////////////////////////////////////////////////////////////

        emit log1("1) SSS отправил команду <b>CВОБОДНА ССС</b>\n");
        emit log1("1 Байт( Cкорость: <b>"+get_Byte2(iss3Free->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Free->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Free->comand)+"</b>)\n");
        emit log1("2 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendLSB)+" </b>)\n");
        emit log1("3 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendMSB)+" </b>)\n");
        emit log1("4 Байт( Резерв: <b>"+get_Byte6(iss3Free->Rezerv1)+"</b> РЧВК: <b>" +get_Byte2(iss3Free_2->RChVK)+"</b>)\n");
        emit log1("5 Байт( Резерв: <b>"+get_Byte4(iss3Free->Rezerv2)+"</b> Приоритет: <b>" +get_Byte4(iss3Free->Priopy)+"</b>)\n");
        emit log2("\n");


        /////////////////////////////////////////////////////////////////////////

        iss3Refusal_2->comand = S3_RTR;


        emit log2("2) SSS отправил команду <b>ОТКАЗ РТР</b>\n");
        emit log2("1 Байт( Причина: <b>"+get_Byte4(iss3Refusal_2->why)+"</b> Команда: <b>"+get_Byte4(iss3Refusal_2->comand)+"</b>)\n");
        emit log2("2 Байт( байт номера: <b>" +get_Byte8(iss3Refusal_2->numberSendLSB)+" </b>)\n");
        emit log2("3 Байт( байт номера: <b>" +get_Byte8(iss3Refusal_2->numberSendMSB)+" </b>)\n");
        emit log2("4 Байт( байт номера: <b>" +get_Byte8(iss3Refusal_2->numberReceiverLSB)+" </b>)\n");
        emit log2("5 Байт( байт номера: <b>" +get_Byte8(iss3Refusal_2->numberReceiverMSB)+" </b>)\n");
        emit log2("\n");



        QByteArray *mass =new QByteArray();

        mass->append(static_cast<char>((iss3Free->speed << 6) +(iss3Free->nbp << 4) + iss3Free->comand));
        mass->append(static_cast<char>(iss3Free->numberSendLSB));
        mass->append(static_cast<char>(iss3Free->numberSendMSB));
        mass->append(static_cast<char>((iss3Free->Rezerv1 << 2) + iss3Free->RChVK));
        mass->append(static_cast<char>((iss3Free->Rezerv2 << 4) + iss3Free->Priopy));



        QByteArray *mass_2 =new QByteArray();

        mass_2->append(static_cast<char>((iss3Refusal_2->why << 4) + iss3Refusal_2->comand));
        mass_2->append(static_cast<char>(iss3Refusal_2->numberSendLSB));
        mass_2->append(static_cast<char>(iss3Refusal_2->numberSendMSB));
        mass_2->append(static_cast<char>(iss3Refusal_2->numberReceiverLSB));
        mass_2->append(static_cast<char>(iss3Refusal_2->numberReceiverMSB));



        for (int i=0; i < mass->count();i++) {

            emit writeSSS(mass->at(i));
            emit writeSSS2(mass_2->at(i));
        }



        return;
    }



    ////////////////////////////////////////////////////////////////
    // "ВЫЗОВ"  "ОТВЕТ ЗС"  "ВЫЗОВ" "ОТВЕТ ЗС"
    if( Mod_Onix_1 == S3_CALL  && Mod_SSS_1 == S3_ZS  && Mod_SSS_2 == S3_CALL  && Mod_Onix_2 == S3_ZS)
    {

        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        ////////////////////////////////////////////////////////////

        iss3Call->comand = S3_ZS;

        emit log1("1) SSS отправил команду <b>ОТВЕТ ЗС</b>\n");
        emit log1("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call->comand)+"</b>)\n");
        emit log1("2 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberSendLSB)+" </b>)\n");
        emit log1("3 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberSendMSB)+" </b>)\n");
        emit log1("4 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberReceiverLSB)+" </b>)\n");
        emit log1("5 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberReceiverMSB)+" </b>)\n");
        emit log1("\n");


        /////////////////////////////////////////////////////////////////////////

         iss3Call_2->comand = S3_CALL;

        emit log2("2) SSS отправил команду <b>ВЫЗОВ</b>\n");
        emit log2("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call_2->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call_2->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call_2->comand)+"</b>)\n");
        emit log2("2 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberSendLSB)+" </b>)\n");
        emit log2("3 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberSendMSB)+" </b>)\n");
        emit log2("4 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberReceiverLSB)+" </b>)\n");
        emit log2("5 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberReceiverMSB)+" </b>)\n");
        emit log2("\n");


        QByteArray *mass =new QByteArray();

        mass->append(static_cast<char>((iss3Call->speed << 6) +(iss3Call->nbp << 4) + iss3Call->comand));
        mass->append(static_cast<char>(iss3Call->numberSendLSB));
        mass->append(static_cast<char>(iss3Call->numberSendMSB));
        mass->append(static_cast<char>(iss3Call->numberReceiverLSB));
        mass->append(static_cast<char>(iss3Call->numberReceiverMSB));

        QByteArray *mass_2 =new QByteArray();

        mass_2->append(static_cast<char>((iss3Call_2->speed << 6) +(iss3Call_2->nbp << 4) + iss3Call_2->comand));
        mass_2->append(static_cast<char>(iss3Call_2->numberSendLSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberSendMSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberReceiverLSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberReceiverMSB));



        for (int i=0; i < mass->count();i++)
        {

            emit writeSSS(mass->at(i));
            emit writeSSS2(mass_2->at(i));
        }

        timerCall = 1;
        timerCall2 = 1;

        return;
    }


    ////////////////////////////////////////////////////////////////
    // "ОТВЕТ ЗС"  "ВЫЗОВ"  "ОТВЕТ ЗС" "ВЫЗОВ"
    if( Mod_Onix_1 == S3_ZS  && Mod_SSS_1 == S3_CALL  && Mod_SSS_2 == S3_ZS  && Mod_Onix_2 == S3_CALL)
    {

        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        ////////////////////////////////////////////////////////////

        iss3Call->comand = S3_CALL;

        emit log1("1) SSS отправил команду <b>ВЫЗОВ</b>\n");
        emit log1("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call->comand)+"</b>)\n");
        emit log1("2 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberSendLSB)+" </b>)\n");
        emit log1("3 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberSendMSB)+" </b>)\n");
        emit log1("4 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberReceiverLSB)+" </b>)\n");
        emit log1("5 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberReceiverMSB)+" </b>)\n");
        emit log1("\n");


        /////////////////////////////////////////////////////////////////////////

         iss3Call_2->comand = S3_ZS;

         emit log2("2) SSS отправил команду <b>ОТВЕТ ЗС</b>\n");
         emit log2("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call_2->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call_2->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call_2->comand)+"</b>)\n");
         emit log2("2 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberSendLSB)+" </b>)\n");
         emit log2("3 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberSendMSB)+" </b>)\n");
         emit log2("4 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberReceiverLSB)+" </b>)\n");
         emit log2("5 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberReceiverMSB)+" </b>)\n");
         emit log2("\n");



        QByteArray *mass =new QByteArray();

        mass->append(static_cast<char>((iss3Call->speed << 6) +(iss3Call->nbp << 4) + iss3Call->comand));
        mass->append(static_cast<char>(iss3Call->numberSendLSB));
        mass->append(static_cast<char>(iss3Call->numberSendMSB));
        mass->append(static_cast<char>(iss3Call->numberReceiverLSB));
        mass->append(static_cast<char>(iss3Call->numberReceiverMSB));

        QByteArray *mass_2 =new QByteArray();

        mass_2->append(static_cast<char>((iss3Call_2->speed << 6) +(iss3Call_2->nbp << 4) + iss3Call_2->comand));
        mass_2->append(static_cast<char>(iss3Call_2->numberSendLSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberSendMSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberReceiverLSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberReceiverMSB));



        for (int i=0; i < mass->count();i++)
        {

            emit writeSSS(mass->at(i));
            emit writeSSS2(mass_2->at(i));
        }

        timerCall = 1;
        timerCall2 = 1;

        return;
    }

    ////////////////////////////////////////////////////////////////

    // "СВОБОДЕН" "ОТВЕТ ЗС" "СВОБОДНА ССС" "ОТВЕТ ЗС"

    if( Mod_Onix_1 == S3_FREE && Mod_SSS_1 == S3_ZS && Mod_SSS_2 == S3_FREE_SSS && Mod_Onix_2 == S3_ZS)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        ////////////////////////////////////////////////////////////


        iss3Call->comand = S3_ZS;

        emit log1("1) SSS отправил команду <b>ОТВЕТ ЗС</b>\n");
        emit log1("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call->comand)+"</b>)\n");
        emit log1("2 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberSendLSB)+" </b>)\n");
        emit log1("3 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberSendMSB)+" </b>)\n");
        emit log1("4 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberReceiverLSB)+" </b>)\n");
        emit log1("5 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberReceiverMSB)+" </b>)\n");
        emit log1("\n");


        /////////////////////////////////////////////////////////////////////////

        emit log2("2) SSS отправил команду <b>CВОБОДНА ССС</b>\n");
        emit log2("1 Байт( Cкорость: <b>"+get_Byte2(iss3Free_2->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Free_2->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Free_2->comand)+"</b>)\n");
        emit log2("2 Байт( байт номера: <b>" +get_Byte8(iss3Free_2->numberSendLSB)+" </b>)\n");
        emit log2("3 Байт( байт номера: <b>" +get_Byte8(iss3Free_2->numberSendMSB)+" </b>)\n");
        emit log2("4 Байт( Резерв: <b>"+get_Byte6(iss3Free_2->Rezerv1)+"</b> РЧВК: <b>" +get_Byte2(iss3Free_2->RChVK)+"</b>)\n");
        emit log2("5 Байт( Резерв: <b>"+get_Byte4(iss3Free_2->Rezerv2)+"</b> Приоритет: <b>" +get_Byte4(iss3Free_2->Priopy)+"</b>)\n");
        emit log2("\n");



        QByteArray *mass =new QByteArray();

        mass->append(static_cast<char>((iss3Call->speed << 6) +(iss3Call->nbp << 4) + iss3Call->comand));
        mass->append(static_cast<char>(iss3Call->numberSendLSB));
        mass->append(static_cast<char>(iss3Call->numberSendMSB));
        mass->append(static_cast<char>(iss3Call->numberReceiverLSB));
        mass->append(static_cast<char>(iss3Call->numberReceiverMSB));

        QByteArray *mass_2 =new QByteArray();

        mass_2->append(static_cast<char>((iss3Free_2->speed << 6) +(iss3Free_2->nbp << 4) + iss3Free_2->comand));
        mass_2->append(static_cast<char>(iss3Free_2->numberSendLSB));
        mass_2->append(static_cast<char>(iss3Free_2->numberSendMSB));
        mass_2->append(static_cast<char>((iss3Free_2->Rezerv1 << 2) + iss3Free_2->RChVK));
        mass_2->append(static_cast<char>((iss3Free_2->Rezerv2 << 4) + iss3Free_2->Priopy));



        for (int i=0; i < mass->count();i++)
        {

            emit writeSSS(mass->at(i));
            emit writeSSS2(mass_2->at(i));
        }



        return;
    }

    ////////////////////////////////////////////////////////////////

    // "ОТВЕТ ЗС" "СВОБОДНА ССС" "ОТВЕТ ЗС" "СВОБОДЕН"

    if( Mod_Onix_1 == S3_ZS && Mod_SSS_1 == S3_FREE_SSS && Mod_SSS_2 ==S3_ZS  && Mod_Onix_2 == S3_FREE )
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        ////////////////////////////////////////////////////////////

        emit log1("1) SSS отправил команду <b>CВОБОДНА ССС</b>\n");
        emit log1("1 Байт( Cкорость: <b>"+get_Byte2(iss3Free->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Free->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Free->comand)+"</b>)\n");
        emit log1("2 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendLSB)+" </b>)\n");
        emit log1("3 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendMSB)+" </b>)\n");
        emit log1("4 Байт( Резерв: <b>"+get_Byte6(iss3Free->Rezerv1)+"</b> РЧВК: <b>" +get_Byte2(iss3Free->RChVK)+"</b>)\n");
        emit log1("5 Байт( Резерв: <b>"+get_Byte4(iss3Free->Rezerv2)+"</b> Приоритет: <b>" +get_Byte4(iss3Free->Priopy)+"</b>)\n");
        emit log1("\n");


        /////////////////////////////////////////////////////////////////////////

        iss3Call_2->comand = S3_ZS;

        emit log2("2) SSS отправил команду <b>ОТВЕТ ЗС</b>\n");
        emit log2("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call_2->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call_2->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call_2->comand)+"</b>)\n");
        emit log2("2 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberSendLSB)+" </b>)\n");
        emit log2("3 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberSendMSB)+" </b>)\n");
        emit log2("4 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberReceiverLSB)+" </b>)\n");
        emit log2("5 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberReceiverMSB)+" </b>)\n");
        emit log2("\n");



        QByteArray *mass =new QByteArray();

        mass->append(static_cast<char>((iss3Free->speed << 6) +(iss3Free->nbp << 4) + iss3Free->comand));
        mass->append(static_cast<char>(iss3Free->numberSendLSB));
        mass->append(static_cast<char>(iss3Free->numberSendMSB));
        mass->append(static_cast<char>((iss3Free->Rezerv1 << 2) + iss3Free->RChVK));
        mass->append(static_cast<char>((iss3Free->Rezerv2 << 4) + iss3Free->Priopy));

        QByteArray *mass_2 =new QByteArray();

        mass_2->append(static_cast<char>((iss3Call_2->speed << 6) +(iss3Call_2->nbp << 4) + iss3Call_2->comand));
        mass_2->append(static_cast<char>(iss3Call_2->numberSendLSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberSendMSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberReceiverLSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberReceiverMSB));





        for (int i=0; i < mass->count();i++)
        {

            emit writeSSS(mass->at(i));
            emit writeSSS2(mass_2->at(i));
        }



        return;
    }

    ////////////////////////////////////////////////////////////////

    // "ОТВЕТ ЗС" "СВОБОДНА ССС" "ОТВЕТ ЗС" "СВОБОДЕН"

    if( Mod_Onix_1 == S3_ZS  && Mod_SSS_1 == S3_FREE_SSS  && Mod_SSS_2 == S3_ZS && Mod_Onix_2 == S3_FREE)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        ////////////////////////////////////////////////////////////




        emit log1("1) SSS отправил команду <b>CВОБОДНА ССС</b>\n");
        emit log1("1 Байт( Cкорость: <b>"+get_Byte2(iss3Free->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Free->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Free->comand)+"</b>)\n");
        emit log1("2 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendLSB)+" </b>)\n");
        emit log1("3 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendMSB)+" </b>)\n");
        emit log1("4 Байт( Резерв: <b>"+get_Byte6(iss3Free->Rezerv1)+"</b> РЧВК: <b>" +get_Byte2(iss3Free->RChVK)+"</b>)\n");
        emit log1("5 Байт( Резерв: <b>"+get_Byte4(iss3Free->Rezerv2)+"</b> Приоритет: <b>" +get_Byte4(iss3Free->Priopy)+"</b>)\n");
        emit log1("\n");





        /////////////////////////////////////////////////////////////////////////

        iss3Call_2->comand = S3_ZS;

        emit log2("2) SSS отправил команду <b>ОТВЕТ ЗС</b>\n");
        emit log2("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call_2->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call_2->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call_2->comand)+"</b>)\n");
        emit log2("2 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberSendLSB)+" </b>)\n");
        emit log2("3 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberSendMSB)+" </b>)\n");
        emit log2("4 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberReceiverLSB)+" </b>)\n");
        emit log2("5 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberReceiverMSB)+" </b>)\n");
        emit log2("\n");



        QByteArray *mass =new QByteArray();

        mass->append(static_cast<char>((iss3Free->speed << 6) +(iss3Free->nbp << 4) + iss3Free->comand));
        mass->append(static_cast<char>(iss3Free->numberSendLSB));
        mass->append(static_cast<char>(iss3Free->numberSendMSB));
        mass->append(static_cast<char>((iss3Free->Rezerv1 << 2) + iss3Free->RChVK));
        mass->append(static_cast<char>((iss3Free->Rezerv2 << 4) + iss3Free->Priopy));

        QByteArray *mass_2 =new QByteArray();

        mass_2->append(static_cast<char>((iss3Call_2->speed << 6) +(iss3Call_2->nbp << 4) + iss3Call_2->comand));
        mass_2->append(static_cast<char>(iss3Call_2->numberSendLSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberSendMSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberReceiverLSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberReceiverMSB));



        for (int i=0; i < mass->count();i++)
        {

            emit writeSSS(mass->at(i));
            emit writeSSS2(mass_2->at(i));
        }



        return;
    }

    ////////////////////////////////////////////////////////////////
    // "СВОБОДЕН" "ОТБОЙ" "ОТБОЙ" "ОТБОЙ"

    if( Mod_Onix_1 == S3_FREE && Mod_SSS_1 == S3_HANGUP && Mod_SSS_2 == S3_HANGUP && Mod_Onix_2 == S3_HANGUP)
    {
        qDebug () << "СВОБОДЕН" "ОТБОЙ" "ОТБОЙ" "ОТБОЙ";

        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        ////////////////////////////////////////////////////////////


        iss3Call->comand = S3_HANGUP;

        emit log1("1) SSS отправил команду <b>ОТБОЙ</b>\n");
        emit log1("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call->comand)+"</b>)\n");
        emit log1("2 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberSendLSB)+" </b>)\n");
        emit log1("3 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberSendMSB)+" </b>)\n");
        emit log1("4 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberReceiverLSB)+" </b>)\n");
        emit log1("5 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberReceiverMSB)+" </b>)\n");
        emit log1("\n");


        /////////////////////////////////////////////////////////////////////////

         iss3Call_2->comand =  S3_HANGUP;

        emit log2("2) SSS отправил команду <b>ОТБОЙ</b>\n");
        emit log2("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call_2->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call_2->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call_2->comand)+"</b>)\n");
        emit log2("2 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberSendLSB)+" </b>)\n");
        emit log2("3 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberSendMSB)+" </b>)\n");
        emit log2("4 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberReceiverLSB)+" </b>)\n");
        emit log2("5 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberReceiverMSB)+" </b>)\n");
        emit log2("\n");



        QByteArray *mass =new QByteArray();

        mass->append(static_cast<char>((iss3Call->speed << 6) +(iss3Call->nbp << 4) + iss3Call->comand));
        mass->append(static_cast<char>(iss3Call->numberSendLSB));
        mass->append(static_cast<char>(iss3Call->numberSendMSB));
        mass->append(static_cast<char>(iss3Call->numberReceiverLSB));
        mass->append(static_cast<char>(iss3Call->numberReceiverMSB));

        QByteArray *mass_2 =new QByteArray();

        mass_2->append(static_cast<char>((iss3Call_2->speed << 6) +(iss3Call_2->nbp << 4) + iss3Call_2->comand));
        mass_2->append(static_cast<char>(iss3Call_2->numberSendLSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberSendMSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberReceiverLSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberReceiverMSB));



        for (int i=0; i < mass->count();i++)
        {

            emit writeSSS(mass->at(i));
            emit writeSSS2(mass_2->at(i));
        }

        emit set_otvetZS(false);
        emit set_otvetZS2(false);

        return;
    }

    ////////////////////////////////////////////////////////////////
    // "ОТБОЙ" "ОТБОЙ" "ОТБОЙ" "СВОБОДЕН"

    if( Mod_Onix_1 == S3_HANGUP && Mod_SSS_1 == S3_HANGUP && Mod_SSS_2 == S3_HANGUP)
    {

        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        ////////////////////////////////////////////////////////////


        iss3Call->comand = S3_HANGUP;

        emit log1("1) SSS отправил команду <b>ОТБОЙ</b>\n");
        emit log1("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call->comand)+"</b>)\n");
        emit log1("2 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberSendLSB)+" </b>)\n");
        emit log1("3 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberSendMSB)+" </b>)\n");
        emit log1("4 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberReceiverLSB)+" </b>)\n");
        emit log1("5 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberReceiverMSB)+" </b>)\n");
        emit log1("\n");


        /////////////////////////////////////////////////////////////////////////

         iss3Call_2->comand =  S3_HANGUP;

        emit log2("2) SSS отправил команду <b>ОТБОЙ</b>\n");
        emit log2("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call_2->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call_2->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call_2->comand)+"</b>)\n");
        emit log2("2 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberSendLSB)+" </b>)\n");
        emit log2("3 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberSendMSB)+" </b>)\n");
        emit log2("4 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberReceiverLSB)+" </b>)\n");
        emit log2("5 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberReceiverMSB)+" </b>)\n");
        emit log2("\n");



        QByteArray *mass =new QByteArray();

        mass->append(static_cast<char>((iss3Call->speed << 6) +(iss3Call->nbp << 4) + iss3Call->comand));
        mass->append(static_cast<char>(iss3Call->numberSendLSB));
        mass->append(static_cast<char>(iss3Call->numberSendMSB));
        mass->append(static_cast<char>(iss3Call->numberReceiverLSB));
        mass->append(static_cast<char>(iss3Call->numberReceiverMSB));

        QByteArray *mass_2 =new QByteArray();

        mass_2->append(static_cast<char>((iss3Call_2->speed << 6) +(iss3Call_2->nbp << 4) + iss3Call_2->comand));
        mass_2->append(static_cast<char>(iss3Call_2->numberSendLSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberSendMSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberReceiverLSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberReceiverMSB));



        for (int i=0; i < mass->count();i++)
        {

            emit writeSSS(mass->at(i));
            emit writeSSS2(mass_2->at(i));
        }

        emit set_otvetZS(false);
        emit set_otvetZS2(false);

        return;
    }


    ////////////////////////////////////////////////////////////////
    // "СВОБОДЕН" "РАБОТА" "СВОБОДНА ССС" "РАБОТА"

    if( Mod_Onix_1 == S3_FREE && Mod_SSS_1 == S3_WORK && Mod_SSS_2 == S3_FREE_SSS && Mod_Onix_2 == S3_WORK)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        ////////////////////////////////////////////////////////////


        iss3Call->comand = S3_WORK;

        emit log1("1) SSS отправил команду <b>РАБОТА</b>\n");
        emit log1("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call->comand)+"</b>)\n");
        emit log1("2 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberSendLSB)+" </b>)\n");
        emit log1("3 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberSendMSB)+" </b>)\n");
        emit log1("4 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberReceiverLSB)+" </b>)\n");
        emit log1("5 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberReceiverMSB)+" </b>)\n");
        emit log1("\n");


        /////////////////////////////////////////////////////////////////////////

         emit log2("2) SSS отправил команду <b>CВОБОДНА ССС</b>\n");
         emit log2("1 Байт( Cкорость: <b>"+get_Byte2(iss3Free_2->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Free_2->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Free_2->comand)+"</b>)\n");
         emit log2("2 Байт( байт номера: <b>" +get_Byte8(iss3Free_2->numberSendLSB)+" </b>)\n");
         emit log2("3 Байт( байт номера: <b>" +get_Byte8(iss3Free_2->numberSendMSB)+" </b>)\n");
         emit log2("4 Байт( Резерв: <b>"+get_Byte6(iss3Free_2->Rezerv1)+"</b> РЧВК: <b>" +get_Byte2(iss3Free_2->RChVK)+"</b>)\n");
         emit log2("5 Байт( Резерв: <b>"+get_Byte4(iss3Free_2->Rezerv2)+"</b> Приоритет: <b>" +get_Byte4(iss3Free_2->Priopy)+"</b>)\n");
         emit log2("\n");



        QByteArray *mass =new QByteArray();

        mass->append(static_cast<char>((iss3Call->speed << 6) +(iss3Call->nbp << 4) + iss3Call->comand));
        mass->append(static_cast<char>(iss3Call->numberSendLSB));
        mass->append(static_cast<char>(iss3Call->numberSendMSB));
        mass->append(static_cast<char>(iss3Call->numberReceiverLSB));
        mass->append(static_cast<char>(iss3Call->numberReceiverMSB));

        QByteArray *mass_2 =new QByteArray();

        mass_2->append(static_cast<char>((iss3Free_2->speed << 6) +(iss3Free_2->nbp << 4) + iss3Free_2->comand));
        mass_2->append(static_cast<char>(iss3Free_2->numberSendLSB));
        mass_2->append(static_cast<char>(iss3Free_2->numberSendMSB));
        mass_2->append(static_cast<char>((iss3Free_2->Rezerv1 << 2) + iss3Free_2->RChVK));
        mass_2->append(static_cast<char>((iss3Free_2->Rezerv2 << 4) + iss3Free_2->Priopy));



        for (int i=0; i < mass->count();i++)
        {

            emit writeSSS(mass->at(i));
            emit writeSSS2(mass_2->at(i));
        }

        return;
    }

    ////////////////////////////////////////////////////////////////
    // "СВОБОДЕН" "РАБОТА" "СВОБОДНА ССС" "РАБОТА"

    if( Mod_Onix_1 == S3_WORK  && Mod_SSS_1 == S3_FREE_SSS && Mod_SSS_2 == S3_WORK  && Mod_Onix_2 == S3_FREE)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        ////////////////////////////////////////////////////////////


        emit log1("1) SSS отправил команду <b>CВОБОДНА ССС</b>\n");
        emit log1("1 Байт( Cкорость: <b>"+get_Byte2(iss3Free_2->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Free_2->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Free_2->comand)+"</b>)\n");
        emit log1("2 Байт( байт номера: <b>" +get_Byte8(iss3Free_2->numberSendLSB)+" </b>)\n");
        emit log1("3 Байт( байт номера: <b>" +get_Byte8(iss3Free_2->numberSendMSB)+" </b>)\n");
        emit log1("4 Байт( Резерв: <b>"+get_Byte6(iss3Free_2->Rezerv1)+"</b> РЧВК: <b>" +get_Byte2(iss3Free_2->RChVK)+"</b>)\n");
        emit log1("5 Байт( Резерв: <b>"+get_Byte4(iss3Free_2->Rezerv2)+"</b> Приоритет: <b>" +get_Byte4(iss3Free_2->Priopy)+"</b>)\n");
        emit log1("\n");


        /////////////////////////////////////////////////////////////////////////

        iss3Call_2->comand = S3_WORK;

        emit log2("2) SSS отправил команду <b>РАБОТА</b>\n");
        emit log2("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call_2->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call_2->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call_2->comand)+"</b>)\n");
        emit log2("2 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberSendLSB)+" </b>)\n");
        emit log2("3 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberSendMSB)+" </b>)\n");
        emit log2("4 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberReceiverLSB)+" </b>)\n");
        emit log2("5 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberReceiverMSB)+" </b>)\n");
        emit log2("\n");



        QByteArray *mass =new QByteArray();

        mass->append(static_cast<char>((iss3Free_2->speed << 6) +(iss3Free_2->nbp << 4) + iss3Free_2->comand));
        mass->append(static_cast<char>(iss3Free_2->numberSendLSB));
        mass->append(static_cast<char>(iss3Free_2->numberSendMSB));
        mass->append(static_cast<char>((iss3Free_2->Rezerv1 << 2) + iss3Free_2->RChVK));
        mass->append(static_cast<char>((iss3Free_2->Rezerv2 << 4) + iss3Free_2->Priopy));

        QByteArray *mass_2 =new QByteArray();

        mass_2->append(static_cast<char>((iss3Call_2->speed << 6) +(iss3Call_2->nbp << 4) + iss3Call_2->comand));
        mass_2->append(static_cast<char>(iss3Call_2->numberSendLSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberSendMSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberReceiverLSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberReceiverMSB));



        for (int i=0; i < mass->count();i++)
        {

            emit writeSSS(mass->at(i));
            emit writeSSS2(mass_2->at(i));
        }

        return;
    }

    ////////////////////////////////////////////////////////////////
    // "ИС" "ИС" "ИС" "ИС"

    if( Mod_Onix_1 == S3_IS && Mod_SSS_1 == S3_IS && Mod_SSS_2 == S3_IS && Mod_Onix_2 == S3_IS)
    {
        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        ////////////////////////////////////////////////////////////


        iss3Call->comand = S3_IS;

        emit log1("1) SSS отправил команду <b>ИС</b>\n");
        emit log1("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call->comand)+"</b>)\n");
        emit log1("2 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberSendLSB)+" </b>)\n");
        emit log1("3 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberSendMSB)+" </b>)\n");
        emit log1("4 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberReceiverLSB)+" </b>)\n");
        emit log1("5 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberReceiverMSB)+" </b>)\n");
        emit log1("\n");


        /////////////////////////////////////////////////////////////////////////

        iss3Call_2->comand = S3_IS;

        emit log2("2) SSS отправил команду <b>ИС</b>\n");
        emit log2("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call_2->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call_2->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call_2->comand)+"</b>)\n");
        emit log2("2 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberSendLSB)+" </b>)\n");
        emit log2("3 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberSendMSB)+" </b>)\n");
        emit log2("4 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberReceiverLSB)+" </b>)\n");
        emit log2("5 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberReceiverMSB)+" </b>)\n");
        emit log2("\n");



        QByteArray *mass =new QByteArray();

        mass->append(static_cast<char>((iss3Call->speed << 6) +(iss3Call->nbp << 4) + iss3Call->comand));
        mass->append(static_cast<char>(iss3Call->numberSendLSB));
        mass->append(static_cast<char>(iss3Call->numberSendMSB));
        mass->append(static_cast<char>(iss3Call->numberReceiverLSB));
        mass->append(static_cast<char>(iss3Call->numberReceiverMSB));

        QByteArray *mass_2 =new QByteArray();

        mass_2->append(static_cast<char>((iss3Call_2->speed << 6) +(iss3Call_2->nbp << 4) + iss3Call_2->comand));
        mass_2->append(static_cast<char>(iss3Call_2->numberSendLSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberSendMSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberReceiverLSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberReceiverMSB));



        for (int i=0; i < mass->count();i++)
        {

            emit writeSSS(mass->at(i));
            emit writeSSS2(mass_2->at(i));
        }

        return;
    }

    ////////////////////////////////////////////////////////////////
    // "ИС" "ОТБОЙ ПО ПРИОРИТЕТУ" "ОТБОЙ ПО ПРИОРИТЕТУ" "ИС"

    if( Mod_Onix_1 == S3_IS && Mod_SSS_1 == S3_PRIORITY_CALL_OFF && Mod_SSS_2 == S3_PRIORITY_CALL_OFF && Mod_Onix_2 == S3_IS )
    {

        kol++;
        emit log1("-----");
        emit log1("- "+QString::number(kol)+" -");

        kol2++;
        emit log2("-----");
        emit log2("- "+QString::number(kol2)+" -");

        ////////////////////////////////////////////////////////////


        iss3Call->comand = S3_PRIORITY_CALL_OFF;

        emit log1("1) SSS отправил команду <b>ОТБОЙ ПО ПРИОРИТЕТУ</b>\n");
        emit log1("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call->comand)+"</b>)\n");
        emit log1("2 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberSendLSB)+" </b>)\n");
        emit log1("3 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberSendMSB)+" </b>)\n");
        emit log1("4 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberReceiverLSB)+" </b>)\n");
        emit log1("5 Байт( байт номера: <b>" +get_Byte8(iss3Call->numberReceiverMSB)+" </b>)\n");
        emit log1("\n");


        /////////////////////////////////////////////////////////////////////////

         iss3Call_2->comand =  S3_PRIORITY_CALL_OFF;

        emit log2("2) SSS отправил команду <b>ОТБОЙ ПО ПРИОРИТЕТУ</b>\n");
        emit log2("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call_2->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call_2->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call_2->comand)+"</b>)\n");
        emit log2("2 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberSendLSB)+" </b>)\n");
        emit log2("3 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberSendMSB)+" </b>)\n");
        emit log2("4 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberReceiverLSB)+" </b>)\n");
        emit log2("5 Байт( байт номера: <b>" +get_Byte8(iss3Call_2->numberReceiverMSB)+" </b>)\n");
        emit log2("\n");



        QByteArray *mass =new QByteArray();

        mass->append(static_cast<char>((iss3Call->speed << 6) +(iss3Call->nbp << 4) + iss3Call->comand));
        mass->append(static_cast<char>(iss3Call->numberSendLSB));
        mass->append(static_cast<char>(iss3Call->numberSendMSB));
        mass->append(static_cast<char>(iss3Call->numberReceiverLSB));
        mass->append(static_cast<char>(iss3Call->numberReceiverMSB));

        QByteArray *mass_2 =new QByteArray();

        mass_2->append(static_cast<char>((iss3Call_2->speed << 6) +(iss3Call_2->nbp << 4) + iss3Call_2->comand));
        mass_2->append(static_cast<char>(iss3Call_2->numberSendLSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberSendMSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberReceiverLSB));
        mass_2->append(static_cast<char>(iss3Call_2->numberReceiverMSB));



        for (int i=0; i < mass->count();i++)
        {

            emit writeSSS(mass->at(i));
            emit writeSSS2(mass_2->at(i));
        }

        emit set_otvetZS(false);
        emit set_otvetZS2(false);

        return;
    }


}
