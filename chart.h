#ifndef CHART_H
#define CHART_H

#include <QObject>
#include <QWidget>

class chart {
 public:
  chart(QString const& symbol,
        QString const& timeframe,
        QString const& k_period,
        QString const& smooth,
        QString const& D_period);

  int num = 0;
  int D_num = 0;
  std::vector<double> all_K_stochastic;
  QString symbol_value;
};

#endif // CHART_H
