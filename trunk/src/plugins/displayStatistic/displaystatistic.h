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
#ifndef VIEWSTAT_H
#define VIEWSTAT_H

#include <QObject>
#include "displaystatisticframe.h"
#include "interface.h"
#include "serviceprocessminmaxaltitude.h"
#include "serviceprocessminmaxspeed.h"
#include "serviceprocesswaypointacceleration.h"
#include "serviceprocesswaypointdistance.h"
#include "serviceprocesswaypointspeed.h"
#include "displaystatisticoptionsframe.h"
#include "displaystatistichelp.h"

namespace PluginDisplayStatistic {

class DisplayStatistic: public DisplayPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(DisplayPluginInterface)

    private:
        GPSData* gpsdata;
        bool processing;
        DisplayStatisticHelp* mHelp;
        DisplayStatisticFrame* displayStatisticFrame;
        DisplayStatisticOptionsFrame* displayStatisticOptionsFrame;
        ServiceProcessMinMaxAltitude* serviceProcessMinMaxAltitude;
        ServiceProcessMinMaxSpeed* serviceProcessMinMaxSpeed;
        ServiceProcessWaypointAcceleration* serviceProcessWaypointAcceleration;
        ServiceProcessWaypointDistance* serviceProcessWaypointDistance;
        ServiceProcessWaypointSpeed* serviceProcessWaypointSpeed;

    public:
        DisplayStatistic();

        QString getName();
        QWidget* getWidget();
        QIcon getIcon();
        QWidget* getHelp();
        QWidget* getOptions();
        void init(QWidget* , GPSData* gpsdata);
        void update();


    public slots:
        void on_gpsdataChanged();
        void on_selectionChanged();
        void on_fileLoaded();
        void on_about();
        void on_showPlugin();
        void on_hidePlugin() {}

    };
}

#endif // VIEWSTAT_H
