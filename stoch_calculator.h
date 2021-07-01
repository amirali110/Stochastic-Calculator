#ifndef STOCH_CALCULATOR_H
#define STOCH_CALCULATOR_H

#include <QString>
#include <QVariant>

class stoch_calculator final {
 public:
  void set_k_period(int const period);
  void set_smoothing(int const smoothing);
  double get_K_stochastic(int timeframe_num, QString const& symbol);
  double get_D_stochastic(int const preiod);

 private:
  int m_kperiod;
  int m_smooth;

 private:
  struct stoch_data_t final {
    using double_vec = std::vector<double>;
    double_vec high, low;
    double close;
  };

 private:
  double simple_stochastic_calculate(stoch_data_t data);
};

#endif // STOCH_CALCULATOR_H
