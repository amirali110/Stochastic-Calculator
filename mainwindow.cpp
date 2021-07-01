#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "chart.h"
#include "stoch_calculator.h"

#include <QDateTime>
#include <QPushButton>
#include <QTimer>
#include <QLineSeries>
#include <QLineSeries>
#include <QtCharts>
#include <QVBoxLayout>
#include <QThread>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow) {
  ui->setupUi(this);

  connect(ui->pushButton, &QPushButton::clicked, [this] {
    QString timeframe_user = ui->comboBox->currentText();
    QString timeframe;

    if (timeframe_user == "1m"
        || timeframe_user == "3m"
        || timeframe_user == "5m"
        || timeframe_user == "15m"
        || timeframe_user == "30m") {
      timeframe = timeframe_user.remove("m");
    } else if (timeframe_user == "1h")  {
      timeframe = "60";
    } else if (timeframe_user == "4h") {
      timeframe = "240";
    } else if (timeframe_user == "12h") {
      timeframe = "720";
    } else if (timeframe_user == "1d") {
      timeframe = "1440";
    } else if (timeframe_user == "1w") {
      timeframe = "10080";
    }

    auto const timer = new QTimer;
    auto const window = new chart(ui->plainTextEdit->text(),
                                  timeframe,
                                  ui->plainTextEdit_3->text(),
                                  ui->plainTextEdit_4->text(),
                                  ui->plainTextEdit_5->text());

    connect(timer, &QTimer::timeout, [window, this]() {
      ui->text->append(window->symbol_value + " --> " + QDateTime::currentDateTime().toString());
    });
    timer->start(timeframe.toInt() * 60000);
  });
}

MainWindow::~MainWindow() {
  delete ui;
}

