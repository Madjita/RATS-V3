#include "onixb3.h"

OnixB3::OnixB3(QObject *parent) : QObject(parent),
    Mod_Onix(quint16(S3_FREE)),
    Mod_SSS(quint16(S3_FREE))
{
    this->moveToThread(new QThread());

    connect(this->thread(),&QThread::started,this,&OnixB3::process_start);

    this->thread()->start();


}

QString OnixB3::get_Byte8(quint8 data)
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

QString OnixB3::get_Byte6(quint8 data)
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

QString OnixB3::get_Byte4(quint8 data)
{
    QString str = nullptr;

    bytes4* b = (bytes4*)&data;


    str += QString::number(b->bit3,2);
    str += QString::number(b->bit2,2);
    str += QString::number(b->bit1,2);
    str += QString::number(b->bit0,2);

    return str;
}

QString OnixB3::get_Byte2(quint8 data)
{
    QString str = nullptr;

    bytes2* b = (bytes2*)&data;

    str += QString::number(b->bit1,2);
    str += QString::number(b->bit0,2);

    return str;
}

void OnixB3::process_start()
{
    clearAll();

}

void OnixB3::readSSS(char data)
{

    massData.append(data);

   // qDebug () << massData;

    if(massData.count() == 1)
    {
        quint8 komand = data & 0xF;

        switch (komand)
        {

            case S3_FREE_SSS:
            {

                switch (Mod_Onix)
                {
                    case S3_FREE:
                    {
                        flag_messege = 0; //принял команду свободен ССС
                        return;
                    }

                    case S3_ZS:
                    {
                        flag_messege = 6; //принял команду свободен ССС на фоне ОТВЕТ ЗС
                        return;
                    }
                    case S3_HANGUP:
                    {
                        flag_messege = 0; //принял команду свободен ССС
                        timerCall = 0;
                        return;
                    }


                }

                return;

            }

            case S3_Ok_REG:
            {
                switch (Mod_Onix)
                {
                    case S3_REG:
                    {
                        flag_messege = 1; //принял команду ПОДТВЕРЖДЕНИЕ РЕГИСТРАЦИИ ССС
                        return;
                    }

                }
                return;
            }

            case S3_RTR_Otv:
            {
                switch (Mod_Onix)
                {
                    case S3_CALL:
                    {
                        flag_messege = 3; //принял команду ОТВЕТ РТР
                        return;
                    }

                }
                return;
            }

            case S3_RTR:
            {
                switch (Mod_Onix)
                {
                    case S3_CALL:
                    {
                        timerCall = 0;
                        Mod_Onix = S3_FREE;
                        flag_messege = 4; //принял команду ОТКАЗ РТР
                        return;
                    }

                }
                return;
            }

            case S3_ZS:
            {
                switch (Mod_Onix)
                {
                    case S3_CALL:
                    {
                        timerCall = 0;
                        flag_messege = 5; //принял команду ОТВЕТ ЗС
                        return;
                    }

                    case S3_FREE:
                    {
                        flag_messege = 5; //принял команду ОТВЕТ ЗС
                        return;
                    }

                }
                return;
            }

            case S3_HANGUP:
            {
                countEnterCall = 0;
                timerCall = 0;
                flag_messege = 7; //принял команду ОТБОЙ
                return;
            }

            case S3_CALL:
            {
                if(Mod_Onix == S3_FREE)
                {
                    flag_messege = 8; //принял команду ВЫЗОВ
                }

                return;
            }

            case S3_WORK:
            {
                flag_messege = 9; //принял команду РАБОТА
                return;
            }

            case S3_IS:
            {

                flag_messege = 10; //принял команду ИС
                return;
            }

            case S3_Ok_Cansel_REG:
            {
                flag_messege = 11; //принял команду Подтверждение отказа от регистрации
                return;
            }

            case S3_PRIORITY_CALL_OFF:
            {
                countEnterCall = 0;
                timerCall = 0;
                flag_messege = 12; //принял команду ОТБОЙ ПО ПРИОРИТЕТУ
                return;
            }

        default: return;

        }
    }

    if(massData.count() == 5)
    {

        switch (flag_messege)
        {

            case 0:{

               ISS3_Free*  iss3Free_get = (ISS3_Free*)massData.data();

//               if(iss3Free_get == iss3Free)
//               {
//                    qDebug () << " Равны Свободен от ССС с ОНИкс";
//               }
//               else
//               {
//                    qDebug () << " НЕ РАВНЫ ОШИБКА. Свободен от ССС с ОНИкс";
//               }


                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>CВОБОДНА ССС</b>\n");
                emit log("\n");


                emit log(QString::number(index) + ") OnixB отправил от SSS команду <b>CВОБОДЕН</b>\n");
                emit log("1 Байт( Cкорость: <b>"+get_Byte2(iss3Free->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Free->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Free->comand)+"</b>)\n");
                emit log("2 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendLSB)+" </b>)\n");
                emit log("3 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendMSB)+" </b>)\n");
                emit log("4 Байт( байт резерва: <b>" +get_Byte8(iss3Free->Rezerv1)+" </b>)\n");
                emit log("5 Байт( байт резерва: <b>" +get_Byte8(iss3Free->Rezerv2)+" </b>)\n");
                emit log("\n");

                QByteArray *mass =new QByteArray();

                mass->append(static_cast<char>((iss3Free->speed << 6) +(iss3Free->nbp << 4) + iss3Free->comand));
                mass->append(static_cast<char>(iss3Free->numberSendLSB));
                mass->append(static_cast<char>(iss3Free->numberSendMSB));
                mass->append(static_cast<char>(iss3Free->Rezerv1));
                mass->append(static_cast<char>(iss3Free->Rezerv2));


                for (int i=0; i < mass->count();i++) {

                    emit writeSSS_ISS3(mass->at(i));
                }

                break;
            }

            case 1:
            {

                iss3Free->numberSendLSB = iss3Receipt->numberSendLSB;
                iss3Free->numberSendMSB = iss3Receipt->numberSendMSB;

                iss3Free->nbp = iss3Receipt->nbp;
                iss3Free->speed = iss3Receipt->speed;


                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ПОДТВЕРЖЕНИЕ РЕГИСТРАЦИИ</b>\n");
                emit log("\n");

                emit log(QString::number(index) + ") OnixB отправил от SSS команду <b>CВОБОДЕН</b>\n");
                emit log("1 Байт( Cкорость: <b>"+get_Byte2(iss3Free->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Free->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Free->comand)+"</b>)\n");
                emit log("2 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendLSB)+" </b>)\n");
                emit log("3 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendMSB)+" </b>)\n");
                emit log("4 Байт( байт резерва: <b>" +get_Byte8(iss3Free->Rezerv1)+" </b>)\n");
                emit log("5 Байт( байт резерва: <b>" +get_Byte8(iss3Free->Rezerv2)+" </b>)\n");
                emit log("\n");

                QByteArray *mass =new QByteArray();

                mass->append(static_cast<char>((iss3Free->speed << 6) +(iss3Free->nbp << 4) + iss3Free->comand));
                mass->append(static_cast<char>(iss3Free->numberSendLSB));
                mass->append(static_cast<char>(iss3Free->numberSendMSB));
                mass->append(static_cast<char>(iss3Free->Rezerv1));
                mass->append(static_cast<char>(iss3Free->Rezerv2));


                for (int i=0; i < mass->count();i++) {

                    emit writeSSS_ISS3(mass->at(i));
                }

                Mod_Onix = S3_FREE;


                break;
            }

            case 3:
            {

                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ОТВЕТ РТР</b>\n");
                emit log("\n");

                timerCall++;
                emit log("Таймер: <b>" +QString::number(timerCall)+ "</b>\n");

                //Запустить таймер на 5 секунд на ВЫЗОВ

                Call(iss3Call->numberSendLSB,iss3Call->numberSendMSB,iss3Call->numberReceiverLSB,iss3Call->numberReceiverMSB);




                break;
            }

            case 4:
            {


                ISS3_Refusal*  iss3Refusal = (ISS3_Refusal*)massData.data();

                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ОТКАЗ РТР</b>\n");
                emit log("\n");

                emit log(QString::number(index) + ") OnixB отправил от SSS команду <b>CВОБОДЕН</b>\n");
                emit log("1 Байт( Cкорость: <b>"+get_Byte2(iss3Free->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Free->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Free->comand)+"</b>)\n");
                emit log("2 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendLSB)+" </b>)\n");
                emit log("3 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendMSB)+" </b>)\n");
                emit log("4 Байт( байт резерва: <b>" +get_Byte8(iss3Free->Rezerv1)+" </b>)\n");
                emit log("5 Байт( байт резерва: <b>" +get_Byte8(iss3Free->Rezerv2)+" </b>)\n");
                emit log("\n");

                QByteArray *mass =new QByteArray();

                iss3Free->comand = S3_FREE;

                mass->append(static_cast<char>((iss3Free->speed << 6) + (iss3Free->nbp << 4) + iss3Free->comand));
                mass->append(static_cast<char>(iss3Free->numberSendLSB));
                mass->append(static_cast<char>(iss3Free->numberSendMSB));
                mass->append(static_cast<char>(iss3Free->Rezerv1));
                mass->append(static_cast<char>(iss3Free->Rezerv2));


                for (int i=0; i < mass->count();i++) {

                    emit writeSSS_ISS3(mass->at(i));
                }

                Mod_Onix = S3_FREE;


                break;
            }

            case 5:
            {


                ISS3_Call*  iss3Call = (ISS3_Call*)massData.data();

                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ОТВЕТ ЗС</b>\n");
                emit log("\n");


                emit log(QString::number(index) + ") OnixB отправил от SSS команду <b>CВОБОДЕН</b>\n");
                emit log("1 Байт( Cкорость: <b>"+get_Byte2(iss3Free->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Free->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Free->comand)+"</b>)\n");
                emit log("2 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendLSB)+" </b>)\n");
                emit log("3 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendMSB)+" </b>)\n");
                emit log("4 Байт( байт резерва: <b>" +get_Byte8(iss3Free->Rezerv1)+" </b>)\n");
                emit log("5 Байт( байт резерва: <b>" +get_Byte8(iss3Free->Rezerv2)+" </b>)\n");
                emit log("\n");

                QByteArray *mass =new QByteArray();

                iss3Free->comand = S3_FREE;

                mass->append(static_cast<char>((iss3Free->speed << 6) + (iss3Free->nbp << 4) + iss3Free->comand));
                mass->append(static_cast<char>(iss3Free->numberSendLSB));
                mass->append(static_cast<char>(iss3Free->numberSendMSB));
                mass->append(static_cast<char>(iss3Free->Rezerv1));
                mass->append(static_cast<char>(iss3Free->Rezerv2));


                for (int i=0; i < mass->count();i++) {

                    emit writeSSS_ISS3(mass->at(i));
                }

                Mod_Onix = S3_FREE;


                break;
            }

            case 6:
            {
                if(timerCall < 5)
                {
                    timerCall++;

                    ISS3_Call*  iss3Call = (ISS3_Call*)massData.data();

                    emit log(QString::number(index) + ") OnixB принял от SSS команду <b>СВОБОДНА ССС</b>\n");
                    emit log("\n");

                    emit log("Таймер: <b>" +QString::number(timerCall)+ "</b>\n");


                    Mod_Onix = S3_ZS;

                    iss3Call->comand = S3_ZS;
                    iss3Call->nbp = iss3Receipt->nbp;
                    iss3Call->speed = iss3Receipt->speed;

                    emit log(QString::number(index) + ") OnixB отправил от SSS команду <b>ОТВЕТ ЗС</b>\n");
                    emit log("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call->comand)+"</b>)\n");
                    emit log("2 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberSendLSB)+" </b>)\n");
                    emit log("3 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberSendMSB)+" </b>)\n");
                    emit log("4 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberReceiverLSB)+" </b>)\n");
                    emit log("5 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberReceiverMSB)+" </b>)\n");
                    emit log("\n");


                    QByteArray *mass = new QByteArray();

                    mass->append(static_cast<char>((iss3Call->speed << 6) +(iss3Call->nbp << 4) + iss3Call->comand));
                    mass->append(static_cast<char>(iss3Call->numberSendLSB));
                    mass->append(static_cast<char>(iss3Call->numberSendMSB));
                    mass->append(static_cast<char>(iss3Call->numberReceiverLSB));
                    mass->append(static_cast<char>(iss3Call->numberReceiverMSB));


                    for (int i=0; i < mass->count();i++)
                    {
                        emit writeSSS_ISS3(mass->at(i));
                    }

                }
                else
                {


                    Mod_Onix = S3_HANGUP;

                    iss3Call->comand = S3_HANGUP;


                    QByteArray *mass = new QByteArray();

                    mass->append(static_cast<char>((iss3Call->speed << 6) +(iss3Call->nbp << 4) + iss3Call->comand));
                    mass->append(static_cast<char>(iss3Call->numberSendLSB));
                    mass->append(static_cast<char>(iss3Call->numberSendMSB));
                    mass->append(static_cast<char>(iss3Call->numberReceiverLSB));
                    mass->append(static_cast<char>(iss3Call->numberReceiverMSB));

                    emit log(QString::number(index) + ") OnixB отправил от SSS команду <b>ОТБОЙ</b>\n");
                    emit log("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call->comand)+"</b>)\n");
                    emit log("2 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberSendLSB)+" </b>)\n");
                    emit log("3 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberSendMSB)+" </b>)\n");
                    emit log("4 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberReceiverLSB)+" </b>)\n");
                    emit log("5 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberReceiverMSB)+" </b>)\n");
                    emit log("\n");


                    for (int i=0; i < mass->count();i++)
                    {

                        emit writeSSS_ISS3(mass->at(i));
                    }
                }

                break;
            }

           case 7:
            {
                 Mod_Onix = S3_FREE;
                 flagNumber = 0;

                 emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ОТБОЙ</b>\n");
                 emit log("\n");


                 emit log(QString::number(index) + ") OnixB отправил от SSS команду <b>CВОБОДЕН</b>\n");
                 emit log("1 Байт( Cкорость: <b>"+get_Byte2(iss3Free->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Free->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Free->comand)+"</b>)\n");
                 emit log("2 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendLSB)+" </b>)\n");
                 emit log("3 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendMSB)+" </b>)\n");
                 emit log("4 Байт( байт резерва: <b>" +get_Byte8(iss3Free->Rezerv1)+" </b>)\n");
                 emit log("5 Байт( байт резерва: <b>" +get_Byte8(iss3Free->Rezerv2)+" </b>)\n");
                 emit log("\n");

                 QByteArray *mass =new QByteArray();

                 mass->append(static_cast<char>((iss3Free->speed << 6) +(iss3Free->nbp << 4) + iss3Free->comand));
                 mass->append(static_cast<char>(iss3Free->numberSendLSB));
                 mass->append(static_cast<char>(iss3Free->numberSendMSB));
                 mass->append(static_cast<char>(iss3Free->Rezerv1));
                 mass->append(static_cast<char>(iss3Free->Rezerv2));


                 for (int i=0; i < mass->count();i++) {

                     emit writeSSS_ISS3(mass->at(i));
                 }

                 break;
            }

            case 8:
            {
                ISS3_Call*  iss3Call_get = (ISS3_Call*)massData.data();


                iss3Call->numberReceiverLSB = iss3Call_get->numberReceiverLSB;
                iss3Call->numberReceiverMSB = iss3Call_get->numberReceiverMSB;
                iss3Call->nbp = iss3Call_get->nbp;
                iss3Call->speed = iss3Call_get->speed;

                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ВЫЗОВ</b>\n");
                emit log("\n");


                emit log(QString::number(index) + ") OnixB отправил от SSS команду <b>CВОБОДЕН</b>\n");
                emit log("1 Байт( Cкорость: <b>"+get_Byte2(iss3Free->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Free->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Free->comand)+"</b>)\n");
                emit log("2 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendLSB)+" </b>)\n");
                emit log("3 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendMSB)+" </b>)\n");
                emit log("4 Байт( байт резерва: <b>" +get_Byte8(iss3Free->Rezerv1)+" </b>)\n");
                emit log("5 Байт( байт резерва: <b>" +get_Byte8(iss3Free->Rezerv2)+" </b>)\n");
                emit log("\n");

                QByteArray *mass =new QByteArray();

                mass->append(static_cast<char>((iss3Free->speed << 6) +(iss3Free->nbp << 4) + iss3Free->comand));
                mass->append(static_cast<char>(iss3Free->numberSendLSB));
                mass->append(static_cast<char>(iss3Free->numberSendMSB));
                mass->append(static_cast<char>(iss3Free->Rezerv1));
                mass->append(static_cast<char>(iss3Free->Rezerv2));


                for (int i=0; i < mass->count();i++) {

                    emit writeSSS_ISS3(mass->at(i));
                }

                break;
            }

            case 9:
            {
                ISS3_Call*  iss3Call_get = (ISS3_Call*)massData.data();

                Mod_Onix = S3_IS;

                iss3Call->comand = S3_IS;
                iss3Call->nbp = 0x3;
                iss3Call->speed = 0x3;
                iss3Call->numberReceiverLSB = 0xFF;
                iss3Call->numberReceiverMSB = 0xFF;
                iss3Call->numberSendLSB = 0xFF;
                iss3Call->numberSendMSB = 0xFF;

                emit log(QString::number(index) + ") OnixB принял от SSS команду <b>РАБОТА</b>\n");
                emit log("\n");


                emit log(QString::number(index) + ") OnixB отправил от SSS команду <b>ИС</b>\n");
                emit log("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call->comand)+"</b>)\n");
                emit log("2 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberSendLSB)+" </b>)\n");
                emit log("3 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberSendMSB)+" </b>)\n");
                emit log("4 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberReceiverLSB)+" </b>)\n");
                emit log("5 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberReceiverMSB)+" </b>)\n");
                emit log("\n");

                QByteArray *mass =new QByteArray();

                mass->append(static_cast<char>((iss3Call->speed << 6) +(iss3Call->nbp << 4) + iss3Call->comand));
                mass->append(static_cast<char>(iss3Call->numberSendLSB));
                mass->append(static_cast<char>(iss3Call->numberSendMSB));
                mass->append(static_cast<char>(iss3Call->numberReceiverLSB));
                mass->append(static_cast<char>(iss3Call->numberReceiverMSB));



                for (int i=0; i < mass->count();i++) {

                    emit writeSSS_ISS3(mass->at(i));
                }

                break;
            }

        case 10:
        {
            ISS3_Call*  iss3Call_get = (ISS3_Call*)massData.data();

            Mod_Onix = S3_IS;

            iss3Call->comand = 0xF;
            iss3Call->nbp = 0x3;
            iss3Call->speed = 0x3;
            iss3Call->numberReceiverLSB = 0xFF;
            iss3Call->numberReceiverMSB = 0xFF;
            iss3Call->numberSendLSB = 0xFF;
            iss3Call->numberSendMSB = 0xFF;

            emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ИС</b>\n");
            emit log("\n");


            emit log(QString::number(index) + ") OnixB отправил от SSS команду <b>ИС</b>\n");
            emit log("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call->comand)+"</b>)\n");
            emit log("2 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberSendLSB)+" </b>)\n");
            emit log("3 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberSendMSB)+" </b>)\n");
            emit log("4 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberReceiverLSB)+" </b>)\n");
            emit log("5 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberReceiverMSB)+" </b>)\n");
            emit log("\n");

            QByteArray *mass =new QByteArray();

            mass->append(static_cast<char>((iss3Call->speed << 6) +(iss3Call->nbp << 4) + iss3Call->comand));
            mass->append(static_cast<char>(iss3Call->numberSendLSB));
            mass->append(static_cast<char>(iss3Call->numberSendMSB));
            mass->append(static_cast<char>(iss3Call->numberReceiverLSB));
            mass->append(static_cast<char>(iss3Call->numberReceiverMSB));



            for (int i=0; i < mass->count();i++) {

                emit writeSSS_ISS3(mass->at(i));
            }

            break;
        }

        case 11:
        {


            iss3Free->numberSendLSB = 0x00;
            iss3Free->numberSendMSB = 0x00;

            iss3Free->nbp = 0x00;
            iss3Free->speed = 0x00;


            emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ПОДТВЕРЖЕНИЕ ОТКАЗА ОТ РЕГИСТРАЦИИ</b>\n");
            emit log("\n");

            emit log(QString::number(index) + ") OnixB отправил от SSS команду <b>CВОБОДЕН</b>\n");
            emit log("1 Байт( Cкорость: <b>"+get_Byte2(iss3Free->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Free->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Free->comand)+"</b>)\n");
            emit log("2 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendLSB)+" </b>)\n");
            emit log("3 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendMSB)+" </b>)\n");
            emit log("4 Байт( байт резерва: <b>" +get_Byte8(iss3Free->Rezerv1)+" </b>)\n");
            emit log("5 Байт( байт резерва: <b>" +get_Byte8(iss3Free->Rezerv2)+" </b>)\n");
            emit log("\n");

            QByteArray *mass =new QByteArray();

            mass->append(static_cast<char>((iss3Free->speed << 6) +(iss3Free->nbp << 4) + iss3Free->comand));
            mass->append(static_cast<char>(iss3Free->numberSendLSB));
            mass->append(static_cast<char>(iss3Free->numberSendMSB));
            mass->append(static_cast<char>(iss3Free->Rezerv1));
            mass->append(static_cast<char>(iss3Free->Rezerv2));


            for (int i=0; i < mass->count();i++) {

                emit writeSSS_ISS3(mass->at(i));
            }

            Mod_Onix = S3_FREE;


            break;
        }

        case 12:
         {
              Mod_Onix = S3_FREE;
              flagNumber = 0;

              emit log(QString::number(index) + ") OnixB принял от SSS команду <b>ОТБОЙ ПО ПРИОРИТЕТУ</b>\n");
              emit log("\n");


              emit log(QString::number(index) + ") OnixB отправил от SSS команду <b>CВОБОДЕН</b>\n");
              emit log("1 Байт( Cкорость: <b>"+get_Byte2(iss3Free->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Free->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Free->comand)+"</b>)\n");
              emit log("2 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendLSB)+" </b>)\n");
              emit log("3 Байт( байт номера: <b>" +get_Byte8(iss3Free->numberSendMSB)+" </b>)\n");
              emit log("4 Байт( байт резерва: <b>" +get_Byte8(iss3Free->Rezerv1)+" </b>)\n");
              emit log("5 Байт( байт резерва: <b>" +get_Byte8(iss3Free->Rezerv2)+" </b>)\n");
              emit log("\n");

              QByteArray *mass =new QByteArray();

              mass->append(static_cast<char>((iss3Free->speed << 6) +(iss3Free->nbp << 4) + iss3Free->comand));
              mass->append(static_cast<char>(iss3Free->numberSendLSB));
              mass->append(static_cast<char>(iss3Free->numberSendMSB));
              mass->append(static_cast<char>(iss3Free->Rezerv1));
              mass->append(static_cast<char>(iss3Free->Rezerv2));


              for (int i=0; i < mass->count();i++) {

                  emit writeSSS_ISS3(mass->at(i));
              }

              break;
         }


        }

        massData.clear();
    }


}

