/*************************************************************************
*   Copyright (C) 2006 by Christoph Eckert                                *
*   ce@christeck.de                                                       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#include "Versioning.h"
#include "MyMainWindow.h"
#include "displaygpsbabel.h"

using namespace PluginDisplayGPSBabel;

MyMainWindow::MyMainWindow( QWidget * parent )
    : QFrame( parent )
{
    gpsbookOutputFile = new QTemporaryFile(QDir::tempPath() + QDir::separator() + QCoreApplication::applicationName() + "_gpsbabel_output");
    gpsbookOutputFile->open();
    gpsbookOutputFile->close();
    gpsbookOutputFile->setFileName(gpsbookOutputFile->fileName()+".gpx");
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(cleanInfo()));

    // Create those before running checkForConfigs() resp. restoreDefaults()
    PreferencesEditWindow = new MyPreferencesConfig( this );
    // PresetSavingWindow = new MySavePresetWindow( this );
    ProgressDialog = new QProgressDialog( this );

    InputItems = new QList<ListItem * >;
    OutputItems = new QList<ListItem * >;
    FilterItems = new QList<ListItem * >;
    ActiveItem = 0L;
    TempListItem = 0L;

    initGpsBabel();

    setupUi( this );
    statusLabel->setText("");
    this->setupMenus();
    this->setupConnections();

    // Automatically create default app settings if no config file exists
    checkForConfigs();

    //QCoreApplication::setOrganizationName("Gebabbel");
    //QCoreApplication::setOrganizationDomain("christeck.de");
    //QCoreApplication::setApplicationName("Gebabbel");
    //QIcon AppIcon( ":/icons/AppIcon.png" );
    //setWindowIcon( AppIcon );

    ProcessWaypoints = false;
    ProcessRoutes = false;
    ProcessTracks = false;
    RealtimeTracking = false;
    SynthesizeShortnames = false;
    SmartIcons = false;

    InputList->setColumnCount( 2 );
    OutputList->setColumnCount( 2 );
    FilterList->setColumnCount( 2 );

    InputList->setRootIsDecorated( false );
    OutputList->setRootIsDecorated( false );
    FilterList->setRootIsDecorated( false );

    InputHeaders.append( tr("Intype") );
    InputHeaders.append( tr("Source") );
    OutputHeaders.append( tr("Outtype") );
    OutputHeaders.append( tr("Destination") );
    FilterHeaders.append( tr("Filter") );
    FilterHeaders.append( tr( "Options" ) );

    InputList->setHeaderLabels( InputHeaders );
    OutputList->setHeaderLabels( OutputHeaders );
    FilterList->setHeaderLabels( FilterHeaders );

    InputList->resizeColumnToContents( 0 );
    InputList->resizeColumnToContents( 1 );
    OutputList->resizeColumnToContents( 0 );
    OutputList->resizeColumnToContents( 1 );
    FilterList->resizeColumnToContents( 0 );
    FilterList->resizeColumnToContents( 1 );

    CheckboxWaypoints->setCheckState( Qt::Unchecked );
    CheckboxRoutes->setCheckState( Qt::Unchecked );
    CheckboxTracks->setCheckState( Qt::Unchecked );
    CheckboxRealtimeTracking->setCheckState( Qt::Unchecked );
    CheckboxShortnames->setCheckState( Qt::Unchecked );
    CheckboxSmartIcons->setCheckState( Qt::Unchecked );

    // Invoke this after setupUi(), otherwise it seems to overwrite window size
    //setAppGeometry();

    // Reload the last used settings
    // The first element in the combo is the special element tr( "Last used settings" )
    loadPreset( comboPresets->itemText( 0 ) );
    //checkUsageCount();
}

void MyMainWindow::init(GPSData* gpsdata)
{
    mGPSData = gpsdata;
}

void MyMainWindow::displayAboutBox()
{
    AboutBox = new QMessageBox(this);
    QString AboutText;
    AboutText.append( tr( "This is %1, version %2.\n\n" ).arg( APPNAME ).arg( VERSION ) );
    AboutText.append( tr( "It was created and published in %1 by\n%2.\n" ).arg( YEARS ).arg( AUTHOR ) );
    AboutText.append( tr( "It has been released under the following terms and conditions:\n%1.\n").arg( LICENSE ) );
    AboutText.append( tr( "All rights, including the copyright, belong to %1.").arg( COPYRIGHTHOLDER ) );
    AboutBox->setText( AboutText );
    AboutBox->show();
}


void MyMainWindow::displayHelpDialog()
{
    HelpBox = new QMessageBox( this );
    HelpBox->setText( tr( "There is no real manual, but you can consult the following resources:\nThe excellent gpsbabel documentation as found on\n%1\nThe Gebabbel-FAQ as found on\n%2\nIf questions remain, don't hesitate to contact the author\n%3" ).arg( GPSBABELHOMEPAGE ).arg( HOMEPAGE ).arg( AUTHOR) );
    HelpBox->show();
}


void MyMainWindow::displayWarning( QString Caption, QString WarningMessage )
{
    QMessageBox::warning( this, Caption, WarningMessage );
}


void MyMainWindow::displayComment( QString PresetName )
{
    setStatusbarText( SettingsManager::instance()->comment( PresetName ), 5000 );
}


void MyMainWindow::setStatusbarText( QString Text, unsigned int Time )
{
    statusLabel->setText(Text);
    timer->start(Time);
}

void MyMainWindow::cleanInfo( )
{
    statusLabel->setText("");
    timer->stop();
}


// this function gets invoced by the constructor of the main window
void MyMainWindow::setupConnections()
{
    // Preset combobox
    connect( comboPresets, SIGNAL( activated( QString ) ), this, SLOT( loadPreset( QString ) ) );
    connect( comboPresets, SIGNAL( highlighted( QString ) ), this, SLOT( displayComment( QString ) ) );

    // Catching RMB clicks in the lists:
    //connect( InputList, SIGNAL( customContextMenuRequested( QPoint ) ), this, SLOT( showInputContextMenu( QPoint ) ) );
    //connect( OutputList, SIGNAL( customContextMenuRequested( QPoint ) ), this, SLOT( showOutputContextMenu( QPoint ) ) );
    //connect( FilterList, SIGNAL( customContextMenuRequested( QPoint ) ), this, SLOT( showFilterContextMenu( QPoint ) ) );

    connect( CheckboxWaypoints, SIGNAL( stateChanged( int ) ), this, SLOT( setProcessWaypoints( int ) ) );
    connect( CheckboxWaypoints, SIGNAL( stateChanged( int ) ), this, SLOT( finetuneGui() ) );

    connect( CheckboxRoutes, SIGNAL( stateChanged( int ) ), this, SLOT( setProcessRoutes( int ) ) );
    connect( CheckboxRoutes, SIGNAL( stateChanged( int ) ), this, SLOT( finetuneGui() ) );

    connect( CheckboxTracks, SIGNAL( stateChanged( int ) ), this, SLOT( setProcessTracks( int ) ) );
    connect( CheckboxTracks, SIGNAL( stateChanged( int ) ), this, SLOT( finetuneGui() ) );

    connect( CheckboxRealtimeTracking, SIGNAL( stateChanged( int ) ), this, SLOT( setRealtimeTracking( int ) ) );
    connect( CheckboxRealtimeTracking, SIGNAL( stateChanged( int ) ), this, SLOT( finetuneGui() ) );

    connect( CheckboxShortnames, SIGNAL( stateChanged( int ) ), this, SLOT( setSynthesizeShortnames( int ) ) );
    connect( CheckboxSmartIcons, SIGNAL( stateChanged( int ) ), this, SLOT( setSmartIcons( int ) ) );

    connect( InputList, SIGNAL( itemDoubleClicked( QTreeWidgetItem *, int ) ), this, SLOT( doubleClickedInput( QTreeWidgetItem *, int ) ) );
    connect( OutputList, SIGNAL( itemDoubleClicked( QTreeWidgetItem *, int ) ), this, SLOT( doubleClickedOutput( QTreeWidgetItem *, int ) ) );
    connect( FilterList, SIGNAL( itemDoubleClicked( QTreeWidgetItem *, int ) ), this, SLOT( doubleClickedFilter( QTreeWidgetItem *, int ) ) );

    //connect( QCoreApplication::instance(), SIGNAL( aboutToQuit() ), this, SLOT( saveAppGeometry() ) );
    connect( QCoreApplication::instance(), SIGNAL( aboutToQuit() ), this, SLOT( saveLastUsedSettings() ) );

    // The QActions are connected in setupMenus()
}


// I need an instance of gpsbabel to display the current working directory in the UI
void MyMainWindow::initGpsBabel()
{
    ExecGpsBabel = new QProcess;

    // The error output always should be english so I can provide i18n'ed error messages
    QStringList environment = QProcess::systemEnvironment();
    environment.replaceInStrings(QRegExp("^LANG=(.*)", Qt::CaseInsensitive), "LANG=");
    environment.replaceInStrings(QRegExp("^LC_ALL=(.*)", Qt::CaseInsensitive), "LC_ALL=");
    ExecGpsBabel->setEnvironment( environment );

    QDir WorkingDir;
    WorkingDir.setPath( QDir::homePath() );
    // If the Desktop dir doesn't exist, WorkingDir remains in the previous path
    WorkingDir.cd( "Desktop" ) ;

    connect( ExecGpsBabel, SIGNAL( started() ), this, SLOT( processingStarted() ) );
    connect( ExecGpsBabel, SIGNAL( finished( int, QProcess::ExitStatus ) ), this, SLOT( processingStopped() ) );
    // Attention: This returns an error of the *QProcess*, not if the executed application reported an error
    connect( ExecGpsBabel, SIGNAL( error( QProcess::ProcessError ) ), this, SLOT( externalCallFailed() ) );

    connect( ProgressDialog, SIGNAL( canceled() ), ExecGpsBabel, SLOT( terminate() ) );

    ExecGpsBabel->setWorkingDirectory( WorkingDir.absolutePath() );
}


// This function gets invoked by the constructor of the main window
void MyMainWindow::setupMenus()
{
    // Creating the menus
    //MenuFile = menuBar()->addMenu(tr( "&File") );
    //MenuEdit = menuBar()->addMenu(tr( "&Edit") );
    //MenuHelp = menuBar()->addMenu(tr( "&Help") );

    //InputContextMenu = new QMenu;
    //OutputContextMenu = new QMenu;
    //FilterContextMenu = new QMenu;

    // Creating the toolbar
    //MainToolBar = addToolBar( tr( "Main Toolbar" ) );
    MainToolBar = new QToolBar("Main Toolbar",mainToolBarFrame);
    MainToolBar->setIconSize( QSize::QSize( 16, 16 ) );
    MainToolBar->setAllowedAreas( Qt::TopToolBarArea );
    MainToolBar->setMovable( false );
    mainToolBarFrame->layout()->addWidget(MainToolBar);

    // Creating the actions
    ActionFileClear = new QAction(QIcon(":/icons/Delete.png"), tr("&Clear"), this);
    ActionFileClear->setShortcut( Qt::Key_Delete );
    ActionFileClear->setStatusTip(tr("Clears all currently loaded data") );
    ActionFileClear->setWhatsThis( tr("Clears all currently loaded data") );
    ActionFileClear->setEnabled(true);
    connect(ActionFileClear, SIGNAL(triggered() ), this, SLOT( wipeData() ) );

    ActionFileDeletePreset = new QAction(QIcon(":/icons/FileDelete.png"), tr("&Delete Preset..."), this);
    ActionFileDeletePreset->setShortcut( Qt::Key_Backspace );
    ActionFileDeletePreset->setStatusTip(tr("Delete the current preset"));
    ActionFileDeletePreset->setWhatsThis( tr("Delete the current preset. Attention: There is no undo!") );
    ActionFileDeletePreset->setEnabled(true);
    connect(ActionFileDeletePreset, SIGNAL(triggered()), this, SLOT(deletePreset()));

    ActionFileSavePreset = new QAction(QIcon(":/icons/FileSave.png"), tr("&Save Preset..."), this);
    ActionFileSavePreset->setShortcut(tr("Ctrl+S"));
    ActionFileSavePreset->setStatusTip(tr("Save the current settings as new preset"));
    ActionFileSavePreset->setWhatsThis( tr("Save the current settings as new preset for later usage") );
    ActionFileSavePreset->setEnabled(true);
    connect(ActionFileSavePreset, SIGNAL(triggered()), this, SLOT(saveNamedPreset()));

    ActionFileProcess = new QAction(QIcon(":/icons/FileConvert.png"), tr("&Process"), this);
    ActionFileProcess->setShortcut(tr("Ctrl+X"));
    ActionFileProcess->setStatusTip(tr("Execute gpsbabel"));
    ActionFileProcess->setWhatsThis( tr("Invokes gpsbabel according to your settings") );
    ActionFileProcess->setEnabled(true);
    connect(ActionFileProcess, SIGNAL(triggered()), this, SLOT(processData()));
    connect(BtnProcess, SIGNAL(clicked()), this, SLOT(processData()));

    ActionFileQuit = new QAction(QIcon(":/icons/FileQuit.png"), tr("&Quit"), this);
    ActionFileQuit->setShortcut(tr("Ctrl+Q"));
    ActionFileQuit->setStatusTip(tr("Quit this application"));
    ActionFileQuit->setWhatsThis( tr("Quits this application without further confirmation.") );
    ActionFileQuit->setEnabled(true);

    //connect( QCoreApplication::instance(), SIGNAL( aboutToQuit() ), this, SLOT( saveAppGeometry() ) );

    connect(ActionFileQuit, SIGNAL(triggered()), this, SLOT(close()));

    ActionEditEditCommand = new QAction(QIcon(":/icons/EditEditCommand.png"), tr("Edit &Command..."), this);
    ActionEditEditCommand->setShortcut(tr("Ctrl+E"));
    ActionEditEditCommand->setStatusTip(tr("Edit command"));
    ActionEditEditCommand->setWhatsThis( tr("Allows to edit the command to be executed") );
    ActionEditEditCommand->setEnabled(true);
    connect(ActionEditEditCommand, SIGNAL(triggered()), this, SLOT(editCommand()));

    ActionEditEditPreferences = new QAction(QIcon(":/icons/EditEditPreferences.png"), tr("Edit &Preferences..."), this);
    ActionEditEditPreferences->setShortcut(tr("Ctrl+P"));
    ActionEditEditPreferences->setStatusTip(tr("Edit preferences"));
    ActionEditEditPreferences->setWhatsThis( tr("Opens a dialog to modify the preferences") );
    ActionEditEditPreferences->setEnabled(true);
    connect(ActionEditEditPreferences, SIGNAL(triggered()), this, SLOT(editPreferences()));

    ActionHelpManual = new QAction(QIcon(":/icons/HelpManual.png"), tr("&Manual"), this);
    ActionHelpManual->setShortcut(tr("Ctrl+M"));
    ActionHelpManual->setStatusTip(tr("Display the online help"));
    ActionHelpManual->setWhatsThis( tr("Shows the online help") );
    ActionHelpManual->setEnabled(true);
    connect(ActionHelpManual, SIGNAL(triggered() ), this, SLOT( displayHelpDialog() ) );

    ActionHelpAbout = new QAction(QIcon(":/icons/HelpAbout.png"), tr("&About"), this);
    ActionHelpAbout->setShortcut(tr("Ctrl+A"));
    ActionHelpAbout->setStatusTip(tr("Display information about the application") );
    ActionHelpAbout->setWhatsThis( tr("Display information about the application") );
    ActionHelpAbout->setEnabled(true);
    connect(ActionHelpAbout, SIGNAL(triggered() ), this, SLOT( displayAboutBox() ) );

    ActionHelpWhatsThis = QWhatsThis::createAction( this );

    ActionInputAdd = new QAction(QIcon(":/icons/Add.png"), tr("&Add"), this);
    ActionInputAdd->setShortcut(tr("Ctrl+A"));
    ActionInputAdd->setStatusTip(tr("Add an input source") );
    ActionInputAdd->setWhatsThis( tr("Add an input source") );
    ActionInputAdd->setEnabled(true);
    connect(ActionInputAdd, SIGNAL(triggered() ), this, SLOT( AddOrEditInput() ) );

    ActionInputEdit = new QAction(QIcon(":/icons/Edit.png"), tr("&Edit"), this);
    ActionInputEdit->setShortcut(tr("Ctrl+E"));
    ActionInputEdit->setStatusTip(tr("Edit the selected input source") );
    ActionInputEdit->setWhatsThis( tr("Edit the selected input source") );
    ActionInputEdit->setEnabled(true);
    connect(ActionInputEdit, SIGNAL(triggered() ), this, SLOT( AddOrEditInput() ) );

    ActionInputRemove = new QAction(QIcon(":/icons/Delete.png"), tr("&Remove"), this);
    ActionInputRemove->setShortcut(tr("Ctrl+R"));
    ActionInputRemove->setStatusTip(tr("Remove the selected input source") );
    ActionInputRemove->setWhatsThis( tr("Remove the selected input source") );
    ActionInputRemove->setEnabled(true);
    connect(ActionInputRemove, SIGNAL(triggered() ), this, SLOT( removeInput() ) );


    ActionOutputAdd = new QAction(QIcon(":/icons/Add.png"), tr("&Add"), this);
    ActionOutputAdd->setShortcut(tr("Ctrl+A"));
    ActionOutputAdd->setStatusTip(tr("Add an output destination") );
    ActionOutputAdd->setWhatsThis( tr("Add an output destination") );
    ActionOutputAdd->setEnabled(true);
    connect(ActionOutputAdd, SIGNAL(triggered() ), this, SLOT( AddOrEditOutput() ) );

    ActionOutputEdit = new QAction(QIcon(":/icons/Edit.png"), tr("&Edit"), this);
    ActionOutputEdit->setShortcut(tr("Ctrl+E"));
    ActionOutputEdit->setStatusTip(tr("Edit the selected output destination") );
    ActionOutputEdit->setWhatsThis( tr("Edit the selected output destination") );
    ActionOutputEdit->setEnabled(true);
    connect(ActionOutputEdit, SIGNAL(triggered() ), this, SLOT( AddOrEditOutput() ) );

    ActionOutputRemove = new QAction(QIcon(":/icons/Delete.png"), tr("&Remove"), this);
    ActionOutputRemove->setShortcut(tr("Ctrl+R"));
    ActionOutputRemove->setStatusTip(tr("Remove the selected output destination") );
    ActionOutputRemove->setWhatsThis( tr("Remove the selected output destination") );
    ActionOutputRemove->setEnabled(true);
    connect(ActionOutputRemove, SIGNAL(triggered() ), this, SLOT( removeOutput() ) );


    ActionFilterAdd = new QAction(QIcon(":/icons/Add.png"), tr("&Add"), this);
    ActionFilterAdd->setShortcut(tr("Ctrl+A"));
    ActionFilterAdd->setStatusTip(tr("Add a filter") );
    ActionFilterAdd->setWhatsThis( tr("Add a filter") );
    ActionFilterAdd->setEnabled(true);
    connect(ActionFilterAdd, SIGNAL(triggered() ), this, SLOT( AddOrEditFilter() ) );

    ActionFilterEdit = new QAction(QIcon(":/icons/Edit.png"), tr("&Edit"), this);
    ActionFilterEdit->setShortcut(tr("Ctrl+E"));
    ActionFilterEdit->setStatusTip(tr("Edit the selected filter") );
    ActionFilterEdit->setWhatsThis( tr("Edit the selected filter") );
    ActionFilterEdit->setEnabled(true);
    connect(ActionFilterEdit, SIGNAL(triggered() ), this, SLOT( AddOrEditFilter() ) );

    ActionFilterRemove = new QAction(QIcon(":/icons/Delete.png"), tr("&Remove"), this);
    ActionFilterRemove->setShortcut(tr("Ctrl+R"));
    ActionFilterRemove->setStatusTip(tr("Remove the selected filter") );
    ActionFilterRemove->setWhatsThis( tr("Remove the selected filter") );
    ActionFilterRemove->setEnabled(true);
    connect(ActionFilterRemove, SIGNAL(triggered() ), this, SLOT( removeFilter() ) );


    // Populating the menus
    //MenuFile->addAction( ActionFileSavePreset );
    //MenuFile->addAction( ActionFileProcess );
    //MenuFile->addAction( ActionFileClear );
    //MenuFile->addAction( ActionFileDeletePreset );

    // Don't create the separator on Mac OS because Quit automatically gets moved to another menu
#ifndef MACINTOSH
    //MenuFile->addSeparator();
#endif
    //MenuFile->addAction( ActionFileQuit );

    //MenuEdit->addAction( ActionEditEditCommand );

    //MenuEdit->addAction( ActionEditEditPreferences );

    //MenuHelp->addAction(ActionHelpManual);
    //MenuHelp->addAction(ActionHelpAbout);
    //MenuHelp->addAction(ActionHelpWhatsThis);

    // Populating the toolbars

    inputToolbar->addAction( ActionInputAdd );
    inputToolbar->addAction( ActionInputEdit );
    inputToolbar->addAction( ActionInputRemove );

    outputToolbar->addAction( ActionOutputAdd );
    outputToolbar->addAction( ActionOutputEdit );
    outputToolbar->addAction( ActionOutputRemove );

    filterToolbar->addAction( ActionFilterAdd );
    filterToolbar->addAction( ActionFilterEdit );
    filterToolbar->addAction( ActionFilterRemove );

    // Populating the toolbar
    // MainToolBar->addAction( ActionFileQuit );
    MainToolBar->addAction( ActionEditEditCommand );
    MainToolBar->addAction( ActionFileProcess );
    MainToolBar->addAction( ActionFileClear );
    MainToolBar->addSeparator();
    MainToolBar->addAction( ActionEditEditCommand );
    MainToolBar->addAction( ActionEditEditPreferences );
    MainToolBar->addSeparator();

    // Creating the preset popup
    comboPresets = new QComboBox();
    comboPresets->setEditable ( false );
    comboPresets->setMinimumWidth( 250 );
    comboPresets->setToolTip( tr( "Contains presets for your frequently needed gpsbabel commands. Click on the button Save to create a new preset" ) );
    comboPresets->setStatusTip( tr( "Contains presets for your frequently needed gpsbabel commands. Click on the button Save to create a new preset" ) );
    comboPresets->setWhatsThis( tr( "Contains presets for your frequently needed gpsbabel commands. Click on the button Save to create a new preset" ) );
    fillPresetPopup();
    // adjustSize() adjusts the size of the widget to fit the contents
    comboPresets->adjustSize();
    MainToolBar->addWidget( comboPresets );
    MainToolBar->addAction( ActionFileSavePreset );
    MainToolBar->addAction( ActionFileDeletePreset );
    //MainToolBar->addSeparator();
    //MainToolBar->addAction(ActionHelpManual);
    //MainToolBar->addAction(ActionHelpAbout);
    //MainToolBar->addAction(ActionHelpWhatsThis);

    // Some eye candy :)
    InputList->setAlternatingRowColors( true );
    OutputList->setAlternatingRowColors( true );
    FilterList->setAlternatingRowColors( true );

    // Last but not least waking up the status bar
    //statusBar()->showMessage(tr("I am pleased to be used by you :)"), 8000);

}


void MyMainWindow::saveLastUsedSettings()
{
    savePreset( comboPresets->itemText( 0 ), tr( "Settings at last quit or before choosing another preset" ) );
}


void MyMainWindow::saveNamedPreset()
{
    // This function saves the current settings as a new preset including an optional comment
    MySavePresetWindow PresetSavingWindow;
    if ( PresetSavingWindow.exec() )
    {
        if ( PresetSavingWindow.tellName() != "" )
        {
            savePreset( PresetSavingWindow.tellName(), PresetSavingWindow.tellComment() );
        }
    }
}


void MyMainWindow::savePreset( QString PresetName, QString Comment )
{
    if ( PresetName != "" )
    {
        SettingsManager::instance()->savePreset( PresetName, assembleCommand().join( " " ) );
        fillPresetPopup();
        loadPreset( PresetName );
        comboPresets->setCurrentIndex( comboPresets->findText( PresetName ) );
    }
    if ( Comment != "" )
    {
        SettingsManager::instance()->saveComment( PresetName, Comment );
    }
}


void MyMainWindow::deletePreset()
{
    // This function deletes the currently loaded preset from the config file
    if ( !QMessageBox::information
            (this,
             tr( "About to delete preset" ),
             tr( "This will delete the current preset, and there is no undo." ),
             tr( "&OK" ),
             tr( "&Cancel" ),
             0, 1 )
       )
    {
        SettingsManager::instance()->deletePreset( comboPresets->currentText() );
        wipeData();
        fillPresetPopup();
        // Set the combo to the special entry 0 ("Last used settings")
        comboPresets->setCurrentIndex( 0 );
        // Load last used settings
        loadPreset( comboPresets->itemText( 0 ) );
    }
}


void MyMainWindow::processData()
{
    // Maybe I should disable the process button in case the executable wasn't found;
    // But this way an error message will appear, which I find more useful
    QStringList ArgumentList = assembleCommand();

    // The first element contains the path to gpsbabel and was written
    // to the class attribute GpsBabelPath by assembleCommand()
    // It needs to be removed from the argument list
    ArgumentList.removeAt( 0 );

    //Preprocessing for GPSBook.
    int idx = 0;
    while ( ( idx = ArgumentList.indexOf("-f", idx) ) != -1)
    {
        if (ArgumentList.at(++idx) == "gpsbook")
        {
            //Send signal to save current trace into a temp file

            //Modify argument with the temp filename
            ArgumentList.removeAt(idx);
            ArgumentList.insert(idx,mGPSData->filename);
        }
    }

    idx = 0;
     while ( ( idx = ArgumentList.indexOf("-F", idx) ) != -1)
    {
        if (ArgumentList.at(++idx) == "gpsbook")
        {
            //Modify argument with the temp filename
            ArgumentList.removeAt(idx);
            ArgumentList.insert(idx,gpsbookOutputFile->fileName());
        }
    }

    qCritical() << ArgumentList;

    // Using a QProcess to determine if gpsbabel can be run successfully
    // ExecGpsBabel is *not* used here as its error() signal will trigger an alert dialog
    QProcess SearchGpsbabel;
    QString ExecutablePath = searchGpsBabel();

    SearchGpsbabel.start( ExecutablePath );
    int Timeout = 500;
    if ( !SearchGpsbabel.waitForStarted( Timeout ) )
    {
        SearchGpsbabel.waitForFinished( Timeout );
        QMessageBox ErrorDisplay;
        QString ErrorMessage = "";
        ErrorMessage.append( tr( "The file \"%1\" cannot be executed.\n\nPlease enter the complete path to it in the command line or the preferences." ).arg( GpsBabelPath ) );
        ErrorDisplay.setText( ErrorMessage );
        if ( ErrorDisplay.exec() )
        {
            return;
        }
    }

    // SearchGpsbabel has done its job
    SearchGpsbabel.waitForFinished( Timeout );

    // Finally running gpsbabel.
    // Managing its status is done by catching its signals error(), started() and finished()
    ExecGpsBabel->start( ExecutablePath, ArgumentList );
}


void MyMainWindow::processingStarted()
{
    ProgressDialog->setLabelText( tr( "Please wait while gpsbabel is processing data..." ) );
    ProgressDialog->setMinimum( 0 );
    ProgressDialog->setMaximum( 0 );
    ProgressDialog->exec();
    ProgressDialog->setValue( 0 );
}


void MyMainWindow::processingStopped()
{
    ProgressDialog->hide();

    // Returning if no error occured
    if ( ExecGpsBabel->exitCode() == 0 )
    {
        setStatusbarText( tr( "The operation was successfully finished" ), 20000 );
        //Postprocessing for GPSBook
        QStringList ArgumentList = assembleCommand();

        int idx = 0;
        while ( ( idx = ArgumentList.indexOf("-F", idx) ) != -1)
        {
            if (ArgumentList.at(++idx) == "gpsbook")
            {
                //Ask GPSBook to load the tmp file
                mGPSData->blockSignals(true);
                QString strtmp = mGPSData->filename;
                qobject_cast<DisplayGPSBabel*>(pluginOwner)->loadFile(gpsbookOutputFile->fileName());
                mGPSData->filename = strtmp;
                mGPSData->blockSignals(false);
                mGPSData->setGPXModified(true);
            }
        }
        return;
    }

    setStatusbarText( tr( "The operation failed" ), 20000 );

    // The config file even contains some i18n'ed error message strings
    QMessageBox ErrorDisplay;
    QString GpsBabelErrorMessage = ExecGpsBabel->readAllStandardError();
    QString ErrorMessage = "";
    ErrorMessage.append( tr( "A problem was detected by gpsbabel." ) );
    ErrorMessage.append( "\n" );
    ErrorMessage.append( SettingsManager::instance()->humanReadableGpsBabelError( GpsBabelErrorMessage ) );
    ErrorMessage.append( "\n\n" );
    ErrorMessage.append( tr( "The original error message reported by gpsbabel was:" ) );
    ErrorMessage.append( "\n" );
    ErrorMessage.append( GpsBabelErrorMessage );
    ErrorDisplay.setText( ErrorMessage );
    if ( ErrorDisplay.exec() )
    {
        return;
    }
}


void MyMainWindow::externalCallFailed()
{
    // Attention: This function even gets invoked if the user pressed cancel during processing
    // Maybe I should remove output files from disk automatically, but better save than sorry
    QMessageBox ErrorDisplay;
    QString ErrorMessage = tr( "gpsbabel was unexpectedly quit during its operation.\nCreated files should not get used but deleted." );
    ErrorDisplay.setText( ErrorMessage );
    if ( ErrorDisplay.exec() )
    {
        return;
    }
}


void MyMainWindow::checkUsageCount()
{
    // I wanted to create a dialog which asks the user for feedback.
    // But what we learned is that the users don't want to provide feedback
    // Thus this function still is here but I doubt it will be resurrected
    // after he used the application a certain amount of times
    // int count = SettingsManager::instance()->incrementUsageCount();
    /*
    // SettingsManager::instance()->disableUsageCount();
    if ( count > 0 && count % 2 == 0 )
    	{
    		cerr << "Gebabbel was used an equal amount of times" << endl;
    	}
    else if ( count > 0 && count % 2 != 0 )
    	{
    		cerr << "Gebabbel was used an unequal amount of times" << endl;
    	}
    else if ( count == 0 )
    	{
    		cerr << "UsageCounter is disabled" << endl;
    	}
    */
}


