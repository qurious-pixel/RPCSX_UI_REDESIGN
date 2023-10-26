#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QApplication>
#include <QFileDialog>
#include <QSettings>
#include <QDebug>
#include <QFrame>
#include <QResizeEvent>
#include <QDir>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDesktopServices>
#include <QProcess>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    LoadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::LoadSettings()
{
    // Clear all frames and their children
    QLayout* layout = ui->centralwidget->layout();
    if (layout) {
        QLayoutItem *child;
        while ((child = layout->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }
        delete layout;
    }

    QSettings setting("rpcsx", "rpcsx_ui_settings");
    setting.beginGroup("rpcsx_ui_settings");
    QString gamesDirectory = setting.value("GamesDirectory").toString();
    setting.setValue("ChosenGame", "");
    setting.endGroup();

    // Load the Games Folder
    QDir folder(gamesDirectory);

    QStringList directoryList = folder.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    QVBoxLayout *centralLayout = new QVBoxLayout(ui->centralwidget);

    if (directoryList.isEmpty()) {
        // Create a frame for "No Games Found" message
        QFrame *noGamesFrame = new QFrame(ui->centralwidget);
        noGamesFrame->setFrameShape(QFrame::NoFrame);
        noGamesFrame->setLineWidth(0);
        noGamesFrame->setObjectName("noGamesFrame");
        noGamesFrame->setStyleSheet("background-color: #3A3B3C;");

        QVBoxLayout *noGamesLayout = new QVBoxLayout(noGamesFrame);
        noGamesLayout->setAlignment(Qt::AlignCenter);

        QLabel *bigTextLabel = new QLabel("No Games Found<br>RPCSX could not find any games<br>Set a games directory to proceed.", noGamesFrame);
        bigTextLabel->setStyleSheet("font-size: 20pt; font-weight: bold; color: black;");
        bigTextLabel->setAlignment(Qt::AlignCenter);

        noGamesLayout->addWidget(bigTextLabel);

        centralLayout->addWidget(noGamesFrame);

        qDebug() << noGamesLayout;
    } else {
        foreach (QString directoryName, directoryList) {
            qDebug() << directoryName;

            QFrame *frame = new QFrame(ui->centralwidget);
            frame->setFrameShape(QFrame::NoFrame);
            frame->setLineWidth(0);
            frame->setStyleSheet("background-color: lightgray;");
            frame->setProperty("DirectoryName", directoryName);
            frame->installEventFilter(this);
            frame->setFixedHeight(121);

            QHBoxLayout *frameLayout = new QHBoxLayout(frame);
            frameLayout->setSpacing(10);
            frameLayout->setContentsMargins(10, 0, 10, 0);

            QLabel *imageLabel = new QLabel(frame);
            QPixmap imagePixmap(gamesDirectory + "/" + directoryName + "/" + "sce_sys/icon0.png");
            imagePixmap = imagePixmap.scaled(QSize(100, 100), Qt::KeepAspectRatio);
            imageLabel->setPixmap(imagePixmap);

            QLabel *gameNameLabel = new QLabel(frame); // "Game Name Here", 

            QPushButton *button1 = new QPushButton("View Game Files", frame);
            QPushButton *button2 = new QPushButton("Game Settings", frame);

            // Use CSS to set the background image for button1
            button1->setStyleSheet("QPushButton {"
                                   "background-image: url(/home/kevin/Downloads/100-Number-Transparent-Images-PNG.png);"
                                   "background-repeat: no-repeat;"
                                   "background-position: center;"
                                   "}");

            button1->setFixedSize(105, 105);
            button2->setFixedSize(105, 105);

            connect(button1, &QPushButton::clicked, [gamesDirectory, directoryName]() {
                QString gameFolderPath = gamesDirectory + "/" + directoryName;
                QDesktopServices::openUrl(QUrl::fromLocalFile(gameFolderPath));
            });

            frameLayout->addWidget(imageLabel);
            frameLayout->addWidget(gameNameLabel);
            frameLayout->addWidget(button1);
            frameLayout->addWidget(button2);

            centralLayout->addWidget(frame);
        }
    }

    int totalGames = directoryList.count();
    QString actionText = QString("RPCSX - %1 Games - Firmware 9.00").arg(totalGames);
    ui->actionAV_GC_FV->setText(actionText);
}


void MainWindow::SaveSettings()
{
    QSettings setting("rpcsx", "rpcsx_ui_settings");
    setting.beginGroup("rpcsx_ui_settings");
    setting.setValue("FirmwareDirectory", "This is the folder test");
    setting.endGroup();
}

void MainWindow::on_actionAdd_Games_triggered()
{
    QString gamesDirectory = QFileDialog::getExistingDirectory(this, "Select Games Folder");
    QSettings setting("rpcsx", "rpcsx_ui_settings");
    setting.beginGroup("rpcsx_ui_settings");
    setting.setValue("GamesDirectory", gamesDirectory);
    setting.endGroup();

    QLayoutItem *child;
    while ((child = ui->centralwidget->layout()->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    LoadSettings();
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionInstall_Firmware_triggered()
{
    QString firmwareDirectory = QFileDialog::getExistingDirectory(this,"Select Firmware Folder");
    QSettings setting("rpcsx", "rpcsx_ui_settings");
    setting.beginGroup("rpcsx_ui_settings");
    setting.setValue("FirmwareDirectory", firmwareDirectory);
    setting.endGroup();
}

void MainWindow::on_actionBoot_Game_triggered()
{
    QSettings settings("rpcsx", "rpcsx_ui_settings");
    settings.beginGroup("rpcsx_ui_settings");

    QString firmwareDirectory = settings.value("FirmwareDirectory").toString();
    QString chosenGame = settings.value("ChosenGame").toString();

    QString command = QString("rm -f /dev/shm/rpcsx-* && mangohud /home/""/Desktop/rpcsx/build/bin/rpcsx-os --mount %1 /system --mount %2 /app0 /app0/eboot.bin").arg(firmwareDirectory, chosenGame);

    qDebug() << "Command to be executed:" << command;

    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);

    if (!process.startDetached("/bin/bash", QStringList() << "-c" << command)) {
        qDebug() << "Failed to start the process.";
        QMessageBox::critical(this, "Error", "Failed to start the game. Check your setup.");
    } else {
        process.waitForFinished(-1); // Wait indefinitely

        QByteArray output = process.readAll();
        qDebug() << "Command output:" << output;
        qDebug() << "Command executed successfully.";
    }

    settings.endGroup();
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    QWidget *centralWidget = ui->centralwidget;
    int centralWidgetWidth = centralWidget->width();
    int centralWidgetHeight = centralWidget->height();

    QList<QFrame *> frames = centralWidget->findChildren<QFrame *>();
    foreach (QFrame *frame, frames) {
        if (frame->objectName() == "noGamesFrame") {
            frame->setFixedWidth(centralWidgetWidth - 20);
            frame->setFixedHeight(centralWidgetHeight - 20);
            continue;
        }

        frame->setFixedWidth(centralWidgetWidth - 20);
        frame->setFixedHeight(121);

        qDebug() << frame->height();
        qDebug() << frame->width();
    }
}


void MainWindow::onFrameClicked(const QString &directoryName)
{
    QSettings setting("rpcsx", "rpcsx_ui_settings");
    setting.beginGroup("rpcsx_ui_settings");
    QString gamesDirectory = setting.value("GamesDirectory").toString();

    if (setting.value("ChosenGame").toString() == (gamesDirectory + "/" + directoryName)) {
        setting.setValue("ChosenGame", "");
    } else {
        setting.setValue("ChosenGame", gamesDirectory + "/" + directoryName);
    }

    qDebug() << "Frame clicked: " << setting.value("ChosenGame").toString();

    if (setting.value("ChosenGame").toString() == "") {
        ui->actionBoot_Game->setEnabled(false);
    } else {
        ui->actionBoot_Game->setEnabled(true);
    }
    setting.endGroup();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) { // Check if left button is pressed
            QFrame *clickedFrame = qobject_cast<QFrame*>(obj);
            if (clickedFrame) {
                QString newStyle;

                QString currentStyle = clickedFrame->styleSheet();
                if (currentStyle.contains("background-color: blue")) {
                    newStyle = "background-color: lightgray;";
                } else {
                    newStyle = "background-color: blue;";
                }

                foreach (QObject *child, centralWidget()->children()) {
                    QFrame *frame = qobject_cast<QFrame*>(child);
                    if (frame && frame != clickedFrame) {
                        frame->setStyleSheet("background-color: lightgray;");
                    }
                }

                clickedFrame->setStyleSheet(newStyle);

                QString directoryName = clickedFrame->property("DirectoryName").toString();
                onFrameClicked(directoryName);

                return true;
            }
        }
    }
    return QMainWindow::eventFilter(obj, event);
}



void MainWindow::on_actionReset_Settings_triggered()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Reset Settings", "All settings will be wiped. Are you sure?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QLayoutItem *child;
        while ((child = ui->centralwidget->layout()->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }

        QSettings settings("rpcsx", "rpcsx_ui_settings");
        settings.clear();

    } else {

    }
}
