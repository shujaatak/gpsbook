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
#ifndef GPSBOOKAPPLICATION_H
#define GPSBOOKAPPLICATION_H

#include <QApplication>
#include <QHash>
#include <QStringList>
#include <QtSql/QSqlDatabase>

class QDir;
class QTranslator;

typedef QHash<QString, QTranslator*> Translators;

namespace GPSBook {
    class GPSBookApplication : public QApplication
    {
        Q_OBJECT

    public:
            explicit GPSBookApplication(int& argc, char* argv[]);
            ~GPSBookApplication();

            static void loadTranslations(const QDir& dir);
            static const QStringList availableLanguages();
            static const QSqlDatabase db;

    public slots:
            static void setLanguage(const QString& locale);

    private:
            static QString current;
            static Translators translators;
    };
}

#endif // GPSBOOK_H