void OnixB3::Reg(quint8 npb, quint8 speed, quint8 priori, quint8 rchvk,quint8 number_LSB, quint8 number_MSB)
{

    Mod_Onix = S3_REG;

    iss3Receipt->comand = S3_REG;
    iss3Free->nbp = npb;
    iss3Free->speed = speed;

    iss3Receipt->nbp = npb;
    iss3Receipt->speed = speed;
    iss3Receipt->Priopy = priori;
    iss3Receipt->RChVK = rchvk;
    iss3Receipt->numberSendLSB = number_LSB;
    iss3Receipt->numberSendMSB = number_MSB;
    iss3Receipt->Rezerv1 = 0x00;
    iss3Receipt->Rezerv2 = 0x00;


    QByteArray *mass = new QByteArray();

    mass->append(static_cast<char>((iss3Receipt->speed << 6) +(iss3Receipt->nbp << 4) + iss3Receipt->comand));
    mass->append(static_cast<char>(iss3Receipt->numberSendLSB));
    mass->append(static_cast<char>(iss3Receipt->numberSendMSB));
    mass->append(static_cast<char>((iss3Receipt->Rezerv1 << 2) + iss3Receipt->RChVK));
    mass->append(static_cast<char>((iss3Receipt->Rezerv2 << 4) + iss3Receipt->Priopy));

    emit log(QString::number(index) + ") OnixB отправил от SSS команду <b>РЕГИСТРАЦИЯ</b>\n");
    emit log("1 Байт( Cкорость: <b>"+get_Byte2(iss3Receipt->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Receipt->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Receipt->comand)+"</b>)\n");
    emit log("2 Байт(байт номера: <b>" +get_Byte8(iss3Receipt->numberSendLSB)+" </b>)\n");
    emit log("3 Байт(байт номера: <b>" +get_Byte8(iss3Receipt->numberSendMSB)+" </b>)\n");
    emit log("4 Байт( Резерв: <b>"+get_Byte6(iss3Receipt->Rezerv1)+"</b> РЧВК: <b>" +get_Byte2(iss3Receipt->RChVK)+"</b>)\n");
    emit log("5 Байт( Резерв: <b>"+get_Byte4(iss3Receipt->Rezerv2)+"</b> Приоритет: <b>" +get_Byte4(iss3Receipt->Priopy)+"</b>)\n");
    emit log("\n");


    for (int i=0; i < mass->count();i++)
    {

        emit writeSSS_ISS3(mass->at(i));
    }

}

