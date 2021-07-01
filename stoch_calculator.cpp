#include "stoch_calculator.h"

#include <iostream>
#include <vector>

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
#include <QString>

namespace helper {
namespace constants {
auto constexpr binance_url = "https://api.binance.com/api/v3/klines";
} // namespace constants

QString interval(int const time) noexcept {
  QString result;
  QChar suffix;

  if (time < 60) {
    suffix = 'm';        // minute
  } else if (time >= 60) {
    suffix = 'h';        // hour
    result = QString::number(time / 60);
  } else if (time == 1440) {
    suffix = 'd';         // day
    result = "1";
  } else if (time == 10080) {
    suffix = 'w';         // week
    result = "1";
  }

  return result;
}

QString start_time(int const period, int const smooth, int const time) noexcept {
  auto const epoch = QDateTime::currentSecsSinceEpoch();
  auto const lhr = ((period + smooth) * 60 * time);
  return QString::number(epoch - lhr) + "000";
}

QString end_time(int const time) noexcept {
  auto const epoch = QDateTime::currentSecsSinceEpoch();
  return QString::number(epoch - (time * 60)) + "000";
}

inline QUrl url(int const time, int const period, int const smooth, QString const& symbols) noexcept {
  auto const result = QString("%1?symbol=%2&interval=%3&startTime=%4&endTime=%5")
                      .arg(constants::binance_url)
                      .arg(symbols)
                      .arg(interval(time))
                      .arg(start_time(period, smooth, time))
                      .arg(end_time(time));
  return result;
}
} // helper

void stoch_calculator::set_k_period(int const period) {
  m_kperiod = period;
}

void stoch_calculator::set_smoothing(int const smoothing) {
  m_smooth = smoothing;
}

double stoch_calculator::simple_stochastic_calculate(stoch_data_t data) {
  std::stable_sort(data.high.begin(), data.high.end());
  std::stable_sort(data.low.begin(), data.low.end());

  return (data.close - data.low[0]) / (data.high[m_kperiod - 1] - data.low[0]);
}

double stoch_calculator::get_K_stochastic(int timeframe_num, QString const& symbol) {
  auto const access_manager = new QNetworkAccessManager;
  auto const replay = access_manager->get(QNetworkRequest(helper::url(timeframe_num, m_kperiod, m_smooth, symbol)));

  QEventLoop loop;
  QObject::connect(replay, &QNetworkReply::finished, &loop, &QEventLoop::quit);
  loop.exec();

  QJsonArray array = QJsonDocument::fromJson(replay->readAll()).array();
  double data[m_kperiod + m_smooth - 1][4];                     //data[x][1]--> high
  //data[x][2]--> low
  //data[x][3]--> close

  int i = 0;

  for (auto const& json : array) {
    if (json.type() == QJsonValue::Array) {
      int n = 0;
      for (auto const& element : json.toArray()) {
        if (element.type() == QJsonValue::String) {
          data[i][n] = element.toVariant().toDouble();
          n++;
          if (n == 4) { break; }
        }
      }
    }
    i++;
  }

  stoch_data_t stoch_data;
  double sum_stochastic = 0;
  int x = 0;

  for (int i = 1; i <= m_smooth; i++) {
    int index = 0;

    stoch_data.high.clear();
    stoch_data.low.clear();

    while (index < m_kperiod) {
      stoch_data.high.push_back(data[m_kperiod + m_smooth - 2 - index - x][1]);
      stoch_data.low.push_back(data[m_kperiod + m_smooth - 2 - index - x][2]);
      index++;
    }

    stoch_data.close = data[m_kperiod + m_smooth - 2 - x][3];
    sum_stochastic += simple_stochastic_calculate(stoch_data);
    x++;
  }

  return (sum_stochastic / m_smooth) * 100;
}
