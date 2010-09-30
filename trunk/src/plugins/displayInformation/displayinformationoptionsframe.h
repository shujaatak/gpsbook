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
#ifndef DISPLAYINFORMATIONOPTIONSFRAME_H
#define DISPLAYINFORMATIONOPTIONSFRAME_H

#include <QFrame>
#include <QSettings>
#include <QAbstractButton>

namespace Ui {
    class DisplayInformationOptionsFrame;
}

class DisplayInformationOptionsFrame : public QFrame
{
    Q_OBJECT

public:
    explicit DisplayInformationOptionsFrame(QWidget *parent = 0);
    ~DisplayInformationOptionsFrame();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DisplayInformationOptionsFrame *ui;
    QSettings* settings;

private slots:
    void on_buttonBox_clicked(QAbstractButton*);
    void modified();
};

#endif // DISPLAYINFORMATIONOPTIONSFRAME_H
