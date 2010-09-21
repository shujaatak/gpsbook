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
#ifndef DIALOGTRACKPROPERTY_H
#define DIALOGTRACKPROPERTY_H

#include <QtGui/QDialog>

namespace Ui {
    class DialogTrackProperty;
}

namespace GPSBook {

    class DialogTrackProperty : public QDialog {
        Q_OBJECT
        Q_DISABLE_COPY(DialogTrackProperty)
    public:
        explicit DialogTrackProperty(QWidget *parent = 0);
        virtual ~DialogTrackProperty();

        void setFileName(QString value);
        void setDisplayName(QString value);
        void setDescription(QString value);
        QString fileName();
        QString description();
        QString displayName();

    protected:
        virtual void changeEvent(QEvent *e);

    private:
        Ui::DialogTrackProperty *m_ui;

};
}

#endif // DIALOGTRACKPROPERTY_H