void OnixB3::RegCansell()
{
    Mod_Onix = S3_REG_Cansel;

    iss3Receipt->comand = S3_REG_Cansel;

    QByteArray *mass = new QByteArray();

    mass->append(static_cast<char>((iss3Receipt->speed << 6) +(iss3Receipt->nbp << 4) + iss3Receipt->comand));
    mass->append(static_cast<char>(iss3Receipt->numberSendLSB));
    mass->append(static_cast<char>(iss3Receipt->numberSendMSB));
    mass->append(static_cast<char>((iss3Receipt->Rezerv1 << 2) + iss3Receipt->RChVK));
    mass->append(static_cast<char>((iss3Receipt->Rezerv2 << 4) + iss3Receipt->Priopy));

    emit log(QString::number(index) + ") OnixB отправил от SSS команду <b>ОТКАЗ ОТ РЕГИСТРАЦИИ</b>\n");
    emit log("1 Байт( Cкорость: <b>"+get_Byte2(iss3Receipt->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Receipt->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Receipt->comand)+"</b>)\n");
    emit log("2 Байт(байт номера: <b>" +get_Byte8(iss3Receipt->numberSendLSB)+" </b>)\n");
    emit log("3 Байт(байт номера: <b>" +get_Byte8(iss3Receipt->numberSendMSB)+" </b>)\n");
    emit log("4 Байт( Резерв: <b>"+get_Byte6(iss3Receipt->Rezerv1)+"</b> РЧВК: <b>" +get_Byte2(iss3Receipt->RChVK)+"</b>)\n");
    emit log("5 Байт( Резерв: <b>"+get_Byte4(iss3Receipt->Rezerv2)+"</b> Приоритет: <b>" +get_Byte4(iss3Receipt->Priopy)+"</b>)\n");
    emit log("\n");


    for (int i=0; i < mass->count();i++)
    {

        emit writeSSS_ISS3(mass->at(i));
    }

}

