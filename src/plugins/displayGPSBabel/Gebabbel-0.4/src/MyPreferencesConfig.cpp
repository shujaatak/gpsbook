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

#include "MyPreferencesConfig.h"

MyPreferencesConfig::MyPreferencesConfig( QWidget* parent )
    : QDialog( parent )
{
    setupUi( this );
    setupConnections();
    getSettings();

    // Temporarily disabling this feature as I guess noone needs it
    // LabelGpsbabelPath->hide();
    // InputGpsbabelPath->hide();
    // ButtonGpsbabelPath->hide();
}



void MyPreferencesConfig::setupConnections()
{
    connect( ButtonCancel, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( ButtonOk, SIGNAL( clicked() ), this, SLOT( saveSettings() ) );
    connect( ButtonGpsbabelPath, SIGNAL( clicked() ), this, SLOT( selectGpsbabel() ) );

    connect( ButtonRestoreDefaults, SIGNAL( clicked() ), parent(), SLOT( restoreDefaults() ) );
}


void MyPreferencesConfig::getSettings()
{
    QString GpsbabelPath = "";
    if ( SettingsManager::instance()->pathGpsBabel() == "" )
    {
        GpsbabelPath = "gpsbabel";
#ifdef WINDOWS
        GpsbabelPath.append( ".exe" );
#endif
    }
    else
    {
        GpsbabelPath = SettingsManager::instance()->pathGpsBabel();
    }
    InputGpsbabelPath->setText( GpsbabelPath );

    DisplayUserSettingsPath->setText( SettingsManager::instance()->UserConfigFileName() );
    DisplaySystemSettingsPath->setText( SettingsManager::instance()->SystemConfigFileName() );

    // It was planned to make the locale setting accessible via the preferences
    // If so, the language also needs to be set in MyPreferencesConfig::saveSettings()
    // Filling the language combo with the filenames present in :/translations/
    QDir ResourceDir( ":/" );
    ResourceDir.cd( "translations" );

    ComboLanguage->hide();
    LabelLanguage->hide();
    ComboLanguage->addItems( ResourceDir.entryList() );
    ComboLanguage->insertItem( 0, tr( "Automatic" ) );


    // Displaying the current locale
    QLocale DefaultLocale;
    QString Locale = DefaultLocale.name();
    DisplayLocale->setText( Locale );
}


void MyPreferencesConfig::saveSettings()
{
    SettingsManager::instance()->setPathGpsBabel( InputGpsbabelPath->text() );
    this->close();
}


void MyPreferencesConfig::selectGpsbabel()
{
    // TODO: Still buggy. If path only contains "gpsbabel", the selector jumps to the working directory...
    QString SearchPath = SettingsManager::instance()->pathGpsBabel();
    if ( SearchPath == "" )
    {
#ifdef LINUX
        SearchPath = QDir::homePath();
#endif
#ifdef MACINTOSH
        SearchPath = "/Applications";
#endif
#ifdef WINDOWS
        SearchPath = "c:";
#endif
    }
    else
    {
        QFileInfo FileInfo( SearchPath );
        SearchPath = FileInfo.absolutePath();
    }

    QString GpsbabelPath = "";
#ifndef WINDOWS
    GpsbabelPath = QFileDialog::getOpenFileName(this, tr("Select Gpsbabel binary"), SearchPath, tr( "Any file (*)" ));
#endif
#ifdef WINDOWS
    GpsbabelPath = QFileDialog::getOpenFileName(this, tr("Select Gpsbabel binary"), SearchPath, tr( "Executable files (*.exe);;Any file (*)" ));
#endif
    if ( GpsbabelPath != "" )
    {
        InputGpsbabelPath->setText( GpsbabelPath );
    }
}

