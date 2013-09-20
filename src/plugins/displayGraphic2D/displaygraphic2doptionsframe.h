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
#ifndef DISPLAYGRAPHIC2DOPTIONSFRAME_H
#define DISPLAYGRAPHIC2DOPTIONSFRAME_H

#include <QFrame>
#include <QSettings>

namespace Ui {
    class DisplayGraphic2DOptionsFrame;
}

class DisplayGraphic2DOptionsFrame : public QFrame
{
    Q_OBJECT

public:
    explicit DisplayGraphic2DOptionsFrame(QWidget *parent = 0);
    ~DisplayGraphic2DOptionsFrame();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DisplayGraphic2DOptionsFrame *ui;
    QSettings* settings;

private slots:
    void on_checkBoxFullfilDuration_toggled(bool);
    void on_checkBoxFullfilAbsoluteTime_toggled(bool);
    void on_checkBoxFullfilDistance_toggled(bool);
    void on_checkBoxFullfilPoints_toggled(bool);
    void on_checkBoxFullfilCumulDistance_toggled(bool checked);
    void on_checkBoxShowCatalog_toggled(bool checked);
};

#endif // DISPLAYGRAPHIC2DOPTIONSFRAME_H