void OnixB3::clearAll()
{
    Mod_Onix = S3_FREE;

    massData.clear();

    iss3Free->comand = S3_FREE;
    iss3Free->Rezerv1 = 0x00;
    iss3Free->Rezerv2 = 0x00;
    iss3Free->nbp = 0x00;
    iss3Free->speed = 0x00;
    iss3Free->numberSendLSB = 0x00;
    iss3Free->numberSendMSB = 0x00;

    iss3Receipt->comand = S3_REG;
    iss3Receipt->Priopy = 0x00;
    iss3Receipt->RChVK = 0x00;
    iss3Receipt->Rezerv1= 0x00;
    iss3Receipt->Rezerv2= 0x00;
    iss3Receipt->comand = 0x00;
    iss3Receipt->nbp = 0x00;
    iss3Receipt->numberSendLSB = 0x00;
    iss3Receipt->numberSendMSB = 0x00;

    iss3Call->comand = S3_CALL;
    iss3Call->speed = 0x00;
    iss3Call->nbp = 0x00;
    iss3Call->numberSendLSB = 0x00;
    iss3Call->numberSendMSB = 0x00;
    iss3Call->numberReceiverLSB = 0x00;
    iss3Call->numberReceiverMSB = 0x00;


    Mod_Onix = S3_FREE;
    Mod_SSS = S3_FREE;

    Mod_Onix_ms = S3_FREE;

    setNumber = false;
    timerCount = 0;
    flagNumber = false;

    systemSchet = 2;

    timerCall = 0;

    countEnterCall =0;

}

