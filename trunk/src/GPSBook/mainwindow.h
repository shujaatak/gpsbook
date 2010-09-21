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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QSplashScreen>
#include <QAbstractButton>
#include <QDir>
#include <QStringList>
#include <QSettings>
#include <QTreeWidgetItem>
#include <QModelIndex>
#include "interface.h"
#include "webproxy.h"
#include "QProgressIndicator.h"

using namespace ProgressIndicator;

namespace Ui
{
    class MainWindowClass;
}

class QActionGroup;

namespace GPSBook {

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();
        void setSplashScreen(QSplashScreen* splash);
        void init();

    protected:
        virtual void changeEvent(QEvent *e);

    private:
        Ui::MainWindowClass *ui;
        GPSData*            mGPSData;
        QActionGroup* actionGroup;
        QProgressIndicator* progressIndicator;
        QDir pluginsDir;
        QStringList pluginFileNames;
        QString openFilters;
        QString saveFilters;
        QList<InputOutputPluginInterface*> inputOutputPluginList;
        QList<DisplayPluginInterface*> displayPluginList;
        QDate prevDate, nextDate;
        QSplashScreen* splash;
        QMenu* filtersMenu;
        QMenu* aboutMenu;
        InputOutputPluginInterface* gpxPlugin;
        DisplayPluginInterface* visiblePlugin;
        WebProxy* webProxy;
        bool isCatalogVisible;
        QTreeWidgetItem* seletectItem;

        void loadPlugins();
    public slots:
         //void initComboBoxTracklist();
         void initCurrentGPXTreeview();

    private slots:
        void on_treeWidgetHelp_itemClicked(QTreeWidgetItem* item, int);
        void on_treeWidgetOptions_itemClicked(QTreeWidgetItem* item, int);
        void on_treeWidgetCurrentGPX_itemClicked(QTreeWidgetItem* item, int);
        void on_checkBoxToolbarVisibility_toggled(bool checked);
        void on_checkBoxMenuVisibility_toggled(bool checked);
        void on_comboBoxPreferedPlugin_currentIndexChanged(int);
        void on_checkBoxStartFullScreen_toggled(bool checked);
        void on_buttonBoxApply_clicked(QAbstractButton* );
        void on_lineEditStorageLocation_textChanged(QString );
        void on_comboBoxLanguageSelection_currentIndexChanged(int index);
        void on_actionFullScreen_toggled(bool );
        void on_actionClose_current_trace_triggered();
        void on_toolButtonAddToCatalog_clicked();
        void on_actionSaveAs_triggered();
        void on_actionOpen_triggered();
        void on_actionAbout_triggered();
        void on_actionSave_triggered();
        void on_toolButtonDelete_clicked();
        void on_treeWidgetCatalog_itemSelectionChanged();
        void on_treeWidgetCatalog_customContextMenuRequested(QPoint pos);
        void on_toolButtonNext_clicked();
        void on_toolButtonPrev_clicked();
        void on_calendarWidget_selectionChanged();
        void on_toolButtonHome_clicked();
        void on_treeWidgetContextMenuClicked();
        void on_treeWidgetCatalog_doubleClicked(QModelIndex index);
        void on_toolButtonAdd_clicked();
        void on_actionAboutQt_triggered();

        void displayPluginShow();
        void updateNavigationButtons();
        void updateSaveButton();
        void loadFile(QString filename, bool isFromCatalog);
        void saveFile(QString filename);
        //void trackSelectionEnable(bool enable);
        void trackSelectionEnable(bool GPXEnable, bool WaypointEnable, bool RouteEnable, bool Tracksenable, bool SegmentsEnable);

    signals:
        void signalSelectionChanged();
        void signalFileLoaded();

    };
}

#endif // MAINWINDOW_H
