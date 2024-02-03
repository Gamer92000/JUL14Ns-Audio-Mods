#pragma once

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QValueAxis>

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
class QAudioInput;
class QAudioDeviceInfo;
QT_END_NAMESPACE

class Graph : public QWidget
{
    Q_OBJECT

public:
    QValueAxis* axisX;
    QValueAxis* axisY;
    explicit Graph(QWidget *parent);
    ~Graph();
    void addDataPoint(double key, double data);
    void resetData();

private:
    QChart *m_chart;
    QLineSeries *m_series;
    QVector<QPointF> m_buffer;
};