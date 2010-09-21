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
#include "displaystatisticoptionsframe.h"
#include "ui_displaystatisticoptionsframe.h"

namespace PluginDisplayStatistic {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DisplayStatisticOptionsFrame::DisplayStatisticOptionsFrame(QWidget *parent) :
        QFrame(parent),
        ui(new Ui::DisplayStatisticOptionsFrame)
    {
        ui->setupUi(this);
        QSettings settings("GPSBook","GPSBook");
        ui->checkBoxCatalogUpdateAcceleration->setChecked(settings.value("PluginDisplayStatistic_CatalogUpdateAcceleration",false).toBool() );
        ui->checkBoxCatalogUpdateDistance->setChecked(settings.value("PluginDisplayStatistic_CatalogUpdateDistance",false).toBool() ) ;
        ui->checkBoxCatalogUpdateSpeed->setChecked(settings.value("PluginDisplayStatistic_CatalogUpdateSpeed",true).toBool() );
        ui->checkBoxOtherUpdateAcceleration->setChecked(settings.value("PluginDisplayStatistic_OtherUpdateAcceleration",true).toBool() );
        ui->checkBoxOtherUpdateDistances->setChecked(settings.value("PluginDisplayStatistic_OtherUpdateDistances",true).toBool() );
        ui->checkBoxOtherUpdateSpeed->setChecked(settings.value("PluginDisplayStatistic_OtherUpdateSpeed",true).toBool() );
    } //DisplayTemplateOptionsFrame::DisplayTemplateOptionsFrame

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DisplayStatisticOptionsFrame::~DisplayStatisticOptionsFrame()
    {
        delete ui;
    } //DisplayTemplateOptionsFrame::~DisplayTemplateOptionsFrame

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayStatisticOptionsFrame::changeEvent(QEvent *e)
    {
        QFrame::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
        }
    } //DisplayTemplateOptionsFrame::changeEvent

}


void PluginDisplayStatistic::DisplayStatisticOptionsFrame::on_checkBoxCatalogUpdateDistance_toggled(bool)
{
    ui->buttonBox->setEnabled(true);
}

void PluginDisplayStatistic::DisplayStatisticOptionsFrame::on_checkBoxCatalogUpdateSpeed_toggled(bool)
{
    ui->buttonBox->setEnabled(true);
}

void PluginDisplayStatistic::DisplayStatisticOptionsFrame::on_checkBoxCatalogUpdateAcceleration_toggled(bool)
{
    ui->buttonBox->setEnabled(true);
}

void PluginDisplayStatistic::DisplayStatisticOptionsFrame::on_checkBoxOtherUpdateDistances_toggled(bool)
{
    ui->buttonBox->setEnabled(true);
}

void PluginDisplayStatistic::DisplayStatisticOptionsFrame::on_checkBoxOtherUpdateSpeed_toggled(bool)
{
    ui->buttonBox->setEnabled(true);
}

void PluginDisplayStatistic::DisplayStatisticOptionsFrame::on_checkBoxOtherUpdateAcceleration_toggled(bool)
{
    ui->buttonBox->setEnabled(true);
}

void PluginDisplayStatistic::DisplayStatisticOptionsFrame::on_buttonBox_clicked(QAbstractButton*)
{
    QSettings settings("GPSBook","GPSBook");
    settings.setValue("PluginDisplayStatistic_CatalogUpdateAcceleration", QVariant(ui->checkBoxCatalogUpdateAcceleration->checkState()));
    settings.setValue("PluginDisplayStatistic_CatalogUpdateDistance", QVariant(ui->checkBoxCatalogUpdateDistance->checkState()));
    settings.setValue("PluginDisplayStatistic_CatalogUpdateSpeed", QVariant(ui->checkBoxCatalogUpdateSpeed->checkState()));
    settings.setValue("PluginDisplayStatistic_OtherUpdateAcceleration",QVariant(ui->checkBoxOtherUpdateAcceleration->checkState()));
    settings.setValue("PluginDisplayStatistic_OtherUpdateDistances",QVariant(ui->checkBoxOtherUpdateDistances->checkState()));
    settings.setValue("PluginDisplayStatistic_OtherUpdateSpeed", QVariant(ui->checkBoxOtherUpdateSpeed->checkState()));
    ui->buttonBox->setEnabled(false);
}