QString MyMainWindow::searchGpsBabel()
{

    // QProcess segfaults if it gets passed an empty string
    // Hopefully this won't happen :)
    //
    QProcess SearchGpsbabel;
    int Timeout = 100;

    QStringList PossiblePaths;
#ifdef Q_OS_WIN
    PossiblePaths.append( "c:\\progra~1\\gpsbabel\\gpsbabel.exe" );
#endif
    PossiblePaths.append( GpsBabelPath );
    // The path in the preferences gets used if the executable as specified in the command line was not found
    PossiblePaths.append( SettingsManager::instance()->pathGpsBabel() );
    qDebug() << "toto --> " << PossiblePaths;

/*#ifndef Q_OS_LINUX
    for ( int i = 0; i < PossiblePaths.size(); i++ )
    {
        QFileInfo PathInfo;
        PathInfo.setFile( PossiblePaths.at( i ) );
        if ( PathInfo.isRelative() )
        {
            QString TempString;
            TempString = PossiblePaths.at( i );
            TempString.prepend( "/" );
            TempString.prepend( QCoreApplication::applicationDirPath() );
            PossiblePaths.replace( i, TempString );
        }
    }
#endif */


    for ( int i = 0; i < PossiblePaths.size(); i++ )
    {
        SearchGpsbabel.start( PossiblePaths.at( i ) );
        if ( SearchGpsbabel.waitForStarted( Timeout ) )
        {
            SearchGpsbabel.waitForFinished( Timeout );
            return PossiblePaths.at( i );
        }
    }

    // If all of the above paths failed, return GpsBabelPath as specified by the user, even that it wasn't found
    return GpsBabelPath;
}


