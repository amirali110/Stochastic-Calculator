#include "chart.h"
#include <stoch_calculator.h>
#include <QDateTime>
#include <QPushButton>
#include <QTimer>
#include <QLineSeries>
#include <QLineSeries>
#include <QtCharts>





chart::chart(QString symbol,QString timeframe,QString k_period,QString smooth,QString D_period)

{


    auto Timer=new QTimer;

    auto K_seri=new QtCharts::QLineSeries;
    auto D_seri=new QtCharts::QLineSeries;
    auto LINE_20=new QtCharts::QLineSeries;
    auto LINE_80=new QtCharts::QLineSeries;


    auto mainx=new QMainWindow;

    auto chart=new QtCharts::QChart;

    QChartView *chart_v=new QChartView(chart);

    chart_v->setRenderHint(QPainter::Antialiasing);
    chart->setTheme(chart->ChartThemeDark);
    chart->legend()->tr("K");
    chart->setTitle(symbol);




    mainx->setCentralWidget(chart_v);
    mainx->resize(300,300);
    mainx->show();




QObject::connect(Timer,&QTimer::timeout,[this,K_seri,D_seri,LINE_20,LINE_80,chart,timeframe,symbol,k_period,D_period,smooth](){




Stoch_calculator cal;

cal.set_k_period(k_period.toInt());
cal.set_smoothing(smooth.toInt());


double value=cal.Get_K_stochastic(timeframe.toInt(),symbol);

double SUM_D_stochastic=0;

double D_stochastic=0;



this->all_K_stochastic.push_back(value);


/*

if(this->num>=D_period.toInt()){
    
    for(int i=0;i<D_period.toInt();i++){
        SUM_D_stochastic+=this->all_K_stochastic[this->num-i];
    }

    D_stochastic=SUM_D_stochastic/D_period.toInt();
    D_seri->append(this->num,D_stochastic);

}
*/

if(this->num+1>=D_period.toInt()){

    if(this->num+1>D_period.toInt()){
        this->all_K_stochastic.erase(all_K_stochastic.begin());
    }


    for(int i=0;i<D_period.toInt();i++){
        SUM_D_stochastic+=this->all_K_stochastic[i];
    }

    D_stochastic=SUM_D_stochastic/D_period.toInt();
    D_seri->append(this->num,D_stochastic);


}

LINE_20->append(this->num,20);
LINE_80->append(this->num,80);

K_seri->append(this->num,value);

 
this->num++;

this->symbol_value="("+symbol+","+timeframe+","+k_period+","+smooth+")" +" : "+QString::number(value)+"--------------"+QString::number(D_stochastic);


chart->removeSeries(K_seri);
chart->removeSeries(D_seri);
chart->removeSeries(LINE_20);
chart->removeSeries(LINE_80);

chart->addSeries(K_seri);
chart->addSeries(D_seri);
chart->addSeries(LINE_20);
chart->addSeries(LINE_80);

chart->createDefaultAxes();


});



Timer->start(timeframe.toInt()*60000);





}








