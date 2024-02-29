#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QSignalMapper>
#include <QToolButton>
#include <QLabel>
#include <QDragEnterEvent>
#include <QMimeData>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void InstallDragDropPkg(std::string file, int pkgNum, int nPkg);

public slots:
	
private slots:
    void on_actionAdd_Games_triggered();
    void on_actionExit_triggered();
    void on_actionInstall_Firmware_triggered();
    void on_actionBoot_Game_triggered();
    void on_actionReset_Settings_triggered();
    void on_actionSet_FPS_triggered();
    void on_actionInstallPkg_triggered();

private:
    Ui::MainWindow *ui;
    void LoadSettings();
	void SaveSettings();
	void SetGameTitle(QString &gamePath, QToolButton* button);
    void GetFirmwareVersion(QString &firmwareDirectory);
	void LightMode();
	void DarkMode();

protected:
    void dragEnterEvent(QDragEnterEvent* event1) override {
        if (event1->mimeData()->hasUrls()) {
            event1->acceptProposedAction();
        }
    }

    void dropEvent(QDropEvent* event1) override {
        const QMimeData* mimeData = event1->mimeData();
        if (mimeData->hasUrls()) {
            QList<QUrl> urlList = mimeData->urls();
            int pkgNum = 0;
            int nPkg = urlList.size();
            for (const QUrl& url : urlList) {
                pkgNum++;
                InstallDragDropPkg(url.toLocalFile().toStdString(), pkgNum, nPkg);
            }
        }
    }
};

#endif // MAINWINDOW_H
