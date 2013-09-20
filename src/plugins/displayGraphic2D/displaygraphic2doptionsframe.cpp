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
#include "displaygraphic2doptionsframe.h"
#include "ui_displaygraphic2doptionsframe.h"

DisplayGraphic2DOptionsFrame::DisplayGraphic2DOptionsFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::DisplayGraphic2DOptionsFrame)
{
    ui->setupUi(this);
    settings = new QSettings("GPSBook","GPSBook");

    ui->checkBoxFullfilPoints->setChecked(settings->value("pluginGraph2DFullfilPoint",true).toBool());
    ui->checkBoxFullfilCumulDistance->setChecked(settings->value("pluginGraph2DFullfilCumulDistance",true).toBool());
    ui->checkBoxFullfilDistance->setChecked(settings->value("pluginGraph2DFullfilDistance",false).toBool());
    ui->checkBoxFullfilAbsoluteTime->setChecked(settings->value("pluginGraph2DFullfilAbsoluteTime",true).toBool());
    ui->checkBoxFullfilDuration->setChecked(settings->value("pluginGraph2DFullfilDuration",false).toBool());
    ui->checkBoxShowCatalog->setChecked(settings->value("pluginGraph2DShowCatalog",true).toBool());
}

DisplayGraphic2DOptionsFrame::~DisplayGraphic2DOptionsFrame()
{
    delete ui;
}

void DisplayGraphic2DOptionsFrame::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DisplayGraphic2DOptionsFrame::on_checkBoxFullfilPoints_toggled(bool)
{
    settings->setValue("pluginGraph2DFullfilPoint",ui->checkBoxFullfilPoints->isChecked());
}

void DisplayGraphic2DOptionsFrame::on_checkBoxFullfilDistance_toggled(bool)
{
    settings->setValue("pluginGraph2DFullfilDistance",ui->checkBoxFullfilDistance->isChecked());
}

void DisplayGraphic2DOptionsFrame::on_checkBoxFullfilAbsoluteTime_toggled(bool)
{
    settings->setValue("pluginGraph2DFullfilAbsoluteTime",ui->checkBoxFullfilAbsoluteTime->isChecked());
}

void DisplayGraphic2DOptionsFrame::on_checkBoxFullfilDuration_toggled(bool)
{
    settings->setValue("pluginGraph2DFullfilDuration",ui->checkBoxFullfilDuration->isChecked());
}

void DisplayGraphic2DOptionsFrame::on_checkBoxFullfilCumulDistance_toggled(bool)
{
    settings->setValue("pluginGraph2DFullfilCumulDistance",ui->checkBoxFullfilCumulDistance->isChecked());
}

void DisplayGraphic2DOptionsFrame::on_checkBoxShowCatalog_toggled(bool)
{
    settings->setValue("pluginGraph2DShowCatalog",ui->checkBoxShowCatalog->isChecked());
}
