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
#ifndef DIALOGPROCESSSPEED_H
#define DIALOGPROCESSSPEED_H

#include <QtGui/QDialog>
#include "gpsdata.h"

using namespace GPSBook;

namespace Ui {
    class DialogProcessSpeed;
}

namespace PluginDisplayStatistic {

    class DialogProcessSpeed : public QDialog {
        Q_OBJECT
    public:
        DialogProcessSpeed(QWidget *parent = 0);
        ~DialogProcessSpeed();
        void init(GPSData* gpsdata);
        void updateSpeed();

    protected:
        void changeEvent(QEvent *e);

    private:
        Ui::DialogProcessSpeed *m_ui;
        GPSData* mGPSData;

    private slots:
        void on_buttonBox_rejected();
        void on_buttonBox_accepted();
    };
}

#endif // DIALOGPROCESSSPEED_H