void OnixB3::comandCall(QList<quint16> number_1,QList<quint16> number_2)
{

}

void OnixB3::cansel()
{
    Mod_Onix = S3_HANGUP;

    iss3Call->comand = S3_HANGUP;


    QByteArray *mass = new QByteArray();

    mass->append(static_cast<char>((iss3Call->speed << 6) +(iss3Call->nbp << 4) + iss3Call->comand));
    mass->append(static_cast<char>(iss3Call->numberSendLSB));
    mass->append(static_cast<char>(iss3Call->numberSendMSB));
    mass->append(static_cast<char>(iss3Call->numberReceiverLSB));
    mass->append(static_cast<char>(iss3Call->numberReceiverMSB));



    emit log(QString::number(index) + ") OnixB отправил от SSS команду <b>ОТБОЙ</b>\n");
    emit log("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call->comand)+"</b>)\n");
    emit log("2 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberSendLSB)+" </b>)\n");
    emit log("3 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberSendMSB)+" </b>)\n");
    emit log("4 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberReceiverLSB)+" </b>)\n");
    emit log("5 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberReceiverMSB)+" </b>)\n");
    emit log("\n");


    for (int i=0; i < mass->count();i++)
    {

        emit writeSSS_ISS3(mass->at(i));
    }
}

void OnixB3::setNumber_1(QList<quint16> number_1, QList<quint16> number_2)
{

}

