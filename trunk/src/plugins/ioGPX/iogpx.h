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
#ifndef ioGPX_H
#define ioGPX_H

#include <QObject>
#include "interface.h"
#include "gpxreader.h"
#include "gpxwriter.h"
#include "iogpxhelp.h"
#include "iogpxoptionsframe.h"

namespace PluginIOGPX {

    class ioGPX : public InputOutputPluginInterface
    {
        Q_OBJECT
        Q_INTERFACES(InputOutputPluginInterface)

        private:
            GpxReader* gpxreader;
            GpxWriter* gpxwriter;
            IoGPXHelp* help;
            IoGPXOptionsFrame* mOptions;
        public:
            ioGPX();
            QString getName();
            QWidget* getHelp();
            QWidget* getOptions();
            QString getOpenFilter();
            QString getSaveFilter();
            QIcon getIcon();
            void open(QString filename, GPSData* gpsdata);
            void save(QString filename, GPSData* gpsdata);
            void init( QWidget* , GPSData* );
            void update() {}
            QStringList* getServiceList(){ return NULL; }
            ServiceInterface* getService(const char * serviceName,
                                         const char * serviceId) { return NULL; }
            QStringList* getServicePropertyList(const char * serviceName,
                                               const char * serviceId){ return NULL; }
        public slots:
            void on_about();
            void on_all_plugins_loaded(){}
    };
} //ioGPX

#endif // ioGPX_H
