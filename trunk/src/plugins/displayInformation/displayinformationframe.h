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
#ifndef DISPLAYINFORMATIONFRAME_H
#define DISPLAYINFORMATIONFRAME_H

#include <QtGui/QFrame>
#include <gpsdata.h>

namespace Ui {
    class DisplayInformationFrame;
}

using namespace GPSBook;

namespace PluginDisplayInformation {

    class DisplayInformationFrame : public QFrame {
        Q_OBJECT
        Q_DISABLE_COPY(DisplayInformationFrame)
    public:
        explicit DisplayInformationFrame(QWidget *parent = 0);
        virtual ~DisplayInformationFrame();
        void init(GPSData* gpsdata);
        void autoSetEnabled();
        void updateDisplay();

    protected:
        virtual void changeEvent(QEvent *e);

    private:
        Ui::DisplayInformationFrame *m_ui;
        GPSData* mGPSData;


    private slots:
        void on_lineEditWaypointSource_editingFinished();
        void on_lineEditWaypointDescription_editingFinished();
        void on_lineEditWaypointName_editingFinished();
        void on_lineEditRouteSource_editingFinished();
        void on_lineEditRouteDescription_editingFinished();
        void on_lineEditRouteName_cursorPositionChanged(int , int );
        void on_lineEditTrackSource_editingFinished();
        void on_lineEditTrackDescription_editingFinished();
        void on_lineEditTrackName_editingFinished();
        void on_lineEditMetaDataCopyrightLicense_editingFinished();
        void on_lineEditMetaDataLicenseYear_editingFinished();
        void on_lineEditMetaDataCopyrightOwner_editingFinished();
        void on_lineEditMetaDataEMailDomain_editingFinished();
        void on_lineEditMetaDataEMailId_editingFinished();
        void on_lineEditMetaDataAuthorName_editingFinished();
        void on_lineEditMetaDataKeywords_editingFinished();
        void on_dateTimeEditMetaDataDate_editingFinished();
        void on_lineEditMetaDataDescription_editingFinished();
        void on_lineEditMetaDataName_editingFinished();
    };
}

#endif // DISPLAYINFORMATIONFRAME_H