void OnixB3::setMod_Onix(quint16 data)
{

}

QString OnixB3::nomer(QList<quint16> number)
{
    QString str;
    return str;
}


// «РЕГИСТРАЦИЯ», «ПОДТВЕРЖДЕНИЕ РЕГИСТРАЦИИ», «ОТКАЗ ОТ РЕГИСТРАЦИИ», «ПОДТВЕРЖДЕНИЕ ОТКАЗА ОТ РЕГИСТРАЦИИ»
void OnixB3::ISS3_Receipt_fn(quint8 CNS, quint8 numberSendMSB, quint8 numberSendLSB, quint8 RchVK, quint8 Priorytet)
{
    QByteArray mass;

    mass.append(static_cast<char>(CNS));
    mass.append(static_cast<char>(numberSendMSB));
    mass.append(static_cast<char>(numberSendLSB));
    mass.append(static_cast<char>(RchVK));
    mass.append(static_cast<char>(Priorytet));

    ISS3_Receipt *iss3 = (ISS3_Receipt*)mass.data();

    qDebug() << QString::number(iss3->comand, 16);
    qDebug() << QString::number(iss3->nbp, 16);
    qDebug() << QString::number(iss3->speed, 16);
    qDebug() << QString::number(iss3->numberSendMSB, 16) << QString::number(iss3->numberSendLSB, 16);
    qDebug() << QString::number(iss3->RChVK, 16);
    qDebug() << QString::number(iss3->Rezerv1, 16);
    qDebug() << QString::number(iss3->Priopy, 16);
    qDebug() << QString::number(iss3->Rezerv2, 16);


    for (int i=0; i < mass.count();i++) {

        emit writeSSS_ISS3(mass[i]);
    }



}

