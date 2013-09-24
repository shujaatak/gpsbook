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
** Copyright (c) 2010, gpsbook-team
**
****************************************************************************/
#ifndef IOGPXOPTIONSFRAME_H
#define IOGPXOPTIONSFRAME_H

#include <QtGlobal>
#if ( QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) )
#include <QtWidgets/QFrame>
#else
#include <QFrame>
#endif

namespace Ui {
    class IoGPXOptionsFrame;
}

class IoGPXOptionsFrame : public QFrame
{
    Q_OBJECT

public:
    explicit IoGPXOptionsFrame(QWidget *parent = 0);
    ~IoGPXOptionsFrame();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::IoGPXOptionsFrame *ui;
};

#endif // IOGPXOPTIONSFRAME_H
