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
#ifndef DISPLAYINTERNETBROWSER_H
#define DISPLAYINTERNETBROWSER_H

#include "interface.h"
#include "displayinternetbrowseroptionsframe.h"
#include "displayinternetbrowserframe.h"
#include "displayinternetbrowserhelp.h"

namespace PluginDisplayInternetBrowser {

    class DisplayInternetBrowser: public DisplayPluginInterface
    {
            Q_OBJECT
            Q_INTERFACES(DisplayPluginInterface)

        private:
            bool firstDisplay;
            DisplayInternetBrowserOptionsFrame* mOptions;
            DisplayInternetBrowserFrame* mView;
            DisplayInternetBrowserHelp* mHelp;

        public:
            DisplayInternetBrowser();

            QString getName();
            QIcon getIcon();
            QWidget* getHelp();
            QWidget* getOptions();
            QWidget* getWidget();
            void init(QWidget* parent, GPSData* gpsdata);
            void update() {}
            void loadFile(QString filename);
            QStringList* getServiceList(){ return NULL; }
            ServiceInterface* getService(const char * serviceName,
                                         const char * serviceId) { return NULL; }
            QStringList* getServicePropertyList(const char * serviceName,
                                               const char * serviceId){ return NULL; }

        public slots:
            void on_gpsdataChanged();
            void on_selectionChanged() {}
            void on_fileLoaded();
            void on_about();
            void on_showPlugin();
            void on_hidePlugin() {}
            void on_all_plugins_loaded(){}

    };
}

#endif // DISPLAYINTERNETBROWSER_H