//«ВЫЗОВ», «ОТВЕТ РТР», «ОТВЕТ ЗС», «ОТБОЙ ЗС», «ОТБОЙ ПО ПРИОРИТЕТУ», «ЗАНЯТА ЗС»
void OnixB3::ISS3_Call_fn(quint8 CNS, quint8 numberSendMSB, quint8 numberSendLSB, quint8 numberReceiverMSB, quint8 numberReceiverLSB)
{
    QByteArray mass;

    mass.append(static_cast<char>(CNS));
    mass.append(static_cast<char>(numberSendMSB));
    mass.append(static_cast<char>(numberSendLSB));
    mass.append(static_cast<char>(numberReceiverMSB));
    mass.append(static_cast<char>(numberReceiverLSB));

    ISS3_Call *iss3 = (ISS3_Call*)mass.data();

    qDebug() << QString::number(iss3->comand, 16);
    qDebug() << QString::number(iss3->nbp, 16);
    qDebug() << QString::number(iss3->speed, 16);
    qDebug() << QString::number(iss3->numberSendMSB, 16) << QString::number(iss3->numberSendLSB, 16);
    qDebug() << QString::number(iss3->numberReceiverMSB, 16) << QString::number(iss3->numberReceiverLSB, 16);


    for (int i=0; i < mass.count();i++) {

        emit writeSSS_ISS3(mass[i]);
    }

}

// «СВОБОДЕН»
void OnixB3::ISS3_Free_fn(quint8 CNS, quint8 numberSendMSB, quint8 numberSendLSB, quint8 Rezerv1, quint8 Rezerv2)
{
    QByteArray mass;

    mass.append(static_cast<char>(CNS));
    mass.append(static_cast<char>(numberSendMSB));
    mass.append(static_cast<char>(numberSendLSB));
    mass.append(static_cast<char>(Rezerv1));
    mass.append(static_cast<char>(Rezerv2));

    ISS3_Free *iss3 = (ISS3_Free*)mass.data();

    qDebug() << QString::number(iss3->comand, 16);
    qDebug() << QString::number(iss3->nbp, 16);
    qDebug() << QString::number(iss3->speed, 16);
    qDebug() << QString::number(iss3->numberSendMSB, 16) << QString::number(iss3->numberSendLSB, 16);
    qDebug() << QString::number(iss3->Rezerv1, 16) << QString::number(iss3->Rezerv2, 16);


    for (int i=0; i < mass.count();i++) {

        emit writeSSS_ISS3(mass[i]);
    }
}

void OnixB3::ISS3_ISS3_Refusal_fn(quint8 CNS, quint8 numberSendMSB, quint8 numberSendLSB, quint8 numberReceiverMSB, quint8 numberReceiverLSB)
{
    QByteArray mass;

    mass.append(static_cast<char>(CNS));
    mass.append(static_cast<char>(numberSendMSB));
    mass.append(static_cast<char>(numberSendLSB));
    mass.append(static_cast<char>(numberReceiverMSB));
    mass.append(static_cast<char>(numberReceiverLSB));

    ISS3_Refusal *iss3 = (ISS3_Refusal*)mass.data();

    qDebug() << QString::number(iss3->comand, 16);
    qDebug() << QString::number(iss3->why, 16);
    qDebug() << QString::number(iss3->numberSendMSB, 16) << QString::number(iss3->numberSendLSB, 16);
    qDebug() << QString::number(iss3->numberReceiverMSB, 16) << QString::number(iss3->numberReceiverLSB, 16);


    for (int i=0; i < mass.count();i++) {

        emit writeSSS_ISS3(mass[i]);
    }
}

void OnixB3::iss3_setSeitings(quint8 npb, quint8 speed, quint8 priori, quint8 rchvk,quint8 number_LSB, quint8 number_MSB)
{
    iss3Free->nbp = npb;
    iss3Free->speed = speed;

    iss3Receipt->nbp = npb;
    iss3Receipt->speed = speed;
    iss3Receipt->Priopy = priori;
    iss3Receipt->RChVK = rchvk;
    iss3Receipt->numberSendLSB = number_LSB;
    iss3Receipt->numberSendMSB = number_MSB;
    iss3Receipt->Rezerv1 = 0x00;
    iss3Receipt->Rezerv2 = 0x00;



    qDebug () <<  get_Byte4(iss3Receipt->Priopy);

    qDebug () <<  get_Byte2(iss3Receipt->nbp);

    qDebug () <<  get_Byte2(iss3Receipt->speed);

    qDebug () <<  get_Byte2(iss3Receipt->RChVK);



}

