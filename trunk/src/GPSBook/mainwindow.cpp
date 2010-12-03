/****************************************************************************
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor,
** Boston, MA  02110-1301, USA.
**
** ---
** Copyright (C) 2009, gpsbook-team
**
****************************************************************************/
#include <QPluginLoader>
#include <QMessageBox>
#include <QDir>
#include <QString>
#include <QMdiSubWindow>
#include <QDebug>
#include <QLabel>
#include <QtGui>
#include <QTextCharFormat>
#include <QTextBrowser>
#include <QModelIndex>
#include <QListWidgetItem>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogfirststart.h"
#include "dialogtrackproperty.h"
#include "interface.h"
#include "database.h"
#include "gpsdata.h"
#include "gpsbookapplication.h"

namespace GPSBook {

    /*------------------------------------------------------------------------------*
      Capture LanguageChange and retranslate ui
     *------------------------------------------------------------------------------*/
    void MainWindow::changeEvent(QEvent *e)
    {
        switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
        }
    } //MainWindow::changeEvent

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::setSplashScreen(QSplashScreen* splash)
    {
        this->splash = splash;
    } //MainWindow::setSplashScreen

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindowClass)
    {
        ui->setupUi(this);

        //Set main window title including version number
        setWindowTitle(qApp->applicationName() + " v" + qApp->applicationVersion());

        mGPSData = new GPSData();

        seletectItem = NULL;

        settings = new QSettings("GPSBook","GPSBook");



    } //MainWindow::MainWindow

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::init()
    {
        qDebug( )  << __FILE__ << __FUNCTION__;
        //------------------- Window -------------------
        //Maximise window
        setWindowState(Qt::WindowMaximized);

        //Connect customs GPSDataUpdated signals and UpdateSaveButton slot
        connect(mGPSData, SIGNAL(signalGPSDataGPXUpdated()),this,SLOT(updateSaveButton()));
        connect(mGPSData, SIGNAL(signalGPSDataTracksUpdated()),this,SLOT(updateSaveButton()));
        connect(mGPSData, SIGNAL(signalGPSDataRoutesUpdated()),this,SLOT(updateSaveButton()));
        connect(mGPSData, SIGNAL(signalGPSDataWaypointsUpdated()),this,SLOT(updateSaveButton()));
        connect((QObject*)mGPSData, SIGNAL(signalGPSDataGPXUpdated()),this,SLOT(initCurrentGPXTreeview()));

        //------------------- Toolbar -------------------
        //Prepare toolbar for filters plugin buttons
        filtersMenu = new QMenu();
        QToolButton* filterToolButton = new QToolButton( ui->mainToolBar );
        filterToolButton->setMenu( filtersMenu );
        filterToolButton->setPopupMode( QToolButton::MenuButtonPopup );
        filterToolButton->setIcon(QIcon(":/icons/gtk/gnome-run.png"));
        ui->mainToolBar->insertSeparator(ui->actionFilters);
        ui->mainToolBar->insertWidget(ui->actionFilters,filterToolButton);
        ui->mainToolBar->insertSeparator(ui->actionFilters);

        //Prepare toolbar for about buttons
        aboutMenu = new QMenu();
        QToolButton* aboutButton = new QToolButton( ui->mainToolBar );
        aboutButton->setMenu( aboutMenu );
        aboutButton->setPopupMode( QToolButton::MenuButtonPopup );
        aboutButton->setIcon(QIcon(":/icons/gtk/gnome-about.png"));
        ui->mainToolBar->addWidget(aboutButton);
        aboutMenu->addAction(ui->actionAbout);
        aboutMenu->addAction(ui->actionAboutQt);
        aboutMenu->addSeparator();

        //Prepare toolbar for visual plugin buttons
        actionGroup = new QActionGroup(this);
        actionGroup->setExclusive(true);
        actionGroup->addAction(ui->actionHelp);
        ui->actionHelp->setData(0);
        connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(displayPluginShow()));
        actionGroup->addAction(ui->actionPreferences);
        ui->actionPreferences->setData(1);
        connect(ui->actionPreferences, SIGNAL(triggered()), this, SLOT(displayPluginShow()));
        actionGroup->addAction(ui->actionFilters);
        ui->actionFilters->setData(2);
        connect(ui->actionFilters, SIGNAL(triggered()), this, SLOT(displayPluginShow()));


        //------------------- StackWidget -------------------
        //Replace {{VERSION}} keyword
        ui->textEdit->setHtml(ui->textEdit->toHtml().replace("{{VERSION}}",qApp->applicationVersion()));

        //Add/move components to the top right toolbar
        progressIndicator = new QProgressIndicator();
        ui->toolBar->addWidget(progressIndicator);
        
        //------------------- Proxy -------------------
        (void) new WebProxy();

        //------------------- Plugins -------------------
        loadPlugins();

        //------------------- Catalog -------------------
        //Init calendar and  treeview
        Database::initCalendarWidget(ui->calendarWidget);
        Database::updateListWidget(ui->listWidgetTracksOfTheDay, QDate::currentDate());
        Database::updateListWidget(ui->listWidgetNoDateTracks, QDate(0,0,0));
        updateNavigationButtons();

        //------------------- Preferences -------------------
        qDebug() << __FILE__ << __FUNCTION__ << "Preferences";
        ui->comboBoxLanguageSelection->blockSignals(true);
        //Manage Language Combobox
        ui->comboBoxLanguageSelection->addItem(tr("Default language (System)"),"");
        ui->comboBoxLanguageSelection->insertSeparator(1);
        ui->comboBoxLanguageSelection->setInsertPolicy(QComboBox::InsertAlphabetically);
        foreach (QString avail, GPSBookApplication::availableLanguages())
        {
            // Figure out nice names for locales
            QLocale locale(avail);
            QString language = QLocale::languageToString(locale.language());
            QString country = QLocale::countryToString(locale.country());
            QString name = language + " (" + country + ")";

            ui->comboBoxLanguageSelection->addItem(name,avail);
        }
        ui->comboBoxLanguageSelection->setCurrentIndex(ui->comboBoxLanguageSelection->findData(settings->value("Translation","").toString()));

        //Manage Storage Location
        qDebug() << __FILE__ << __FUNCTION__ << "Storage";
        QString tmpstr = settings->value("StorageLocation","").toString();
        if (tmpstr == "")
        {
            tmpstr = QDir::homePath()+"/gpsbook/";
        }
        ui->lineEditStorageLocation->setText(tmpstr);
        ui->comboBoxLanguageSelection->blockSignals(false);

        //------------------- Mainwindow -------------------
        // Display the favorite plugin
        qDebug() << __FILE__ << __FUNCTION__ << "Display favorite plugin";
        int preferedPluginId = settings->value("PreferedPluginId",0).toInt();
        ui->stackedWidget->setCurrentIndex(preferedPluginId);
        ui->comboBoxPreferedPlugin->setCurrentIndex(preferedPluginId);
        actionGroup->actions().at(preferedPluginId)->setChecked(true);
        ui->dockWidgetTraceManagement->setVisible(actionGroup->actions().at(preferedPluginId)->data().toInt() > 1);
        if ( (( preferedPluginId-3 ) >0) && ( (preferedPluginId-3) < displayPluginList.count()) )
        {
            visiblePlugin=displayPluginList[preferedPluginId-3];
        }
        connect(visiblePlugin, SIGNAL(signalSetTrackSelection(bool,bool,bool,bool,bool)),this,SLOT(trackSelectionEnable(bool,bool,bool,bool,bool)));
        connect(visiblePlugin, SIGNAL(signalLoadFile(QString,bool)),this,SLOT(loadFile(QString,bool)));
        connect(this, SIGNAL(signalFileLoaded()),visiblePlugin,SLOT(on_fileLoaded()));
        connect(this, SIGNAL(signalSelectionChanged()),visiblePlugin,SLOT(on_selectionChanged()));
        connect(mGPSData, SIGNAL(signalGPSDataGPXUpdated()),visiblePlugin,SLOT(on_gpsdataGPXChanged()));
        connect(mGPSData, SIGNAL(signalGPSDataTracksUpdated()),visiblePlugin,SLOT(on_gpsdataTracksChanged()));
        connect(mGPSData, SIGNAL(signalGPSDataRoutesUpdated()),visiblePlugin,SLOT(on_gpsdataRoutesChanged()));
        connect(mGPSData, SIGNAL(signalGPSDataWaypointsUpdated()),visiblePlugin,SLOT(on_gpsdataWaypointsChanged()));

        qDebug() << __FILE__ << __FUNCTION__ << "Display favorite plugin 0";
        visiblePlugin->on_showPlugin();
        qDebug() << __FILE__ << __FUNCTION__ << "Display favorite plugin 1";

        //Start full screen
        qDebug() << __FILE__ << __FUNCTION__ << "Fullscreen";
        bool startFullScreen = settings->value("StartFullScreen",false).toBool();
        ui->checkBoxStartFullScreen->setChecked(startFullScreen);
        if (startFullScreen) {
            setWindowState(Qt::WindowFullScreen);
        }
        //Manage menu and toolbar visibility
        bool menuVisibility = settings->value("MenuVisibility",false).toBool();
