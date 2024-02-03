#pragma once
#include "graph.h"
#include <mutex>
#include <QTime>
#include <QDialog>

namespace Ui {
	class visualizeui;
}

class Visualize : public QDialog
{
	Q_OBJECT
public:
	Visualize(QWidget* parent = nullptr);
	~Visualize();
	void addAgcData(double data);
	void addLufsData(double data);
	void addLufsAgcData(double data);

protected:
	void showEvent(QShowEvent* e) override;

private:
	std::unique_ptr<Ui::visualizeui> m_ui;
	QTime time;
};