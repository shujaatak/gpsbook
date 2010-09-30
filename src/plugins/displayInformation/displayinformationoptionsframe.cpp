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
#include "displayinformationoptionsframe.h"
#include "ui_displayinformationoptionsframe.h"

DisplayInformationOptionsFrame::DisplayInformationOptionsFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::DisplayInformationOptionsFrame)
{
    ui->setupUi(this);
    settings = new QSettings("GPSBook","GPSBook");
    connect(ui->groupBoxAutoFill, SIGNAL(toggled(bool)), this, SLOT(modified()));
    ui->groupBoxAutoFill->setChecked(settings->value("displayInformationDefaultFilling",false).toBool());

    connect(ui->lineEditAuthorName, SIGNAL(editingFinished()), this, SLOT(modified()));
    ui->lineEditAuthorName->setText(settings->value("displayInformationDefaultAuthorName","").toString());

    connect(ui->lineEditEMailId, SIGNAL(editingFinished()), this, SLOT(modified()));
    ui->lineEditEMailId->setText(settings->value("displayInformationDefaultEMailId","").toString());

    connect(ui->lineEditEMailDomain, SIGNAL(editingFinished()), this, SLOT(modified()));
    ui->lineEditEMailDomain->setText(settings->value("displayInformationDefaultEMailDomain","").toString());

    connect(ui->lineEditOwner, SIGNAL(editingFinished()), this, SLOT(modified()));
    ui->lineEditOwner->setText(settings->value("displayInformationDefaultOwner","").toString());

    connect(ui->lineEditLicense, SIGNAL(editingFinished()), this, SLOT(modified()));
    ui->lineEditLicense->setText(settings->value("displayInformationDefaultLicense","").toString());
}

DisplayInformationOptionsFrame::~DisplayInformationOptionsFrame()
{
    delete settings;
    delete ui;
}

void DisplayInformationOptionsFrame::changeEvent(QEvent *e)
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

void DisplayInformationOptionsFrame::modified()
{
    ui->buttonBox->setEnabled(true);
}

void DisplayInformationOptionsFrame::on_buttonBox_clicked(QAbstractButton*)
{
    settings->setValue("displayInformationDefaultFilling",     ui->groupBoxAutoFill->isChecked());
    settings->setValue("displayInformationDefaultAuthorName",  ui->lineEditAuthorName->text());
    settings->setValue("displayInformationDefaultEMailId",     ui->lineEditEMailId->text());
    settings->setValue("displayInformationDefaultEMailDomain", ui->lineEditEMailDomain->text());
    settings->setValue("displayInformationDefaultOwner",       ui->lineEditOwner->text());
    settings->setValue("displayInformationDefaultLicense",     ui->lineEditLicense->text());
    ui->buttonBox->setEnabled(false);
}
