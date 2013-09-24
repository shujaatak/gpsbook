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
#ifndef DIALOGPROCESSALTITUDE_H
#define DIALOGPROCESSALTITUDE_H

#include <QtGlobal>
#if ( QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) )
#include <QtWidgets/QDialog>
#else
#include <QDialog>
#endif
#include <gpsdata.h>
#if defined(Q_OS_LINUX)
#include "../displaySRTM/srtm.h"
#endif

using namespace GPSBook;

namespace Ui {
    class DialogProcessAltitude;
}

namespace PluginDisplayStatistic {

    class DialogProcessAltitude : public QDialog {
        Q_OBJECT
    public:
        DialogProcessAltitude(QWidget *parent = 0);
        ~DialogProcessAltitude();
        void init(GPSData* gpsdata);


    protected:
        void changeEvent(QEvent *e);
        void updateAltitude();

    private:
        Ui::DialogProcessAltitude *ui;
        GPSData* mGPSData;
#if defined(Q_OS_LINUX)
        SrtmDownloader* srtmDownloader;
#endif

    private slots:
        void on_buttonBox_rejected();
        void on_buttonBox_accepted();
    };
}

#endif // DIALOGPROCESSALTITUDE_H
