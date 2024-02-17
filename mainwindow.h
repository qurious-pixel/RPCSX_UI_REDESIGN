#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QSignalMapper>
#include <QToolButton>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
	
private slots:
    void on_actionAdd_Games_triggered();
    void on_actionExit_triggered();
    void on_actionInstall_Firmware_triggered();
    void on_actionBoot_Game_triggered();
    void on_actionReset_Settings_triggered();
    void on_actionSet_FPS_triggered();

private:
    Ui::MainWindow *ui;
    void LoadSettings();
    void SaveSettings();
    void SetGameTitle(QString &gamePath, QToolButton* button);
    void LightMode();
    void DarkMode();
};

#endif // MAINWINDOW_H