QStringList MyMainWindow::assembleCommand()
{
    QStringList CommandElements;

    // Ensuring GpsBabelPath is never empty, otherwise qprocess will crash
    // In case the user starts from scratch, it will be empty and therefore get filled here
    // TODO: Check if it makes sense to use the preference path instead of a static string
    if ( GpsBabelPath == "" )
    {
        GpsBabelPath = "gpsbabel";
#ifdef Q_OS_WIN
        GpsBabelPath.append( ".exe" );
#endif
    }

    // GpsBabelPath now shouldn't be empty
    CommandElements.append( GpsBabelPath );

    // If the user has a gpsbabel configuration file installed, it shouldn't get read
    // See http://www.gpsbabel.org/htmldoc-development/inifile.html for details
    CommandElements.append( "-p" );
    // Yes, an empty qstringlist element to pass an "empty file" to the -p option
    CommandElements.append( "" );

    // The realtime tracking option -T is exclusive to -wrtsN
    if ( RealtimeTracking )
    {
        CommandElements.append( "-T" );
    }
    else
    {
        if ( ProcessWaypoints )
        {
            CommandElements.append( "-w" );
        }
        if ( ProcessRoutes )
        {
            CommandElements.append( "-r" );
        }
        if ( ProcessTracks )
        {
            CommandElements.append( "-t" );
        }
        if ( SynthesizeShortnames )
        {
            CommandElements.append( "-Sn" );
        }
        if ( SmartIcons )
        {
            CommandElements.append( "-Si" );
        }
    }


    QStringList Items;
    for ( int i = 0; i < InputItems->count(); i++ )
    {
        CommandElements.append( "-i" );
        Items = InputItems->at( i )->content();
        for ( int i = 0; i < Items.count(); i++ )
        {
            CommandElements.append( Items.at( i ) );
        }
    }

    for ( int i = 0; i < FilterItems->count(); i++ )
    {
        CommandElements.append( "-x" );
        Items = FilterItems->at( i )->content();
        for ( int i = 0; i < Items.count(); i++ )
        {
            CommandElements.append( Items.at( i ) );
        }
    }

    for ( int i = 0; i < OutputItems->count(); i++ )
    {
        CommandElements.append( "-o" );
        Items = OutputItems->at( i )->content();
        for ( int i = 0; i < Items.count(); i++ )
        {
            CommandElements.append( Items.at( i ) );
        }
    }
    return CommandElements;
}


