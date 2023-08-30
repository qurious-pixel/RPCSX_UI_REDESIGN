#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QFrame>
#include <QSignalMapper>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionAdd_Games_triggered();
    void on_actionExit_triggered();
    void on_actionInstall_Firmware_triggered();
    void on_actionBoot_Game_triggered();
    void onFrameClicked(const QString &directoryName);
    void on_actionReset_Settings_triggered();

private:
    Ui::MainWindow *ui;
    void SaveSettings();
    void LoadSettings();
    void CreateNoGamesFrame();

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // MAINWINDOW_H
