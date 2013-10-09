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
#ifndef VIEWGRAPH_H
#define VIEWGRAPH_H

#include <QObject>
#include "interface.h"
#include "displaygraphic2doptionsframe.h"
#include "displaygraphic2dhelp.h"
#include "displaygraphic2Dframe.h"

namespace PluginDisplayGraphic2D {

    class DisplayGraphic2D: public DisplayPluginInterface
    {
        Q_OBJECT
#if ( QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) )
        Q_PLUGIN_METADATA(IID "org.gpsbook.DisplayPluginInterface" FILE "graphic2D.json")
#endif
        Q_INTERFACES(DisplayPluginInterface)

    private:
        DisplayGraphic2DOptionsFrame* mOptions;
        DisplayGraphic2DHelp*         mHelp;
        DisplayGraphic2DFrame*        mView;
        GPSData*                      mGPSData;

    public:
        DisplayGraphic2D();

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
        void on_gpsdataGPXChanged();
        void on_gpsdataTracksChanged();
        void on_gpsdataRoutesChanged();
        void on_gpsdataWaypointsChanged(){}
        void on_selectionChanged();
        void on_fileLoaded();
        void on_about();
        void on_showPlugin();
        void on_hidePlugin() {}
        void on_all_plugins_loaded(){}

    };
}

#endif // VIEWGRAPH_H
