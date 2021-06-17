#include "chart.h"
#include <stoch_calculator.h>
#include <QDateTime>
#include <QPushButton>
#include <QTimer>
#include <QLineSeries>
#include <QLineSeries>
#include <QtCharts>





chart::chart(QString symbol,QString timeframe,QString period)

{


    auto Timer=new QTimer;

    auto seri=new QtCharts::QLineSeries;

    auto mainx=new QMainWindow;

    auto chart=new QtCharts::QChart;

    QChartView *chart_v=new QChartView(chart);

    chart_v->setRenderHint(QPainter::Antialiasing);
    chart->setTheme(chart->ChartThemeDark);
    chart->legend()->hide();
    chart->setTitle(symbol);




    mainx->setCentralWidget(chart_v);
    mainx->resize(300,300);
    mainx->show();




QObject::connect(Timer,&QTimer::timeout,[this,seri,chart,timeframe,symbol,period](){




Stoch_calculator cal;

cal.set_k_period(period.toInt());


double value=cal.Get_stochastic(timeframe.toInt(),symbol);


seri->append(this->num++,value);


this->symbol_value="("+symbol+","+timeframe+","+period+")" +" : "+QString::number(value);


chart->removeSeries(seri);
chart->addSeries(seri);
chart->createDefaultAxes();


});



Timer->start(timeframe.toInt()*60000);



}