void MyMainWindow::editCommand()
{
    QString NewCommand = assembleCommand().join( " " );

    // For displaying the command to the user, the -p option should be removed to avoid confusion
    NewCommand = NewCommand.remove( "-p  " );

    bool OKpressed = false;
    NewCommand = QInputDialog::getText( this, tr("Command Editor"), tr("The current command looks as follows. Please use the syntax\ngpsbabel -w -r -t -Sn -Si -T -i intype,inoptions -f infile.txt -x filtertype,filteroptions -o outtype,outoptions -F outfile.txt:"), QLineEdit::Normal, NewCommand, &OKpressed);
    if ( OKpressed == true )
    {
        splitData( NewCommand );
    }
}


void MyMainWindow::fillPresetPopup()
{
    // This function fills the preset popup with all pressets that exist in the config file
    QStringList PresetNames = SettingsManager::instance()->presetNames();
    // "Last Used Settings" is a special entry which contains the last used settings
    PresetNames.prepend( tr( "Last Used Settings" ) );
    comboPresets->clear();
    comboPresets->addItems( PresetNames );
    finetuneGui();
}


void MyMainWindow::loadPreset( QString PresetName )
{
    // Load the preset, either as selected by the user or last used settings during startup.
    // Even after deleting a preset, the combo's special entry at( 0 ) will get autoselected
    // and trigger to load the special preset "last used settings"
    QString Command = SettingsManager::instance()->preset( PresetName );
    QString Comment = SettingsManager::instance()->comment( PresetName );

    setStatusbarText( Comment, 20000 );
    splitData( Command );
    // This isn't necessarily needed as the combo itself keeps the last selected item
    comboPresets->setCurrentIndex( comboPresets->findText( PresetName ) );

    // Ensure all items are completely visible
    // Will lead to a horizontal scrollbar in case of long options
    InputList->resizeColumnToContents( 0 );
    InputList->resizeColumnToContents( 1 );
    OutputList->resizeColumnToContents( 0 );
    OutputList->resizeColumnToContents( 1 );
    FilterList->resizeColumnToContents( 0 );
    FilterList->resizeColumnToContents( 1 );

}