#if defined(Q_OS_MAC)
        menuVisibility = true;
        ui->checkBoxMenuVisibility->setEnabled(false);
#endif
        ui->checkBoxMenuVisibility->setChecked(menuVisibility);
        ui->menuBar->setVisible(menuVisibility);

        bool toolbarVisibility = settings->value("ToolbarVisibility",true).toBool();
        ui->checkBoxToolbarVisibility->setChecked(toolbarVisibility);
        ui->mainToolBar->setVisible(toolbarVisibility);
        ui->toolBar->setVisible(toolbarVisibility);

        ui->tabWidgetLeftPanel->setCurrentIndex(0);

        qDebug() << __FILE__ << __FUNCTION__ << "END";

    } //MainWindow::init

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::loadPlugins()
    {
        qDebug( )  << __FILE__ << __FUNCTION__;
        pluginsDir = QDir(qApp->applicationDirPath());

    #if defined(Q_OS_MAC)
        if (pluginsDir.dirName() == "MacOS") {
            pluginsDir.cdUp();
            pluginsDir.cdUp();
            pluginsDir.cdUp();
        }
    #endif
        pluginsDir.cd("../plugins/");
        QStringList filters;
    #if defined(Q_OS_LINUX)
        filters << "*.so";
    #elif defined(Q_OS_WIN)
        filters << "*.dll";
    #elif defined(Q_OS_MAC)
        filters << "*.dylib";
    #endif
        pluginsDir.setNameFilters(filters);
        //qDebug( )  << __FILE__ << __FUNCTION__ << "Loading plugin from " << pluginsDir.absolutePath() << "/" << filters;

        //int visualPluginIndex = 0;
        //int filterPluginIndex = 0;
        //int ioPluginIndex = 0;

        bool firstDisplayPlugin = true;
        foreach (QString fileName, pluginsDir.entryList(QDir::Files))
        {
            qDebug( )  << __FILE__ << __FUNCTION__ << "Try to load " << fileName;
            QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
            QObject *plugin = loader.instance();
            if (plugin)
            {
                PluginInterface* m_plugin = (PluginInterface*)plugin;
                splash->showMessage(tr("Loading plugin") + " " + m_plugin->getName());
                qApp->processEvents();
                if (pluginFileNames.indexOf(m_plugin->getName()) == -1)
                {
                    //Add About Plugin menu
                    QAction *aboutAction = new QAction(m_plugin->getName(), plugin);
                    aboutAction->setIcon(m_plugin->getIcon());
                    connect(aboutAction, SIGNAL(triggered()), m_plugin, SLOT(on_about()));
                    ui->menuAboutPlugins->addAction(aboutAction);
                    aboutMenu->addAction(aboutAction);

                    //Load Input/Output Plugin
                    InputOutputPluginInterface *inputOutputPlugin = qobject_cast<InputOutputPluginInterface *>(plugin);
                    if (inputOutputPlugin)
                    {
                        m_plugin->init(NULL , mGPSData);
                        if (inputOutputPlugin->getOpenFilter().contains("*.gpx"))
                        {
                            qDebug( )  << __FILE__ << __FUNCTION__ << "Found GPX plugin";
                            gpxPlugin=inputOutputPlugin;
                        }

                        if (openFilters != "")
                            openFilters += ";;";
                        openFilters += inputOutputPlugin->getOpenFilter();

                        if (saveFilters != "")
                            saveFilters += ";;";
                        saveFilters += inputOutputPlugin->getSaveFilter();

                        inputOutputPluginList.append(inputOutputPlugin);
                        int idx = ui->stackedWidgetHelp->addWidget(inputOutputPlugin->getHelp());
                        QTreeWidgetItem* optionHelp = new QTreeWidgetItem();
                        optionHelp->setText(0,inputOutputPlugin->getName());
                        optionHelp->setIcon(0,inputOutputPlugin->getIcon());
                        optionHelp->setData(0,Qt::UserRole+1,idx);
                        ui->treeWidgetHelp->itemAt(0,60)->addChild(optionHelp);

                        idx = ui->stackedWidgetOptions->addWidget(inputOutputPlugin->getOptions());
                        QTreeWidgetItem* optionItem = new QTreeWidgetItem();
                        optionItem->setText(0,inputOutputPlugin->getName());
                        optionItem->setIcon(0,inputOutputPlugin->getIcon());
                        optionItem->setData(0,Qt::UserRole+1,idx);
                        ui->treeWidgetOptions->itemAt(0,60)->addChild(optionItem);
                     }

                    //Load Display Plugin
                    DisplayPluginInterface *displayPlugin = qobject_cast<DisplayPluginInterface*>(plugin);
                    if (displayPlugin)
                    {
                        //qDebug( )  << __FILE__ << __FUNCTION__ << "This is a vusual plugin " << fileName;
                        QWidget* widget = displayPlugin->getWidget();
                        if ( widget != NULL )
                        {
                            //Create Action
                            QAction *action = new QAction(displayPlugin->getName(), plugin);

                            //Add icon and tooltip to action
                            action->setIcon(displayPlugin->getIcon());
                            action->setToolTip(displayPlugin->getName());
                            action->setCheckable(true);
                            actionGroup->addAction(action);

                            //Connect action signal to slot
                            connect(action, SIGNAL(triggered()), this, SLOT(displayPluginShow()));
                            connect(action, SIGNAL(triggered()), displayPlugin, SLOT(on_showPlugin()));

                            //Add action to toolbar
                            ui->mainToolBar->insertAction(ui->actionFilters,action);
                            ui->menu_View->insertAction(ui->actionFilters,action);

                            //Add action to stack widget
                            int idx = ui->stackedWidget->addWidget(widget);
                            qDebug( )  << __FILE__ << __FUNCTION__ << "Init Plugin " << fileName;
                            m_plugin->init(ui->stackedWidget->widget(idx) , mGPSData);
                            action->setData(idx);

                            //define first plugin a visible plugin (could be overwrited later by the preference selection)
                            if (firstDisplayPlugin)
                            {
                                visiblePlugin = displayPlugin;
                                firstDisplayPlugin = false;
                            }
                            displayPluginList.append(displayPlugin);
                        }

                        //Add help page of plugin
                        //ui->tabWidgetVisualPluginHelp->addTab(displayPlugin->getHelp(),displayPlugin->getIcon(),displayPlugin->getName());
                        int idx = ui->stackedWidgetHelp->addWidget(displayPlugin->getHelp());
                        QTreeWidgetItem* optionHelp = new QTreeWidgetItem();
                        optionHelp->setText(0,displayPlugin->getName());
                        optionHelp->setIcon(0,displayPlugin->getIcon());
                        optionHelp->setData(0,Qt::UserRole+1,idx);
                        ui->treeWidgetHelp->itemAt(0,24)->addChild(optionHelp);

                        //Add help Options Pages of plugin
                        //ui->tabWidgetVisualPluginOptions->addTab(displayPlugin->getOptions(),displayPlugin->getIcon(),displayPlugin->getName());
                        idx = ui->stackedWidgetOptions->addWidget(displayPlugin->getOptions());
                        QTreeWidgetItem* optionItem = new QTreeWidgetItem();
                        optionItem->setText(0,displayPlugin->getName());
                        optionItem->setIcon(0,displayPlugin->getIcon());
                        optionItem->setData(0,Qt::UserRole+1,idx);
                        ui->treeWidgetOptions->itemAt(0,24)->addChild(optionItem);

                        //Add plugins info to preferences widgets
                        ui->comboBoxPreferedPlugin->addItem(tr("Plugin")+" "+displayPlugin->getName());
                        QListWidgetItem* listItem = new QListWidgetItem();
                        listItem->setText(displayPlugin->getName());
                        listItem->setIcon(displayPlugin->getIcon());
                        listItem->setCheckState(Qt::Checked);
                        ui->listWidgetProfilePlugins->addItem(listItem);

                    }

                    //Load Filter Plugin
                    FilterPluginInterface *filterPlugin = qobject_cast<FilterPluginInterface*>(plugin);
                    if (filterPlugin)
                    {
                        QAction *action = new QAction(filterPlugin->getName(), filterPlugin);
                        action->setIcon(filterPlugin->getIcon());
                        connect(action, SIGNAL(triggered()), filterPlugin, SLOT(on_showPlugin()));
                        ui->menuFilters->addAction(action);
                        filtersMenu->addAction(action);
                        m_plugin->init(NULL , mGPSData);

                        //ui->tabWidgetFilterPluginHelp->addTab(filterPlugin->getHelp(),filterPlugin->getIcon(),filterPlugin->getName());
                        int idx = ui->stackedWidgetHelp->addWidget(filterPlugin->getHelp());
                        QTreeWidgetItem* optionHelp = new QTreeWidgetItem();
                        optionHelp->setText(0,filterPlugin->getName());
                        optionHelp->setIcon(0,filterPlugin->getIcon());
                        optionHelp->setData(0,Qt::UserRole+1,idx);
                        ui->treeWidgetHelp->itemAt(0,40)->addChild(optionHelp);

                        //Append options page
                        //ui->tabWidgetFilterPluginOptions->addTab(filterPlugin->getOptions(),filterPlugin->getIcon(),filterPlugin->getName());
                        idx = ui->stackedWidgetOptions->addWidget(filterPlugin->getOptions());
                        QTreeWidgetItem* optionItem = new QTreeWidgetItem();
                        optionItem->setText(0,filterPlugin->getName());
                        optionItem->setIcon(0,filterPlugin->getIcon());
                        optionItem->setData(0,Qt::UserRole+1,idx);
                        ui->treeWidgetOptions->itemAt(0,40)->addChild(optionItem);
                    }
                    pluginFileNames += m_plugin->getName();
                    qDebug( )  << __FILE__ << __FUNCTION__ << "Loaded";
                }
                else
                {
                   //qDebug( )  << __FILE__ << __FUNCTION__ << "Already loaded: " << m_plugin->getName();
                }
            }
            else
            {
                qCritical() << __FILE__ << __FUNCTION__ << "Cannot load " << fileName;
                qCritical() << __FILE__ << __FUNCTION__ << "ERROR: " << loader.errorString();
            }
        }

        qDebug( )  << __FILE__ << __FUNCTION__ << "Preparing open dialog box";
        openDialog = new QFileDialog(this, tr("Open GPS trace"), QString(), openFilters);
        openDialog->restoreState(settings->value("openstate","").toByteArray());
        openDialog->selectNameFilter(settings->value("openstatefilter",gpxPlugin->getOpenFilter()).toString());

        ui->treeWidgetHelp->expandAll();
        ui->treeWidgetOptions->expandAll();
        qDebug( )  << __FILE__ << __FUNCTION__ << "Plugins loaded";

    } //MainWindow::loadPlugins

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    MainWindow::~MainWindow()
    {
        delete ui;
    } //MainWindow::~MainWindow

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_actionAbout_triggered()
    {
            QMessageBox::about(this, tr("About") + " " + qApp->applicationName(),
                    "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                    "<html>"
                        "<head>"
                            "<meta name=\"qrichtext\" content=\"1\" />"
                        "</head>"
                        "<body>"
                        "<p style=\" font-family:'Sans'; font-size:14pt; font-weight:400; font-style:vold; \">"
                            + qApp->applicationName() + " v" + qApp->applicationVersion() +
                            + "<sub>  (" + QString(BUILD_DATE) + ")<sub>"
                        "</p>"
                        "<p style=\" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:vold; \">"
                            + qApp->applicationName() + tr(" is an application dedicated to managed and view GPX tracklist captured with GPS devices.\n The feature of the application are provided by plugins designed to import/export, display and filter GPS data.") + "<br><br>" +
                            tr("License:") + "<br>&nbsp;&nbsp;&nbsp;&nbsp;" +
                            "GNU GPL <a href=\"http://www.gnu.org/licenses/gpl.txt\">http://www.gnu.org/licenses/gpl.txt</a>" + "<br><br>" +
                            tr("Authors:") + "<br>&nbsp;&nbsp;&nbsp;&nbsp;" +
                            "gpsbook-team &copy;2009"+ "<br><br>" +
                            tr("Translation:") + "<br>&nbsp;&nbsp;&nbsp;&nbsp;" +
                            tr("English by gpsbook-team") +
                        "</p>"
                        "</body>"
                    "</html>"

                    );
    } //MainWindow::on_actionAbout_triggered

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_actionOpen_triggered()
    {
        if (openDialog->exec() == QDialog::Accepted)
        {
            //If one file is opened, we load the file else, we add files to the catalog
            if (openDialog->selectedFiles().count() == 1) {
                loadFile(QString(openDialog->selectedFiles()[0]),false);
            }
            else
            {
                if (openDialog->selectedFilter() == gpxPlugin->getOpenFilter())
                {
                    mGPSData->clearData();
                    foreach (QString filename, openDialog->selectedFiles()) {
                        //Copy file into the storage directory
                        QString storagePath = settings->value("StorageLocation","").toString();
                        QFileInfo info(filename);
                        QFile::copy(filename, storagePath + "/data/"+info.fileName());
                        //Add the file into the database
                        Database::addFileInDatabase(storagePath +"/data/"+info.fileName());
                        //Update gpx list
                        Database::updateListWidget(ui->listWidgetNoDateTracks, QDate(0,0,0));

                    }
                }
                else
                {
                    QMessageBox::warning(NULL,qApp->applicationName(),tr("Multiple file loading is only allowed for GPX files."),QMessageBox::Ok);
                }
            }
        }
        openDialog->setFileMode(QFileDialog::ExistingFile);

    } //MainWindow::on_actionOpen_triggered

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::loadFile(QString filename, bool isFromCatalog) {
        qDebug( )  << __FILE__ << __FUNCTION__ <<  filename;
        //Block signals of GPSData during loading
        mGPSData->blockSignals(true);

        //Start progress indicators
        progressIndicator->startAnimation();
        qApp->processEvents();
        qApp->setOverrideCursor(QCursor(Qt::WaitCursor));



        //Load the new trace (gpsDataLoc managed by the plugin)
        foreach (InputOutputPluginInterface* inputPlugin, inputOutputPluginList) {
            if (inputPlugin->getOpenFilter().contains(QFileInfo(filename).suffix())) {
                int response = QMessageBox::Yes;
                if (mGPSData->isModified)
                {
                    response = QMessageBox::warning(this,
                                                    tr("Loading new data"),
                                                    tr("Modification done into current file will be lost. Continue anyway?"),
                                                    QMessageBox::Yes|QMessageBox::No);
                }

                if ( response == QMessageBox::Yes )
                {
                    on_actionClose_current_trace_triggered();
                    inputPlugin->open(filename, mGPSData);
                }
            }
        }
        //Update mainwindows state
        ui->actionClose_current_trace->setEnabled(true);
        ui->actionReload->setEnabled(true);
        ui->toolButtonInCatalog->setChecked(isFromCatalog);
        ui->toolButtonAddToCatalog->setChecked(isFromCatalog);
        ui->toolButtonAddToCatalog->setDisabled(isFromCatalog);
        ui->labelTrackDate->setText( mGPSData->metadata->time.toString("dd/MM/yyyy") );
        ui->labelTrackName->setText( QFileInfo(mGPSData->filename).fileName());
        initCurrentGPXTreeview();
        ui->tabWidgetLeftPanel->setCurrentIndex(1);
        mGPSData->setFromCatalog(isFromCatalog);

        //Unbloc signals
        mGPSData->blockSignals(false);

        //Emit file loaded signal
        emit signalFileLoaded();

        //Stop progress indicator
        qApp->setOverrideCursor(QCursor(Qt::ArrowCursor));
        progressIndicator->stopAnimation();
        updateSaveButton();
    } //MainWindow::loadFile

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::initCurrentGPXTreeview(){
        qDebug( )  << __FILE__ << __FUNCTION__ ;
        ui->labelTrackName->setText(QFileInfo(mGPSData->filename).fileName());
        ui->treeWidgetCurrentGPX->clear();

        QTreeWidgetItem* gpxItem = new QTreeWidgetItem();
        gpxItem->setText(0,"GPX");
        gpxItem->setIcon(0,QIcon(":/icons/silk/script.png"));
        gpxItem->setData(0,Qt::UserRole + 1,GPX);  
        ui->treeWidgetCurrentGPX->addTopLevelItem(gpxItem);
        ui->treeWidgetCurrentGPX->expandItem(gpxItem);

        QTreeWidgetItem* waypointListItem = new QTreeWidgetItem();
        waypointListItem->setText(0,"Waypoint (" + QVariant(mGPSData->wayPointList.count()).toString() + ")");
        waypointListItem->setIcon(0,QIcon(":/icons/silk/script_edit.png"));
        waypointListItem->setData(0,Qt::UserRole + 1,WAYPOINTLIST);
        gpxItem->addChild(waypointListItem);
        if (mGPSData->displayedWaypointIndex == -1)
        {
            ui->treeWidgetCurrentGPX->setCurrentItem(waypointListItem);
        }

        int waypointIndex = 0;
        foreach (WayPoint* waypoint, mGPSData->wayPointList) {
            QTreeWidgetItem* waypointItem = new QTreeWidgetItem();
            waypointItem->setText(0,(waypoint->name != "") ? waypoint->name : tr("No name"));
            waypointItem->setIcon(0,QIcon(":/icons/silk/script_code.png"));
            waypointItem->setData(0,Qt::UserRole + 1,WAYPOINT);
            waypointItem->setData(0,Qt::UserRole + 2,waypointIndex);
            waypointListItem->addChild(waypointItem);
            if (mGPSData->displayedWaypointIndex == waypointIndex)
            {
                ui->treeWidgetCurrentGPX->expandItem(waypointItem);
                ui->treeWidgetCurrentGPX->setCurrentItem(waypointItem);
            }
            waypointIndex++;
        }

        QTreeWidgetItem* routeListItem = new QTreeWidgetItem();
        routeListItem->setText(0,"Route");
        routeListItem->setText(0,"Route (" + QVariant(mGPSData->routeList.count()).toString() + ")");
        routeListItem->setIcon(0,QIcon(":/icons/silk/script_code_red.png"));
        routeListItem->setData(0,Qt::UserRole + 1,ROUTELIST);
        gpxItem->addChild(routeListItem);
        if (mGPSData->displayedRouteIndex == -1)
        {
            ui->treeWidgetCurrentGPX->setCurrentItem(routeListItem);
        }

        int routeIndex = 0;
        foreach (Route* route, mGPSData->routeList) {
            QTreeWidgetItem* routeItem = new QTreeWidgetItem();
            routeItem->setText(0,(route->name != "") ? route->name : tr("No name"));
            routeItem->setIcon(0,QIcon(":/icons/silk/script_code_red.png"));
            routeItem->setData(0,Qt::UserRole + 1,ROUTE);
            routeItem->setData(0,Qt::UserRole + 2,routeIndex);
            routeListItem->addChild(routeItem);
            if (mGPSData->displayedRouteIndex == routeIndex)
            {
                ui->treeWidgetCurrentGPX->expandItem(routeItem);
                ui->treeWidgetCurrentGPX->setCurrentItem(routeItem);
            }
            routeIndex++;
        }

        QTreeWidgetItem* trackListItem = new QTreeWidgetItem();
        trackListItem->setText(0,"Track (" + QVariant(mGPSData->trackList.count()).toString() + ")");
        trackListItem->setIcon(0,QIcon(":/icons/silk/script_code.png"));
        trackListItem->setData(0,Qt::UserRole + 1,TRACKLIST);
        gpxItem->addChild(trackListItem);
        if (mGPSData->displayedTrackIndex == -1)
        {
            ui->treeWidgetCurrentGPX->setCurrentItem(trackListItem);
        }


        int trackIndex =0;
        foreach (Track* track, mGPSData->trackList) {
            QTreeWidgetItem* trackItem = new QTreeWidgetItem();
            trackItem->setText(0,(track->name != "") ? track->name : tr("No name"));
            trackItem->setIcon(0,QIcon(":/icons/silk/script_code.png"));
            trackItem->setData(0,Qt::UserRole + 1,TRACK);
            trackItem->setData(0,Qt::UserRole + 2,trackIndex);
            trackListItem->addChild(trackItem);
            if (mGPSData->displayedTrackIndex == trackIndex)
            {
                ui->treeWidgetCurrentGPX->expandItem(trackItem);
                ui->treeWidgetCurrentGPX->setCurrentItem(trackItem);
            }

            int segmentIndex = 0;
            foreach (TrackSeg* trackseg, track->trackSegList)
            {
                QTreeWidgetItem* trackSegItem = new QTreeWidgetItem();
                trackSegItem->setText(0,tr("Segment") + " " + QVariant(segmentIndex).toString() );
                trackSegItem->setIcon(0,QIcon(":/icons/silk/script_code.png"));
                trackSegItem->setData(0,Qt::UserRole + 1,TRACKSEG);
                trackSegItem->setData(0,Qt::UserRole + 2,segmentIndex);
                trackItem->addChild(trackSegItem);
                if ((mGPSData->displayedTrackIndex == trackIndex) &&
                    (mGPSData->displayedSegmentIndex == segmentIndex))
                {
                    ui->treeWidgetCurrentGPX->expandItem(trackListItem);
                    ui->treeWidgetCurrentGPX->setCurrentItem(trackSegItem);
                }
                segmentIndex++;
            }
            trackIndex++;
        }

        if (ui->treeWidgetCurrentGPX->currentItem() == NULL)
             ui->treeWidgetCurrentGPX->setCurrentItem(gpxItem);

        ui->treeWidgetCurrentGPX->setEnabled(true);

    } //MainWindow::initCurrentGPXTreeview


    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_treeWidgetCurrentGPX_itemSelectionChanged()
    {
        qDebug( )  << __FILE__ << __FUNCTION__ ;
        QTreeWidgetItem* item = ui->treeWidgetCurrentGPX->currentItem();
        if (item == NULL)
            return;

        if (!item->isDisabled())
        {
            qDebug() << __FILE__ << __FUNCTION__;
            mGPSData->displayedWaypointIndex = -2;
            mGPSData->displayedRouteIndex    = -2;
            mGPSData->displayedTrackIndex    = -2;
            mGPSData->displayedSegmentIndex  = -2;
            mGPSData->selectedWaypointIndex = -1;
            mGPSData->selectedRouteIndex    = -1;
            mGPSData->selectedTrackIndex    = -1;
            mGPSData->selectedSegmentIndex  = -1;

            switch (item->data(0, Qt::UserRole +1 ).toInt()){
            case (GPX):
                break;
            case (WAYPOINTLIST):
                mGPSData->displayedWaypointIndex = -1;
                break;
            case (WAYPOINT):
                mGPSData->displayedWaypointIndex = item->data(0, Qt::UserRole + 2 ).toInt();
                mGPSData->selectedWaypointIndex  = mGPSData->displayedWaypointIndex;
                break;
            case (ROUTELIST):
                mGPSData->displayedRouteIndex    = -1;
                break;
            case (ROUTE):
                mGPSData->displayedRouteIndex    = item->data(0, Qt::UserRole + 2 ).toInt();
                mGPSData->selectedRouteIndex     = mGPSData->displayedRouteIndex;
                break;
            case (TRACKLIST):
                mGPSData->displayedTrackIndex    = -1;
                mGPSData->displayedSegmentIndex  = -1;
                break;
            case (TRACK):
                mGPSData->displayedTrackIndex      = item->data(0, Qt::UserRole + 2 ).toInt();
                mGPSData->displayedSegmentIndex    = -1;
                mGPSData->selectedTrackIndex       = mGPSData->displayedTrackIndex;
                break;
            case (TRACKSEG):
                mGPSData->displayedTrackIndex      = item->parent()->data(0, Qt::UserRole + 2 ).toInt();
                mGPSData->displayedSegmentIndex    = item->data(0, Qt::UserRole + 2 ).toInt();
                mGPSData->selectedTrackIndex       = mGPSData->displayedTrackIndex;
                mGPSData->selectedSegmentIndex     = mGPSData->displayedSegmentIndex;
                break;
            }
            emit signalSelectionChanged();
        }
    } //MainWindow::on_treeWidgetCurrentGPX_clicked

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    //void MainWindow::trackSelectionEnable(bool enable)
    void MainWindow::trackSelectionEnable(bool GPXEnable, bool WaypointEnable, bool RouteEnable, bool Tracksenable, bool SegmentsEnable)
    {
        qDebug() << __FILE__ << __FUNCTION__;
        //ui->treeWidgetCurrentGPX->setEnabled(GPXEnable);
        if ( ui->treeWidgetCurrentGPX->selectedItems().count() != 0 )
        {
            seletectItem = ui->treeWidgetCurrentGPX->currentItem();
        }

        QTreeWidgetItemIterator it(ui->treeWidgetCurrentGPX);
        while (*it) {
            switch ((*it)->data(0, Qt::UserRole +1 ).toInt()){
            case (GPX):
                (*it)->setDisabled(!GPXEnable);
                break;
            case (WAYPOINTLIST):
            case (WAYPOINT):
                (*it)->setDisabled(!WaypointEnable);
                break;
            case (ROUTELIST):
            case (ROUTE):
                (*it)->setDisabled(!RouteEnable);
                break;
            case (TRACKLIST):
            case (TRACK):
                (*it)->setDisabled(!Tracksenable);
                break;
            case (TRACKSEG):
                (*it)->setDisabled(!SegmentsEnable);
                break;
            }
            ++it;
        }
        if (!GPXEnable) {
            ui->treeWidgetCurrentGPX->setCurrentItem(NULL,0);
        }
        else {
             ui->treeWidgetCurrentGPX->setCurrentItem(seletectItem,0);
        }

    } //MainWindow::trackSelectionEnable

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::displayPluginShow()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        QAction *action = qobject_cast<QAction *>(sender());

        if ( visiblePlugin != NULL) {
            disconnect(visiblePlugin, SIGNAL(signalSetTrackSelection(bool,bool,bool,bool,bool)),this,SLOT(trackSelectionEnable(bool,bool,bool,bool,bool)));
            disconnect(visiblePlugin, SIGNAL(signalLoadFile(QString,bool)),this,SLOT(loadFile(QString,bool)));
            disconnect(this, SIGNAL(signalFileLoaded()),visiblePlugin,SLOT(on_fileLoaded()));
            disconnect(this, SIGNAL(signalSelectionChanged()),visiblePlugin,SLOT(on_selectionChanged()));
            disconnect(mGPSData, SIGNAL(signalGPSDataGPXUpdated()),visiblePlugin,SLOT(on_gpsdataGPXChanged()));
            disconnect(mGPSData, SIGNAL(signalGPSDataTracksUpdated()),visiblePlugin,SLOT(on_gpsdataTracksChanged()));
            disconnect(mGPSData, SIGNAL(signalGPSDataRoutesUpdated()),visiblePlugin,SLOT(on_gpsdataRoutesChanged()));
            disconnect(mGPSData, SIGNAL(signalGPSDataWaypointsUpdated()),visiblePlugin,SLOT(on_gpsdataWaypointsChanged()));
        }

        visiblePlugin = NULL;
        int idx = action->data().toInt() - 3;
        qDebug() << __FILE__ << __FUNCTION__ << idx << "-" << displayPluginList.count();
        if ( idx > 0 ){
            visiblePlugin = displayPluginList[idx];
            connect(visiblePlugin, SIGNAL(signalSetTrackSelection(bool,bool,bool,bool,bool)),this,SLOT(trackSelectionEnable(bool,bool,bool,bool,bool)));
            connect(visiblePlugin, SIGNAL(signalLoadFile(QString,bool)),this,SLOT(loadFile(QString,bool)));
            connect(this, SIGNAL(signalFileLoaded()),visiblePlugin,SLOT(on_fileLoaded()));
            connect(this, SIGNAL(signalSelectionChanged()),visiblePlugin,SLOT(on_selectionChanged()));
            connect(mGPSData, SIGNAL(signalGPSDataGPXUpdated()),visiblePlugin,SLOT(on_gpsdataGPXChanged()));
            connect(mGPSData, SIGNAL(signalGPSDataTracksUpdated()),visiblePlugin,SLOT(on_gpsdataTracksChanged()));
            connect(mGPSData, SIGNAL(signalGPSDataRoutesUpdated()),visiblePlugin,SLOT(on_gpsdataRoutesChanged()));
            connect(mGPSData, SIGNAL(signalGPSDataWaypointsUpdated()),visiblePlugin,SLOT(on_gpsdataWaypointsChanged()));
        }

        ui->stackedWidget->setCurrentIndex(action->data().toInt());
        ui->dockWidgetTraceManagement->setVisible(action->data().toInt() > 1);
    } //MainWindow::displayPluginShow

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_actionAboutQt_triggered()
    {
        QApplication::aboutQt();
    } //MainWindow::on_actionAboutQt_triggered

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_toolButtonAdd_clicked()
    {
        //
        openDialog->setFileMode(QFileDialog::ExistingFiles);

        //Open the file (we don't care about the format, the plugin is here for that)
        on_actionOpen_triggered();

        //Add it into the catalog
        if (!mGPSData->trackList.isEmpty()) {
            on_toolButtonAddToCatalog_clicked();
        }

    } //MainWindow::on_toolButtonAdd_clicked

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::treeWidgetContextMenuClicked()
    {
        QListWidgetItem *index = ui->listWidgetTracksOfTheDay->currentItem();
        DialogTrackProperty *trackProperty = new DialogTrackProperty();
        trackProperty->setFileName(   index->data(Qt::UserRole).toString());
        trackProperty->setDisplayName(index->data(Qt::UserRole + 1).toString());
        trackProperty->setDescription(index->data(Qt::UserRole + 2).toString());
        trackProperty->exec();
        if ( mGPSData->filename == trackProperty->fileName() ) {
            ui->labelTrackName->setText( trackProperty->displayName() );
        }
        Database::updateTrackProperties(trackProperty->fileName(), trackProperty->displayName(), trackProperty->description());
        Database::updateListWidget(ui->listWidgetTracksOfTheDay, ui->calendarWidget->selectedDate());
        Database::updateListWidget(ui->listWidgetNoDateTracks, QDate(0,0,0));
        delete trackProperty;
    } //MainWindow::treeWidgetContextMenuClicked



    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_listWidgetNoDateTracks_doubleClicked(QModelIndex index){
        if ( index.data(Qt::UserRole).toString() != mGPSData->filename)
        {
            loadFile(index.data(Qt::UserRole).toString(),true);
        }
        ui->labelTrackName->setText(ui->listWidgetNoDateTracks->currentItem()->text());
    } //MainWindow::on_listWidgetNoDateTracks_doubleClicked

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_listWidgetTracksOfTheDay_doubleClicked(QModelIndex index)
    {
        if ( index.data(Qt::UserRole).toString() != mGPSData->filename)
        {
            loadFile(index.data(Qt::UserRole).toString(),true);
        }
        ui->labelTrackName->setText(ui->listWidgetTracksOfTheDay->currentItem()->text());
    } //MainWindow::on_listWidgetTracksOfTheDay_doubleClicked

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_toolButtonHome_clicked()
    {
        ui->calendarWidget->setSelectedDate(QDate::currentDate());
    } //MainWindow::on_toolButtonHome_clicked

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_calendarWidget_selectionChanged()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        Database::updateListWidget(ui->listWidgetTracksOfTheDay, ui->calendarWidget->selectedDate());
        updateNavigationButtons();
    } //MainWindow::on_calendarWidget_selectionChanged

    /*------------------------------------------------------------------------------*
      Refresh the navigation button of the catalog
     *------------------------------------------------------------------------------*/
    void MainWindow::updateNavigationButtons()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        prevDate = Database::getPrevDate(ui->calendarWidget->selectedDate());
        nextDate = Database::getNextDate(ui->calendarWidget->selectedDate());
        ui->toolButtonNext->setEnabled( nextDate.isValid() );
        ui->toolButtonPrev->setEnabled( prevDate.isValid() );
    } //MainWindow::updateNavigationButtons

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_toolButtonPrev_clicked()
    {
        ui->calendarWidget->setSelectedDate(prevDate);
    } //MainWindow::on_toolButtonPrev_clicked

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_toolButtonNext_clicked()
    {
        ui->calendarWidget->setSelectedDate(nextDate);
    } //MainWindow::on_toolButtonNext_clicked


    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_listWidgetNoDateTracks_customContextMenuRequested(QPoint pos) {
        on_listWidgetTracksOfTheDay_customContextMenuRequested(pos);
    } //MainWindow::on_listWidgetNoDateTracks_customContextMenuRequested

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_listWidgetTracksOfTheDay_customContextMenuRequested(QPoint pos)
    {
        QMenu menu(ui->listWidgetTracksOfTheDay);
        QAction *action = new QAction(tr("Properties..."),ui->listWidgetTracksOfTheDay);
        menu.addAction(action);
        QObject::connect(action, SIGNAL(triggered()), this, SLOT(treeWidgetContextMenuClicked()));
        menu.exec(ui->listWidgetTracksOfTheDay->mapToGlobal(pos));
    } //MainWindow::on_listWidgetTracksOfTheDay_customContextMenuRequested

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_listWidgetTracksOfTheDay_itemSelectionChanged()
    {
        ui->toolButtonDelete->setEnabled(ui->listWidgetTracksOfTheDay->selectedItems().count() == 1);
    } //MainWindow::on_listWidgetTracksOfTheDay_itemSelectionChanged

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_toolButtonDelete_clicked()
    {
        if ( QMessageBox::question(NULL,qApp->applicationName(),tr("Are you sure you want to delete this track?"),QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes )
        {
            QListWidgetItem *index = ui->listWidgetTracksOfTheDay->currentItem();
            Database::deleteTrack(index->data(Qt::UserRole).toString());
            if ( index->data(Qt::UserRole).toString() == mGPSData->filename )
            {
                on_actionClose_current_trace_triggered();
            }
            Database::updateListWidget(ui->listWidgetTracksOfTheDay, ui->calendarWidget->selectedDate());
            Database::initCalendarWidget(ui->calendarWidget);
            updateNavigationButtons();
        }
    } //MainWindow::on_toolButtonDelete_clicked

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::updateSaveButton()
    {
        ui->actionSave->setEnabled(QFileInfo(mGPSData->filename).exists() && mGPSData->isModified);
        ui->actionSaveAs->setEnabled(mGPSData->filename != "");
    } //MainWindow::updateSaveButton

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::saveFile(QString filename)
    {
        qDebug( )  << __FILE__ << __FUNCTION__ ;
        progressIndicator->startAnimation();
        qApp->setOverrideCursor(QCursor(Qt::WaitCursor));

        gpxPlugin->save(filename, mGPSData);
        if ( mGPSData->isFromCatalog )
        {
            Database::updateDate(mGPSData->filename, mGPSData->metadata->time.date());
            Database::updateListWidget(ui->listWidgetTracksOfTheDay, ui->calendarWidget->selectedDate());
            Database::updateListWidget(ui->listWidgetNoDateTracks, QDate(0,0,0));
            Database::initCalendarWidget(ui->calendarWidget);
            updateNavigationButtons();

        }
        qApp->setOverrideCursor(QCursor(Qt::ArrowCursor));

        ui->actionSave->setEnabled(false);
        progressIndicator->stopAnimation();
    } //MainWindow::saveFile

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_actionSave_triggered()
    {
       saveFile(mGPSData->filename);
    } //MainWindow::on_actionSave_triggered

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_actionSaveAs_triggered()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        QFileDialog* saveDialog = new QFileDialog(this, tr("Save GPS trace"), QString(), saveFilters);
        saveDialog->setConfirmOverwrite(true);
        saveDialog->setAcceptMode(QFileDialog::AcceptSave);
        saveDialog->restoreState(settings->value("savestate").toByteArray());
        saveDialog->selectNameFilter(settings->value("savestatefilter",gpxPlugin->getSaveFilter()).toString());
        if (saveDialog->exec() == QDialog::Accepted)
        {
            saveFile(QString(saveDialog->selectedFiles()[0]));
        }
    } //MainWindow::on_actionSaveAs_triggered

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_toolButtonAddToCatalog_clicked()
    {
        QFileInfo info(mGPSData->filename);
        QString storagePath = settings->value("StorageLocation","").toString();
        mGPSData->filename = storagePath + "/data/" + info.fileName();

        saveFile(mGPSData->filename);

        //Update database, treeview and calandar view
        Database::addTrackInDatabase(mGPSData);
        Database::initCalendarWidget(ui->calendarWidget);
        ui->calendarWidget->setSelectedDate(mGPSData->metadata->time.date());
        Database::updateListWidget(ui->listWidgetTracksOfTheDay, ui->calendarWidget->selectedDate());
        Database::updateListWidget(ui->listWidgetNoDateTracks, QDate(0,0,0));
        updateNavigationButtons();
        ui->toolButtonInCatalog->setChecked(true);
        ui->toolButtonAddToCatalog->setChecked(true);
        ui->toolButtonAddToCatalog->setDisabled(true);
        ui->labelTrackDate->setText(    mGPSData->metadata->time.toString("dd/MM/yyyy") );
        ui->labelTrackName->setText(    QFileInfo(mGPSData->filename).fileName());
    } //MainWindow::on_toolButtonAddToCatalog_clicked

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_actionClose_current_trace_triggered()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        //mGPSData->lockGPSDataForWrite();

        ui->treeWidgetCurrentGPX->blockSignals(true);
        ui->treeWidgetCurrentGPX->clear();
        QTreeWidgetItem * clearItem = new QTreeWidgetItem ();
        clearItem->setText(0,tr("No trace loaded"));
        ui->treeWidgetCurrentGPX->addTopLevelItem(clearItem);
        ui->treeWidgetCurrentGPX->setEnabled(false);
        ui->tabWidgetLeftPanel->setCurrentIndex(0);
        ui->treeWidgetCurrentGPX->blockSignals(false);

        mGPSData->clearData();

        ui->toolButtonInCatalog->setChecked(false);
        ui->labelTrackDate->setText( "-" );
        ui->labelTrackName->setText( "-" );
        ui->actionClose_current_trace->setEnabled(false);
        ui->actionReload->setEnabled(false);
        ui->toolButtonInCatalog->setChecked(false);
        ui->toolButtonAddToCatalog->setChecked(false);
        ui->toolButtonAddToCatalog->setDisabled(false);

        mGPSData->setGPXModified(false);
        //mGPSData->unlockGPSData();
    } //MainWindow::on_actionClose_current_trace_triggered


    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_actionReload_triggered()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        QString strtmp = mGPSData->filename;
        bool catalog = mGPSData->isFromCatalog;
        mGPSData->blockSignals(true);
        on_actionClose_current_trace_triggered();
        qDebug() << __FILE__ << __FUNCTION__ << "clean";
        loadFile(strtmp,catalog);
        mGPSData->blockSignals(false);
        qDebug() << __FILE__ << __FUNCTION__ << "done";


    } //MainWindow::on_actionReload_triggered()

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_actionFullScreen_toggled(bool checked)
    {
        if (checked)
            setWindowState(Qt::WindowFullScreen);
        else
            setWindowState(Qt::WindowMaximized);
    } //MainWindow::on_actionFullScreen_toggled

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_comboBoxLanguageSelection_currentIndexChanged(int index)
    {
        GPSBookApplication::setLanguage(ui->comboBoxLanguageSelection->itemData(index).toString());
        ui->buttonBoxApply->setEnabled(true);
    } //MainWindow::on_comboBoxLanguageSelection_currentIndexChanged

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_lineEditStorageLocation_textChanged(QString )
    {
        ui->buttonBoxApply->setEnabled(true);
    } //MainWindow::on_lineEditStorageLocation_textChanged

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_buttonBoxApply_clicked(QAbstractButton* button)
    {
        switch (ui->buttonBoxApply->standardButton(button)) {
            case QDialogButtonBox::Apply:
                settings->setValue("Translation",ui->comboBoxLanguageSelection->itemData(ui->comboBoxLanguageSelection->currentIndex()).toString());
                settings->setValue("StorageLocation",ui->lineEditStorageLocation->text());
                settings->setValue("PreferedPluginId",ui->comboBoxPreferedPlugin->currentIndex());
                settings->setValue("StartFullScreen",ui->checkBoxStartFullScreen->checkState());
                settings->setValue("MenuVisibility",ui->checkBoxMenuVisibility->checkState());
                settings->setValue("ToolbarVisibility",ui->checkBoxToolbarVisibility->checkState());
                ui->buttonBoxApply->setEnabled(false);
            break;
            case QDialogButtonBox::Reset:
                settings->clear();
            break;
            default:
            break;
        }
    } //MainWindow::on_buttonBoxApply_clicked

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_checkBoxStartFullScreen_toggled(bool checked)
    {
        ui->buttonBoxApply->setEnabled(true);
        if (checked) {
            setWindowState(Qt::WindowFullScreen);
        }
        else
        {
            setWindowState(Qt::WindowMaximized);
        }
    } //MainWindow::on_checkBoxStartFullScreen_toggled

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_comboBoxPreferedPlugin_currentIndexChanged(int)
    {
        ui->buttonBoxApply->setEnabled(true);
    } //MainWindow::on_comboBoxPreferedPlugin_currentIndexChanged

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_checkBoxMenuVisibility_toggled(bool checked)
    {
        ui->buttonBoxApply->setEnabled(true);
        if (! checked )
            ui->checkBoxToolbarVisibility->setChecked(true);
        ui->menuBar->setVisible(ui->checkBoxMenuVisibility->checkState());
        ui->mainToolBar->setVisible(ui->checkBoxToolbarVisibility->checkState());
        ui->toolBar->setVisible(ui->checkBoxToolbarVisibility->checkState());

    } //MainWindow::on_checkBoxMenuVisibility_toggled

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void MainWindow::on_checkBoxToolbarVisibility_toggled(bool checked)
    {
        ui->buttonBoxApply->setEnabled(true);
        if (! checked )
            ui->checkBoxMenuVisibility->setChecked(true);
        ui->menuBar->setVisible(ui->checkBoxMenuVisibility->checkState());
        ui->mainToolBar->setVisible(ui->checkBoxToolbarVisibility->checkState());
        ui->toolBar->setVisible(ui->checkBoxToolbarVisibility->checkState());
    } //MainWindow::on_checkBoxToolbarVisibility_toggled
}

