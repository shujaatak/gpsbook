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
#ifndef FILTERCLEANUPDIALOG_H
#define FILTERCLEANUPDIALOG_H

#include <QtGui/QDialog>
#include <math.h>
#include "gpsdata.h"

using namespace GPSBook;

namespace Ui {
    class FilterCleanupDialog;
}

namespace PluginFilterCleanup {

    class FilterCleanupDialog : public QDialog {
        Q_OBJECT
        Q_DISABLE_COPY(FilterCleanupDialog)
    public:
        explicit FilterCleanupDialog(QWidget *parent = 0);
        virtual ~FilterCleanupDialog();
        GPSData* mGPSData;

    protected:
        virtual void changeEvent(QEvent *e);

    private:
        Ui::FilterCleanupDialog *m_ui;
        double calculateDistance(double lon1, double lat1, double lon2, double lat2);


    private slots:
        void on_toolButtonHigherOrLowerAltitude_clicked();
        void on_toolButtonRemoveLowerHigherAltitude_clicked();
        void on_toolButtonRemoveNegativeSpeed_clicked();
        void on_toolButtonRemoveRadius_clicked();
        void on_toolButtonRemoveHighAcceleration_clicked();
        void on_toolButtonRemoveHighSpeed_clicked();
        void on_toolButtonRemoveDesc_clicked();
    };
}

#endif // FILTERCLEANUPDIALOG_H
