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
#include <QDir>
#include <QFileInfo>
#include <QTranslator>
#include <QLocale>
#include <QDebug>
#include "gpsbookapplication.h"

namespace GPSBook {

    QString GPSBookApplication::current = "";
    Translators GPSBookApplication::translators;

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    GPSBookApplication::GPSBookApplication(int& argc, char* argv[])
            : QApplication(argc, argv)
    {
    } //GPSBookApplication::GPSBookApplication

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    GPSBookApplication::~GPSBookApplication()
    {
    } //GPSBookApplication::~GPSBookApplication

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GPSBookApplication::loadTranslations(const QDir& dir)
    {
        //<prefix>_<language>_<country>.qm
        QString filter = "*_*_*.qm";
        QDir::Filters filters = QDir::Files | QDir::Readable;
        QDir::SortFlags sort = QDir::Name;
        QFileInfoList entries = dir.entryInfoList(QStringList() << filter, filters, sort);
        foreach (QFileInfo file, entries)
        {
            // pick country and language out of the file name
            QStringList parts = file.baseName().split("_");
            QString language = parts.at(parts.count() - 2).toLower();
            QString country  = parts.at(parts.count() - 1).toUpper();

            // construct and load translator
            QTranslator* translator = new QTranslator(instance());
            if (translator->load(file.absoluteFilePath()))
            {
                QString locale = language + "_" + country;
                translators.insertMulti(locale, translator);
            }
        }
    } //GPSBookApplication::loadTranslations

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    const QStringList GPSBookApplication::availableLanguages()
    {
        // the content won't get copied thanks to implicit sharing and constness
        return QStringList(translators.uniqueKeys());
    } //GPSBookApplication::availableLanguage

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void GPSBookApplication::setLanguage(const QString& locale)
    {
        // remove all previous translators
        foreach (QTranslator* translator, translators.values(current))
        {
            removeTranslator(translator);
        }

        //Remember new language
        if (locale == "")
        {
            current = QLocale::system().name();
        }
        else
        {
            current = locale;
        }

        // install new translators
        foreach (QTranslator* translator, translators.values(current))
        {
            installTranslator(translator);
        }
    } //GPSBookApplication::setLanguage
}