void GPSBook::MainWindow::on_treeWidgetOptions_itemClicked(QTreeWidgetItem* item, int)
{
    int idx = ui->treeWidgetOptions->indexOfTopLevelItem(item);
    if ( idx != -1)
    {
        if ( item->child(0) )
        {
            ui->stackedWidgetOptions->setCurrentIndex(item->child(0)->data(0,Qt::UserRole+1).toInt());
            ui->treeWidgetOptions->expandItem(item);
            ui->treeWidgetOptions->setCurrentItem(item->child(0),0);
        }
        else
        {
            ui->stackedWidgetOptions->setCurrentIndex(idx);
            ui->treeWidgetOptions->setCurrentItem(item,0);
        }
    }
    else
    {
        ui->stackedWidgetOptions->setCurrentIndex( item->data(0,Qt::UserRole+1).toInt() );
        ui->treeWidgetOptions->setCurrentItem(item,0);
    }
}

void GPSBook::MainWindow::on_treeWidgetHelp_itemClicked(QTreeWidgetItem* item, int)
{
    int idx = ui->treeWidgetHelp->indexOfTopLevelItem(item);
    if ( idx != -1)
    {
        if ( item->child(0) )
        {
            ui->stackedWidgetHelp->setCurrentIndex(item->child(0)->data(0,Qt::UserRole+1).toInt());
            ui->treeWidgetHelp->expandItem(item);
            ui->treeWidgetHelp->setCurrentItem(item->child(0),0);
        }
        else
        {
            ui->stackedWidgetHelp->setCurrentIndex(idx);
            ui->treeWidgetHelp->setCurrentItem(item,0);
        }
    }
    else
    {
        ui->stackedWidgetHelp->setCurrentIndex( item->data(0,Qt::UserRole+1).toInt() );
        ui->treeWidgetHelp->setCurrentItem(item,0);
    }
}



