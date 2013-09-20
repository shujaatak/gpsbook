/*************************************************************************
*   Copyright (C) 2006 by Christoph Eckert                                *
*   ce@christeck.de                                                       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/


#ifndef SETTINGSMANAGER_h
#define SETTINGSMANAGER_h

#include <iostream>
#include <unistd.h>
#include <time.h>

#include <QtGui/QtGui>

using namespace std;


class SettingsManager : public QObject
{
    Q_OBJECT
protected:
    SettingsManager();

private:
    static SettingsManager* StaticInstancePointer;

    QSettings * UserSettings;
    QSettings * SystemSettings;

public:
    ~SettingsManager();
    static SettingsManager* instance();

    QString UserConfigFileName();
    QString SystemConfigFileName();

    QString pathGpsBabel();
    QString pathInput();
    QString pathOutput();
    QString pathFilter();

    QString inputFilter();
    QString outputFilter();
    QString filterFilter();

    void setPathInput( QString );
    void setPathOutput( QString );
    void setPathFilter( QString );

    void setFilterInput( QString );
    void setFilterOutput( QString );
    void setFilterFilter( QString );

    void setPathGpsBabel( QString );

    //void saveWindowGeometry( QPoint, QSize );
    //QPoint AppPos();
    //QSize AppSize();

    QStringList deviceTypes();
    QStringList devicePorts();
    QStringList inputTypes();
    QStringList outputTypes();
    QStringList filters();
    QStringList fileTypes();
    QStringList characterSets();
    QStringList Options( QString, QString );

    QString TypeName( QString, QString );
    QString OptionName( QString, QString, QString );
    QString CharsetName( QString );
    QString OptionExample( QString, QString, QString );

    QString humanReadableGpsBabelError( QString );

    QString preset( QString PresetName );
    QString comment( QString PresetName );

    QStringList presetNames();
    QStringList commentNames();

    //int incrementUsageCount();
    //void disableUsageCount();

    QString language();

    // signals:
public slots:
    void restoreSystemDefaults();
    void restoreUserDefaults();

    void savePreset( QString, QString );
    void saveComment( QString, QString );
    void deletePreset( QString );
    //void setLanguage( QString );
};

#endif


