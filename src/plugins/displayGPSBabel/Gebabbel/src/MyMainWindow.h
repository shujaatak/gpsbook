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

#ifndef MYMAINWINDOW_h
#define MYMAINWINDOW_h

#include <iostream>

#include <QSettings>
#include <QtGui>

#include <QStringList>

#include "ui_MainWindow.h"
#include "MyIOConfigWindow.h"
#include "MyFilterConfigWindow.h"
#include "MyPreferencesConfig.h"
#include "MySavePresetWindow.h"
#include "ListItem.h"
#include "SettingsManager.h"
#include "gpsdata.h"
#include "interface.h"

using namespace std;
using namespace GPSBook;

class MyMainWindow: public QFrame, private Ui::MainWindow
{
    Q_OBJECT

public:
    MyMainWindow( QWidget* parent = 0 );
    void init(GPSData* gpsdata);
    DisplayPluginInterface* pluginOwner;

private:
    GPSData* mGPSData;
    QTemporaryFile* gpsbookOutputFile;
    QTimer* timer;

    QProcess * ExecGpsBabel;
    MyPreferencesConfig * PreferencesEditWindow;
    QTreeWidgetItem * ActiveItem;

    QMenu * MenuFile;
    QMenu * MenuEdit;
    QMenu * MenuHelp;

    QMenu * InputContextMenu;
    QMenu * OutputContextMenu;
    QMenu * FilterContextMenu;

    QComboBox * comboPresets;

    QToolBar * MainToolBar;

    QMessageBox * AboutBox;
    QMessageBox * HelpBox;

    QProgressDialog * ProgressDialog;

    QString FilePath;
    QString GpsBabelPath;
    QStringList InputHeaders;
    QStringList OutputHeaders;
    QStringList FilterHeaders;

    QAction * ActionFileSavePreset;
    QAction * ActionFileDeletePreset;
    QAction * ActionFileProcess;
    QAction * ActionFileClear;
    QAction * ActionFileQuit;
    QAction * ActionEditEditCommand;
    QAction * ActionEditEditPresetFile;
    QAction * ActionEditEditPreferences;
    QAction * ActionHelpManual;
    QAction * ActionHelpAbout;
    QAction * ActionHelpWhatsThis;

    QAction * ActionInputAdd;
    QAction * ActionInputEdit;
    QAction * ActionInputRemove;

    QAction * ActionOutputAdd;
    QAction * ActionOutputEdit;
    QAction * ActionOutputRemove;

    QAction * ActionFilterAdd;
    QAction * ActionFilterEdit;
    QAction * ActionFilterRemove;

    ListItem * TempListItem;

    bool ProcessWaypoints;
    bool ProcessRoutes;
    bool ProcessTracks;
    bool RealtimeTracking;
    bool SynthesizeShortnames;
    bool SmartIcons;

    QList<ListItem * > * InputItems;
    QList<ListItem * > * OutputItems;
    QList<ListItem * > * FilterItems;
    QList<ListItem * > * CurrentItems;
    QTreeWidget * CurrentList;

    void setupMenus();
    void setupConnections();

signals:

public slots:
    void initGpsBabel();

    void displayAboutBox();
    void displayHelpDialog();
    void displayWarning( QString, QString );
    void setStatusbarText( QString, unsigned int );
    void displayComment( QString );
    void showProgress();

    void saveLastUsedSettings();
    void saveNamedPreset();
    void savePreset( QString, QString );
    void deletePreset();
    void processData();

    void fillPresetPopup();
    void loadPreset( QString );
    void splitData( QString );
    void wipeData();

    void finetuneGui();
    void setProcessButton();

    QString searchGpsBabel();
    QStringList assembleCommand();
    void editCommand();

    void showInputContextMenu( QPoint );
    void showOutputContextMenu( QPoint );
    void showFilterContextMenu( QPoint );

    void doubleClickedInput( QTreeWidgetItem *, int );
    void doubleClickedOutput( QTreeWidgetItem *, int );
    void doubleClickedFilter( QTreeWidgetItem *, int );

    void AddOrEditInput();
    void AddOrEditOutput();
    void AddOrEditFilter();
    void AddOrEditItem( QString );


    void removeInput();
    void removeOutput();
    void removeFilter();
    void removeItem( QString );

    void editPreferences();

    // QCheckBox emits int but bool
    void setProcessWaypoints( int );
    void setProcessRoutes( int );
    void setProcessTracks( int );
    void setRealtimeTracking( int );
    void setSynthesizeShortnames( int );
    void setSmartIcons( int );

    //void saveAppGeometry();
    //void setAppGeometry();

    void checkForConfigs();
    void restoreDefaults();

    void processingStarted();
    void processingStopped();
    void externalCallFailed();

    void checkUsageCount();

    void cleanInfo();
};

#endif

