#include "visualize.h"
#include "ui_visualize.h"

Visualize::Visualize(QWidget* parent /*= nullptr*/) : QDialog(parent),
m_ui(std::make_unique<Ui::visualizeui>())
{
	m_ui->setupUi(this);

	setWindowTitle("JUL14Ns Audio Mods :: Visualizer");

	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	m_ui->agcPlot->axisY->setRange(0, 2);
	m_ui->agcPlot->axisY->setTitleText("Calculated AGC Adjustment (Factor)");
	m_ui->agcPlot->axisX->setTitleText("Time since monitoring start");
	m_ui->lufsPlot->axisY->setRange(-70, 10);
	m_ui->lufsPlot->axisY->setTitleText("Volume before Compressor (LUFS)");
	m_ui->lufsPlot->axisX->setTitleText("Time since monitoring start");
	m_ui->lufsAgcPlot->axisY->setRange(-70, 10);
	m_ui->lufsAgcPlot->axisY->setTitleText("Volume after Compressor (LUFS)");
	m_ui->lufsAgcPlot->axisX->setTitleText("Time since monitoring start");

	time = QTime::currentTime();
}

void Visualize::showEvent(QShowEvent*)
{
	time.restart();
	m_ui->agcPlot->resetData();
	m_ui->lufsPlot->resetData();
	m_ui->lufsAgcPlot->resetData();
}

void Visualize::addAgcData(double data)
{
	if (!isVisible()) return;
	if (m_ui->tabWidget->currentIndex() != 0) return;
	double key = time.elapsed() / 1000.0;
	m_ui->agcPlot->addDataPoint(key, data);
}

void Visualize::addLufsData(double data)
{
	if (!isVisible()) return;
	if (m_ui->tabWidget->currentIndex() != 1) return;
	double key = time.elapsed() / 1000.0;
	m_ui->lufsPlot->addDataPoint(key, data);
}

void Visualize::addLufsAgcData(double data)
{
	if (!isVisible()) return;
	if (m_ui->tabWidget->currentIndex() != 2) return;
	double key = time.elapsed() / 1000.0;
	m_ui->lufsAgcPlot->addDataPoint(key, data);
}