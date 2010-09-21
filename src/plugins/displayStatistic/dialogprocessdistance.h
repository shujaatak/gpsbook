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
#ifndef DIALOGPROCESSDISTANCE_H
#define DIALOGPROCESSDISTANCE_H

#include <QtGui/QDialog>
#include <math.h>
#include "gpsdata.h"

using namespace GPSBook;

namespace Ui {
    class DialogProcessDistance;
}

namespace PluginDisplayStatistic {

    class DialogProcessDistance : public QDialog {
        Q_OBJECT
    public:
        DialogProcessDistance(QWidget *parent = 0);
        ~DialogProcessDistance();
        void init(GPSData* gpsdata);

    protected:
        void changeEvent(QEvent *e);
        double calculateDistance(double lon1, double lat1, double lon2, double lat2);
        void updateDistance();

    private:
        GPSData* mGPSData;
        Ui::DialogProcessDistance *m_ui;

    private slots:
        void on_buttonBox_rejected();
        void on_buttonBox_accepted();
    };
}

#endif // DIALOGPROCESSDISTANCE_H
