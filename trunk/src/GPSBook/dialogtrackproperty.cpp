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
#include <QDebug>
#include "dialogtrackproperty.h"
#include "ui_dialogtrackproperty.h"
#include "gpsdata.h"

namespace GPSBook {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DialogTrackProperty::DialogTrackProperty(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::DialogTrackProperty)
    {
        m_ui->setupUi(this);
    } //DialogTrackProperty::DialogTrackProperty

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DialogTrackProperty::~DialogTrackProperty()
    {
        delete m_ui;
    } //DialogTrackProperty::~DialogTrackProperty

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogTrackProperty::changeEvent(QEvent *e)
    {
        QDialog::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            break;
        default:
            break;
        }
    } //DialogTrackProperty::changeEvent

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogTrackProperty::setFileName(QString value)
    {
        m_ui->label_filenamevalue->setText(value);
    } //DialogTrackProperty::setFileName

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogTrackProperty::setDisplayName(QString value)
    {
        m_ui->lineEdit_texttodisplay->setText(value);
    } //DialogTrackProperty::setDisplayName

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogTrackProperty::setDescription(QString value)
    {
        m_ui->textEdit_Description->setText(value);
    } //DialogTrackProperty::setDescription

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QString DialogTrackProperty::fileName()
    {
        return m_ui->label_filenamevalue->text();
    } //DialogTrackProperty::fileName

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QString DialogTrackProperty::displayName()
    {
        return m_ui->lineEdit_texttodisplay->text();
    } //DialogTrackProperty::displayName

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    QString DialogTrackProperty::description()
    {
        return m_ui->textEdit_Description->toPlainText();
    } //DialogTrackProperty::description

}

