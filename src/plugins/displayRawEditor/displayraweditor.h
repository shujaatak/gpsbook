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
#ifndef DISPLAYRAWEDITOR_H
#define DISPLAYRAWEDITOR_H

#include "interface.h"
#include "displayraweditorhelp.h"
#include "displayraweditorframe.h"
#include "displayraweditoroptionsframe.h"

using namespace GPSBook;

namespace PluginDisplayRawEditor {

    class DisplayRawEditor: public DisplayPluginInterface
    {
        Q_OBJECT
        Q_INTERFACES(DisplayPluginInterface)

    private:
        DisplayRawEditorFrame*        mView;
        DisplayRawEditorHelp*         mHelp;
        DisplayRawEditorOptionsFrame* mOptions;
        GPSData*                      mGPSData;

    public:
        DisplayRawEditor();

        QString  getName();
        QWidget* getWidget();
        QIcon    getIcon();
        QWidget* getHelp();
        QWidget* getOptions();
        void init(QWidget* , GPSData* gpsdata);
        QStringList getServiceList(){
            return mServicesList;
        }
        QStringList getServicePropertyList(const char * serviceName ){
            return mServicesPropertiesList[serviceName];
        }
        ServiceInterface* getService(const char * /*serviceName*/ ) {
            return NULL;
        }

    public slots:
        void on_gpsdataGPXChanged(){}
        void on_gpsdataTracksChanged(){}
        void on_gpsdataRoutesChanged(){}
        void on_gpsdataWaypointsChanged(){}
        void on_selectionChanged(){}
        void on_fileLoaded();
        void on_about();
        void on_showPlugin();
        void on_hidePlugin(){}
        void on_all_plugins_loaded(){}

    };
}

#endif // DISPLAYRAWEDITOR_H
