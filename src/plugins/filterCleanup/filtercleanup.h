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
** Copyright (C) 2010, gpsbook-team
**
****************************************************************************/
#ifndef FILTERCLEANUP_H
#define FILTERCLEANUP_H

#include <QtGui>
#include <QObject>
#include <QIcon>
#include "filtercleanupdialog.h"
#include "filtercleanupoptionsframe.h"
#include "filtercleanuphelp.h"
#include "interface.h"
#include "gpsdata.h"

namespace Ui
{
    class FilterCleanupClass;
}

namespace PluginFilterCleanup {

    class FilterCleanup : public FilterPluginInterface
    {
        Q_OBJECT
        Q_INTERFACES(FilterPluginInterface)
    private:
        FilterCleanupOptionsFrame* mOptions;
        FilterCleanupHelp* mHelp;
    public:
        FilterCleanup();
        QString getName();
        QIcon getIcon();
        QWidget* getHelp();
        QWidget* getOptions();
        void init( QWidget* , GPSData* gpsdata );
        GPSData* mGPSData;
        QStringList getServiceList(){
            return mServicesList;
        }
        QStringList getServicePropertyList(const char * serviceName ){
            return mServicesPropertiesList[serviceName];
        }
        ServiceInterface* getService(const char * serviceName ) {
            return NULL;
        }

    public slots:
        void on_showPlugin();
        void on_about();
        void on_all_plugins_loaded(){}
    };
}

#endif // FILTERCLEANUP_H
