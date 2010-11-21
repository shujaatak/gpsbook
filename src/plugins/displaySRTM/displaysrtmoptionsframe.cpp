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
#include "displaysrtmoptionsframe.h"
#include "ui_displaysrtmoptionsframe.h"
#include <QMessageBox>
#include <QSettings>
#include <QDir>

DisplaySRTMOptionsFrame::DisplaySRTMOptionsFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::DisplaySRTMOptionsFrame)
{
    ui->setupUi(this);
    updateTileDirList();
}

void DisplaySRTMOptionsFrame::updateTileDirList()
{
    QSettings settings("GPSBook","GPSBook");
    QString storagePath = settings.value("StorageLocation","").toString();
    ui->lineEditSRTMStorage->setText(storagePath+"srtm/");
    QDir dir(storagePath+"srtm/");
    if (dir.entryList(QStringList() << "*.zip").count() != 0) {
        ui->listWidgetSRTMTilesList->clear();
        ui->listWidgetSRTMTilesList->setEnabled(true);
        ui->listWidgetSRTMTilesList->addItems(dir.entryList(QStringList() << "*.zip"));
        QFileInfoList fileInfoList = dir.entryInfoList(QStringList() << "*.zip");
        double size=0;
        foreach (QFileInfo fileInfo, fileInfoList)
        {
            size += fileInfo.size();
        }
        ui->labelSize->setText(QString::number(size/1024/1024)+tr("MB"));
    }
    else
    {
        ui->listWidgetSRTMTilesList->setEnabled(false);
        ui->listWidgetSRTMTilesList->clear();
        QListWidgetItem* item = new QListWidgetItem(tr("No SRTM data available."));
        ui->listWidgetSRTMTilesList->addItem(item);
    }
}

DisplaySRTMOptionsFrame::~DisplaySRTMOptionsFrame()
{
    delete ui;
}

void DisplaySRTMOptionsFrame::changeEvent(QEvent *e)
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

void DisplaySRTMOptionsFrame::on_toolButtonDeleteSelectedTile_clicked()
{
    if (QMessageBox::warning(this,"SRTM plugin",tr("Please confirm deletion of ")+ui->listWidgetSRTMTilesList->currentItem()->text(),QMessageBox::Ok|QMessageBox::Cancel) == QMessageBox::Ok)
    {
        QSettings settings("GPSBook","GPSBook");
        QString storagePath = settings.value("StorageLocation","").toString();
        QFile(storagePath+"srtm/"+ui->listWidgetSRTMTilesList->currentItem()->text()).remove();
        updateTileDirList();
    }
}

void DisplaySRTMOptionsFrame::on_listWidgetSRTMTilesList_itemSelectionChanged()
{
    ui->toolButtonDeleteSelectedTile->setEnabled(ui->listWidgetSRTMTilesList->currentItem() != NULL);
}
