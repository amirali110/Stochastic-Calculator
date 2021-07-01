#include "chart.h"
#include "stoch_calculator.h"

#include <QDateTime>
#include <QPushButton>
#include <QTimer>
#include <QLineSeries>
#include <QLineSeries>
#include <QtCharts>

chart::chart(QString const& symbol,
             QString const& timeframe,
             QString const& k_period,
             QString const& smooth,
             QString const& D_period) {
  auto const timer = new QTimer;
  auto const K_seri = new  QLineSeries;
  auto const D_seri = new  QLineSeries;
  auto const LINE_20 = new QLineSeries;
  auto const LINE_80 = new QLineSeries;
  auto const mainx = new QMainWindow;
  auto const chart = new QChart;
  auto const chart_v = new QChartView(chart);

  chart_v->setRenderHint(QPainter::Antialiasing);
  chart->setTheme(chart->ChartThemeDark);
  chart->legend()->tr("K");
  chart->setTitle(symbol);

  mainx->setCentralWidget(chart_v);
  mainx->resize(300, 300);
  mainx->show();

  QObject::connect(timer, &QTimer::timeout, [ = ] {
    stoch_calculator cal;
    cal.set_k_period(k_period.toInt());
    cal.set_smoothing(smooth.toInt());

    double value = cal.get_K_stochastic(timeframe.toInt(), symbol);
    double SUM_D_stochastic = 0;
    double D_stochastic = 0;

    all_K_stochastic.push_back(value);

    if (num + 1 >= D_period.toInt()) {
      if (num + 1 > D_period.toInt()) {
        all_K_stochastic.erase(all_K_stochastic.begin());
      }

      for (int i = 0; i < D_period.toInt(); i++) {
        SUM_D_stochastic += all_K_stochastic[i];
      }

      D_stochastic = SUM_D_stochastic / D_period.toInt();
      D_seri->append(num, D_stochastic);
    }

    LINE_20->append(num, 20);
    LINE_80->append(num, 80);
    K_seri->append(num, value);

    num++;
    symbol_value = "(" + symbol + "," + timeframe + ","
                   + k_period + "," + smooth + ")"
                   + " : " + QString::number(value) + "--------------" + QString::number(D_stochastic);

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

  timer->start(timeframe.toInt() * 60000);
}
