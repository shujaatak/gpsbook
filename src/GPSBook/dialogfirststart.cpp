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
#include "dialogfirststart.h"
#include "ui_dialogfirststart.h"
#include <QDir>
#include <QTranslator>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>
#include "gpsbookapplication.h"

namespace GPSBook {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DialogFirstStart::DialogFirstStart(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::DialogFirstStart)
    {
        m_ui->setupUi(this);

        setWindowFlags(Qt::SplashScreen);

        //Replace {{VERSION}} keyword
        m_ui->textEditFirstStartInfo->setHtml(m_ui->textEditFirstStartInfo->toHtml().replace("{{VERSION}}",qApp->applicationVersion()));

        //Manage Language Combobox
        m_ui->comboBoxLanguage->addItem("Default language (System)","");
        m_ui->comboBoxLanguage->insertSeparator(1);
        m_ui->comboBoxLanguage->setInsertPolicy(QComboBox::InsertAlphabetically);
        foreach (QString avail, GPSBookApplication::availableLanguages())
        {
            // figure out nice names for locales
            QLocale locale(avail);
            QString language = QLocale::languageToString(locale.language());
            QString country = QLocale::countryToString(locale.country());
            QString name = language + " (" + country + ")";

            m_ui->comboBoxLanguage->addItem(name,avail);
        }
        QSettings settings("GPSBook","GPSBook");
        m_ui->comboBoxLanguage->setCurrentIndex(m_ui->comboBoxLanguage->findData(settings.value("Translation","").toString()));

        //Manage Storage Location
        QString tmpstr = settings.value("StorageLocation","").toString();
        if (tmpstr == "")
        {
            tmpstr = QDir::homePath()+"/gpsbook/";
        }
        m_ui->lineEditStorageLocation->setText(tmpstr);
    } //DialogFirstStart::DialogFirstStart

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DialogFirstStart::~DialogFirstStart()
    {
        delete m_ui;
    } //DialogFirstStart::~DialogFirstStart

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogFirstStart::changeEvent(QEvent *e)
    {
        switch (e->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            break;
        default:
            break;
        }
    } //DialogFirstStart::changeEvent

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogFirstStart::on_comboBoxLanguage_currentIndexChanged(int i)
    {
        GPSBookApplication::setLanguage(m_ui->comboBoxLanguage->itemData(i).toString());
    } //DialogFirstStart::on_combobox_changed


    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DialogFirstStart::on_buttonBox_accepted()
    {
        QSettings settings("GPSBook","GPSBook");
        settings.setValue("Translation",m_ui->comboBoxLanguage->itemData(m_ui->comboBoxLanguage->currentIndex()).toString());
        settings.setValue("StorageLocation",m_ui->lineEditStorageLocation->text());
    } //DialogFirstStart::on_buttonBox_accepted
}

