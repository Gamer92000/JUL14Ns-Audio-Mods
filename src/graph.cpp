#include "graph.h"

#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioInput>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>

#include <QtWidgets/QVBoxLayout>

QT_CHARTS_USE_NAMESPACE

Graph::Graph(QWidget* parent) :
    QWidget(parent),
    m_chart(new QChart),
    m_series(new QLineSeries)
{
    QChartView* chartView = new QChartView(m_chart);
    chartView->setMinimumSize(500, 300);
    m_chart->addSeries(m_series);
    axisX = new QValueAxis;
    axisX->setLabelFormat("%.02f");
    axisY = new QValueAxis;
    m_chart->addAxis(axisX, Qt::AlignBottom);
    m_series->attachAxis(axisX);
    m_chart->addAxis(axisY, Qt::AlignLeft);
    m_series->attachAxis(axisY);
    m_chart->legend()->hide();
    //m_chart->setTitle("Some cool data!");

    parent->layout()->addWidget(chartView);
}

Graph::~Graph()
{
}

void Graph::addDataPoint(double key, double data)
{
    m_buffer.push_back(QPointF(key, data));
    if (m_buffer.size() > 3200)
    {
        m_buffer.pop_front();
    }
    m_series->replace(m_buffer);
    axisX->setRange(key-30, key);
}

void Graph::resetData()
{
    m_buffer.clear();
    m_series->replace(m_buffer);
}