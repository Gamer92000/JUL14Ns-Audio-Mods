#include "config.h"
#include "ui_config.h"
#include <QRegularExpression>

Config::Config(const QString& configLocation, QWidget* parent /* = nullptr */) : QDialog(parent),
m_ui(std::make_unique<Ui::configui>()),
m_settings(std::make_unique<QSettings>(configLocation, QSettings::IniFormat, this))
{
	m_ui->setupUi(this);

	setWindowTitle("JUL14Ns Audio Mods :: Config");

	// Connect UI Elements.
	connect(m_ui->pbOk, &QPushButton::clicked, this, [&] {
		this->saveSettings();
		this->close();
	});
	connect(m_ui->pbApply, &QPushButton::clicked, this, [&] {
		this->saveSettings();
	});
	connect(m_ui->pbCancel, &QPushButton::clicked, this, &QWidget::close);

	connect(m_ui->vad_cutoff, &QSlider::valueChanged, this, [&](int value) {
		m_ui->vad_cutoff_percentage->setText(QString::asprintf("%d %", value));
	});
	connect(m_ui->vad_rolloff, &QSlider::valueChanged, this, [&](int value) {
		m_ui->vad_rolloff_time->setText(QString::asprintf("%d ms", value * 10));
	});

	adjustSize();
	setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

Config::~Config() {
	m_settings->sync();
}


void Config::setConfigOption(const QString& option, const QVariant& value) {
	m_settings->setValue(option, value);
}

QVariant Config::getConfigOption(const QString& option) const {
	return m_settings->value(option);
}

void Config::showEvent(QShowEvent* /* e */) {
	adjustSize();
	loadSettings();
}

void Config::changeEvent(QEvent* e) {
	if (e->type() == QEvent::StyleChange && isVisible()) {
		adjustSize();
	}
}

void Config::saveSettings() {
	setConfigOption("inputFilter", m_ui->input_filter->isChecked());
	setConfigOption("inputVAD", m_ui->input_vad->isChecked());
	setConfigOption("vadCutoff", m_ui->vad_cutoff->value());
	setConfigOption("vadRolloff", m_ui->vad_rolloff->value());
	setConfigOption("inputAGC", m_ui->input_agc->isChecked());

	setConfigOption("outputFilter", m_ui->output_filter->isChecked());
	QStringList uuids{};
	//for (auto& u : m_ui->uuids->toPlainText().split(QRegExp{ "[\n,;|:]" })) {
	for (auto& u : m_ui->uuids->toPlainText().split(QRegularExpression{ "[\n,;|:]" })) {
		uuids.push_back(u.trimmed());
	}
	setConfigOption("filterIncomingUuids", uuids);
}

void Config::loadSettings() {
	m_ui->input_filter->setChecked(getConfigOption("inputFilter").toBool());
	m_ui->input_vad->setChecked(getConfigOption("inputVAD").toBool());
	m_ui->vad_cutoff->setValue(getConfigOption("vadCutoff").toInt());
	m_ui->vad_rolloff->setValue(getConfigOption("vadRolloff").toInt());
	m_ui->input_agc->setChecked(getConfigOption("inputAGC").toBool());

	m_ui->output_filter->setChecked(getConfigOption("outputFilter").toBool());
	QStringList uuids = getConfigOption("filterIncomingUuids").toStringList();
	bool first = true;
	QString uuid_string = "";
	for (auto& uuid : uuids) {
		if (!first) uuid_string += "\n";
		else first = false;
		uuid_string += uuid;
	}
	m_ui->uuids->setPlainText(uuid_string);
}