void MyMainWindow::splitData( QString Command )
{
    // Some very basic error checking :)
    if ( Command == "" )
    {
        return;
    }

    // Unloading all currently loaded data
    wipeData();

    // Removing whitespace at the beginning and the end
    Command = Command.trimmed();

    // Using a QRegExp to find separators
    // First looking for separators which can follow directly after the executable name to separate it
    QRegExp Separators;

    // The options -Si and -Sn replace -s and -N since gpsbabel-1.3.3
    Separators.setPattern( " -[pwrtTixoS][in]? " );

    // Command must start with the path to the gpsbabel executable,
    // which can be an absolute path or only consist of the executable name
    // GpsBabelPath is a QString class member
    GpsBabelPath = Command.split( Separators ).at( 0 );
    // Removing the executable path from Command
    Command = Command.remove( GpsBabelPath );

    // Removing whitespace at the beginning and the end
    GpsBabelPath = GpsBabelPath.trimmed();

    // Attention: *Never* trim "Command" here, because it needs a leading blank for the separators to work properly!
    // Command ! Command.trimmed();

    // The option -p "" causes gpsbabel to not read any configuration file
    // As this gets appended statically in assembleCommand(), dropping it here if it exists
    if( Command.contains( " -p " ) )
    {
        Command = Command.remove( " -p" );
    }
    if( Command.contains( " \"\" " ) )
    {
        Command = Command.remove( " \"\"" );
    }

    // Extracting the switches -wrtTsN
    if( Command.contains( " -w " ) )
    {
        ProcessWaypoints = true;
        Command = Command.remove( " -w" );
    }
    if( Command.contains( " -r " ) )
    {
        ProcessRoutes = true;
        Command = Command.remove( " -r" );
    }
    if( Command.contains( " -t " ) )
    {
        ProcessTracks = true;
        Command = Command.remove( " -t" );
    }
    if( Command.contains( " -T " ) )
    {
        RealtimeTracking = true;
        Command = Command.remove( " -T" );
    }

    // Since gpsbabel 1.3.3, -Sn and -Si replace -s and -N to adapt names and icons
    if( Command.contains( " -Sn " ) )
    {
        SynthesizeShortnames = true;
        Command = Command.remove( " -Sn" );
    }
    if( Command.contains( " -Si " ) )
    {
        SmartIcons = true;
        Command = Command.remove( " -Si" );
    }


    // Preparing to split inputs, filters and outputs
    Separators.setPattern( " -[ixo] " );

    QStringList Inputs;
    QStringList Outputs;
    QStringList Filters;

    // I'd prefer using split(), but split removes the separators so there's no chance
    // to determine by 100% which elements are inputs, filters or outputs
    while ( Command.lastIndexOf( " -o " ) != -1 )
    {
        Outputs.prepend( Command.right( Command.size() - Command.lastIndexOf( " -o " ) ) );
        Command.remove( Outputs.at( 0 ) );
    }

    while ( Command.lastIndexOf( " -x " ) != -1 )
    {
        Filters.prepend( Command.right( Command.size() - Command.lastIndexOf( " -x " ) ) );
        Command.remove( Filters.at( 0 ) );
    }

    while ( Command.lastIndexOf( " -i " ) != -1 )
    {
        Inputs.prepend( Command.right( Command.size() - Command.lastIndexOf( " -i " ) ) );
        Command.remove( Inputs.at( 0 ) );
    }
    Command.clear();

    // Creating the items
    for ( int i = 0; i < Inputs.count(); i++ )
    {
        TempListItem = new ListItem( InputList );
        TempListItem->setup( Inputs.at( i ), ExecGpsBabel->workingDirectory() );
        InputItems->append( TempListItem );
    }
    for ( int i = 0; i < Outputs.count(); i++ )
    {
        TempListItem = new ListItem( OutputList );
        TempListItem->setup( Outputs.at( i ), ExecGpsBabel->workingDirectory() );
        OutputItems->append( TempListItem );
    }

    for ( int i = 0; i < Filters.count(); i++ )
    {
        TempListItem = new ListItem( FilterList );
        TempListItem->setup( Filters.at( i ), ExecGpsBabel->workingDirectory() );
        FilterItems->append( TempListItem );
    }
    finetuneGui();
}


