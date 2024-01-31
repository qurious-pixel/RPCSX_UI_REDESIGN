#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "flowlayout.h"
#include "param_sfo.h"
#include "fps_dialog.h"

#include <iostream>
#include <regex>
#include <sstream>

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
#include <QPixmap>
#include <QXmlStreamReader>
#include <QDialog>
#include <QListWidget>
#include <QMainWindow>
#include <QToolButton>
#include <QMenu>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	SaveSettings();
    LoadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SaveSettings()
{	
    QSettings settings("rpcsx", "rpcsx_ui_settings");
    settings.beginGroup("rpcsx_ui_settings");
	QString hudDisplay = settings.value("HUDdisplay").toString();
	qDebug() << hudDisplay;
	if (hudDisplay.isEmpty()) {
    	
    	settings.setValue("HUDdisplay", "no_display");
    }
    settings.endGroup();
    qDebug() << hudDisplay;
}
   		
void MainWindow::LoadSettings()
{
///*
    // Clear all frames and their children
    QLayout* layout = ui->scrollWidget->layout();
    if (layout) {
        QLayoutItem *child;
        while ((child = layout->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }
        delete layout;
    }
    //*/
	QWidget *scrollWidget = new QWidget(ui->scrollWidget);
	FlowLayout *flowLayout = new FlowLayout(scrollWidget);
	ui->scrollWidget->setLayout(flowLayout);
    ui->toolBar->addWidget(ui->sizeSlider);
    ui->sizeSlider->setMaximumSize(200, 30);
    ui->sizeSlider->setRange(150, 500); // Set the range of the ui->sizeSlider
    ui->sizeSlider->setValue(250); // Set the initial value of the ui->sizeSlider
    
    QSettings setting("rpcsx", "rpcsx_ui_settings");
    setting.beginGroup("rpcsx_ui_settings");
    QString gamesDirectory = setting.value("GamesDirectory").toString();
    setting.setValue("ChosenGame", "");
    setting.endGroup();
     
	QDir folder(gamesDirectory);
    QStringList directoryList = folder.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

   	if (gamesDirectory.isEmpty()) {


    QHBoxLayout* noGamesLayout = new QHBoxLayout(ui->scrollArea);

        QLabel *bigTextLabel = new QLabel("No Games Found<br>RPCSX could not find any games<br>Set a games directory to proceed.");
        bigTextLabel->setStyleSheet("background-color: #3A3B3C; font-size: 20pt; font-weight: bold; color: black;");
        bigTextLabel->setAlignment(Qt::AlignCenter);
    	noGamesLayout->addWidget(bigTextLabel);

        //scrollWidget->resize(800, 600);
        //QPushButton button("No Games found");
        //QPushButton *button = new QPushButton;
        //button->setText("No Games Found");
        //button->setFlat(true);
        //button->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        //QRect rec = QApplication::desktop()->availableGeometry();
        //button->setFixedSize(rec);
        //flowLayout->addWidget(button);
    	//button.setGeometry(0, 0, scrollWidget->width(), scrollWidget->height());
        /*
        // Create a frame for "No Games Found" message
        //QFrame *noGamesFrame = new QFrame(ui->centralwidget); //ui->centralwidget
        //noGamesFrame->setFixedSize(1000, 500);
        //noGamesFrame->setFrameShape(QFrame::NoFrame);
        //noGamesFrame->setLineWidth(0);
        //noGamesFrame->setObjectName("noGamesFrame");
        //noGamesFrame->setStyleSheet("background-color: #3A3B3C;");

        QVBoxLayout *noGamesLayout = new QVBoxLayout(); //noGamesFrame
        noGamesLayout->setAlignment(Qt::AlignCenter); //

        QLabel *bigTextLabel = new QLabel("No Games Found<br>RPCSX could not find any games<br>Set a games directory to proceed.", noGamesLayout);
        bigTextLabel->setStyleSheet("font-size: 20pt; font-weight: bold; color: black;");
        bigTextLabel->setAlignment(Qt::AlignCenter);

        noGamesLayout->addWidget(bigTextLabel);
        //flowLayout->addWidget(noGamesFrame);
        //noGamesFrame->setLayout(noGamesLayout);
        //setCentralWidget(noGamesFrame);
    	*/
    } else {  
		// Add buttons to the flow layout using a for each loop
    	for (qsizetype i = 0; i < directoryList.size(); ++i) {
    		QString directoryName = directoryList.at(i);
        	QPixmap pixmap(gamesDirectory + "/" + directoryName + "/" + "sce_sys/icon0.png");
        	qDebug() << directoryName;
        
        	QSettings settings("rpcsx", "rpcsx_ui_settings");
    		settings.beginGroup("rpcsx_ui_settings");
        	int iconSize = settings.value("iconSize", 256).toInt();	
        	settings.endGroup();
			
			QToolButton *button = new QToolButton;
			QString gamePath = QString (gamesDirectory + "/" + directoryName + "/");
			SetGameTitle(gamePath, button);
			QFont font = button->font();
    		font.setPointSize(12);
        	QObject::connect(ui->sizeSlider, &QSlider::valueChanged, [this, pixmap, button]() {
        		int iconSize = ui->sizeSlider->value();
        		QPixmap imagePixmap = pixmap.scaled(QSize(iconSize * .8, iconSize * .8), Qt::KeepAspectRatio);
				QIcon ButtonIcon(imagePixmap);
				button->setIconSize(QSize(iconSize * .8, iconSize * .8));
				button->setFixedSize(iconSize, iconSize);
				button->setIcon(ButtonIcon);
        		QSettings settings("rpcsx", "rpcsx_ui_settings");
    			settings.beginGroup("rpcsx_ui_settings");
				settings.setValue("iconSize", iconSize);
				settings.endGroup();
        	});	
        	
        	QPixmap imagePixmap = pixmap.scaled(QSize(iconSize * .8, iconSize * .8), Qt::KeepAspectRatio);
			QIcon ButtonIcon(imagePixmap);
			button->setIconSize(QSize(iconSize * .8, iconSize * .8));
			button->setFixedSize(iconSize, iconSize);
			button->setIcon(ButtonIcon);
        	button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        	button->setFont(font);
        	button->setCheckable(true);
        	button->setAutoExclusive(true);
        	button->setContextMenuPolicy(Qt::CustomContextMenu);
        	flowLayout->addWidget(button);
        	
    		QObject::connect(button, &QWidget::customContextMenuRequested, [gamesDirectory, directoryName]() {
                QMenu* contextMenu = new QMenu();
				contextMenu->addAction("View Game Files", [gamesDirectory, directoryName](){
				    QString gameFolderPath = gamesDirectory + "/" + directoryName;
                	QDesktopServices::openUrl(QUrl::fromLocalFile(gameFolderPath));
				});
				contextMenu->addAction("Game Settings");
				contextMenu->exec(QCursor::pos());
            });
            QObject::connect(button, &QPushButton::clicked, [this, gamesDirectory, directoryName]() {
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
            });            
    	}   	
	}
    int totalGames = directoryList.count();
    QString actionText = QString("RPCSX - %1 Games - Firmware 9.00").arg(totalGames);
    ui->actionAV_GC_FV->setText(actionText);	
}

void MainWindow::on_actionAdd_Games_triggered()
{
    QString gamesDirectory = QFileDialog::getExistingDirectory(this, "Select Games Folder");
    QSettings setting("rpcsx", "rpcsx_ui_settings");
    setting.beginGroup("rpcsx_ui_settings");
    setting.setValue("GamesDirectory", gamesDirectory);
    setting.endGroup();
///*
    QLayout* layout = ui->scrollWidget->layout();
    if (layout) {
        QLayoutItem *child;
        while ((child = layout->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }
        delete layout;

    }
//*/		    
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
    QString fpsSlider = settings.value("FPSSlider").toString();
    QString hudDisplay = settings.value("HUDdisplay").toString();
    

    QString command = QString("rm -f /dev/shm/rpcsx-* && MANGOHUD=1 MANGOHUD_CONFIG=fps_limit=%3,%4 rpcsx-os --mount \"%1\" /system --mount \"%2\" /app0 /app0/eboot.bin").arg(firmwareDirectory, chosenGame, fpsSlider, hudDisplay);
    
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

void MainWindow::on_actionReset_Settings_triggered()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Reset Settings", "All settings will be wiped. Are you sure?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QLayoutItem *child;
        while ((child = ui->scrollWidget->layout()->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }
        QSettings settings("rpcsx", "rpcsx_ui_settings");
        settings.clear();
        
    } else {

    }
    SaveSettings();
}

void MainWindow::on_actionSet_FPS_triggered()
{
    fps_dialog* fpsDialog = new fps_dialog(this);
    fpsDialog->show();
}

void MainWindow::SetGameTitle(QString &gamePath, QToolButton* button)
{
    if (QFile::exists(gamePath + "sce_sys/param.sfo")) {
		QFile file(gamePath + "sce_sys/param.sfo");
		if (!file.open(QIODevice::ReadOnly)) {
				qWarning() << "Failed to open file for reading";
		}
		QString qstr = file.fileName();
		std::string sfo_path = qstr.toStdString();
		param_sfo::param_sfo_file sfo(sfo_path);
  				
		std::ostringstream oss;
		std::streambuf* coutBuffer = std::cout.rdbuf(); // Save cout buffer
		std::cout.rdbuf(oss.rdbuf()); // Redirect cout to oss
		sfo.print(std::cout);
		std::cout.rdbuf(coutBuffer); // Restore cout buffer
		std::string input = oss.str(); // Get the string from oss
		
		std::smatch match;
		std::regex pattern("TITLE\n(.*)");  // Match pattern
		std::regex_search(input, match, pattern);
		std::string final (match[1]);
		
		std::string gameTitleMatch = final;
			if (gameTitleMatch.empty()) {
				std::string final = "unknown";
			}
		QString gameTitle = QString::fromStdString(final);
		button->setText(gameTitle);
    } else {
		QFile file(gamePath + "sce_sys/pronunciation.xml");
		if (!file.open(QIODevice::ReadOnly)) {
			qWarning() << "Failed to open file for reading";
		}
			
		QXmlStreamReader reader(&file);
		QString gameTitle;
		while (!reader.atEnd()) {
		reader.readNext();
		if (reader.isStartElement()) {
			if (reader.name() == QLatin1String("text"))
				gameTitle = reader.readElementText();
			}
		} 	
    	button->setText(gameTitle);
    }   
}
