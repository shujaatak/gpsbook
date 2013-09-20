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
#include <QSettings>
#include "displayraweditoroptionsframe.h"
#include "ui_displayraweditoroptionsframe.h"

DisplayRawEditorOptionsFrame::DisplayRawEditorOptionsFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::DisplayRawEditorOptionsFrame)
{
    ui->setupUi(this);
    QSettings settings("GPSBook","GPSBook");
    ui->checkBoxShowCatalog->setChecked(settings.value("PluginRawEditorShowCatalog",false).toBool() );
}

DisplayRawEditorOptionsFrame::~DisplayRawEditorOptionsFrame()
{
    delete ui;
}

void DisplayRawEditorOptionsFrame::changeEvent(QEvent *e)
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

void DisplayRawEditorOptionsFrame::on_checkBoxShowCatalog_toggled(bool)
{
    QSettings settings("GPSBook","GPSBook");
    settings.setValue("pluginRawEditorShowCatalog",ui->checkBoxShowCatalog->isChecked());
}