void MyMainWindow::wipeData()
{
    GpsBabelPath = "";

    for ( int i = 0; i < InputItems->count(); i++ )
    {
        delete InputItems->at( i );
    }
    InputItems->clear();

    for ( int i = 0; i < OutputItems->count(); i++ )
    {
        delete OutputItems->at( i );
    }
    OutputItems->clear();

    for ( int i = 0; i < FilterItems->count(); i++ )
    {
        delete FilterItems->at( i );
    }
    FilterItems->clear();

    ProcessWaypoints = false;
    ProcessRoutes = false;
    ProcessTracks = false;
    RealtimeTracking = false;
    SynthesizeShortnames = false;
    SmartIcons = false;

    finetuneGui();
}



void MyMainWindow::finetuneGui()
{
    // I had the idea to display gpsbabel version of the current data in the GUI
    // Currently this is postponed due to laziness :)

    // The special entry at 0 must not be deleted
    if( comboPresets->currentIndex() == 0 )
    {
        ActionFileDeletePreset->setDisabled( true );
    }
    else
    {
        ActionFileDeletePreset->setEnabled( true );
    }


    if( ProcessWaypoints )
    {
        CheckboxWaypoints->setChecked( true );
    }
    else
    {
        CheckboxWaypoints->setChecked( false );
    }
    if( ProcessRoutes )
    {
        CheckboxRoutes->setChecked( true );
    }
    else
    {
        CheckboxRoutes->setChecked( false );
    }
    if( ProcessTracks )
    {
        CheckboxTracks->setChecked( true );
    }
    else
    {
        CheckboxTracks->setChecked( false );
    }
    if( RealtimeTracking )
    {
        CheckboxRealtimeTracking->setChecked( true );
    }
    else
    {
        CheckboxRealtimeTracking->setChecked( false );
    }
    if( SynthesizeShortnames )
    {
        CheckboxShortnames->setChecked( true );
    }
    else
    {
        CheckboxShortnames->setChecked( false );
    }
    if( SmartIcons )
    {
        CheckboxSmartIcons->setChecked( true );
    }
    else
    {
        CheckboxSmartIcons->setChecked( false );
    }

    // The option -T is exclusive, so doing some Gui magic to visualize this to the user
    if	( CheckboxRealtimeTracking->checkState() == Qt::Checked )
    {
        CheckboxWaypoints->setEnabled( false );
        CheckboxRoutes->setEnabled( false );
        CheckboxTracks->setEnabled( false );
        CheckboxShortnames->setEnabled( false );
        CheckboxSmartIcons->setEnabled( false );
    }
    else
    {
        CheckboxWaypoints->setEnabled( true );
        CheckboxRoutes->setEnabled( true );
        CheckboxTracks->setEnabled( true );
        CheckboxShortnames->setEnabled( true );
        CheckboxSmartIcons->setEnabled( true );
    }

    setProcessButton();
}


