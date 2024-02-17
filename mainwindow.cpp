#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "flowlayout.h"
#include "param_sfo.h"
#include "fps_dialog.h"

#include <iostream>
#include <regex>
#include <sstream>

#include <QFileDialog>
#include <QDebug>
#include <QPushButton>
#include <QDesktopServices>
#include <QProcess>
#include <QMessageBox>
#include <QPixmap>
#include <QXmlStreamReader>
#include <QMenu>
#include <QPainter>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	ui->stackedWidget->addWidget(ui->gamesWindow);
	ui->stackedWidget->addWidget(ui->noGamesWindow);

	SaveSettings();
	
	QSettings settings("rpcsx", "rpcsx_ui_settings");
    settings.beginGroup("rpcsx_ui_settings");
    QString darkMode = settings.value("DarkMode").toString();
    int windowWidth = settings.value("windowWidth").toInt();
    int windowHeight = settings.value("windowHeight").toInt();
    resize(windowWidth, windowHeight);
    settings.endGroup();
    
    if(darkMode == "Dark") {
    	DarkMode();
    }
    
	QObject::connect(ui->modeButton, &QPushButton::clicked, [this]() {
		QString modeName = ui->modeButton->text();
		if(modeName == "Dark") {
		DarkMode();
		} else {
		LightMode();
		}

	});
    
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
	if (hudDisplay.isEmpty()) {
    	
    	settings.setValue("HUDdisplay", "no_display");
    }
    QString iconSize = settings.value("iconSize").toString();
	if (iconSize.isEmpty()) {
    	
    	settings.setValue("iconSize", 250);
    }
    settings.endGroup();
    LoadSettings();
}
   		
void MainWindow::LoadSettings()
{

	QWidget *scrollWidget = new QWidget(ui->scrollWidget);
	FlowLayout *flowLayout = new FlowLayout(scrollWidget);
	
	QSettings settings("rpcsx", "rpcsx_ui_settings");
    settings.beginGroup("rpcsx_ui_settings");
    int iconSize = settings.value("iconSize", 256).toInt();	
    settings.endGroup();
	
    ui->toolBar->addWidget(ui->sizeSlider);
    ui->sizeSlider->setMaximumSize(200, 30);
    ui->sizeSlider->setRange(150, 500); // Set the range of the ui->sizeSlider
    ui->sizeSlider->setValue(iconSize); // Set the initial value of the ui->sizeSlider
    
    QWidget *spacer = new QWidget();
	spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	spacer->setStyleSheet("background-color: transparent;");
	ui->toolBar->addWidget(spacer);
    ui->toolBar->addWidget(ui->modeButton);
    ui->modeButton->setFixedSize(50, 50);
    ui->modeButton->setIconSize(QSize(50, 50));
    ui->modeButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->modeButton->setText("Dark");
    
    QSettings setting("rpcsx", "rpcsx_ui_settings");
    setting.beginGroup("rpcsx_ui_settings");
    QString gamesDirectory = setting.value("GamesDirectory").toString();
    setting.setValue("ChosenGame", "");
    setting.endGroup();
     
	QDir folder(gamesDirectory);
    QStringList directoryList = folder.entryList(QDir::Dirs | QDir::NoDotAndDotDot);


   	if (gamesDirectory.isEmpty()) {
    	ui->stackedWidget->addWidget(ui->noGamesWindow);
		ui->stackedWidget->setCurrentIndex(1);
    } else {  
		ui->gamesWindow->setLayout(flowLayout);
		ui->stackedWidget->setCurrentIndex(0);
    	
		// Add buttons to the flow layout using a for each loop
    	for (qsizetype i = 0; i < directoryList.size(); ++i) {
    		QString directoryName = directoryList.at(i);
        	QPixmap pixmap(gamesDirectory + "/" + directoryName + "/" + "sce_sys/icon0.png");
        	qDebug() << directoryName;
			
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
        	
    		QObject::connect(button, &QWidget::customContextMenuRequested, [gamesDirectory, directoryName, button]() {
                QMenu* contextMenu = new QMenu();
				contextMenu->addAction("View Game Files", [gamesDirectory, directoryName](){
				    QString gameFolderPath = gamesDirectory + "/" + directoryName;
                	QDesktopServices::openUrl(QUrl::fromLocalFile(gameFolderPath));
				});
				contextMenu->addAction("Game Settings");
				contextMenu->exec(button->mapToGlobal(QPoint(0, button->height())));
            });
            QObject::connect(button, &QPushButton::clicked, [this, gamesDirectory, directoryName, button]() {

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
        			button->setStyleSheet("QToolButton { background-color: rgb(255 ,255, 255); }");
                	setWindowTitle(QCoreApplication::translate("MainWindow", "RPCSX"));
    			} else {
        			ui->actionBoot_Game->setEnabled(true);
        			button->setStyleSheet("QToolButton:checked { background-color: rgb(102 ,178, 255); }");
                	QString gameName = button->text();
                	setWindowTitle(QCoreApplication::translate("MainWindow", "RPCSX -- [ %1 ]").arg(gameName));
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

    QString command = QString("rm -f /dev/shm/rpcsx-* && MANGOHUD=1 MANGOHUD_CONFIG=\"fps_limit=%3,%4\" rpcsx-os --mount \"%1\" /system --mount \"%2\" /app0 /app0/eboot.bin").arg(firmwareDirectory, chosenGame, fpsSlider, hudDisplay);
       
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

void MainWindow::LightMode()
{
	qApp->setStyleSheet("");
	ui->modeButton->setText("Dark");

	QList<QAction*> actions = ui->toolBar->actions();
	
	foreach (QAction* action, actions) {
    	QIcon icon = action->icon();
    	QPixmap pixmap = icon.pixmap(QSize(32, 32)); // Adjust the size as needed
    	action->setIcon(QIcon(pixmap));

		if(!pixmap.size().isNull()) { 
		
    		QPainter painter(&pixmap);
    		painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    		painter.fillRect(pixmap.rect(), QColor(0, 0, 0)); // Set the desired color
		
    		action->setIcon(QIcon(pixmap));
    	}
	}
	QSettings setting("rpcsx", "rpcsx_ui_settings");
    setting.beginGroup("rpcsx_ui_settings");
    setting.setValue("DarkMode", "Light");
    setting.endGroup(); 
}

void MainWindow::DarkMode()
{
	ui->modeButton->setText("Light");
	QFile f(":dark/darkstyle.qss");
	
	if (!f.exists())   {
    	printf("Unable to set stylesheet, file not found\n");
	}
	else   {
    	f.open(QFile::ReadOnly | QFile::Text);
    	QTextStream ts(&f);
    	qApp->setStyleSheet(ts.readAll());
	}
	
	QList<QAction*> actions = ui->toolBar->actions();
	
	foreach (QAction* action, actions) {
    	QIcon icon = action->icon();
    	QPixmap pixmap = icon.pixmap(QSize(32, 32)); // Adjust the size as needed
	
		if(!pixmap.size().isNull()) { 
    		QPainter painter(&pixmap);
    		painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    		painter.fillRect(pixmap.rect(), QColor(255, 255, 255)); // Set the desired color
		
    		action->setIcon(QIcon(pixmap));
    	}
	}
	QSettings setting("rpcsx", "rpcsx_ui_settings");
    setting.beginGroup("rpcsx_ui_settings");
    setting.setValue("DarkMode", "Dark");
    setting.endGroup();
}

