#ifndef STOCH_CALCULATOR_H
#define STOCH_CALCULATOR_H
#include <QString>
#include <QVariant>
class Stoch_calculator
{

private:

    int k_period;
    int smooth;
    double simple_stochastic_calculate(std::vector<double> high,std::vector<double> low,double close);

public:

    Stoch_calculator();
    void set_k_period(int period);
    void set_smoothing(int smoothing);
    double Get_K_stochastic(int timeframe_num, QString symbol);
    double Get_D_stochastic(int preiod);



};

#endif // STOCH_CALCULATOR_H
