#ifndef CHART_H
#define CHART_H

#include <QObject>
#include <QWidget>


class chart
{
public:

    chart(QString, QString, QString k_period, QString smooth,QString D_period);
    int num=0;
    int D_num=0;
    std::vector<double> all_K_stochastic;

    QString symbol_value;

};

#endif // CHART_H
