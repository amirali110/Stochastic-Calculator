#include "stoch_calculator.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QObject>
#include <QString>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QVariant>
#include <iostream>
#include <vector>
#include <QString>



Stoch_calculator::Stoch_calculator()
{

}



void Stoch_calculator::set_k_period(int period){

    Stoch_calculator::k_period=period;
}



double Stoch_calculator::Get_stochastic(int timeframe_num, QString symbol){



    QString suffix;         //timeframe type


    if(timeframe_num<60) suffix="m";    //minute

    else if(timeframe_num>=60){
        suffix="h";          //hour
        timeframe_num=timeframe_num/60;

    }else if(timeframe_num==1440){
        suffix="d";           //day
        timeframe_num=1;

    }else if(timeframe_num==10080){
        suffix="w";           //week
        timeframe_num=1;
    }




    QNetworkAccessManager *acc=new QNetworkAccessManager;


    const QString url="https://api.binance.com/api/v3/klines";        //get data from binance



    QNetworkRequest req((QUrl(url+"?symbol="+symbol+"&interval="+QString::number(timeframe_num)+suffix+"&startTime="+QString::number(QDateTime::currentDateTime().toTime_t()-((Stoch_calculator::k_period+1)*60*timeframe_num))+"000"+"&endTime="+QString::number(QDateTime::currentDateTime().toTime_t()-(timeframe_num*60))+"000")));


    QNetworkReply *rep=acc->get(req);


    QEventLoop loop;
    QObject::connect(rep,&QNetworkReply::finished,&loop,&QEventLoop::quit);
    loop.exec();


    auto doc=QJsonDocument::fromJson(rep->readAll());

    double data[Stoch_calculator::k_period][4];


    QJsonArray array=doc.array();


    int i=0;

for(auto const json:array){


    if(json.type()==QJsonValue::Array){

        int n=0;


        for(auto const element:json.toArray()){


            if(element.type()==QJsonValue::String){


            QVariant var=element.toVariant();

            data[i][n]=var.toDouble();

            n++;

            if(n==4) break;

            }

        }

    }


    i++;


}



    std::vector <double> high;
    std::vector <double> low;


    int index=0;


    while (index<Stoch_calculator::k_period) {

        high.push_back(data[index][1]);
        low.push_back(data[index][2]);
        index++;

    }




    std::stable_sort(high.begin(),high.end());
    std::stable_sort(low.begin(),low.end());


    double stochastic_level=(data[Stoch_calculator::k_period-1][3]-low[0])/(high[Stoch_calculator::k_period-1]-low[0]);       //Stochastic_formula


    return stochastic_level*100;




}
































