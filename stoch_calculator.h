#ifndef STOCH_CALCULATOR_H
#define STOCH_CALCULATOR_H
#include <QString>
#include <QVariant>
class Stoch_calculator
{
public:
    Stoch_calculator();
    int k_period;



    void set_k_period(int period);
    double Get_stochastic(int timeframe_num, QString symbol);



};

#endif // STOCH_CALCULATOR_H