void OnixB3::Call(quint8 number_LSB, quint8 number_MSB, quint8 numberCall_LSB, quint8 numberCall_MSB)
{

    Mod_Onix = S3_CALL;

    iss3Call->comand = S3_CALL;
    iss3Call->nbp = iss3Receipt->nbp;
    iss3Call->speed = iss3Receipt->speed;

    iss3Call->numberSendLSB = number_LSB;
    iss3Call->numberSendMSB = number_MSB;
    iss3Call->numberReceiverLSB = numberCall_LSB;
    iss3Call->numberReceiverMSB = numberCall_MSB;

    qDebug () << number_LSB << endl <<  number_MSB << endl << numberCall_LSB << endl << numberCall_MSB;


    qDebug () << iss3Receipt->numberSendLSB << " = " << iss3Call->numberSendLSB;
    qDebug () << iss3Receipt->numberSendMSB << " = " << iss3Call->numberSendMSB;


    QByteArray *mass = new QByteArray();

    mass->append(static_cast<char>((iss3Call->speed << 6) +(iss3Call->nbp << 4) + iss3Call->comand));
    mass->append(static_cast<char>(iss3Call->numberSendLSB));
    mass->append(static_cast<char>(iss3Call->numberSendMSB));
    mass->append(static_cast<char>(iss3Call->numberReceiverLSB));
    mass->append(static_cast<char>(iss3Call->numberReceiverMSB));

    emit log(QString::number(index) + ") OnixB отправил от SSS команду <b>ВЫЗОВ</b>\n");
    emit log("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call->comand)+"</b>)\n");
    emit log("2 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberSendLSB)+" </b>)\n");
    emit log("3 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberSendMSB)+" </b>)\n");
    emit log("4 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberReceiverLSB)+" </b>)\n");
    emit log("5 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberReceiverMSB)+" </b>)\n");
    emit log("\n");


    for (int i=0; i < mass->count();i++)
    {

        emit writeSSS_ISS3(mass->at(i));
    }

}

void OnixB3::CallEnter()
{
    countEnterCall++;

    switch (countEnterCall) {
        case 1:
        {
                timerCall = 0;

                Mod_Onix = S3_ZS;

                iss3Call->comand = S3_ZS;
                iss3Call->nbp = iss3Receipt->nbp;
                iss3Call->speed = iss3Receipt->speed;


                QByteArray *mass = new QByteArray();

                mass->append(static_cast<char>((iss3Call->speed << 6) +(iss3Call->nbp << 4) + iss3Call->comand));
                mass->append(static_cast<char>(iss3Call->numberSendLSB));
                mass->append(static_cast<char>(iss3Call->numberSendMSB));
                mass->append(static_cast<char>(iss3Call->numberReceiverLSB));
                mass->append(static_cast<char>(iss3Call->numberReceiverMSB));

                emit log(QString::number(index) + ") OnixB отправил от SSS команду <b>ОТВЕТ ЗС</b>\n");
                emit log("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call->comand)+"</b>)\n");
                emit log("2 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberSendLSB)+" </b>)\n");
                emit log("3 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberSendMSB)+" </b>)\n");
                emit log("4 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberReceiverLSB)+" </b>)\n");
                emit log("5 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberReceiverMSB)+" </b>)\n");
                emit log("\n");


                for (int i=0; i < mass->count();i++)
                {

                    emit writeSSS_ISS3(mass->at(i));
                }

            break;
        }
        case 2:
        {
            flag_messege = -2;

            Mod_Onix = S3_WORK;

            iss3Call->comand = S3_WORK;
            iss3Call->nbp = iss3Receipt->nbp;
            iss3Call->speed = iss3Receipt->speed;


            QByteArray *mass = new QByteArray();

            mass->append(static_cast<char>((iss3Call->speed << 6) +(iss3Call->nbp << 4) + iss3Call->comand));
            mass->append(static_cast<char>(iss3Call->numberSendLSB));
            mass->append(static_cast<char>(iss3Call->numberSendMSB));
            mass->append(static_cast<char>(iss3Call->numberReceiverLSB));
            mass->append(static_cast<char>(iss3Call->numberReceiverMSB));

            emit log(QString::number(index) + ") OnixB отправил от SSS команду <b>РАБОТА</b>\n");
            emit log("1 Байт( Cкорость: <b>"+get_Byte2(iss3Call->speed)+"</b> НПБ: <b>" +get_Byte2(iss3Call->nbp)+"</b> Команда: <b>"+get_Byte4(iss3Call->comand)+"</b>)\n");
            emit log("2 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberSendLSB)+" </b>)\n");
            emit log("3 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberSendMSB)+" </b>)\n");
            emit log("4 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberReceiverLSB)+" </b>)\n");
            emit log("5 Байт(байт номера: <b>" +get_Byte8(iss3Call->numberReceiverMSB)+" </b>)\n");
            emit log("\n");


            for (int i=0; i < mass->count();i++)
            {

                emit writeSSS_ISS3(mass->at(i));
            }

            break;
        }
        default: countEnterCall=0;break;
    }

}