void MyMainWindow::setProcessButton()
{
    // This function does some rudimentary checks and disables the process button if necessary
    // TODO: In case of USB inputs, don't disable the process button
    BtnProcess->setEnabled( true );

    // If there is no input or output at all, the button should remain disabled
    if ( InputItems->count() == 0 || OutputItems->count() == 0 )
    {
        return;
    }

    // ListItems provide isAlright() to check if something is wrong
    for ( int i = 0; i < InputItems->count(); i++ )
    {
        if ( InputItems->at( i )->isAlright() == false )
        {
            return;
        }
    }
    for ( int i = 0; i < OutputItems->count(); i++ )
    {
        if ( OutputItems->at( i )->isAlright() == false )
        {
            return;
        }
    }
    for ( int i = 0; i < FilterItems->count(); i++ )
    {
        if ( FilterItems->at( i )->isAlright() == false )
        {
            return;
        }
    }

    // One of the checks should be set. Actually if none was checked, the -w option gets silently used by GPSbabel
    // But as this seems to be a bit confusing to the user, the GUI will force him to set one
    if	(
        CheckboxWaypoints->checkState() == Qt::Unchecked &&
        CheckboxRoutes->checkState() == Qt::Unchecked &&
        CheckboxTracks->checkState() == Qt::Unchecked &&
        CheckboxRealtimeTracking->checkState() == Qt::Unchecked
    )
    {
        return;
    }

    // As all checks have been passed successfully, it seems that processing is possible
    BtnProcess->setEnabled( true );
}


void MyMainWindow::editPreferences()
{
    PreferencesEditWindow->getSettings();
    PreferencesEditWindow->show();
}


// Three helper methods to catch the signal sent by Qt
// I never use the passed parameters, thus the compiler comlains...
void MyMainWindow::doubleClickedInput( QTreeWidgetItem *, int )
{
    AddOrEditItem( "Input" );
}


void MyMainWindow::doubleClickedOutput( QTreeWidgetItem *, int )
{
    AddOrEditItem( "Output" );
}


void MyMainWindow::doubleClickedFilter( QTreeWidgetItem *, int )
{
    AddOrEditItem( "Filter" );
}




void MyMainWindow::AddOrEditInput()
{
    AddOrEditItem( "Input" );
}


void MyMainWindow::AddOrEditOutput()
{
    AddOrEditItem( "Output" );
}


void MyMainWindow::AddOrEditFilter()
{
    AddOrEditItem( "Filter" );
}




void MyMainWindow::AddOrEditItem( QString DataType )
{
    // Creating a new edit window.
    MyIOConfigWindow IOConfigWindow( this );
    IOConfigWindow.setDataType( DataType );

    // Helper variables
    TempListItem = 0L;
    ActiveItem = 0L;
    if ( DataType == "Input" )
    {
        CurrentItems = InputItems;
        CurrentList = InputList;
        IOConfigWindow.setWindowTitle( tr( "Configure Input" ) );
    }
    if ( DataType == "Output" )
    {
        CurrentItems = OutputItems;
        CurrentList = OutputList;
        IOConfigWindow.setWindowTitle( tr( "Configure Output" ) );
    }
    if ( DataType == "Filter" )
    {
        CurrentItems = FilterItems;
        CurrentList = FilterList;
        IOConfigWindow.setWindowTitle( tr( "Configure Filter" ) );
    }


    // This function is used for adding and editing items.
    // So first determining if the user wants to add or edit
    // If there's no active item in the lists, it's adding
    // Otherwise edit the active item
    bool Editing = false;
    if ( CurrentList->selectedItems().count() > 0 )
    {
        Editing = true;

        // If there is more than one item, the first one gets edited
        // I'd prefer to set the list to single selection, but then it's not possible to deselect an item
        // So currently the user can select more than one item by shift or ctrl clicking
        ActiveItem = CurrentList->selectedItems().at( 0 );

        // Determining the data object counterpart of ActiveItem
        for ( int i = 0; i < CurrentItems->count(); i++ )
        {
            if ( CurrentItems->at( i )->tellListViewItem() == ActiveItem )
            {
                TempListItem = CurrentItems->at( i );
            }
        }

        IOConfigWindow.setData( TempListItem->tellType(), TempListItem->tellOptions(), TempListItem->tellCharSet(), TempListItem->tellValue() );
    }
    else
    {
        Editing = false;
    }

    // Showing the dialog and closing it if the user discards it
    if( IOConfigWindow.exec() == false )
    {
        return;
    }

    // Return if there is no text in one of the edits.
    // In case of inputs or outputs, both need to contain text
    // In case of a filter, only type must contain text
    if( DataType != "Filter" )
    {
        if ( IOConfigWindow.Type() == "" || IOConfigWindow.Value() == "" )
        {
            return;
        }
    }

    if( DataType == "Filter" )
    {
        if ( IOConfigWindow.Type() == "" )
        {
            return;
        }
    }

    if ( TempListItem == 0L )
    {
        TempListItem = new ListItem( CurrentList );
        CurrentItems->append( TempListItem );
    }

    // An item gets setup by passing it its complete data as a string and the gpsbabel working dir
    TempListItem->setup( IOConfigWindow.data(), ExecGpsBabel->workingDirectory() );
    TempListItem = 0L;

    // This call seems to be unnecessary, but it helps debugging the program as the data always passes the command splitter
    // Thus errors will be immediately visible in the GUI
    // splitData() will first wipeData() and then reload it
    splitData( assembleCommand().join( " " ) );
}


