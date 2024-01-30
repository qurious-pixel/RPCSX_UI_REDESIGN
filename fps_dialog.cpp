#include "fps_dialog.h"
#include "ui_fps_dialog.h"

#include <QDesktopServices>
#include <QSettings>

fps_dialog::fps_dialog(QWidget* parent) : QDialog(parent), ui(new Ui::fps_dialog)
{
	ui->setupUi(this);
	
	ui->fpsSlider->setMinimum(0);
	ui->fpsSlider->setMaximum(120);
}

fps_dialog::~fps_dialog()
{
}

void fps_dialog::on_btnClose_clicked()
{
	accept();
}

void fps_dialog::on_fpsSlider_valueChanged(int value)
{
	
	if (value == 0) {
		ui->fpsCount->setText("unlocked");
	} else {
		ui->fpsCount->setText(QString::number(value));
	}
	QString fpsSlider = (QString::number(value));
    QSettings setting("rpcsx", "rpcsx_ui_settings");
    setting.beginGroup("rpcsx_ui_settings");
    setting.setValue("FPSslider", fpsSlider);
    setting.endGroup();
}

void fps_dialog::on_hudDisplay_stateChanged(int arg1)
{
	QSettings setting("rpcsx", "rpcsx_ui_settings");
    setting.beginGroup("rpcsx_ui_settings");
	if (arg1) {
	QString hudDisplay = "fps";
	setting.setValue("HUDdisplay", hudDisplay);
	} else {
	QString hudDisplay = "no_display";
    setting.setValue("HUDdisplay", hudDisplay);
    }
    setting.endGroup();
}
