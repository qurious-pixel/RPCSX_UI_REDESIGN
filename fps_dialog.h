#pragma once

#include <QDialog>
//#include <QtCore>

namespace Ui
{
	class fps_dialog;
}

class fps_dialog : public QDialog
{
	Q_OBJECT

public:
	fps_dialog(QWidget*);
	~fps_dialog();

private slots:
	void on_btnClose_clicked();
	void on_fpsSlider_valueChanged(int value);
	void on_hudDisplay_stateChanged(int arg1);
	
private:
	std::unique_ptr<Ui::fps_dialog> ui;
};