// The following three functions only are helper functions to catch the three QActions
// and pass them to removeItem()
void MyMainWindow::removeInput()
{
    removeItem( "Input" );
}


void MyMainWindow::removeOutput()
{
    removeItem( "Output" );
}


void MyMainWindow::removeFilter()
{
    removeItem( "Filter" );
}


void MyMainWindow::removeItem( QString DataType )
{

    if ( DataType == "Input" )
    {
        CurrentItems = InputItems;
        CurrentList = InputList;
    }
    if ( DataType == "Output" )
    {
        CurrentItems = OutputItems;
        CurrentList = OutputList;
    }
    if ( DataType == "Filter" )
    {
        CurrentItems = FilterItems;
        CurrentList = FilterList;
    }

    ActiveItem = CurrentList->currentItem();
    for (int i = 0; i < CurrentItems->count(); i++ )
    {
        if ( CurrentItems->at( i )->tellListViewItem() == ActiveItem )
        {
            delete CurrentItems->at( i );
            CurrentItems->removeAt( i );
        }
    }

    finetuneGui();
}


void MyMainWindow::showInputContextMenu( QPoint Position )
{
    // Due to the signal/slot concept, this function needs to take a QPoint
    // Actually, it contains global screen coordinates, and unaccording to the Qt docs,
    // I didn't manage to translate it to the expected results by using mapFromGlobal() etc.
    // So, finally, the next line is a dirty hack to avoid compiler warnings :) :
    Position = QCursor::pos();

    // Disabling menu entries.
    if ( InputList->selectedItems().count() > 0 )
    {
        ActionInputAdd->setEnabled( false );
        ActionInputEdit->setEnabled( true );
        ActionInputRemove->setEnabled( true );
    }
    else
    {
        ActionInputAdd->setEnabled( true );
        ActionInputEdit->setEnabled( false );
        ActionInputRemove->setEnabled( false );
    }
    InputContextMenu->exec( Position );
}


void MyMainWindow::showOutputContextMenu( QPoint Position )
{
    Position = QCursor::pos();
    if ( OutputList->selectedItems().count() > 0 )
    {
        ActionOutputAdd->setEnabled( false );
        ActionOutputEdit->setEnabled( true );
        ActionOutputRemove->setEnabled( true );
    }
    else
    {
        ActionOutputAdd->setEnabled( true );
        ActionOutputEdit->setEnabled( false );
        ActionOutputRemove->setEnabled( false );
    }
    OutputContextMenu->exec( Position );
}


void MyMainWindow::showFilterContextMenu( QPoint Position )
{
    Position = QCursor::pos();
    if ( FilterList->selectedItems().count() > 0 )
    {
        ActionFilterAdd->setEnabled( false );
        ActionFilterEdit->setEnabled( true );
        ActionFilterRemove->setEnabled( true );
    }
    else
    {
        ActionFilterAdd->setEnabled( true );
        ActionFilterEdit->setEnabled( false );
        ActionFilterRemove->setEnabled( false );
    }
    FilterContextMenu->exec( Position );
}


void MyMainWindow::showProgress()
{
    cerr << "gpsbabel is running..." << endl;
}


// Needed because the QCheckBox emits int but bool
void MyMainWindow::setProcessWaypoints( int ProcessWPs )
{
    if ( ProcessWPs == 2 )
    {
        ProcessWaypoints = true;
    }
    else
    {
        ProcessWaypoints = false;
    }
}


void MyMainWindow::setProcessRoutes( int ProcessRTs )
{
    if ( ProcessRTs == 2 )
    {
        ProcessRoutes = true;
    }
    else
    {
        ProcessRoutes = false;
    }
}


void MyMainWindow::setProcessTracks( int ProcessTKs )
{
    if ( ProcessTKs == 2 )
    {
        ProcessTracks = true;
    }
    else
    {
        ProcessTracks = false;
    }
}


void MyMainWindow::setRealtimeTracking( int RealtimeTrkg )
{
    if ( RealtimeTrkg == 2 )
    {
        RealtimeTracking = true;
    }
    else
    {
        RealtimeTracking = false;
    }
}


void MyMainWindow::setSynthesizeShortnames( int SynthShorts )
{
    if ( SynthShorts == 2 )
    {
        SynthesizeShortnames = true;
    }
    else
    {
        SynthesizeShortnames = false;
    }
}


void MyMainWindow::setSmartIcons( int SmIcons )
{
    if ( SmIcons == 2 )
    {
        SmartIcons = true;
    }
    else
    {
        SmartIcons = false;
    }
}


/*
void MyMainWindow::saveAppGeometry()
	{
		// See the Qt4 docs: Window Geometry
		// Doesn't save anything if connected to aboutToClose() :(
		SettingsManager::instance()->saveWindowGeometry( this->pos(), this->size() );
	}


void MyMainWindow::setAppGeometry()
	{
		// See the Qt4 docs: Window Geometry
		this->move( SettingsManager::instance()->AppPos() );
		this->resize( SettingsManager::instance()->AppSize() );
	}
*/

void MyMainWindow::checkForConfigs()
{
    // During application start, default config files should be created if they don't exist
    // The SystemConfig will automatically get recreated after an update, as the version number got appended to the file name
    // The user config will be kept after an update to preserve the user's settings (including presets)
    // The user config only will be recreated if the user requests to restore defaults via the preferences dialog
    QFileInfo FileInfoUserConfig( SettingsManager::instance()->UserConfigFileName() );
    QFileInfo FileInfoSystemConfig( SettingsManager::instance()->SystemConfigFileName() );
    if ( !FileInfoUserConfig.exists() )
    {
        SettingsManager::instance()->restoreUserDefaults();
    }
    if ( !FileInfoSystemConfig.exists() )
    {
        SettingsManager::instance()->restoreSystemDefaults();
    }
    fillPresetPopup();
}


void MyMainWindow::restoreDefaults()
{
    // Display a warning, but only if config files already exist
    QFileInfo FileInfoUserConfig( SettingsManager::instance()->UserConfigFileName() );
    QFileInfo FileInfoSystemConfig( SettingsManager::instance()->SystemConfigFileName() );

    if ( FileInfoUserConfig.exists() || FileInfoSystemConfig.exists() )
    {
        QMessageBox MessageBox( tr( "Restore Default Settings" ), tr( "Restoring default settings will reset all your current settings.\nDo you really want to continue?" ), QMessageBox::Question, QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape, QMessageBox::NoButton );
        if ( MessageBox.exec() == QMessageBox::No )
        {
            return;
        }
    }

    SettingsManager::instance()->restoreUserDefaults();
    SettingsManager::instance()->restoreSystemDefaults();

    // Refilling the preset combo with the new preset names
    fillPresetPopup();

    // In case this function was run from the preferences dialog, refresh its contents
    PreferencesEditWindow->getSettings();
    //setAppGeometry();
}




