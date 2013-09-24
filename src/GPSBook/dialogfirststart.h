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
#ifndef DIALOGFISRTSTART_H
#define DIALOGFIRSTSTART_H

#include <QtGlobal>
#if ( QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) )
#include <QtWidgets/QDialog>
#else
#include <QtGui/QDialog>
#endif



namespace Ui {
    class DialogFirstStart;
}

namespace GPSBook {

    class DialogFirstStart : public QDialog {
        Q_OBJECT
        Q_DISABLE_COPY(DialogFirstStart)
    public:
        explicit DialogFirstStart(QWidget *parent = 0);
        virtual ~DialogFirstStart();

    protected:
        virtual void changeEvent(QEvent *e);

    private:
        Ui::DialogFirstStart *m_ui;

     private slots:
        void on_buttonBox_accepted();
        void on_comboBoxLanguage_currentIndexChanged(int i);

    };
}

#endif // DIALOGPREFERENCE_H
