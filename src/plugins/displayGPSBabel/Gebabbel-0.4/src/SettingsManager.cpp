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


#include "Versioning.h"

#include "SettingsManager.h"


SettingsManager * SettingsManager::StaticInstancePointer = 0L;


SettingsManager * SettingsManager::instance()
{
    if ( StaticInstancePointer == 0L )
    {
        StaticInstancePointer = new SettingsManager();
    }
    return StaticInstancePointer;
}

// A QObject always needs a pointer, even if it's a nullpointer
SettingsManager::SettingsManager() : QObject( 0L )
{
    QString UserSettingsFileName = "";
    QString SystemSettingsFileName = "";
    UserSettingsFileName.append( APPNAME );
    SystemSettingsFileName.append( APPNAME );
    UserSettingsFileName.append( "UserSettings" );
    SystemSettingsFileName.append( "SystemSettings" );
    SystemSettingsFileName.append( "-" );
    SystemSettingsFileName.append( VERSION );

    UserSettings = new QSettings( QSettings::IniFormat, QSettings::UserScope, APPNAME, UserSettingsFileName );
    SystemSettings = new QSettings( QSettings::IniFormat, QSettings::UserScope, APPNAME, SystemSettingsFileName );
}



SettingsManager::~SettingsManager()
{
    StaticInstancePointer = 0L;
    // Ensuring the config file gets written to disc at application quit
    UserSettings->sync();
    SystemSettings->sync();
}

/*
QString SettingsManager::guiName( QString Type, QString Format )
	{
		SystemSettings->beginGroup( Type );
		QStringList Types = SystemSettings->allKeys();
		for (  int i = 0; i < Types.count(); i++ )
			{
				if ( Types.at( i ) == Format )
					{
						return SystemSettings->value( Types.at( i ), "" ).toString();
					}
			}
		SystemSettings->endGroup();
	}
*/


/*
QString SettingsManager::format( QString Type, QString GuiName )
	{

	}
*/


QString SettingsManager::UserConfigFileName()
{
    return UserSettings->fileName();
}


QString SettingsManager::SystemConfigFileName()
{
    return SystemSettings->fileName();
}


QStringList SettingsManager::deviceTypes()
{
    SystemSettings->beginGroup( "DeviceFormats" );
    QStringList AllKeys = SystemSettings->childGroups();
    SystemSettings->endGroup();
    return AllKeys;
}


QStringList SettingsManager::devicePorts()
{
    SystemSettings->beginGroup( "DevicePorts" );
    QStringList AllKeys = SystemSettings->childKeys();
    SystemSettings->endGroup();
    return AllKeys;
}


QStringList SettingsManager::inputTypes()
{
    SystemSettings->beginGroup( "Input" );
    QStringList AllKeys = SystemSettings->childGroups();
    SystemSettings->endGroup();
    return AllKeys;
}


QStringList SettingsManager::outputTypes()
{
    SystemSettings->beginGroup( "Output" );
    QStringList AllKeys = SystemSettings->childGroups();
    SystemSettings->endGroup();
    return AllKeys;
}


QStringList SettingsManager::filters()
{
    SystemSettings->beginGroup( "Filter" );
    QStringList AllKeys = SystemSettings->childGroups();
    SystemSettings->endGroup();
    return AllKeys;
}


QStringList SettingsManager::fileTypes()
{
    SystemSettings->beginGroup( "FileTypes" );
    QStringList AllKeys = SystemSettings->childKeys();

    for ( int i = 0; i < AllKeys.size(); i++ )
    {
        QString Item = SystemSettings->value( AllKeys.at( i ) ).toString();
        Item.append( " (" );
        Item.append( AllKeys.at( i ) );
        Item.append( ")" );
        AllKeys.replace( i, Item );
    }

    SystemSettings->endGroup();
    return AllKeys;
}


QStringList SettingsManager::characterSets()
{
    SystemSettings->beginGroup( "CharacterSets" );
    QStringList AllKeys = SystemSettings->childKeys();
    SystemSettings->endGroup();
    return AllKeys;
}


QStringList SettingsManager::Options( QString DataType, QString Type )
{
    // Helper variables
    QStringList AllKeys;
    QStringList TempList;

    SystemSettings->beginGroup( DataType );

    // The current group is either Input, Filter or Output
    // Check if there is a subgroup named "Type" and if there is a subgroup "Options"
    // In case a group named "Type" or a subgroup "Options" does not exist, return an empty stringlist
    TempList = SystemSettings->childGroups();
    for ( int i = 0; i < TempList.size(); i++ )
    {
        if ( TempList.at( i ) == Type )
        {
            SystemSettings->beginGroup( Type );
            TempList = SystemSettings->childGroups();
            for ( int i = 0; i < TempList.size(); i++ )
            {
                if ( TempList.at( i ) == "Options" )
                {
                    SystemSettings->beginGroup( "Options" );
                    AllKeys = SystemSettings->childKeys();
                    SystemSettings->endGroup();
                }
            }
            SystemSettings->endGroup();
        }
    }
    // Don't forget to return to the top level of the config file's hierarchy
    SystemSettings->endGroup();
    return AllKeys;
}


QString SettingsManager::TypeName( QString DataType, QString Type )
{
    QString TempString;

    SystemSettings->beginGroup( DataType );
    QStringList TempList = SystemSettings->childGroups();
    for ( int i = 0; i < TempList.size(); i++ )
    {
        if ( TempList.at( i ) == Type )
        {
            SystemSettings->beginGroup( Type );
            TempString = SystemSettings->value( "Name", "" ).toString();
            SystemSettings->endGroup();
        }
    }
    // Don't forget to return to the top level of the config file's hierarchy
    SystemSettings->endGroup();
    return TempString;
}


QString SettingsManager::OptionName( QString DataType, QString Type, QString Option )
{
    // Attention: "Option" contains the complete content of the current option combobox
    // Therefore do not only compare, but search for options in it

    // Helper variables
    QString TempString;
    QStringList TempList;

    SystemSettings->beginGroup( DataType );

    // The current group is either Input, Filter or Output
    // Check if there is a subgroup named "Type" and if there is a subgroup "Options"
    // In case a group named "Type" or a subgroup "Options" does not exist, return an empty stringlist
    TempList = SystemSettings->childGroups();
    for ( int i = 0; i < TempList.size(); i++ )
    {
        if ( TempList.at( i ) == Type )
        {
            SystemSettings->beginGroup( Type );
            TempList = SystemSettings->childGroups();
            for ( int i = 0; i < TempList.size(); i++ )
            {
                if ( TempList.at( i ) == "Options" )
                {
                    SystemSettings->beginGroup( "Options" );
                    TempList = SystemSettings->childKeys();
                    for ( int i = 0; i < TempList.count(); i++)
                    {
                        // TODO: contains() is not enough, as it will compose a very long text even in case of 'hdopandvdop'
                        if ( Option.contains( TempList.at( i ) ) )
                        {
                            if ( TempString != "" )
                            {
                                TempString.append( "; " );
                            }
                            TempString.append( SystemSettings->value( TempList.at( i ) ).toString() );
                        }
                    }
                    SystemSettings->endGroup();
                }
            }
            SystemSettings->endGroup();
        }
    }
    // Don't forget to return to the top level of the config file's hierarchy
    SystemSettings->endGroup();
    return TempString;
}


QString SettingsManager::CharsetName( QString CharSet )
{
    QString TempString;
    SystemSettings->beginGroup( "CharacterSets" );
    TempString = SystemSettings->value( CharSet, "" ).toString();
    SystemSettings->endGroup();
    return TempString;
}


QString SettingsManager::OptionExample( QString DataType, QString Type, QString Option )
{
    // Attention: "option" contains the complete content of the current option combobox
    // Therefore do not only compare, but search for options in it

    // Helper variables
    QString TempString;
    QStringList TempList;

    SystemSettings->beginGroup( DataType );

    // The current group is either Input, Filter or Output
    // Check if there is a subgroup named "Type" and if there is a subgroup "Options"
    // In case a group named "Type" or a subgroup "Options" does not exist, return an empty stringlist
    TempList = SystemSettings->childGroups();
    for ( int i = 0; i < TempList.size(); i++ )
    {
        if ( TempList.at( i ) == Type )
        {
            SystemSettings->beginGroup( Type );
            TempList = SystemSettings->childGroups();
            for ( int i = 0; i < TempList.size(); i++ )
            {
                if ( TempList.at( i ) == "Options" )
                {
                    SystemSettings->beginGroup( "Options" );
                    TempList = SystemSettings->childKeys();
                    for ( int i = 0; i < TempList.count(); i++)
                    {
                        if ( Option.contains( TempList.at( i ) ) )
                        {
                            TempString.append( SystemSettings->value( TempList.at( i ) ).toString() );
                            TempString.append( ", " );
                        }
                    }
                    SystemSettings->endGroup();
                }
            }
            SystemSettings->endGroup();
        }
    }
    // Don't forget to return to the top level of the config file's hierarchy
    SystemSettings->endGroup();
    return TempString;

}


QString SettingsManager::humanReadableGpsBabelError( QString Error )
{
    SystemSettings->beginGroup( "GpsBabelErrorMessages" );
    QStringList AllKeys = SystemSettings->childKeys();
    QString TempString = "";
    for ( int i = 0; i < AllKeys.count(); i++ )
    {
        if ( Error.contains( AllKeys.at( i ), Qt::CaseInsensitive ) )
        {
            TempString = SystemSettings->value( AllKeys.at( i ), "Error" ).toString();
        }
    }
    SystemSettings->endGroup();
    return TempString;
}


void SettingsManager::savePreset( QString PresetName, QString Command )
{
    UserSettings->beginGroup( "Presets" );
    UserSettings->setValue( PresetName, Command );
    UserSettings->endGroup();
    // The sync() is very important.
    // Otherwise LastUsedSetting doesn't get written to disc at application quit
    UserSettings->sync();
}


void SettingsManager::saveComment( QString PresetName, QString Comment )
{
    UserSettings->beginGroup( "Comments" );
    UserSettings->setValue( PresetName, Comment );
    UserSettings->endGroup();
}


void SettingsManager::deletePreset( QString PresetName )
{
    UserSettings->beginGroup( "Presets" );
    UserSettings->remove( PresetName );
    UserSettings->endGroup();
    UserSettings->beginGroup( "Comments" );
    UserSettings->remove( PresetName );
    UserSettings->endGroup();
}


QString SettingsManager::pathGpsBabel()
{
#ifndef WINDOWS
    return UserSettings->value( "GpsbabelPath", "gpsbabel" ).toString();
#endif
#ifdef WINDOWS
    return UserSettings->value( "GpsbabelPath", "gpsbabel.exe" ).toString();
#endif
}


QStringList SettingsManager::presetNames()
{
    UserSettings->beginGroup( "Presets" );
    QStringList AllKeys = UserSettings->allKeys();
    UserSettings->endGroup();
    return AllKeys;
}



QStringList SettingsManager::commentNames()
{
    UserSettings->beginGroup( "Comments" );
    QStringList AllKeys = UserSettings->allKeys();
    UserSettings->endGroup();
    return AllKeys;
}


QString SettingsManager::preset( QString PresetName )
{
    UserSettings->beginGroup( "Presets" );
    QString Command = UserSettings->value( PresetName, "" ).toString();
    UserSettings->endGroup();
    return Command;
}


QString SettingsManager::comment( QString PresetName )
{
    UserSettings->beginGroup( "Comments" );
    QString Comment = UserSettings->value( PresetName, "" ).toString();
    UserSettings->endGroup();
    return Comment;
}


QString SettingsManager::pathInput()
{
    // The default path is the user's home directory
    // In case Desktop is writable, it gets set to Desktop
    QString Path = UserSettings->value( "LastInputPath", "" ).toString();
    if ( Path == "" )
    {
        QFileInfo DesktopCheck;
        Path = QDir::homePath().append( "/Desktop" );
        DesktopCheck.setFile( Path );
        if ( !DesktopCheck.isWritable() )
        {
            Path = QDir::homePath();
        }
    }
    return Path;
}


QString SettingsManager::pathOutput()
{
    // The default path is the user's home directory
    // In case Desktop is writable, it gets set to Desktop
    QString Path = UserSettings->value( "LastOutputPath", "" ).toString();
    if ( Path == "" )
    {
        QFileInfo DesktopCheck;
        Path = QDir::homePath().append( "/Desktop" );
        DesktopCheck.setFile( Path );
        if ( !DesktopCheck.isWritable() )
        {
            Path = QDir::homePath();
        }
    }
    return Path;
}


QString SettingsManager::pathFilter()
{
    // The default path is the user's home directory
    // In case Desktop is writable, it gets set to Desktop
    QString Path = UserSettings->value( "LastFilterPath", "" ).toString();
    if ( Path == "" )
    {
        QFileInfo DesktopCheck;
        Path = QDir::homePath().append( "/Desktop" );
        DesktopCheck.setFile( Path );
        if ( !DesktopCheck.isWritable() )
        {
            Path = QDir::homePath();
        }
    }
    return Path;
}


QString SettingsManager::inputFilter()
{
    return UserSettings->value( "LastInputFilter" ).toString();
}


QString SettingsManager::outputFilter()
{
    return UserSettings->value( "LastOutputFilter" ).toString();
}


QString SettingsManager::filterFilter()
{
    return UserSettings->value( "LastFilterFilter" ).toString();
}




void SettingsManager::setPathInput( QString Path )
{
    UserSettings->setValue( "LastInputPath", Path );
}


void SettingsManager::setPathOutput( QString Path )
{
    UserSettings->setValue( "LastOutputPath", Path );
}


void SettingsManager::setPathFilter( QString Path )
{
    UserSettings->setValue( "LastFilterPath", Path );
}


void SettingsManager::setFilterInput( QString Filter )
{
    UserSettings->setValue( "LastInputFilter", Filter );
}


void SettingsManager::setFilterOutput( QString Filter )
{
    UserSettings->setValue( "LastOutputFilter", Filter );
}


void SettingsManager::setFilterFilter( QString Filter )
{
    UserSettings->setValue( "LastFilterFilter", Filter );
}


void SettingsManager::setPathGpsBabel( QString Path )
{
    // Trying to avoid an empty string as this would cause QProcess to segfault
    if ( Path == "" )
    {
        UserSettings->setValue( "GpsbabelPath", "gpsbabel" );
    }
    else
    {
        UserSettings->setValue( "GpsbabelPath", Path );
    }
}

/*
void SettingsManager::saveWindowGeometry( QPoint Pos, QSize Size )
	{
		SystemSettings->setValue( "WindowPosition", Pos );
		SystemSettings->setValue( "WindowSize", Size );
		// sync() seems to be important if you want your changes to be saved at application quit
		SystemSettings->sync();
	}


QPoint SettingsManager::AppPos()
	{
		return SystemSettings->value( "WindowPosition", QPoint( 200,200 ) ).toPoint();
	}


QSize SettingsManager::AppSize()
	{
		return SystemSettings->value( "WindowSize", QSize( 650, 400 ) ).toSize();
	}



// The main window will call this function and ask the user for feedback if gebabbel was used x times
// Of course the user can disable the feedback agent by settign the counter to 0
int SettingsManager::incrementUsageCount()
	{
		if ( SystemSettings->value( "UsageCount", "0" ).toInt() > 0 )
			{
				SystemSettings->setValue( "UsageCount", SystemSettings->value( "UsageCount", "0" ).toInt() + 1 );
			}
		return SystemSettings->value( "UsageCount", "0" ).toInt();
	}


void SettingsManager::disableUsageCount()
	{
		SystemSettings->setValue( "UsageCount", "0" );
	}

QString SettingsManager::language()
	{
		QString LanguageName;
		if ( UserSettings->value( "Language", "auto" ).toString() == "auto" )
			{
				// See http://web.mit.edu/qt/www/i18n.html#internationalization
				QLocale DefaultLocale;
				LanguageName = DefaultLocale.name();
			}
		else
			{
				LanguageName = UserSettings->value( "Language", "auto" ).toString();
			}

		LanguageName.append( ".qm" );
		LanguageName.prepend( ":/translations/" );
		return LanguageName;
	}


void SettingsManager::setLanguage( QString Language )
	{
		UserSettings->setValue( "Language", Language );
	}
*/

void SettingsManager::restoreUserDefaults()
{
    // Try to create a backup of the current configfiles first
    // Attention: If the destination file exists, rename will return false
    // It even can happen that the file cannot be removed or the new one cannot be created
    QFile::rename( UserSettings->fileName(), UserSettings->fileName().append( "-" ).append( QDateTime::currentDateTime().toString( "yyyyMMdd-HHmmss" ) ) );

    // DefaultPresets
    QString TempName, TempCommand, ExecName;
#ifndef WINDOWS
    ExecName = "gpsbabel";
#endif
#ifdef WINDOWS
    ExecName = "gpsbabel.exe";
#endif


    // Default presets to get the user started immediately

    UserSettings->beginGroup( "Presets" );
    TempName = tr( "Cut Track based on times" );
    TempCommand = ExecName ;
    TempCommand.append( " " );
    TempCommand.append( "-t -i gpx -f %1 -x track,start=2007010110,stop=2007010118 -o gpx,gpxver=1.1 -F %2" );
    //UserSettings->setValue( TempName, TempCommand.arg( tr( "OriginalTrack.gpx" ) ).arg( tr( "TruncatedTrack.gpx" ) ) );
    UserSettings->setValue( TempName, TempCommand.arg( "gpsbook" ).arg( "gpsbook" ) );
    UserSettings->endGroup();
    UserSettings->beginGroup( "Comments" );
    UserSettings->setValue( TempName, tr( "Only keep trackpoints between the start and stop time" ) );
    UserSettings->endGroup();

    UserSettings->beginGroup( "Presets" );
    TempName = tr( "Drop all data except track data" );
    TempCommand = ExecName ;
    TempCommand.append( " " );
    TempCommand.append( "-t -i gpx -f %1 -x nuketypes,waypoints,routes -o gpx,gpxver=1.1 -F %2" );
    //UserSettings->setValue( TempName, TempCommand.arg( tr( "HugeFile.gpx" ) ).arg( tr( "TracksOnly.gpx" ) ) );
    UserSettings->setValue( TempName, TempCommand.arg( "gpsbook" ).arg( "gpsbook" ) );
    UserSettings->endGroup();
    UserSettings->beginGroup( "Comments" );
    UserSettings->setValue( TempName, tr( "Keep tracks but drop waypoints and routes" ) );
    UserSettings->endGroup();

    UserSettings->beginGroup( "Presets" );
    TempName = tr( "Fill time gaps in track" );
    TempCommand = ExecName ;
    TempCommand.append( " " );
    TempCommand.append( "-t -i gpx -f %1 -x interpolate,time=10 -o gpx,gpxver=1.1 -F %2" );
    //UserSettings->setValue( TempName, TempCommand.arg( tr( "InputTrack.gpx" ) ).arg( tr( "InterpolatedTrack.gpx" ) ) );
    UserSettings->setValue( TempName, TempCommand.arg( "gpsbook" ).arg( "gpsbook" ) );
    UserSettings->endGroup();
    UserSettings->beginGroup( "Comments" );
    UserSettings->setValue( TempName, tr( "Insert points if two adjacent points are more than the given time apart" ) );
    UserSettings->endGroup();

    UserSettings->beginGroup( "Presets" );
    TempName = tr( "Fill distance gaps in track" );
    TempCommand = ExecName ;
    TempCommand.append( " " );
    TempCommand.append( "-t -i gpx -f %1 -x interpolate,distance=2m -o gpx,gpxver=1.1 -F %2" );
    //UserSettings->setValue( TempName, TempCommand.arg( tr( "InputTrack.gpx" ) ).arg( tr( "InterpolatedTrack.gpx" ) ) );
    UserSettings->setValue( TempName, TempCommand.arg( "gpsbook" ).arg( "gpsbook" ) );
    UserSettings->endGroup();
    UserSettings->beginGroup( "Comments" );
    UserSettings->setValue( TempName, tr( "Insert points if two adjacent points are more than the given distance apart" ) );
    UserSettings->endGroup();

    /*
    UserSettings->beginGroup( "Presets" );
    TempName = tr( "Garmin text format example" );
    TempCommand = ExecName ;
    TempCommand.append( " " );
    TempCommand.append( "-w -t -r -N -i garmin_txt,date=\"MM/DD/YYYY\",time=\"hh:mm:ss xx\" -f %1 -o garmin_txt -F %2" );
    UserSettings->setValue( TempName, TempCommand.arg( tr( "GarminTextInput.txt" ) ).arg( tr( "GarminTextOutput.txt" ) ) );
    UserSettings->endGroup();
    UserSettings->beginGroup( "Comments" );
    UserSettings->setValue( TempName, tr( "Example showing all options of the garmin text format" ) );
    UserSettings->endGroup();
    */

    UserSettings->beginGroup( "Presets" );
    TempName = tr( "Get waypoints from Garmin" );
    TempCommand = ExecName ;
    TempCommand.append( " " );
    TempCommand.append( "-w -N -i garmin -f usb: -o gpx,gpxver=1.1 -F %1" );
    //UserSettings->setValue( TempName, TempCommand.arg( tr( "GarminWaypoints.gpx" ) ) );
    UserSettings->setValue( TempName, TempCommand.arg( "gpsbook" ) );
    UserSettings->endGroup();
    UserSettings->beginGroup( "Comments" );
    UserSettings->setValue( TempName, tr( "Get waypoints from a Garmin device" ) );
    UserSettings->endGroup();

    UserSettings->beginGroup( "Presets" );
    TempName = tr( "Get routes from Garmin" );
    TempCommand = ExecName ;
    TempCommand.append( " " );
    TempCommand.append( "-r -N -i garmin -f usb: -o gpx,gpxver=1.1 -F %1" );
    //UserSettings->setValue( TempName, TempCommand.arg( tr( "GarminRoutes.gpx" ) ) );
    UserSettings->setValue( TempName, TempCommand.arg( "gpsbook" ) );
    UserSettings->endGroup();
    UserSettings->beginGroup( "Comments" );
    UserSettings->setValue( TempName, tr( "Get routes from a Garmin device" ) );
    UserSettings->endGroup();

    UserSettings->beginGroup( "Presets" );
    TempName = tr( "Get tracks from Garmin" );
    TempCommand = ExecName ;
    TempCommand.append( " " );
    TempCommand.append( "-t -N -i garmin -f usb: -o gpx,gpxver=1.1 -F %1" );
    //UserSettings->setValue( TempName, TempCommand.arg( tr( "GarminTracks.gpx" ) ) );
    UserSettings->setValue( TempName, TempCommand.arg( "gpsbook" ) );
    UserSettings->endGroup();
    UserSettings->beginGroup( "Comments" );
    UserSettings->setValue( TempName, tr( "Get tracks from a Garmin device" ) );
    UserSettings->endGroup();

    /*
    // See http://www.gpsbabel.org/htmldoc-development/tracking.html
    UserSettings->beginGroup( "Presets" );
    TempName = tr( "Get realtime position from Garmin" );
    TempCommand = ExecName ;
    TempCommand.append( " " );
    TempCommand.append( "-T -i garmin -f usb: -o kml -F %1" );
    UserSettings->setValue( TempName, TempCommand.arg( tr( "PositionLogging.kml" ) ) );
    UserSettings->endGroup();
    UserSettings->beginGroup( "Comments" );
    UserSettings->setValue( TempName, tr( "Get the current position from a Garmin device and stream it to a file" ) );
    UserSettings->endGroup();
    */

    /*
    UserSettings->beginGroup( "Presets" );
    TempName = tr( "Get and erase data from WBT" );
    TempCommand = ExecName ;
    TempCommand.append( " " );
    TempCommand.append( "-w -r -t -N -i wbt,erase -f %1 -o gpx,gpxver=1.1 -F %2" );
    UserSettings->setValue( TempName, TempCommand.arg( tr( "/dev/cu.WBT200-SPPslave-1" ) ).arg( tr( "DataFromWBT.gpx" ) ) );
    UserSettings->endGroup();
    UserSettings->beginGroup( "Comments" );
    UserSettings->setValue( TempName, tr( "Download data from a WBT device and erase its memory contents afterwards") );
    UserSettings->endGroup();
    */

    UserSettings->beginGroup( "Presets" );
    TempName = tr( "Pack (merge) Tracks" );
    TempCommand = ExecName ;
    TempCommand.append( " " );
    TempCommand.append( "-t -i gpx -f %1 -i gpx -f %2 -x track,pack,title=\"CompleteTrack\" -o gpx,gpxver=1.1 -F %3" );
    //UserSettings->setValue( TempName, TempCommand.arg( tr( "JourneyThere.gpx" ) ).arg( tr( "JourneyBack.gpx" ) ).arg( tr( "CompleteJourney.gpx" ) ) );
    UserSettings->setValue( TempName, TempCommand.arg( "gpsbook" ).arg( tr( "JourneyBack.gpx" ) ).arg( "gpsbook" ) );
    UserSettings->endGroup();
    UserSettings->beginGroup( "Comments" );
    UserSettings->setValue( TempName, tr( "Merge multiple tracks into one" ) );
    UserSettings->endGroup();

    UserSettings->beginGroup( "Presets" );
    TempName = tr( "Purge track to 500 points (Garmin)" );
    TempCommand = ExecName ;
    TempCommand.append( " " );
    TempCommand.append( "-t -i gpx -f %1 -x simplify,count=500 -o gpx,gpxver=1.1 -F %2" );
    //UserSettings->setValue( TempName, TempCommand.arg( tr( "HugeTrack.gpx" ) ).arg( tr( "PurgedTrack.gpx" ) ) );
    UserSettings->setValue( TempName, TempCommand.arg( "gpsbook" ).arg( "gpsbook" ) );
    UserSettings->endGroup();
    UserSettings->beginGroup( "Comments" );
    UserSettings->setValue( TempName, tr( "Reduce the amount of trackpoints to a maximum of 500 (for Garmin devices)" ) );
    UserSettings->endGroup();

    UserSettings->beginGroup( "Presets" );
    TempName = tr( "Purge track for openstreetmap.org usage" );
    TempCommand = ExecName ;
    TempCommand.append( " " );
    TempCommand.append( "-t -i gpx -f %1 -x simplify,error=0.001k -o gpx,gpxver=1.1 -F %2" );
    //UserSettings->setValue( TempName, TempCommand.arg( tr( "OriginalTrack.gpx" ) ).arg( tr( "SimplifiedTrack.gpx" ) ) );
    UserSettings->setValue( TempName, TempCommand.arg( "gpsbook" ).arg( "gpsbook" ) );
    UserSettings->endGroup();
    UserSettings->beginGroup( "Comments" );
    UserSettings->setValue( TempName, tr( "Remove trackpoints which are less than 1m apart from track" ) );
    UserSettings->endGroup();

    /*
    UserSettings->beginGroup( "Presets" );
    TempName = tr( "Purge close points" );
    TempCommand = ExecName ;
    TempCommand.append( " " );
    TempCommand.append( "-t -w -r -N -i geo -f %1 -i geo -f %2 -x position,distance=1m -o mapsend -F %3" );
    UserSettings->setValue( TempName, TempCommand.arg( tr( "Infile1.loc" ) ).arg( tr( "Infile2.loc" ) ).arg( tr( "Outfile.wpt" ) ) );
    UserSettings->endGroup();
    UserSettings->beginGroup( "Comments" );
    UserSettings->setValue( TempName, tr( "Remove points closer than the given distance to adjacent points" ) );
    UserSettings->endGroup();
    */

    /*
    UserSettings->beginGroup( "Presets" );
    TempName = tr( "Purge waypoints based on a file" );
    TempCommand = ExecName ;
    TempCommand.append( " " );
    TempCommand.append( "-w -N -i gpx -f %1 -i csv -f %2 -x duplicate,shortname,all -o gpx,gpxver=1.1 -F %3" );
    UserSettings->setValue( TempName, TempCommand.arg( tr( "WaypointsHuge.gpx" ) ).arg( tr( "WaypointsToDrop.csv" ) ).arg( tr( "WaypointsReduced.gpx" ) ) );
    UserSettings->endGroup();
    UserSettings->beginGroup( "Comments" );
    UserSettings->setValue( TempName, tr( "Removes any waypoint from the data if it is in the exclude file" ) );
    UserSettings->endGroup();
    */

    UserSettings->beginGroup( "Presets" );
    TempName = tr( "Purge data based on polygon and arc files" );
    TempCommand = ExecName ;
    TempCommand.append( " " );
    TempCommand.append( "-w -t -r -N -i gpx -f %1 -x stack,push -x polygon,file=%2 -x stack,swap -x arc,file=%3,distance=1k -x stack,pop,append -x duplicate,shortname -o gpx,gpxver=1.1 -F %4" );
    //UserSettings->setValue( TempName, TempCommand.arg( tr( "Points.gpx" ) ).arg( tr( "MyCounty.txt" ) ).arg( tr( "MyCounty.txt" ) ).arg( tr( "OutsideMyCounty.gpx" ) ) );
    UserSettings->setValue( TempName, TempCommand.arg( "gpsbook" ).arg( tr( "MyCounty.txt" ) ).arg( tr( "MyCounty.txt" ) ).arg( "gpsbook" ) );
    UserSettings->endGroup();
    UserSettings->beginGroup( "Comments" );
    UserSettings->setValue( TempName, tr( "Drop points according to files describing a polygon resp. an arc" ) );
    UserSettings->endGroup();

    UserSettings->beginGroup( "Presets" );
    TempName = tr( "Purge track based on a circle" );
    TempCommand = ExecName ;
    TempCommand.append( " " );
    TempCommand.append( "-t -i gpx -f %1 -x radius,distance=1.5M,lat=30.0,lon=-90.0 -o gpx,gpxver=1.1 -F %2" );
    //UserSettings->setValue( TempName, TempCommand.arg( tr( "Input.gpx" ) ).arg( tr( "Output.wpt" ) ) );
    UserSettings->setValue( TempName, TempCommand.arg( "gpsbook" ).arg( "gpsbook" ) );
    UserSettings->endGroup();
    UserSettings->beginGroup( "Comments" );
    UserSettings->setValue( TempName, tr( "Radius Filter" ) );
    UserSettings->endGroup();

    UserSettings->beginGroup( "Presets" );
    TempName = tr( "Purge trackpoints with an obvious aberration" );
    TempCommand = ExecName ;
    TempCommand.append( " " );
    TempCommand.append( "-t -i gpx -f %1 -x discard,hdop=10,vdop=20,hdopandvdop -o gpx,gpxver=1.1 -F %2" );
    //UserSettings->setValue( TempName, TempCommand.arg( tr( "Infile.gpx" ) ).arg( tr( "Discarded.gpx" ) ) );
    UserSettings->setValue( TempName, TempCommand.arg( "gpsbook" ).arg( "gpsbook" ) );
    UserSettings->endGroup();
    UserSettings->beginGroup( "Comments" );
    UserSettings->setValue( TempName, tr( "Remove points where the GPS device obviously was wrong" ) );
    UserSettings->endGroup();

    UserSettings->beginGroup( "Presets" );
    TempName = tr( "Purge waypoint duplicates" );
    TempCommand = ExecName ;
    TempCommand.append( " " );
    TempCommand.append( "-w -N -i gpx -f %1 -x duplicate,location,shortname -o gpx,gpxver=1.1 -F %2" );
    //UserSettings->setValue( TempName, TempCommand.arg( tr( "WaypointsWithDuplicates.gpx" ) ).arg( tr( "WaypointsWithoutDuplicates.gpx" ) ) );
    UserSettings->setValue( TempName, TempCommand.arg( "gpsbook" ).arg( "gpsbook" ) );
    UserSettings->endGroup();
    UserSettings->beginGroup( "Comments" );
    UserSettings->setValue( TempName, tr( "Remove waypoints with the same name or location of another waypoint" ) );
    UserSettings->endGroup();

    UserSettings->beginGroup( "Presets" );
    TempName = tr( "Remove points based on a polygon file" );
    TempCommand = ExecName ;
    TempCommand.append( " " );
    TempCommand.append( "-w -t -r -N -i gpx -f %1 -x polygon,file=%2 -o gpx,gpxver=1.1 -F %3" );
    //UserSettings->setValue( TempName, TempCommand.arg( tr( "InputFile.loc" ) ).arg( tr( "MyCounty.txt" ) ).arg( tr( "PurgedByPolygon.wpt" ) ) );
    UserSettings->setValue( TempName, TempCommand.arg( "gpsbook" ).arg( tr( "MyCounty.txt" ) ).arg( "gpsbook" ) );
    UserSettings->endGroup();
    UserSettings->beginGroup( "Comments" );
    UserSettings->setValue( TempName, tr( "Drop points according to a file describing a polygon" ) );
    UserSettings->endGroup();

    UserSettings->beginGroup( "Presets" );
    TempName = tr( "Remove points based on an arc file" );
    TempCommand = ExecName ;
    TempCommand.append( " " );
    TempCommand.append( "-w -t -r -N -i gpx -f %1 -x arc,file=%2,distance=1k -o gpx,gpxver=1.1 -F %3" );
    //UserSettings->setValue( TempName, TempCommand.arg( tr( "InputFile.loc" ) ).arg( tr( "MyCounty.txt" ) ).arg( tr( "PurgedByArc.wpt" ) ) );
    UserSettings->setValue( TempName, TempCommand.arg( "gpsbook" ).arg( tr( "MyCounty.txt" ) ).arg( "gpsbook" ) );
    UserSettings->endGroup();
    UserSettings->beginGroup( "Comments" );
    UserSettings->setValue( TempName, tr( "Drop points according to a file describing an arc" ) );
    UserSettings->endGroup();

    UserSettings->beginGroup( "Presets" );
    TempName = tr( "Send waypoints, routes or tracks to Garmin" );
    TempCommand = ExecName ;
    TempCommand.append( " " );
    TempCommand.append( "-w -r -t -N -i gpx -f %1 -o garmin -F usb:" );
    //UserSettings->setValue( TempName, TempCommand.arg( tr( "GpxData.gpx" ) ) );
    UserSettings->setValue( TempName, TempCommand.arg( "gpsbook" ) );
    UserSettings->endGroup();
    UserSettings->beginGroup( "Comments" );
    UserSettings->setValue( TempName, tr( "Upload GPX data like waypoints, routes or tracks to a Garmin device" ) );
    UserSettings->endGroup();

    /*
    UserSettings->beginGroup( "Presets" );
    TempName = tr( "Split routes for Motorrad Routenplaner" );
    TempCommand = ExecName ;
    TempCommand.append( " " );
    TempCommand.append( "-r -N -i gpx -f %1 -o bcr,index=1,name=\"Route1\",radius=6371012 -F %2 -o bcr,index=2,name=\"Route2\",radius=6371012 -F %3" );
    UserSettings->setValue( TempName, TempCommand.arg( tr( "TwoRoutes.gpx" ) ).arg( tr( "Route1.bcr" ) ).arg( tr( "Route2.bcr" ) ) );
    UserSettings->endGroup();
    UserSettings->beginGroup( "Comments" );
    UserSettings->setValue( TempName, tr( "The BCR format only supports one route per file, so the gpx file gets split" ) );
    UserSettings->endGroup();
    */

    UserSettings->beginGroup( "Presets" );
    TempName = tr( "Timeshift a track" );
    TempCommand = ExecName ;
    TempCommand.append( " " );
    TempCommand.append( "-t -i gpx -f %1 -x track,move=+1h -o gpx,gpxver=1.1 -F %2" );
    //UserSettings->setValue( TempName, TempCommand.arg( tr( "OriginalTrack.gpx" ) ).arg( tr( "TimeshiftedTrack.gpx" ) ) );
    UserSettings->setValue( TempName, TempCommand.arg( "gpsbook" ).arg( "gpsbook" ) );
    UserSettings->endGroup();
    UserSettings->beginGroup( "Comments" );
    UserSettings->setValue( TempName, tr( "Shift trackpoint times by one hour" ) );
    UserSettings->endGroup();
}


void SettingsManager::restoreSystemDefaults()
{
    // Try to create a backup of the current configfiles first
    // Attention: If the destination file exists, rename will return false
    // It even can happen that the file cannot be removed or the new one cannot be created
    QFile::rename( SystemSettings->fileName(), SystemSettings->fileName().append( "-" ).append( QDateTime::currentDateTime().toString( "yyyyMMdd-HHmmss" ) ) );

    // Starting to fill the system config file with general settings
    SystemSettings->setValue( "GebabbelVersion", VERSION );

    // Starting to fill the system config file with general settings
    SystemSettings->setValue( "UsageCount", "1" );

    // Ports
    SystemSettings->beginGroup( "DevicePorts" );
    SystemSettings->setValue( "usb:0", tr( "USB port N 1" ) );
    SystemSettings->setValue( "usb:1", tr( "USB port N 2" ) );
    SystemSettings->setValue( "usb:2", tr( "USB port N 3" ) );
#ifdef LINUX
    // TODO: QSettings treats those devices as config file groups
    // is appears in the config as dev/ttyS0, so seems I need to search another way
    SystemSettings->setValue( "/dev/ttyS0", tr( "Serial port 1" ) );
    SystemSettings->setValue( "/dev/ttyS1", tr( "Serial port 2" ) );
    SystemSettings->setValue( "/dev/ttyS2", tr( "Serial port 3" ) );
    SystemSettings->setValue( "/dev/ttyUSB0", tr( "USB to serial port 1" ) );
    SystemSettings->setValue( "/dev/ttyUSB1", tr( "USB to serial port 2" ) );
    SystemSettings->setValue( "/dev/ttyUSB2", tr( "USB to serial port 3" ) );
#endif
#ifdef WINDOWS
    SystemSettings->setValue( "com1", tr( "Serial port 1" ) );
    SystemSettings->setValue( "com2", tr( "Serial port 2" ) );
    SystemSettings->setValue( "com3", tr( "Serial port 3" ) );
#endif
    // I have no clue if there are Mac serial ports
    SystemSettings->endGroup();


    // Defining the IO formats which can be used for direct device communication
    SystemSettings->beginGroup( "DeviceFormats" );

    SystemSettings->beginGroup( "garmin" );
    SystemSettings->setValue( "Name", tr( "Garmin GPS device" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "magellan" );
    SystemSettings->setValue( "Name", tr( "Magellan GPS device" ) );
    SystemSettings->endGroup();

    SystemSettings->endGroup();



    // List of all valid formats according to the gpsbabel webpage
    // Creating one list for inputs and one for outputs is a bit redundant,
    // but allows to distinguish formats which only can be written or read

    // Inputs

    /*
    Template for adding options to inputs and outputs
    			SystemSettings->beginGroup( "Options" );
    				SystemSettings->setValue( "", tr( "" ) );
    				SystemSettings->setValue( "", tr( "" ) );
    				SystemSettings->setValue( "", tr( "" ) );
    			SystemSettings->endGroup();
    */

    SystemSettings->beginGroup( "Input" );

    SystemSettings->beginGroup( "magellan" );
    SystemSettings->setValue( "Name", tr( "Magellan GPS device format" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "baud=", tr( "Numeric value of bitrate. Valid options are 1200, 2400, 4800, 9600, 19200, 57600, and 115200. Example: baud=4800" ) );
    SystemSettings->setValue( "noack", tr( "Suppress use of handshaking in name of speed" ) );
    SystemSettings->setValue( "nukewpt", tr( "Setting this option erases all waypoints in the receiver before doing a transfer" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "baroiq" );
    SystemSettings->setValue( "Name", tr( "Brauniger IQ Series Barograph Download" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "cambridge" );
    SystemSettings->setValue( "Name", tr( "Cambridge/Winpilot glider software" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "cst" );
    SystemSettings->setValue( "Name", tr( "CarteSurTable data file" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "cetus" );
    SystemSettings->setValue( "Name", tr( "Cetus for Palm/OS" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "coastexp" );
    SystemSettings->setValue( "Name", tr( "CoastalExplorer XML" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "csv" );
    SystemSettings->setValue( "Name", tr( "Comma separated values" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "compegps" );
    SystemSettings->setValue( "Name", tr( "CompeGPS data files (.wpt/.trk/.rte)" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "copilot" );
    SystemSettings->setValue( "Name", tr( "CoPilot Flight Planner for Palm/OS" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "coto" );
    SystemSettings->setValue( "Name", tr( "cotoGPS for Palm/OS" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "axim_gpb" );
    SystemSettings->setValue( "Name", tr( "Dell Axim Navigation System file format (.gpb)" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "an1" );
    SystemSettings->setValue( "Name", tr( "DeLorme an1 (drawing) file" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "gpl" );
    SystemSettings->setValue( "Name", tr( "DeLorme GPL" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "saplus" );
    SystemSettings->setValue( "Name", tr( "DeLorme Street Atlas Plus" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "saroute" );
    SystemSettings->setValue( "Name", tr( "DeLorme Street Atlas Route" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "turns_important", tr( "Keep turns. This option only makes sense in conjunction with the 'simplify' filter" ) );
    SystemSettings->setValue( "turns_only", tr( "Only read turns but skip all other points. Only keeps waypoints associated with named turns" ) );
    SystemSettings->setValue( "split", tr( "Split into multiple routes at turns. Create separate routes for each street resp. at each turn point. Cannot be used together with the 'turns_only' or 'turns_important' options" ) );
    SystemSettings->setValue( "controls=", tr( "Read control points (start, end, vias, and stops) as waypoint/route/none. Default value is \"none\". Example: controls=waypoint" ) );
    SystemSettings->setValue( "times", tr( "Read the route as if it was a track, synthesizing times starting from the current time and using the estimated travel times specified in your route file" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "xmap" );
    SystemSettings->setValue( "Name", tr( "DeLorme XMap HH Native .WPT" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "xmap2006" );
    SystemSettings->setValue( "Name", tr( "DeLorme XMap/SAHH 2006 Native .TXT" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "xmapwpt" );
    SystemSettings->setValue( "Name", tr( "DeLorme XMat HH Street Atlas USA .WPT (PPC)" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "easygps" );
    SystemSettings->setValue( "Name", tr( "EasyGPS binary format" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "igc" );
    SystemSettings->setValue( "Name", tr( "FAI/IGC Flight Recorder Data Format" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "fugawi" );
    SystemSettings->setValue( "Name", tr( "Fugawi" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "garmin301" );
    SystemSettings->setValue( "Name", tr( "Garmin 301 Custom position and heartrate" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "glogbook" );
    SystemSettings->setValue( "Name", tr( "Garmin Logbook XML" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "gdb" );
    SystemSettings->setValue( "Name", tr( "Garmin MapSource - gdb" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "mapsource" );
    SystemSettings->setValue( "Name", tr( "Garmin MapSource - mps" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "garmin_txt" );
    SystemSettings->setValue( "Name", tr( "Garmin MapSource - txt (tab delimited)" ) );
    SystemSettings->setValue( "Name", tr( "Garmin MapSource - txt (tab delimited)" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "date=", tr( "Interpret date as specified. The format is written similarly to those in Windows. Example: date=YYYY/MM/DD" ) );
    SystemSettings->setValue( "datum=", tr( "The option datum=\"datum name\" can be used to specify how the datum is interpreted. Supported datum formats can be found in the gpsbabel documentation, Appendix A, Supported Datums. Example: datum=\"NAD27\"" ) );
    SystemSettings->setValue( "time=", tr( "This option specifies the input and output format for the time. The format is written similarly to those in Windows. An example format is 'hh:mm:ss xx'" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "pcx" );
    SystemSettings->setValue( "Name", tr( "Garmin PCX5" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "garmin_poi" );
    SystemSettings->setValue( "Name", tr( "Garmin POI database" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "garmin" );
    SystemSettings->setValue( "Name", tr( "Garmin GPS device" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "get_posn", tr( "Return current position as a waypoint" ) );
    SystemSettings->setValue( "power_off", tr( "Command unit to power itself down" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "geo" );
    SystemSettings->setValue( "Name", tr( "Geocaching.com (.loc)" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "nuke_placer", tr( "If this option is specified, geocache placer information will not be processed" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "gcdb" );
    SystemSettings->setValue( "Name", tr( "GeocachingDB for Palm/OS" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "geonet" );
    SystemSettings->setValue( "Name", tr( "GEOnet Names Server (GNS)" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "geoniche" );
    SystemSettings->setValue( "Name", tr( "GeoNiche (.pdb)" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "kml" );
    SystemSettings->setValue( "Name", tr( "Google Earth (Keyhole) Markup Language" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "google" );
    SystemSettings->setValue( "Name", tr( "Google Maps XML tracks" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "gpilots" );
    SystemSettings->setValue( "Name", tr( "GpilotS" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "gtm" );
    SystemSettings->setValue( "Name", tr( "GPS TrackMaker" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "arc" );
    SystemSettings->setValue( "Name", tr( "GPSBabel arc filter file" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "gpsdrive" );
    SystemSettings->setValue( "Name", tr( "GpsDrive Format" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "gpsdrivetrack" );
    SystemSettings->setValue( "Name", tr( "GpsDrive Format for Tracks" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "gpsman" );
    SystemSettings->setValue( "Name", tr( "GPSman" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "gpspilot" );
    SystemSettings->setValue( "Name", tr( "GPSPilot Tracker for Palm/OS waypoints" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "gpsutil" );
    SystemSettings->setValue( "Name", tr( "gpsutil is a simple command line tool dealing with waypoint data" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "gpx" );
    SystemSettings->setValue( "Name", tr( "Universal GPS XML file format" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "hiketech" );
    SystemSettings->setValue( "Name", tr( "HikeTech" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "holux" );
    SystemSettings->setValue( "Name", tr( "Holux gm-100 waypoints (.wpo)" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "hsandv" );
    SystemSettings->setValue( "Name", tr( "HSA Endeavour Navigator export File" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "ignrando" );
    SystemSettings->setValue( "Name", tr( "IGN Rando track files" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "ktf2" );
    SystemSettings->setValue( "Name", tr( "Kartex 5 Track File" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "kwf2" );
    SystemSettings->setValue( "Name", tr( "Kartex 5 Waypoint File" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "psitrex" );
    SystemSettings->setValue( "Name", tr( "KuDaTa PsiTrex text" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "lowranceusr" );
    SystemSettings->setValue( "Name", tr( "Lowrance USR" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "ignoreicons", tr( "If this option is added, event marker icons are ignored and therefore not converted to waypoints" ) );
    SystemSettings->setValue( "break", tr( "This option breaks track segments into separate tracks when reading a .USR file" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "mapsend" );
    SystemSettings->setValue( "Name", tr( "Magellan Mapsend" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "magnav" );
    SystemSettings->setValue( "Name", tr( "Magellan NAV Companion for Palm/OS waypoints" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "magellanx" );
    SystemSettings->setValue( "Name", tr( "Magellan SD files (as for eXplorist)" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "tef" );
    SystemSettings->setValue( "Name", tr( "Map&Guide 'Tour Exchange Format' XML routes" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "routevia", tr( "Set this option to eliminate calculated route points from the route, only preserving only via stations in route" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "mag_pdb" );
    SystemSettings->setValue( "Name", tr( "Map&Guide to Palm/OS exported files, containing waypoints and routes (.pdb)" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "mapconverter" );
    SystemSettings->setValue( "Name", tr( "Mapopolis.com Mapconverter CSV waypoints" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "mxf" );
    SystemSettings->setValue( "Name", tr( "MapTech Exchange Format waypoints" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    // TODO: Recheck these formats and their file extensions
    SystemSettings->beginGroup( "msroute" );
    SystemSettings->setValue( "Name", tr( "Microsoft AutoRoute 2002 resp. Streets and Trips (pin/route reader)" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "s_and_t" );
    SystemSettings->setValue( "Name", tr( "Microsoft Streets and Trips 2002-2006 waypoints" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "bcr" );
    SystemSettings->setValue( "Name", tr( "Motorrad Routenplaner/Map&Guide routes (.bcr)" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "psp" );
    SystemSettings->setValue( "Name", tr( "MS PocketStreets 2002 Pushpin waypoints; not fully supported yet" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "tpg" );
    SystemSettings->setValue( "Name", tr( "National Geographic Topo waypoints (.tpg)" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "tpo2" );
    SystemSettings->setValue( "Name", tr( "National Geographic Topo 2.x tracks (.tpo)" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "tpo3" );
    SystemSettings->setValue( "Name", tr( "National Geographic Topo 3.x/4.x waypoints, routes and tracks (.tpo)" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "navicache" );
    SystemSettings->setValue( "Name", tr( "Navicache.com XML" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "noretired", tr( "Suppress retired geocaches" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "nmn4" );
    SystemSettings->setValue( "Name", tr( "Navigon Mobile Navigator (.rte)" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "dna" );
    SystemSettings->setValue( "Name", tr( "Navitrak DNA marker format" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "netstumbler" );
    SystemSettings->setValue( "Name", tr( "NetStumbler Summary File (text)" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "nseicon=", tr( "Specifies the name of the icon to use for non-stealth, encrypted access points" ) );
    SystemSettings->setValue( "nsneicon=", tr( "Specifies the name of the icon to use for non-stealth, non-encrypted access points" ) );
    SystemSettings->setValue( "seicon=", tr( "Specifies the name of the icon to use for stealth, encrypted access points" ) );
    SystemSettings->setValue( "sneicon=", tr( "Specifies the name of the icon to use for stealth, non-encrypted access points" ) );
    SystemSettings->setValue( "snmac", tr( "Use the MAC address as the short name for the waypoint. The unmodified SSID is included in the waypoint description" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "nima" );
    SystemSettings->setValue( "Name", tr( "NIMA/GNIS Geographic Names File" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "ozi" );
    SystemSettings->setValue( "Name", tr( "OziExplorer" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "pathaway" );
    SystemSettings->setValue( "Name", tr( "PathAway Database for Palm/OS" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "date", tr( "Specifies the input and output format for the date. The format is written similarly to those in Windows. Example: date=YYMMDD" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "quovadis" );
    SystemSettings->setValue( "Name", tr( "QuoVadis for Palm OS" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "raymarine" );
    SystemSettings->setValue( "Name", tr( "Raymarine Waypoint File (.rwf)" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "cup" );
    SystemSettings->setValue( "Name", tr( "See You flight analysis data" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "sportsim" );
    SystemSettings->setValue( "Name", tr( "Sportsim track files (part of zipped .ssz files)" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "stmsdf" );
    SystemSettings->setValue( "Name", tr( "Suunto Trek Manager STM (.sdf) files" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "stmwpp" );
    SystemSettings->setValue( "Name", tr( "Suunto Trek Manager STM WaypointPlus files" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "openoffice" );
    SystemSettings->setValue( "Name", tr( "Tab delimited fields useful for OpenOffice, Ploticus etc." ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "tomtom" );
    SystemSettings->setValue( "Name", tr( "TomTom POI file" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "tmpro" );
    SystemSettings->setValue( "Name", tr( "TopoMapPro Places File" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "dmtlog" );
    SystemSettings->setValue( "Name", tr( "TrackLogs digital mapping (.trl)" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "tiger" );
    SystemSettings->setValue( "Name", tr( "U.S. Census Bureau Tiger Mapping Service" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "unicsv" );
    SystemSettings->setValue( "Name", tr( "Universal csv with field structure defined in first line" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "vitosmt" );
    SystemSettings->setValue( "Name", tr( "Vito Navigator II tracks" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "wfff" );
    SystemSettings->setValue( "Name", tr( "WiFiFoFum 2.0 for PocketPC XML waypoints" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "aicicon=", tr( "Infrastructure closed icon name" ) );
    SystemSettings->setValue( "aioicon=", tr( "Infrastructure open icon name" ) );
    SystemSettings->setValue( "ahcicon=", tr( "Ad-hoc closed icon name" ) );
    SystemSettings->setValue( "ahoicon=", tr( "Ad-hoc open icon name" ) );
    SystemSettings->setValue( "snmac", tr( "Create shortname based on the MAC hardware address" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "wbt-bin" );
    SystemSettings->setValue( "Name", tr( "Wintec WBT-100/200 data logger format, as created by Wintec's Windows application" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "wbt" );
    SystemSettings->setValue( "Name", tr( "Wintec WBT-100/200 GPS logger download" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "erase", tr( "Add this option to erase the data from the device after the download finished" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "yahoo" );
    SystemSettings->setValue( "Name", tr( "Yahoo Geocode API data" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "xcsv" );
    SystemSettings->setValue( "Name", tr( "Character separated values" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "nmea" );
    SystemSettings->setValue( "Name", tr( "NMEA 0183 sentences" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "gprmc=", tr( "Specifies if GPRMC sentences are processed. If not specified, this option is enabled. Example to disable GPRMC sentences: gprmc=0" ) );
    SystemSettings->setValue( "gpgga=", tr( "Specifies if GPGGA sentences are processed. If not specified, this option is enabled. Example to disable GPGGA sentences: gpgga=0" ) );
    SystemSettings->setValue( "gpvtg=", tr( "Specifies if GPVTG sentences are processed. If not specified, this option is enabled. Example to disable GPVTG sentences: gpvtg=0" ) );
    SystemSettings->setValue( "gpgsa=", tr( "Specifies if GPGSA sentences are processed. If not specified, this option is enabled. Example to disable GPGSA sentences: gpgsa=0" ) );
    SystemSettings->setValue( "get_posn", tr( "Returns the current position as a single waypoint. This option does not require a value" ) );
    SystemSettings->setValue( "baud=", tr( "Specifies the baud rate of the serial connection when used with the real-time tracking option -T. Example: baud=4800" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "garmin_gpi" );
    SystemSettings->setValue( "Name", tr( "Garmin Points of Interest" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "vitovtt" );
    SystemSettings->setValue( "Name", tr( "Vito SmartMap tracks" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "ggv_log" );
    SystemSettings->setValue( "Name", tr( "Geogrid Viewer tracklogs" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "g7towin" );
    SystemSettings->setValue( "Name", tr( "G7ToWin data files" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "tomtom_asc" );
    SystemSettings->setValue( "Name", tr( "TomTom POI file" ) );
    SystemSettings->endGroup();

    SystemSettings->endGroup(); // Inputs


    /*
    Template for adding options to inputs and outputs
    			SystemSettings->beginGroup( "Options" );
    				SystemSettings->setValue( "", tr( "" ) );
    				SystemSettings->setValue( "", tr( "" ) );
    				SystemSettings->setValue( "", tr( "" ) );
    				SystemSettings->setValue( "", tr( "" ) );
    				SystemSettings->setValue( "", tr( "" ) );
    			SystemSettings->endGroup();
    */

    // Outputs
    SystemSettings->beginGroup( "Output" );

    SystemSettings->beginGroup( "magellan" );
    SystemSettings->setValue( "Name", tr( "Magellan GPS device format" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "deficon=", tr( "This option specifies the icon or waypoint type to write for each waypoint on output" ) );
    SystemSettings->setValue( "maxcmts=", tr( "Max number of comments to write. Example: maxcmts=200" ) );
    SystemSettings->setValue( "baud=", tr( "Numeric value of bitrate. Valid options are 1200, 2400, 4800, 9600, 19200, 57600, and 115200. Example: baud=4800" ) );
    SystemSettings->setValue( "noack", tr( "Suppress use of handshaking in name of speed" ) );
    SystemSettings->setValue( "nukewpt", tr( "Setting this option erases all waypoints in the receiver before doing a transfer" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "garmin" );
    SystemSettings->setValue( "Name", tr( "Garmin serial/USB device format" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "snlen=", tr( "Maximum length of generated shortnames" ) );
    SystemSettings->setValue( "snwhite", tr( "Set this option to allow whitespace in generated shortnames" ) );
    SystemSettings->setValue( "deficon=", tr( "Default icon name" ) );
    SystemSettings->setValue( "get_posn", tr( "Return current position as a waypoint" ) );
    SystemSettings->setValue( "category=", tr( "This numeric option adds the specified category number to the waypoints" ) );
    SystemSettings->setValue( "power_off", tr( "Command unit to power itself down" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "baroiq" );
    SystemSettings->setValue( "Name", tr( "Brauniger IQ Series Barograph Download" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "cambridge" );
    SystemSettings->setValue( "Name", tr( "Cambridge/Winpilot glider software" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "cst" );
    SystemSettings->setValue( "Name", tr( "CarteSurTable data file" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "cetus" );
    SystemSettings->setValue( "Name", tr( "Cetus for Palm/OS" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "dbname=", tr( "Internal database name of the output file, which is not equal to the file name" ) );
    SystemSettings->setValue( "appendicon=", tr( "Append icon_descr at the end of a waypoint description" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "coastexp" );
    SystemSettings->setValue( "Name", tr( "CoastalExplorer XML" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "csv" );
    SystemSettings->setValue( "Name", tr( "Comma separated values" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "compegps" );
    SystemSettings->setValue( "Name", tr( "CompeGPS data files (.wpt/.trk/.rte)" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "deficon=", tr( "Default icon name. Example: deficon=RedPin" ) );
    SystemSettings->setValue( "index=", tr( "If the source file contains more than one route or track, use this option to specify which one you want to output (as this format only allows one per file). Example: index=1" ) );
    SystemSettings->setValue( "radius=", tr( "Default radius (proximity)" ) );
    SystemSettings->setValue( "snlen=", tr( "Length of the generated shortnames. Default is 16 characters. Example: snlen=16" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "copilot" );
    SystemSettings->setValue( "Name", tr( "CoPilot Flight Planner for Palm/OS" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "coto" );
    SystemSettings->setValue( "Name", tr( "cotoGPS for Palm/OS" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "zerocat=", tr( "Specify another name for not categorized data (the default reads as Not Assigned). Example: zerocat=NotAssigned" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "an1" );
    SystemSettings->setValue( "Name", tr( "DeLorme an1 (drawing) file" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "type=", tr( "Type of the drawing layer to be created. Valid values include drawing, road, trail, waypoint or track. Example: type=waypoint" ) );
    SystemSettings->setValue( "road=", tr( "Type of the road to be created. Valid values include limited, toll, ramp, us, primary, state, major, ferry, local or editable. As the syntax is very special, see the gpsbabel docs for details" ) );
    SystemSettings->setValue( "nogc", tr( "If geocache data is available, do not write it to the output" ) );
    SystemSettings->setValue( "deficon=", tr( "Symbol to use for point data. Example: deficon=\"Red Flag\"" ) );
    SystemSettings->setValue( "color=", tr( "Colour for lines or mapnote data. Any color as defined by the CSS specification is understood. Example for red color: color=#FF0000" ) );
    SystemSettings->setValue( "zoom=", tr( "A value of 0 will disable reduced symbols when zooming. The default is 10." ) );
    SystemSettings->setValue( "wpt_type=", tr( "Specifies the appearence of point data. Valid values include symbol (the default), text, mapnote, circle or image. Example: wpt_type=symbol" ) );
    SystemSettings->setValue( "radius=", tr( "If the waypoint type is circle, this option is used to specify its radius. The default is 0.1 miles (m). Kilometres also can be used (k). Example: wpt_type=circle,radius=0.01k" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "gpl" );
    SystemSettings->setValue( "Name", tr( "DeLorme GPL" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "saplus" );
    SystemSettings->setValue( "Name", tr( "DeLorme Street Atlas Plus" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "xmap" );
    SystemSettings->setValue( "Name", tr( "DeLorme XMap HH Native .WPT" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "xmap2006" );
    SystemSettings->setValue( "Name", tr( "DeLorme XMap/SAHH 2006 Native .TXT" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "xmapwpt" );
    SystemSettings->setValue( "Name", tr( "DeLorme XMat HH Street Atlas USA .WPT (PPC)" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "easygps" );
    SystemSettings->setValue( "Name", tr( "EasyGPS binary format" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "igc" );
    SystemSettings->setValue( "Name", tr( "FAI/IGC Flight Recorder Data Format" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "timeadj=", tr( "Barograph to GPS time diff. Either use auto or an integer value for seconds. Example: timeadj=auto" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "gpssim" );
    SystemSettings->setValue( "Name", tr( "Franson GPSGate Simulation" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "wayptspd=", tr( "This option specifies the speed of the simulation in knots per hour" ) );
    SystemSettings->setValue( "split", tr( "Adding this option splits the output into multiple files using the output filename as a base. Waypoints, any route and any track will result in an additional file" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "fugawi" );
    SystemSettings->setValue( "Name", tr( "Fugawi" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "garmin301" );
    SystemSettings->setValue( "Name", tr( "Garmin 301 Custom position and heartrate" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "glogbook" );
    SystemSettings->setValue( "Name", tr( "Garmin Logbook XML" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "gdb" );
    SystemSettings->setValue( "Name", tr( "Garmin MapSource - gdb" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "cat=", tr( "This option specifies the default category for gdb output. It should be a number from 1 to 16" ) );
    SystemSettings->setValue( "ver=", tr( "Version of gdb file to generate (1 or 2)." ) );
    SystemSettings->setValue( "via", tr( "Drop calculated hidden points (route points that do not have an equivalent waypoint." ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "mapsource" );
    SystemSettings->setValue( "Name", tr( "Garmin MapSource - mps" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "snlen=", tr( "Maximum length of generated shortnames" ) );
    SystemSettings->setValue( "snwhite", tr( "Set this option to allow whitespace in generated shortnames" ) );
    SystemSettings->setValue( "mpsverout=", tr( "Version of mapsource file to generate (3, 4 or 5)" ) );
    SystemSettings->setValue( "mpsmergeout", tr( "Merges output with an already existing file" ) );
    SystemSettings->setValue( "mpsusedepth", tr( "Use depth values on output (the default is to ignore them)" ) );
    SystemSettings->setValue( "mpsuseprox", tr( "Use proximity values on output (the default is to ignore them)" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "garmin_txt" );
    SystemSettings->setValue( "Name", tr( "Garmin MapSource - txt (tab delimited)" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "date=", tr( "Interpret date as specified. The format is written similarly to those in Windows. Example: date=YYYY/MM/DD" ) );
    SystemSettings->setValue( "datum=", tr( "The option datum=\"datum name\" can be used to specify how the datum is interpreted. Supported datum formats can be found in the gpsbabel documentation, Appendix A, Supported Datums. Example: datum=\"NAD27\"" ) );
    SystemSettings->setValue( "dist=", tr( "This option specifies the unit to be used when outputting distance values. Valid values are M for metric (meters/kilometres) or S for statute (feet/miles)" ) );
    SystemSettings->setValue( "grid=", tr( "This value specifies the grid to be used on write. Idx or short are valid parameters for this option" ) );
    SystemSettings->setValue( "prec=", tr( "This option specifies the precision to be used when writing coordinate values. Precision is the number of digits after the decimal point. The default precision is 3" ) );
    SystemSettings->setValue( "temp=", tr( "This option specifies the unit to be used when writing temperature values. Valid values are C for Celsius or F for Fahrenheit" ) );
    SystemSettings->setValue( "time=", tr( "This option specifies the input and output format for the time. The format is written similarly to those in Windows. An example format is 'hh:mm:ss xx'" ) );
    SystemSettings->setValue( "utc=", tr( "This option specifies the local time zone to use when writing times. It is specified as an offset from Universal Coordinated Time (UTC) in hours. Valid values are from -23 to +23" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "pcx" );
    SystemSettings->setValue( "Name", tr( "Garmin PCX5" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "deficon=", tr( "Default icon name" ) );
    SystemSettings->setValue( "cartoexploreur", tr( "Write tracks compatible with Carto Exploreur" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "garmin_poi" );
    SystemSettings->setValue( "Name", tr( "Garmin POI database" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "gtrnctr" );
    SystemSettings->setValue( "Name", tr( "Garmin Training Center xml" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "geo" );
    SystemSettings->setValue( "Name", tr( "Geocaching.com .loc" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "deficon=", tr( "This option specifies the icon or waypoint type to write for each waypoint" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "gcdb" );
    SystemSettings->setValue( "Name", tr( "GeocachingDB for Palm/OS" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "geonet" );
    SystemSettings->setValue( "Name", tr( "GEOnet Names Server (GNS)" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "geoniche" );
    SystemSettings->setValue( "Name", tr( "GeoNiche (.pdb)" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "dbname=", tr( "Internal database name of the output file, which is not equal to the file name" ) );
    SystemSettings->setValue( "category=", tr( "This numeric option adds the specified category number to the waypoints" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "kml" );
    SystemSettings->setValue( "Name", tr( "Google Earth (Keyhole) Markup Language" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "deficon=", tr( "This option specifies the default name for waypoint icons " ) );
    SystemSettings->setValue( "lines=", tr( "Per default lines are drawn between points in tracks and routes. Example to disable line-drawing: lines=0" ) );
    SystemSettings->setValue( "points=", tr( "Per default placemarks for tracks and routes are drawn. Example to disable drawing of placemarks: points=0" ) );
    SystemSettings->setValue( "line_width=", tr( "Per default lines are drawn in a width of 6 pixels. Example to get thinner lines: line_width=3" ) );
    SystemSettings->setValue( "line_color=", tr( "This option specifies the line color as a hexadecimal number in AABBGGRR format, where A is alpha, B is blue, G is green, and R is red" ) );
    SystemSettings->setValue( "floating=", tr( "Per default this option is zero, so that altitudes are clamped to the ground. When this option is nonzero, altitudes are allowed to float above or below the ground surface. Example: floating=1" ) );
    SystemSettings->setValue( "extrude=", tr( "Specifies whether Google Earth should draw lines from trackpoints to the ground or not. It defaults to '0', which means no extrusion lines are drawn. Example to set the lines: extrude=1" ) );
    SystemSettings->setValue( "trackdata=", tr( "Per default, extended data for trackpoints (computed speed, timestamps, and so on) is included. Example to reduce the size of the generated file substantially: trackdata=0" ) );
    SystemSettings->setValue( "units=", tr( "Units used when writing comments. Specify 's' for \"statute\" (miles, feet etc.) or 'm' for \"metric\". Example: units=m" ) );
    SystemSettings->setValue( "labels=", tr( "Display labels on track and routepoints. Example to switch them off: labels=0" ) );
    SystemSettings->setValue( "max_position_points=", tr( "This option allows you to specify the number of points kept in the 'snail trail' generated in the realtime tracking mode" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "gpilots" );
    SystemSettings->setValue( "Name", tr( "GpilotS" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "dbname=", tr( "Internal database name of the output file, which is not equal to the file name" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "gtm" );
    SystemSettings->setValue( "Name", tr( "GPS TrackMaker" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "arc" );
    SystemSettings->setValue( "Name", tr( "GPSBabel arc filter file" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "gpsdrive" );
    SystemSettings->setValue( "Name", tr( "GpsDrive Format" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "gpsdrivetrack" );
    SystemSettings->setValue( "Name", tr( "GpsDrive Format for Tracks" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "gpsman" );
    SystemSettings->setValue( "Name", tr( "GPSman" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "gpspilot" );
    SystemSettings->setValue( "Name", tr( "GPSPilot Tracker for Palm/OS waypoints" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "dbname=", tr( "Internal database name of the output file, which is not equal to the file name" ) );
    SystemSettings->endGroup();

    SystemSettings->endGroup();

    SystemSettings->beginGroup( "gpsutil" );
    SystemSettings->setValue( "Name", tr( "gpsutil is a simple command line tool dealing with waypoint data" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "gpx" );
    SystemSettings->setValue( "Name", tr( "Universal GPS XML file format" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "snlen=", tr( "Length of generated shortnames" ) );
    SystemSettings->setValue( "suppresswhite", tr( "Suppress whitespace in generated shortnames" ) );
    SystemSettings->setValue( "logpoint", tr( "Create waypoints from geocache log entries" ) );
    SystemSettings->setValue( "urlbase=", tr( "Base URL for link tags" ) );
    SystemSettings->setValue( "gpxver=", tr( "Target GPX version. The default version is 1.0, but you can even specify 1.1" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "hiketech" );
    SystemSettings->setValue( "Name", tr( "HikeTech" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "holux" );
    SystemSettings->setValue( "Name", tr( "Holux gm-100 waypoints (.wpo)" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "hsandv" );
    SystemSettings->setValue( "Name", tr( "HSA Endeavour Navigator export File" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "html" );
    SystemSettings->setValue( "Name", tr( "Write waypoints to HTML" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "stylesheet=", tr( "Use this option to specify a CSS style sheet to be used with the resulting HTML file" ) );
    SystemSettings->setValue( "encrypt", tr( "Use this option to encrypt hints from Groundspeak GPX files using ROT13" ) );
    SystemSettings->setValue( "logs", tr( "Use this option to include Groundspeak cache logs in the created document" ) );
    SystemSettings->setValue( "degformat=", tr( "Defines the format of the coordinates. Supported formats include decimal degrees (degformat=ddd), decimal minutes (degformat=dmm) or degrees, minutes, seconds (degformat=dms). If this option is not specified, the default is dmm" ) );
    SystemSettings->setValue( "altunits", tr( "This option should be 'f' if you want the altitude expressed in feet and 'm' for meters. The default is 'f'" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "ignrando" );
    SystemSettings->setValue( "Name", tr( "IGN Rando track files" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "index=", tr( "If the source file contains more than one route or track, use this option to specify which one you want to output (as this format only allows one per file). Example: index=1" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "ktf2" );
    SystemSettings->setValue( "Name", tr( "Kartex 5 Track File" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "kwf2" );
    SystemSettings->setValue( "Name", tr( "Kartex 5 Waypoint File" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "psitrex" );
    SystemSettings->setValue( "Name", tr( "KuDaTa PsiTrex text" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "lowranceusr" );
    SystemSettings->setValue( "Name", tr( "Lowrance USR" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "merge", tr( "This option merges all tracks into a single track with multiple segments" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "maggeo" );
    SystemSettings->setValue( "Name", tr( "Magellan Explorist Geocaching waypoints (extension: .gs)." ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "mapsend" );
    SystemSettings->setValue( "Name", tr( "Magellan Mapsend" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "trkver=", tr( "MapSend version TRK file to generate (3 or 4). Example: trkver=3" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "magnav" );
    SystemSettings->setValue( "Name", tr( "Magellan NAV Companion for Palm/OS waypoints" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "magellanx" );
    SystemSettings->setValue( "Name", tr( "Magellan SD files (as for eXplorist)" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "deficon=", tr( "This option specifies the icon or waypoint type to write for each waypoint on output" ) );
    SystemSettings->setValue( "maxcmts=", tr( "Max number of comments to write. Example: maxcmts=200" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "mapconverter" );
    SystemSettings->setValue( "Name", tr( "Mapopolis.com Mapconverter CSV waypoints" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "mxf" );
    SystemSettings->setValue( "Name", tr( "MapTech Exchange Format waypoints" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "s_and_t" );
    SystemSettings->setValue( "Name", tr( "Microsoft Streets and Trips 2002-2006 waypoints" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "bcr" );
    SystemSettings->setValue( "Name", tr( "Motorrad Routenplaner (Map&Guide) routes (bcr files)" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "name=", tr( "Specifies the name of the route to create" ) );
    SystemSettings->setValue( "index=", tr( "If the source file contains more than one route or track, use this option to specify which one you want to output (as this format only allows one per file). Example: index=1" ) );
    SystemSettings->setValue( "radius=", tr( "Radius of our big earth (default 6371000 meters). Careful experimentation with this value may help to reduce conversion errors" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "psp" );
    SystemSettings->setValue( "Name", tr( "MS PocketStreets 2002 Pushpin waypoints; not fully supported yet" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "tpg" );
    SystemSettings->setValue( "Name", tr( "National Geographic Topo .tpg (waypoints)" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "datum=", tr( "The option datum=\"datum name\" can be used to override the default of NAD27 (N. America 1927 mean) which is correct for the continental U.S. Supported datum formats can be found in the gpsbabel documentation, Appendix A, Supported Datums. Example: datum=\"NAD27\"" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "nmn4" );
    SystemSettings->setValue( "Name", tr( "Navigon Mobile Navigator .rte files" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "index=", tr( "If the source file contains more than one route or track, use this option to specify which one you want to output (as this format only allows one per file). Example: index=1" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "dna" );
    SystemSettings->setValue( "Name", tr( "Navitrak DNA marker format" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "netstumbler" );
    SystemSettings->setValue( "Name", tr( "NetStumbler Summary File (text)" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "nseicon=", tr( "Specifies the name of the icon to use for non-stealth, encrypted access points" ) );
    SystemSettings->setValue( "nsneicon=", tr( "Specifies the name of the icon to use for non-stealth, non-encrypted access points" ) );
    SystemSettings->setValue( "seicon=", tr( "Specifies the name of the icon to use for stealth, encrypted access points" ) );
    SystemSettings->setValue( "sneicon=", tr( "Specifies the name of the icon to use for stealth, non-encrypted access points" ) );
    SystemSettings->setValue( "snmac", tr( "Use the MAC address as the short name for the waypoint. The unmodified SSID is included in the waypoint description" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "nima" );
    SystemSettings->setValue( "Name", tr( "NIMA/GNIS Geographic Names File" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "ozi" );
    SystemSettings->setValue( "Name", tr( "OziExplorer" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "snlen=", tr( "The maximum synthesized shortname length" ) );
    SystemSettings->setValue( "snwhite", tr( "Set this option to allow whitespace in generated shortnames" ) );
    SystemSettings->setValue( "snupper", tr( "Allow UPPERCASE CHARACTERS in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique", tr( "Make synthesized shortnames unique" ) );
    SystemSettings->setValue( "wptfgcolor=", tr( "Waypoint foreground color" ) );
    SystemSettings->setValue( "wptbgcolor=", tr( "Waypoint background color" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "palmdoc" );
    SystemSettings->setValue( "Name", tr( "PalmDoc Output" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "nosep", tr( "Use this option to suppress the dashed lines between waypoints" ) );
    SystemSettings->setValue( "dbname=", tr( "Internal database name of the output file, which is not equal to the file name. Example: dbname=Unfound" ) );
    SystemSettings->setValue( "encrypt", tr( "Use this option to encrypt hints from Groundspeak GPX files with ROT13" ) );
    SystemSettings->setValue( "logs", tr( "Use this option to include Groundspeak cache logs in the created document if there are any" ) );
    SystemSettings->setValue( "bookmarks_short", tr( "If you would like the generated bookmarks to start with the short name for the waypoint, specify this option" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "pathaway" );
    SystemSettings->setValue( "Name", tr( "PathAway Database for Palm/OS" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "dbname=", tr( "Internal database name of the output file, which is not equal to the file name" ) );
    SystemSettings->setValue( "date", tr( "Specifies the input and output format for the date. The format is written similarly to those in Windows. Example: date=YYMMDD" ) );
    SystemSettings->setValue( "deficon=", tr( "Default icon name" ) );
    SystemSettings->setValue( "snlen=", tr( "Length of generated shortnames" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "quovadis" );
    SystemSettings->setValue( "Name", tr( "QuoVadis for Palm OS" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "dbname=", tr( "Internal database name of the output file, which is not equal to the file name" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "raymarine" );
    SystemSettings->setValue( "Name", tr( "Raymarine Waypoint File (.rwf)" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "location", tr( "Default location" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "cup" );
    SystemSettings->setValue( "Name", tr( "See You flight analysis data" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "sportsim" );
    SystemSettings->setValue( "Name", tr( "Sportsim track files (part of zipped .ssz files)" ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "stmsdf" );
    SystemSettings->setValue( "Name", tr( "Suunto Trek Manager (STM) .sdf files" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "index=", tr( "If the source file contains more than one route or track, use this option to specify which one you want to output (as this format only allows one per file). Example: index=1" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "stmwpp" );
    SystemSettings->setValue( "Name", tr( "Suunto Trek Manager (STM) WaypointPlus files" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "index=", tr( "If the source file contains more than one route or track, use this option to specify which one you want to output (as this format only allows one per file). Example: index=1" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "openoffice" );
    SystemSettings->setValue( "Name", tr( "Tab delimited fields useful for OpenOffice, Ploticus etc." ) );
    // This format is derived from the xcsv format, so it has all of the same options as that format.
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "text" );
    SystemSettings->setValue( "Name", tr( "Textual Output" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "nosep", tr( "Use this option to suppress the dashed lines between waypoints" ) );
    SystemSettings->setValue( "encrypt", tr( "Use this option to encrypt hints from Groundspeak GPX files using ROT13" ) );
    SystemSettings->setValue( "logs", tr( "Use this option to include Groundspeak cache logs in the created document" ) );
    SystemSettings->setValue( "degformat=", tr( "Defines the format of the coordinates. Supported formats include decimal degrees (degformat=ddd), decimal minutes (degformat=dmm) or degrees, minutes, seconds (degformat=dms). If this option is not specified, the default is dmm" ) );
    SystemSettings->setValue( "altunits", tr( "This option should be 'f' if you want the altitude expressed in feet and 'm' for meters. The default is 'f'" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "tomtom" );
    SystemSettings->setValue( "Name", tr( "TomTom POI file" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "tmpro" );
    SystemSettings->setValue( "Name", tr( "TopoMapPro Places File" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "dmtlog" );
    SystemSettings->setValue( "Name", tr( "TrackLogs digital mapping (.trl)" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "index=", tr( "If the source file contains more than one route or track, use this option to specify which one you want to output (as this format only allows one per file). Example: index=1" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "tiger" );
    SystemSettings->setValue( "Name", tr( "U.S. Census Bureau Tiger Mapping Service" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "nolabels", tr( "No labels on the pins are generated, thus the descriptions of the waypoints are dropped" ) );
    SystemSettings->setValue( "genurl", tr( "Generate file with lat/lon for centering map. Example: genurl=tiger.ctr" ) );
    SystemSettings->setValue( "margin", tr( "Margin for map in degrees or percentage. Only useful in conjunction with the genurl option" ) );
    SystemSettings->setValue( "snlen=", tr( "The snlen option controls the maximum length of generated shortnames" ) );
    SystemSettings->setValue( "oldthresh=", tr( "Days after which points are considered old" ) );
    SystemSettings->setValue( "oldmarker=", tr( "This option specifies the pin to be used if a waypoint has a creation time newer than specified by the 'oldthresh' option. The default is redpin" ) );
    SystemSettings->setValue( "newmarker=", tr( "This option specifies the pin to be used if a waypoint has a creation time older than specified by the 'oldthresh' option. The default is greenpin" ) );
    SystemSettings->setValue( "suppresswhite", tr( "Suppress whitespace in generated shortnames" ) );
    SystemSettings->setValue( "unfoundmarker", tr( "Marker type for unfound points" ) );
    SystemSettings->setValue( "xpixels", tr( "Lets you specify the number of pixels to be generated by the Tiger server along the horizontal axis when using the 'genurl' option" ) );
    SystemSettings->setValue( "ypixels", tr( "Lets you specify the number of pixels to be generated by the Tiger server along the vertical axis when using the 'genurl' option" ) );
    SystemSettings->setValue( "iconismarker", tr( "The icon description already is the marker" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "vcard" );
    SystemSettings->setValue( "Name", tr( "Vcard Output (for iPod)" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "encrypt", tr( "By default geocaching hints are unencrypted; use this option to encrypt them using ROT13" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "vitosmt" );
    SystemSettings->setValue( "Name", tr( "Vito Navigator II tracks" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "yahoo" );
    SystemSettings->setValue( "Name", tr( "Yahoo Geocode API data" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "addrsep=", tr( "Street addresses will be added to the notes field of waypoints, separated by ,  per default. Use this option to specify another delimiter. Example: addrsep=\" - \"" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "xcsv" );
    SystemSettings->setValue( "Name", tr( "Character separated values" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "style=", tr( "Path to the xcsv style file. See the appendix C of the gpsbabel Documentation for details" ) );
    SystemSettings->setValue( "snlen=", tr( "Forbids (0) or allows (1) long names in synthesized shortnames" ) );
    SystemSettings->setValue( "snwhite=", tr( "Forbids (0) or allows (1) white spaces in synthesized shortnames" ) );
    SystemSettings->setValue( "snupper=", tr( "Disables (0) or enables (1) UPPERCASE NAMES in synthesized shortnames" ) );
    SystemSettings->setValue( "snunique=", tr( "Disables (0) or enables (1) unique synthesized shortnames" ) );
    SystemSettings->setValue( "urlbase=", tr( "The value specified will be prepended to URL names (e.g. turn relative paths to absolute paths)" ) );
    SystemSettings->setValue( "prefer_shortnames=", tr( "Use shortname (0) instead of the description (1) of the waypoints to synthesize shortnames" ) );
    SystemSettings->setValue( "datum=", tr( "Specifies the datum format to use. See the appendix A of the gpsbabel documentation for supported formats" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "nmea" );
    SystemSettings->setValue( "Name", tr( "NMEA 0183 sentences" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "snlen=", tr( "Max length of waypoint name to write" ) );
    SystemSettings->setValue( "gprmc=", tr( "Specifies if GPRMC sentences are processed. If not specified, this option is enabled. Example to disable GPRMC sentences: gprmc=0" ) );
    SystemSettings->setValue( "gpgga=", tr( "Specifies if GPGGA sentences are processed. If not specified, this option is enabled. Example to disable GPGGA sentences: gpgga=0" ) );
    SystemSettings->setValue( "gpvtg=", tr( "Specifies if GPVTG sentences are processed. If not specified, this option is enabled. Example to disable GPVTG sentences: gpvtg=0" ) );
    SystemSettings->setValue( "gpgsa=", tr( "Specifies if GPGSA sentences are processed. If not specified, this option is enabled. Example to disable GPGSA sentences: gpgsa=0" ) );
    SystemSettings->setValue( "date=", tr( "Complete date-free tracks with given date (YYYYMMDD). Example: date=20071224" ) );
    SystemSettings->setValue( "get_posn", tr( "Returns the current position as a single waypoint. This option does not require a value" ) );
    SystemSettings->setValue( "pause=", tr( "Decimal seconds to pause between groups of strings. Example for one second: pause=10" ) );
    SystemSettings->setValue( "append_positioning", tr( "Append realtime positioning data to the output file instead of truncating. This option does not require a value" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "garmin_gpi" );
    SystemSettings->setValue( "Name", tr( "Garmin Points of Interest" ) );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "bitmap=", tr( "Use this bitmap, 24x24 or smaller, 24 or 32 bit RGB colors or 8 bit indexed colors. Example: bitmap=\"tux.bmp\"" ) );
    SystemSettings->setValue( "category=", tr( "The default category is \"My points\". Example to change this: category=\"Best Restaurants\"" ) );
    SystemSettings->setValue( "hide", tr( "Use this if you don't want a bitmap to be shown on the device. This option does not require a value" ) );
    SystemSettings->setValue( "descr", tr( "Add descriptions to list views of the device. This option does not require a value" ) );
    SystemSettings->setValue( "notes", tr( "Add notes to list views of the device. This option does not require a value" ) );
    SystemSettings->setValue( "position", tr( "Add position to the address field as seen in list views of the device. This option does not require a value" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "ggv_log" );
    SystemSettings->setValue( "Name", tr( "Geogrid Viewer tracklogs" ) );
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "tomtom_asc" );
    SystemSettings->setValue( "Name", tr( "TomTom POI file" ) );
    SystemSettings->endGroup();

    SystemSettings->endGroup(); // Outputs





    /*
    Template for adding options to inputs and outputs
    			SystemSettings->beginGroup( "Options" );
    				SystemSettings->setValue( "", tr( "" ) );
    				SystemSettings->setValue( "", tr( "" ) );
    				SystemSettings->setValue( "", tr( "" ) );
    			SystemSettings->endGroup();
    */






    // Filters
    SystemSettings->beginGroup( "Filter" );
    // TODO: Create cool images for filter types and options

    SystemSettings->beginGroup( "arc" );
    SystemSettings->setValue( "Name", tr( "Only keep points within a certain distance of the given arc file" ) );
    SystemSettings->setValue( "Image", ".png" );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "file=", tr( "File containing the vertices of the polygonal arc (required). Example: file=MyArcFile.txt" ) );
    SystemSettings->setValue( "distance=", tr( "Distance from the polygonal arc (required). The default unit is miles, but it is recommended to always specify the unit. Examples: distance=3M or distance=5K" ) );
    SystemSettings->setValue( "exclude", tr( "Only keep points outside the given distance instead of including them (optional). Inverts the behaviour of this filter." ) );
    SystemSettings->setValue( "points", tr( "Use distance from the vertices instead of the lines (optional). Inverts the behaviour of this filter to a multi point filter instead of an arc filter (similar to the radius filter)" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "discard" );
    SystemSettings->setValue( "Name", tr( "Remove unreliable points with high dilution of precision (horizontal and/or vertical)" ) );
    SystemSettings->setValue( "Image", ".png" );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "hdop=", tr( "Remove waypoints with horizontal dilution of precision higher than the given value. Example: hdop=10" ) );
    SystemSettings->setValue( "vdop=", tr( "Remove waypoints with vertical dilution of precision higher than the given value. Example: vdop=20" ) );
    SystemSettings->setValue( "hdopandvdop", tr( "Only remove waypoints with vertical and horizontal dilution of precision higher than the given values. Requires both hdop and vdop to be specified. Example: hdop=10,vdop=20,hdopandvdop" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "duplicate" );
    SystemSettings->setValue( "Name", tr( "This filter will work on waypoints and remove duplicates, either based on their name, location or on a corrections file" ) );
    SystemSettings->setValue( "Image", ".png" );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "shortname", tr( "Remove duplicated waypoints based on their name. A, B, B and C will result in A, B and C" ) );
    SystemSettings->setValue( "location", tr( "Remove duplicated waypoints based on their coordinates. A, B, B and C will result in A, B and C" ) );
    SystemSettings->setValue( "all", tr( "Suppress all instances of duplicates. A, B, B and C will result in A and C" ) );
    SystemSettings->setValue( "correct", tr( "Keep the first occurence, but use the coordintaes of later points and drop them" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "interpolate" );
    SystemSettings->setValue( "Name", tr( "This filter will work on tracks or routes and fills gaps between points that are more than the specified amount of seconds, kilometres or miles apart" ) );
    SystemSettings->setValue( "Image", ".png" );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "time=", tr( "Adds points if two points are more than the specified time interval in seconds apart. Cannot be used in conjunction with routes, as route points don't include a time stamp. Example: time=10" ) );
    SystemSettings->setValue( "distance=", tr( "Adds points if two points are more than the specified distance in miles or kilometres apart. Example: distance=1k or distance=2m" ) );
    SystemSettings->setValue( "route", tr( "Work on a route instead of a track" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "nuketypes" );
    SystemSettings->setValue( "Name", tr( "Remove waypoints, tracks, or routes from the data. It's even possible to remove all three datatypes from the data, though this doesn't make much sense. Example: nuketypes,waypoints,routes,tracks" ) );
    SystemSettings->setValue( "Image", ".png" );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "waypoints", tr( "Remove all waypoints from the data. Example: waypoints" ) );
    SystemSettings->setValue( "routes", tr( "Remove all routes from the data. Example: routes" ) );
    SystemSettings->setValue( "tracks", tr( "Remove all tracks from the data. Example: tracks" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "polygon" );
    SystemSettings->setValue( "Name", tr( "Remove points outside a polygon specified by a special file. Example: polygon,file=MyCounty.txt" ) );
    SystemSettings->setValue( "Image", ".png" );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "file=", tr( "File containing the vertices of the polygon (required). Example: file=MyCounty.txt" ) );
    SystemSettings->setValue( "exclude", tr( "Adding this option inverts the behaviour of this filter. It then keeps points outside the polygon instead of points inside. Example: file=MyCounty.txt,exclude" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "position" );
    SystemSettings->setValue( "Name", tr( "Remove points close to other points, as specified by distance. Examples: position,distance=30f or position,distance=40m" ) );
    SystemSettings->setValue( "Image", ".png" );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "distance=", tr( "The distance is required. Distances can be specified by feet or meters (feet is the default). Examples: distance=0.1f or distance=0.1m" ) );
    SystemSettings->setValue( "all", tr( "This option removes all points that are within the specified distance of one another, rather than leaving just one of them" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "radius" );
    SystemSettings->setValue( "Name", tr( "Includes or excludes waypoints based on their proximity to a central point. The remaining points are sorted so that points closer to the center appear earlier in the output file" ) );
    SystemSettings->setValue( "Image", ".png" );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "lat=", tr( "Latitude for center point in miles (m) or kilometres (k). (D.DDDDD) (required)" ) );
    SystemSettings->setValue( "lon=", tr( "Longitude for center point (D.DDDDD) (required)" ) );
    SystemSettings->setValue( "distance=", tr( "Distance from center (required). Example: distance=1.5k,lat=30.0,lon=-90.0" ) );
    SystemSettings->setValue( "exclude", tr( "Don't keep but remove points close to the center. Example: distance=1.5k,lat=30.0,lon=-90.0,exclude" ) );
    SystemSettings->setValue( "nosort", tr( "Don't sort the remaining points by their distance to the center. Example: distance=1.5k,lat=30.0,lon=-90.0,nosort" ) );
    SystemSettings->setValue( "maxcount=", tr( "Limit the number of kept points. Example: distance=1.5k,lat=30.0,lon=-90.0,maxcount=400" ) );
    SystemSettings->setValue( "asroute=", tr( "Create a named route containing the resulting waypoints. Example: distance=1.5k,lat=30.0,lon=-90.0,asroute=MyRoute" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "reverse" );
    SystemSettings->setValue( "Name", tr( "Reverse a route or track. Rarely needed as most applications can do this by themselves" ) );
    SystemSettings->setValue( "Image", ".png" );
    SystemSettings->beginGroup( "Options" );
    // This filter doesn't support any options
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "simplify" );
    SystemSettings->setValue( "Name", tr( "Simplify routes or tracks, either by the amount of points (see the count option) or the maximum allowed aberration (see the error option)" ) );
    SystemSettings->setValue( "Image", ".png" );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "count=", tr( "Maximum number of points to keep. Example: count=500" ) );
    SystemSettings->setValue( "error=", tr( "Drop points except the given error value in miles (m) or kilometres (k) gets reached. Example: error=0.001k" ) );
    SystemSettings->setValue( "crosstrack", tr( "Use cross-track error (this is the default). Removes points close to a line drawn between the two points adjacent to it" ) );
    SystemSettings->setValue( "length", tr( "This disables the default crosstrack option. Instead, points that have less effect to the overall length of the path are removed" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "stack" );
    SystemSettings->setValue( "Name", tr( "Advanced stack operations on tracks. Please see the gpsbabel docs for details" ) );
    SystemSettings->setValue( "Image", ".png" );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "push", tr( "Push waypoint list onto stack" ) );
    SystemSettings->setValue( "pop", tr( "Pop waypoint list from stack" ) );
    SystemSettings->setValue( "swap", tr( "Swap waypoint list with <depth> item on stack (requires the depth option to be set)" ) );
    SystemSettings->setValue( "copy", tr( "(push) Copy waypoint list" ) );
    SystemSettings->setValue( "append", tr( "(pop) Append list" ) );
    SystemSettings->setValue( "discard", tr( "(pop) Discard top of stack" ) );
    SystemSettings->setValue( "replace", tr( "(pop) Replace list (default)" ) );
    SystemSettings->setValue( "depth", tr( "(swap) Item to use (default=1)" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "sort" );
    SystemSettings->setValue( "Name", tr( "Sort waypoints by exactly one of the available options" ) );
    SystemSettings->setValue( "Image", ".png" );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "gcid", tr( "Sort by numeric geocache ID" ) );
    SystemSettings->setValue( "shortname", tr( "Sort by waypoint short name" ) );
    SystemSettings->setValue( "description", tr( "Sort by waypoint description" ) );
    SystemSettings->setValue( "time", tr( "Sort waypoints chronologically" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "track" );
    SystemSettings->setValue( "Name", tr( "Manipulate track lists (timeshifting, time or distance based cutting, combining, splitting, merging)" ) );
    SystemSettings->setValue( "Image", ".png" );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "move=", tr( "Correct trackpoint timestamps by a delta. Example: move=+1h" ) );
    SystemSettings->setValue( "pack", tr( "Combine multiple tracks into one. Useful if you have multiple tracks of one tour, maybe caused by an overnight stop." ) );
    SystemSettings->setValue( "split", tr( "Split by date or time interval. Example to split a single track into separate tracks for each day: split,title=\"ACTIVE LOG # %Y%m%d\". See the gpsbabel docs for more examples" ) );
    SystemSettings->setValue( "merge", tr( "Merge multiple tracks for the same way, e.g. as recorded by two GPS devices, sorted by the point's timestamps. Example: merge,title=\"COMBINED LOG\"" ) );
    SystemSettings->setValue( "name=", tr( "Only use points of tracks whose (non case-sensitive) title matches the given name" ) );
    SystemSettings->setValue( "start=", tr( "Only use trackpoints after this timestamp. Example (year, month, day, hour): start=2007010110,stop=2007010118" ) );
    SystemSettings->setValue( "stop=", tr( "Only use trackpoints before this timestamp. Example (year, month, day, hour): start=2007010110,stop=2007010118" ) );
    SystemSettings->setValue( "title=", tr( "Base title for newly created tracks" ) );
    SystemSettings->setValue( "fix=", tr( "Synthesize GPS fixes (valid values are PPS, DGPS, 3D, 2D, NONE), e.g. when converting from a format that doesn't contain GPS fix status to one that requires it. Example: fix=PPS" ) );
    SystemSettings->setValue( "course", tr( "Synthesize course. This option computes resp. recomputes a value for the GPS heading at each trackpoint, e.g. when working on trackpoints from formats that don't support heading information or when trackpoints have been synthesized by the interpolate filter" ) );
    SystemSettings->setValue( "speed", tr( "Synthesize speed computes a speed value at each trackpoint, based on the neighboured points. Especially useful for interpolated trackpoints" ) );
    SystemSettings->setValue( "sdistance=", tr( "Split track if points differ more than x kilometers (k) or miles (m). See the gpsbabel docs for more details" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->beginGroup( "transform" );
    SystemSettings->setValue( "Name", tr( "This filter can be used to convert GPS data between different data types, e.g. convert waypoints to a track" ) );
    SystemSettings->setValue( "Image", ".png" );
    SystemSettings->beginGroup( "Options" );
    SystemSettings->setValue( "wpt=", tr( "Creates waypoints from tracks or routes, e.g. to create waypoints from a track, use: wpt=trk" ) );
    SystemSettings->setValue( "rte=", tr( "Creates routes from waypoints or tracks, e.g. to create a route from waypoints, use: rte=wpt" ) );
    SystemSettings->setValue( "trk=", tr( "Creates tracks from waypoints or routes, e.g. to create a track from waypoints, use: trk=wpt" ) );
    SystemSettings->setValue( "del", tr( "Delete source data after transformation. This is most useful if you are trying to avoid duplicated data in the output. Example: wpt=trk,del" ) );
    SystemSettings->endGroup();
    SystemSettings->endGroup();

    SystemSettings->endGroup(); // Filters




    // Filetypes
    SystemSettings->beginGroup( "FileTypes" );
    SystemSettings->setValue( "*.an1", tr( "DeLorme" ) );
    SystemSettings->setValue( "*.bcr", tr( "Map&Guide/Motorrad Routenplaner" ) );
    SystemSettings->setValue( "*.cst", tr( "Carte sur table" ) );
    SystemSettings->setValue( "*.csv", tr( "Comma separated values" ) );
    SystemSettings->setValue( "*.cup", tr( "See You flight analysis data" ) );
    SystemSettings->setValue( "*.dna", tr( "Navitrak DNA marker format" ) );
    SystemSettings->setValue( "*.gpb", tr( "Dell Axim Navigation System" ) );
    SystemSettings->setValue( "*.geo", tr( "Geocaching.com" ) );
    SystemSettings->setValue( "*.gtm", tr( "GPS TrackMaker" ) );
    SystemSettings->setValue( "*.gpx", tr( "GPS XML format" ) );
    SystemSettings->setValue( "*.gpl", tr( "DeLorme GPL" ) );
    SystemSettings->setValue( "*.gtm", tr( "GPS TrackMaker" ) );
    SystemSettings->setValue( "*.gdb", tr( "Garmin MapSource" ) );
    SystemSettings->setValue( "*.igc", tr( "FAI/IGC Flight Recorder" ) );
    SystemSettings->setValue( "*.kml", tr( "Google Earth Keyhole Markup Language" ) );
    SystemSettings->setValue( "*.loc", tr( "Geocaching.com or EasyGPS" ) );
    SystemSettings->setValue( "*.mps", tr( "Garmin MapSource" ) );
    SystemSettings->setValue( "*.mxf", tr( "MapTech Exchange Format" ) );
    SystemSettings->setValue( "*.ozi", tr( "OziExplorer" ) );
    SystemSettings->setValue( "*.pdb", tr( "Map&Guide to Palm/OS exported files" ) );
    SystemSettings->setValue( "*.pcx", tr( "Garmin PCX5" ) );
    SystemSettings->setValue( "*.psp", tr( "MS PocketStreets 2002 Pushpin" ) );
    SystemSettings->setValue( "*.rte", tr( "CompeGPS & Navigon Mobile Navigator" ) );
    SystemSettings->setValue( "*.sdf", tr( "Suunto Trek Manager" ) );
    SystemSettings->setValue( "*.tpg", tr( "National Geographic Topo waypoints" ) );
    SystemSettings->setValue( "*.trl", tr( "TrackLogs digital mapping" ) );
    SystemSettings->setValue( "*.tpo", tr( "National Geographic Topo" ) );
    SystemSettings->setValue( "*.txt", tr( "Plain text" ) );
    SystemSettings->setValue( "*.trk", tr( "CompeGPS track" ) );
    SystemSettings->setValue( "*.wbt", tr( "Wintec WBT" ) );
    SystemSettings->setValue( "*.wpt", tr( "CompeGPS waypoints" ) );
    SystemSettings->endGroup();


    // Character sets
    SystemSettings->beginGroup( "CharacterSets" );
    SystemSettings->setValue( "CP1250", "1250, ms-ee, windows-1250, WIN-CP1250" );
    SystemSettings->setValue( "CP1251", "1251, ms-cyrl, windows-1251, WIN-CP1251" );
    SystemSettings->setValue( "CP1252", "1252, ms-ansi, windows-1252, WIN-CP1252" );
    SystemSettings->setValue( "CP1253", "1253, ms-greek, windows-1253, WIN-CP1253" );
    SystemSettings->setValue( "CP1254", "1254, ms-turk, windows-1254, WIN-CP1254" );
    SystemSettings->setValue( "CP1255", "1255, ms-hebr, windows-1255, WIN-CP1255" );
    SystemSettings->setValue( "CP1256", "1256, ms-arab, windows-1256, WIN-CP1256" );
    SystemSettings->setValue( "CP1257", "1257, WinBaltRim, windows-1257, WIN-CP1257" );
    SystemSettings->setValue( "IBM437", "437, CP437" );
    SystemSettings->setValue( "IBM850", "850, CP850, csPC850Multilingual" );
    SystemSettings->setValue( "IBM851", "851, CP851" );
    SystemSettings->setValue( "IBM852", "852, CP852, pcl2, pclatin2" );
    SystemSettings->setValue( "IBM855", "855, CP855" );
    SystemSettings->setValue( "IBM857", "857, CP857" );
    SystemSettings->setValue( "IBM860", "860, CP860" );
    SystemSettings->setValue( "IBM861", "861, CP861, cp-is" );
    SystemSettings->setValue( "IBM862", "862, CP862" );
    SystemSettings->setValue( "IBM863", "863, CP863" );
    SystemSettings->setValue( "IBM864", "864, CP864" );
    SystemSettings->setValue( "IBM865", "865, CP865" );
    SystemSettings->setValue( "IBM868", "868, CP868, cp-ar" );
    SystemSettings->setValue( "IBM869", "869, CP869, cp-gr" );
    SystemSettings->setValue( "ISO-8859-1", "819, CP819, csISOLatin1, IBM819, ISO8859-1, iso-ir-100, ISO_8859-1, ISO_8859-1:1987, l1, lat1, latin1, Latin-1" );
    SystemSettings->setValue( "ISO-8859-10", "csISOLatin6, ISO8859-10, iso-ir-157, ISO_8859-10, ISO_8859-10:1992, ISO_8859-10:1993, l6, lat6, latin6" );
    SystemSettings->setValue( "ISO-8859-13", "ISO8859-13, iso-baltic, ISO-IR-179, iso-ir-179a, ISO_8859-13, ISO_8859-13:1998, l7, lat7, latin7" );
    SystemSettings->setValue( "ISO-8859-14", "ISO8859-14, iso-celtic, iso-ir-199, ISO_8859-14, ISO_8859-14:1998, l8, lat8, latin8" );
    SystemSettings->setValue( "ISO-8859-15", "ISO8859-15, iso-ir-203, ISO_8859-15, ISO_8859-15:1998, l9, lat9, latin9" );
    SystemSettings->setValue( "ISO-8859-2", "912, CP912, csISOLatin2, IBM912, ISO8859-2, iso-ir-101, ISO_8859-2, ISO_8859-2:1987, l2, lat2, latin2" );
    SystemSettings->setValue( "ISO-8859-3", "csISOLatin3, ISO8859-3, iso-ir-109, ISO_8859-3, ISO_8859-3:1988, l3, lat3, latin3" );
    SystemSettings->setValue( "ISO-8859-4", "csISOLatin4, ISO8859-4, iso-ir-110, ISO_8859-4, ISO_8859-4:1988, l4, lat4, latin4" );
    SystemSettings->setValue( "ISO-8859-5", "csISOLatinCyrillic, cyrillic, ISO8859-5, iso-ir-144, ISO_8859-5, ISO_8859-5:1988" );
    SystemSettings->setValue( "ISO-8859-6", "arabic, ASMO-708, csISOLatinArabic, ECMA-114, ISO8859-6, iso-ir-127, ISO_8859-6, ISO_8859-6:1987" );
    SystemSettings->setValue( "ISO-8859-7", "csISOLatinGreek, ECMA-118, ELOT_928, greek, greek8, ISO8859-7, iso-ir-126, ISO_8859-7, ISO_8859-7:1987" );
    SystemSettings->setValue( "ISO-8859-8", "csISOLatinHebrew, hebrew, ISO8859-8, iso-ir-138, ISO_8859-8, ISO_8859-8:1988" );
    SystemSettings->setValue( "ISO-8859-9", "csISOLatin5, ISO8859-9, iso-ir-148, ISO_8859-9, ISO_8859-9:1989, l5, lat5, latin5" );
    SystemSettings->setValue( "KOI-8", "GOST_19768-74" );
    SystemSettings->setValue( "KOI8-R", "csKOI8R" );
    SystemSettings->setValue( "KOI8-RU", "KOI8-RU" );
    SystemSettings->setValue( "Latin-greek-1", "iso-ir-27" );
    SystemSettings->setValue( "macintosh", "csMacintosh, mac, MacRoman" );
    SystemSettings->setValue( "US-ASCII", "ANSI_X3.4-1968, 367, ANSI_X3.4-1986, ASCII, CP367, csASCII, IBM367, ISO646-US, ISO646.1991-IRV, iso-ir-6, ISO_646.irv:1991, us" );
    SystemSettings->setValue( "UTF-8", "utf8" );
    SystemSettings->endGroup();







    // Error messages of gpsbabel
    // Expects that gpsbabel outputs english messages
    // The search strings often are not complete because they often contain placeholders like %filename%
    SystemSettings->beginGroup( "GpsBabelErrorMessages" );
    SystemSettings->setValue( "error reading an1 file.  Perhaps this isn't really an an1 file.", tr( "This file does not seem to be an an1 file." ) );
    SystemSettings->setValue( "Sorry, the radius should be greater than zero!", tr( "The radius should be greater than zero." ) );
    SystemSettings->setValue( "Can't open port", tr( "The port could not be opened." ) );
    SystemSettings->setValue( "Can't configure port", tr( "The port could not be configured." ) );
    SystemSettings->setValue( "Could not synchronise", tr( "The syncronisation failed." ) );
    SystemSettings->setValue( "Bad internal state", tr( "Bad internal state detected." ) );
    SystemSettings->setValue( "Serial error", tr( "Serial error detected." ) );
    SystemSettings->setValue( "Incomplete download", tr( "The download was not complete." ) );
    SystemSettings->setValue( /*Inva*/"id track data or unsupported version", tr( "This track data is invalid or the format is an unsupported version." ) );
    SystemSettings->setValue( "Invalid track header", tr( "The track header is invalid." ) );
    SystemSettings->setValue( "pdb_Read failed", tr( "Failed to perform pdb_Read." ) );
    SystemSettings->setValue( "Not a Cetus file", tr( "The file is not a Cetus file." ) );
    SystemSettings->setValue( "libpdb couldn't create record", tr( "libpdb could not create record." ) );
    SystemSettings->setValue( "libpdb couldn't append record", tr( "libpdb could not append record." ) );
    SystemSettings->setValue( "Unknown character set", tr( "The character set is unknown." ) );
    SystemSettings->setValue( "link-table unsorted", tr( "The link table is unsorted." ) );
    SystemSettings->setValue( "extra-table unsorted", tr( "The extra table is unsorted." ) );
    SystemSettings->setValue( "Unsupported character set", tr( "The character set is unsupported." ) );
    SystemSettings->setValue( "cet_disp_character_set_names", tr( "An internal error in cet_disp_character_set_names occured." ) );
    SystemSettings->setValue( "Internal error", tr( "An internal error occured." ) );
    SystemSettings->setValue( "Unsupported character set", tr( "The character set is unsupported." ) );
    SystemSettings->setValue( "This build excluded CoastalExplorer support because expat was not installed", tr( "There is no CoastalExplorer support because expat was not installed." ) );
    SystemSettings->setValue( "Cannot create XML parser", tr( "Unable to create an XML parser." ) );
    SystemSettings->setValue( "Parse error at", tr( "A parse error occured." ) );
    SystemSettings->setValue( "Unsupported datum", tr( "The datum is unsupported." ) );
    SystemSettings->setValue( "Sorry, UTM is not supported yet", tr( "UTM is not supported yet." ) );
    SystemSettings->setValue( "Invalid system of coordinates", tr( "The system of coordinates is invalid." ) );
    SystemSettings->setValue( "No waypoint data before", tr( "A problem with waypoint data occured." ) );
    SystemSettings->setValue( "Invalid length for generated shortnames", tr( "The length for generated shortnames is invalid." ) );
    SystemSettings->setValue( "Invalid value for radius", tr( "The value for radius is invalid." ) );
    SystemSettings->setValue( "Realtime positioning not supported", tr( "Realtime positioning is not supported." ) );
    SystemSettings->setValue( "pdb_Read failed", tr( "pdb_Read failed." ) );
    SystemSettings->setValue( "Not a CoPilot file", tr( "The file is not a CoPilot file." ) );
    SystemSettings->setValue( "libpdb couldn't create record", tr( "libpdb could not create record." ) );
    SystemSettings->setValue( "libpdb couldn't append record", tr( "libpdb could not append record." ) );
    SystemSettings->setValue( "Not a cotoGPS file", tr( "The file is not a cotoGPS file." ) );
    SystemSettings->setValue( "Unexpected end of file", tr( "Unexpected end of file." ) );
    SystemSettings->setValue( "Number of points expected", tr( "A different number of points was expected." ) );
    SystemSettings->setValue( "Could not interprete line", tr( "The line could not be interpreted." ) );
    SystemSettings->setValue( "too many format specifiers", tr( "There are too many format specifiers." ) );
    SystemSettings->setValue( "invalid format specifier", tr( "The format specifier is invalid." ) );
    SystemSettings->setValue( "gpl_point is", tr( "The value for gpl_point is wrong." ) );
    SystemSettings->setValue( "Unsupported file version", tr( "The version of the file is unsupported." ) );
    SystemSettings->setValue( "Unsupported combination of datum", tr( "The combination of datum and grid is unsupported." ) );
    SystemSettings->setValue( "Unknown or invalid track file", tr( "The track file is unknown or invalid." ) );
    SystemSettings->setValue( "Zlib was not included in this build", tr( "Zlib is not available in this build of gpsbabel." ) );
    SystemSettings->setValue( "Unknown or unsupported file type", tr( "The file type is unknown or unsupported." ) );
    SystemSettings->setValue( "is not an EasyGPS file", tr( "This is not an EasyGPS file." ) );
    SystemSettings->setValue( "String too long at", tr( "The string is to long." ) );
    SystemSettings->setValue( "Can't init", tr( "Cannot init." ) );
    SystemSettings->setValue( "does not support waypoint xfer", tr( "The Garmin unit does not support waypoint xfer." ) );
    SystemSettings->setValue( "Can't get waypoint from", tr( "The waypoints cannot be read from the interface." ) );
    SystemSettings->setValue( "Fatal error reading position", tr( "An error occured while trying to read positioning data. Maybe the device has no satellite fix yet." ) );
    SystemSettings->setValue( "Nothing to do", tr( "There is nothing to do." ) );
    SystemSettings->setValue( "not enough memory", tr( "There is not enough memory." ) );
    SystemSettings->setValue( "communication error sending wayoints", tr( "A communication error occured while sending wayoints." ) );
    SystemSettings->setValue( "unknown garmin format", tr( "This garmin format is unknown." ) );
    SystemSettings->setValue( "Invalid precision", tr( "The precision is invalid." ) );
    SystemSettings->setValue( "Invalid or unknown gps datum", tr( "The GPS datum is unknown or invalid." ) );
    SystemSettings->setValue( "Unknown distance unit", tr( "The distance unit is unknown." ) );
    SystemSettings->setValue( "Invalid date or/and time", tr( "Either date or time is invalid." ) );
    SystemSettings->setValue( "Unknown temperature unit", tr( "The temperature unit is unknown." ) );
    SystemSettings->setValue( "Invalid temperature", tr( "The temperature is invalid." ) );
    SystemSettings->setValue( "Incomplete or invalid file header", tr( "The file header is incomplete or invalid." ) );
    SystemSettings->setValue( "Unsupported grid", tr( "The grid is unsupported." ) );
    SystemSettings->setValue( "Missing grid headline", tr( "The grid headline is missing." ) );
    SystemSettings->setValue( "Unsupported GPS datum", tr( "The GPS datum is unsupported." ) );
    SystemSettings->setValue( "Missing GPS datum headline", tr( "The GPS datum headline is missing." ) );
    SystemSettings->setValue( "Route waypoint without name", tr( "A waypoint without name has been found." ) );
    SystemSettings->setValue( "not in waypoint list", tr( "A waypoint which is not in the waypoint list was found." ) );
    SystemSettings->setValue( "Unknwon identifier", tr( "An unknwon identifier was found." ) );
    SystemSettings->setValue( "zlib returned error", tr( "Zlib reported an error." ) );
    SystemSettings->setValue( "occured during read of file", tr( "An error occured while reading the file." ) );
    SystemSettings->setValue( "Could not write", tr( "Could not write data." ) );
    SystemSettings->setValue( "Unsupported serial speed", tr( "The given serial speed is unsupported." ) );
    SystemSettings->setValue( "Unsupported bits setting", tr( "The given bit setting is unsupported." ) );
    SystemSettings->setValue( "Unsupported parity setting", tr( "The given parity setting is unsupported." ) );
    SystemSettings->setValue( "Unsupported stop setting", tr( "The given stop setting is unsupported." ) );
    SystemSettings->setValue( "Not a GeocachingDB file", tr( "The file is not a GeocachingDB file." ) );
    SystemSettings->setValue( "libpdb couldn't create record", tr( "libpdb couldn't create record." ) );
    SystemSettings->setValue( "libpdb couldn't append record", tr( "libpdb couldn't append record." ) );
    SystemSettings->setValue( "unexpected end of file", tr( "An unexpected end of file occured." ) );
    SystemSettings->setValue( "I/O error occured during read", tr( "An I/O error occured during read." ) );
    SystemSettings->setValue( "local buffer overflow detected", tr( "A local buffer overflow was detected." ) );
    SystemSettings->setValue( "Internal error", tr( "An internal error occured." ) );
    SystemSettings->setValue( "unsupported bit count", tr( "An error was detected in the data." ) );
    SystemSettings->setValue( "Invalid file", tr( "The file is invalid." ) );
    SystemSettings->setValue( "Non supported GDB version", tr( "This GDB version is not supported." ) );
    SystemSettings->setValue( "Empty routes are not allowed", tr( "Empty routes are not allowed." ) );
    SystemSettings->setValue( "Unexpected end of route", tr( "Unexpected end of route." ) );
    SystemSettings->setValue( "Unsupported data size", tr( "Unsupported data size." ) );
    SystemSettings->setValue( "Unsupported category", tr( "Unsupported category." ) );
    SystemSettings->setValue( "Unsupported version", tr( "Unsupported version number." ) );
    SystemSettings->setValue( "This build excluded GEO support because expat was not installed", tr( "This build excluded GEO support because expat was not installed." ) );
    SystemSettings->setValue( "Bad record 0, not a GeoNiche file", tr( "This file is not a GeoNiche file." ) );
    SystemSettings->setValue( "Couldn't allocate waypoint", tr( "The waypoint could not be allocated." ) );
    SystemSettings->setValue( "Premature EOD processing field 1 (target)", tr( "Premature EOD processing field 1 (target)." ) );
    SystemSettings->setValue( "Route record type is not implemented", tr( "This route record type is not implemented." ) );
    SystemSettings->setValue( "Unknown record type", tr( "This record type is unknown." ) );
    SystemSettings->setValue( "Premature EOD processing field", tr( "Premature EOD processing." ) );
    SystemSettings->setValue( "Not a GeoNiche file", tr( "This file is not a GeoNiche file." ) );
    SystemSettings->setValue( "Unsupported GeoNiche file", tr( "This file is an unsupported GeoNiche file." ) );
    SystemSettings->setValue( "pdb_Read failed", tr( "pdb_Read failed." ) );
    SystemSettings->setValue( "libpdb couldn't get record memory", tr( "libpdb could not get record memory." ) );
    SystemSettings->setValue( "Reserved database name", tr( "The name is a reserved database name and must not get used." ) );
    SystemSettings->setValue( "This build excluded Google Maps support because expat was not installed", tr( "This build excluded Google Maps support because expat was not installed." ) );
    SystemSettings->setValue( "Unknown file type", tr( "The file type is unknown." ) );
    SystemSettings->setValue( "track point type", tr( "This track point type is unknown." ) );
    SystemSettings->setValue( "input record type", tr( "This input record type is unknown." ) );
    SystemSettings->setValue( "Not a gpspilot file", tr( "This file is not a gpspilot file." ) );
    SystemSettings->setValue( "output file already open", tr( "The output file already is open and cannot get reopened." ) );
    SystemSettings->setValue( "This build excluded GPX support because expat was not installed", tr( "This build excluded GPX support because expat was not installed." ) );
    SystemSettings->setValue( "Cannot create XML Parser", tr( "Cannot create an XML Parser." ) );
    SystemSettings->setValue( "Unable to allocate", tr( "Memory allocation failed." ) );
    SystemSettings->setValue( "XML parse error at", tr( "Error while parsing XML." ) );
    SystemSettings->setValue( "gpx version number of", tr( "The GPX version number is invalid." ) );
    SystemSettings->setValue( "Uncompress the file first", tr( "Please Uncompress the file first." ) );
    SystemSettings->setValue( "Invalid file format", tr( "The file format is invalid." ) );
    SystemSettings->setValue( "Invalid format version", tr( "The file format version is invalid." ) );
    SystemSettings->setValue( "this format does not support reading", tr( "Reading this file format is not supported." ) );
    SystemSettings->setValue( "Error reading data from .wpo file", tr( "There was an error while reading data from .wpo file." ) );
    SystemSettings->setValue( "Error writing", tr( "There was an error writing to the output file." ) );
    SystemSettings->setValue( "This build excluded HSA Endeavour support because expat was not installed", tr( "This build excluded HSA Endeavour support because expat was not installed." ) );
    SystemSettings->setValue( "is not an IGC file", tr( "The file is not an IGC file." ) );
    SystemSettings->setValue( "record parse error", tr( "There was an error while parsing a record." ) );
    SystemSettings->setValue( "record internal error", tr( "There was an internal error while working on a record." ) );
    SystemSettings->setValue( "bad date", tr( "A bad date occured." ) );
    SystemSettings->setValue( "failure reading file", tr( "Reading the file failed." ) );
    SystemSettings->setValue( "Bad waypoint format", tr( "There was a waypoint with bad format." ) );
    SystemSettings->setValue( "Bad date format", tr( "There was a date in a bad format." ) );
    SystemSettings->setValue( "Bad time of day format", tr( "There was a time of day in bad format." ) );
    SystemSettings->setValue( /*Bb*/"ad track timestamp", tr( "There was a bad timestamp in the track ." ) );
    SystemSettings->setValue( "Empty task route", tr( "Empty task route." ) );
    SystemSettings->setValue( "Too few waypoints in task route", tr( "Too few waypoints in task route." ) );
    SystemSettings->setValue( "Bad task route timestamp", tr( "There was a bad timestamp in the task route." ) );
    SystemSettings->setValue( "bad timeadj argument", tr( "There was a bad timeadj argument." ) );
    SystemSettings->setValue( "input support because expat was not installed", tr( "There is no support for this input type because expat is not available." ) );
    SystemSettings->setValue( "Error in XML structure", tr( "There was an error in the XML structure." ) );
    SystemSettings->setValue( "nvalid coordinates", tr( "There have been invalid coordinates." ) );
    SystemSettings->setValue( "Invalid altitude", tr( "There has been an invalid altitude." ) );
    SystemSettings->setValue( "error in vsnprintf.", tr( "There was an error in vsnprintf." ) );
    SystemSettings->setValue( "Invalid track index", tr( "There was an invalid track index." ) );
    SystemSettings->setValue( "invalid section header", tr( "There was an invalid section header." ) );
    SystemSettings->setValue( "Can't interpolate on both time and distance", tr( "Cannot interpolate on both time and distance." ) );
    SystemSettings->setValue( "Can't interpolate routes on time", tr( "Cannot interpolate routes on time." ) );
    SystemSettings->setValue( "No interval specified", tr( "No interval was specified." ) );
    SystemSettings->setValue( "This build excluded KML support because expat was not installed", tr( "This build excluded KML support because expat was not installed." ) );
    SystemSettings->setValue( "Units argument", tr( "Argument should be 's' for statute units or 'm' for metrics." ) );
    SystemSettings->setValue( "requested to read", tr( "There was an error reading a byte." ) );
    SystemSettings->setValue( "input file is from an old version of the USR file and is not supported", tr( "The input file is from an old version of the USR file and is not supported." ) );
    SystemSettings->setValue( "eXplorist does not support more than 200 waypoints in one .gs file", tr( "eXplorist does not support more than 200 waypoints in one .gs file.\nPlease decrease the number of waypoints sent." ) );
    SystemSettings->setValue( "Reading maggeo is not implemented yet", tr( "Reading the maggeo format is not implemented yet." ) );
    SystemSettings->setValue( "Not a Magellan Navigator file", tr( "This is not a Magellan Navigator file." ) );
    SystemSettings->setValue( "Unknown receiver type", tr( "The receiver type resp. model version is unknown." ) );
    SystemSettings->setValue( "No data received from GPS", tr( "No data was received from the GPS device." ) );
    SystemSettings->setValue( "bad communication.  Check bit rate.", tr( "The communication was not OK. Please check the bit rate used." ) );
    SystemSettings->setValue( "Can't configure port", tr( "There was an error configuring the port." ) );
    SystemSettings->setValue( "Could not open serial port", tr( "There was an error opening the serial port." ) );
    SystemSettings->setValue( "Read error", tr( "There was an error while reading data." ) );
    SystemSettings->setValue( "Write error", tr( "There was an error while writing data." ) );
    SystemSettings->setValue( "No acknowledgment from GPS on ", tr( "There was no acknowledgment from the GPS device." ) );
    SystemSettings->setValue( "Unexpected response to waypoint delete command", tr( "There was an unexpected error deleting a waypoint." ) );
    SystemSettings->setValue( "Unknown objective", tr( "An unknown objective occured." ) );
    SystemSettings->setValue( "Invalid point in time to call 'pop_args'", tr( "Invalid point in time to call 'pop_args'." ) );
    SystemSettings->setValue( "Unknown filter", tr( "There is an unknown filter type." ) );
    SystemSettings->setValue( "Unknown option", tr( "There is an unknown option." ) );
    SystemSettings->setValue( "Realtime tracking (-T) is not suppored by this input type", tr( "Realtime tracking (-T) is not suppored by this input type." ) );
    SystemSettings->setValue( "Realtime tracking (-T) is exclusive of other modes", tr( "Realtime tracking (-T) is exclusive of other modes." ) );
    SystemSettings->setValue( "Not a Magellan Navigator file", tr( "The file is not a Magellan Navigator file." ) );
    SystemSettings->setValue( "Unsupported MapSend TRK version", tr( "This version of MapSend TRK is unsupported." ) );
    SystemSettings->setValue( "out of data reading", tr( "Out of data reading waypoints." ) );
    SystemSettings->setValue( "GPS logs not supported", tr( "GPS logs are not supported." ) );
    SystemSettings->setValue( "GPS regions not supported", tr( "GPS regions are not supported." ) );
    SystemSettings->setValue( "unknown file type", tr( "The file type is unknown." ) );
    SystemSettings->setValue( "Unknown track version", tr( "The track version is unknown." ) );
    SystemSettings->setValue( "unknown garmin format", tr( "This garmin format is unknown." ) );
    SystemSettings->setValue( "This doesn't look like a mapsource file", tr( "This does not seem to be a MapSource file." ) );
    SystemSettings->setValue( "Unsuppported version of mapsource file", tr( "This version of the MapSource file is unsupported." ) );
    SystemSettings->setValue( "setshort_defname called without a valid name", tr( "setshort_defname was called without a valid name." ) );
    SystemSettings->setValue( "Could not seek file to sector", tr( "Could not seek file." ) );
    SystemSettings->setValue( "Read error", tr( "There was a reading error." ) );
    SystemSettings->setValue( "not in property catalog", tr( "Not in property catalog." ) );
    SystemSettings->setValue( "Broken stream", tr( "The stream was broken." ) );
    SystemSettings->setValue( "Error in fat1 chain, sector =", tr( "There was an error in the fat1 chain." ) );
    SystemSettings->setValue( "No MS document", tr( "No MS document." ) );
    SystemSettings->setValue( "Unsupported byte-order", tr( "This byte-order is unsupported." ) );
    SystemSettings->setValue( "Unsupported sector size", tr( "The sector size is unsupported." ) );
    SystemSettings->setValue( "Invalid file (no property catalog)", tr( "The file is invalid (no property catalog)." ) );
    SystemSettings->setValue( "Invalid or unknown data", tr( "The data is invalid or of unknown type." ) );
    SystemSettings->setValue( "Unsupported byte order within data", tr( "An unsupported byte order was detected." ) );
    SystemSettings->setValue( "This build excluded GPX support because expat was not installed", tr( "This build excluded GPX support because expat was not installed." ) );
    SystemSettings->setValue( "Cannot create XML parser", tr( "The XML parser cannot be created." ) );
    SystemSettings->setValue( "Parse error at", tr( "A parse error occured." ) );
    SystemSettings->setValue( "Does not support writing Navicache files", tr( "There is no support writing Navicache files." ) );
    SystemSettings->setValue( "illegal read_mode", tr( "Illegal read mode." ) );
    SystemSettings->setValue( "Invalid date", tr( "There was an invalid date." ) );
    SystemSettings->setValue( "is out of range (have to be 19700101 or later)", tr( "The date is out of range. It has to be later than 1970-01-01." ) );
    SystemSettings->setValue( "Could not open", tr( "There was an error opening a file." ) );
    SystemSettings->setValue( "Unable to set baud rate", tr( "There was an error setting the baud rate." ) );
    SystemSettings->setValue( "No data received on", tr( "No data was received." ) );
    SystemSettings->setValue( "realtime positioning not supported", tr( "Realtime positioning is not supported." ) );
    SystemSettings->setValue( "libpdb couldn't create summary record", tr( "libpdb couldn't create summary record." ) );
    SystemSettings->setValue( "libpdb couldn't insert summary record", tr( "libpdb couldn't insert summary record." ) );
    SystemSettings->setValue( "ibpdb couldn't create bookmark record", tr( "libpdb couldn't create bookmark record." ) );
    SystemSettings->setValue( "libpdb couldn't append bookmark record", tr( "libpdb couldn't append bookmark record." ) );
    SystemSettings->setValue( "libpdb couldn't create record", tr( "libpdb couldn't create record." ) );
    SystemSettings->setValue( "libpdb couldn't append record", tr( "libpdb couldn't append record." ) );
    SystemSettings->setValue( "Error in data structure", tr( "There was an error detected in the data structure." ) );
    SystemSettings->setValue( "Unable to convert date", tr( "There was an error converting the date." ) );
    SystemSettings->setValue( "pdb_Read failed", tr( "There was an error in pdb_Read." ) );
    SystemSettings->setValue( "Not a PathAway pdb file", tr( "This file is not a PathAway .pdb file." ) );
    SystemSettings->setValue( "This file is from an untested version", tr( "This file is from an unsupported version of PathAway." ) );
    SystemSettings->setValue( "Invalid database subtype", tr( "An invalid database subtype was detected." ) );
    SystemSettings->setValue( "It looks like a PathAway pdb, but has no gps magic", tr( "The file looks like a PathAway .pdb file, but it has no gps magic." ) );
    SystemSettings->setValue( "Unrecognized track line", tr( "There was an unrecognized track line." ) );
    SystemSettings->setValue( "requested to read", tr( "Error while reading the requested amount of bytes." ) );
    SystemSettings->setValue( "input file does not appear to be a valid .PSP file", tr( "The input file does not appear to be a valid .psp file." ) );
    SystemSettings->setValue( "variable string size", tr( "Variable string size exceeded." ) );
    SystemSettings->setValue( "attempt to output too many pushpins", tr( "An attempt to output too many pushpins occured." ) );
    SystemSettings->setValue( "Not a QuoVadis file", tr( "This file is not a QuoVadis file." ) );
    SystemSettings->setValue( "cannot store more than", tr( "There was an error storing further records." ) );
    SystemSettings->setValue( "This filter only works in track", tr( "This filter only can work on tracks." ) );
    SystemSettings->setValue( "unrecognized color name", tr( "The color name was unrecognized." ) );
    SystemSettings->setValue( "Attempt to read past EOF", tr( "An attempt to read past the end of the file was detected." ) );
    SystemSettings->setValue( "ot enough information is known about this format to write it", tr( "This file format cannot be written." ) );
    SystemSettings->setValue( "Cannot open shp file", tr( "An error occured opening the .shp file." ) );
    SystemSettings->setValue( "Cannot open dbf file", tr( "An error occured opening the .dbf file." ) );
    SystemSettings->setValue( "dbf file for ", tr( "A name or field error occured in the .dbf file." ) );
    SystemSettings->setValue( "Cannot open", tr( "There was an error opening a file or a port." ) );
    SystemSettings->setValue( "Routes are not supported", tr( "Routes are not supported." ) );
    SystemSettings->setValue( "Realtime positioning not supported", tr( "Realtime positioning is not supported." ) );
    SystemSettings->setValue( "You must specify either count or error, but not both", tr( "You must specify either count or error, but not both." ) );
    SystemSettings->setValue( "crosstrack and length may not be used together", tr( "crosstrack and length may not be used together." ) );
    SystemSettings->setValue( "stack empty", tr( "The stack was empty." ) );
    SystemSettings->setValue( "Unsupported file type", tr( "This file type is unsupported." ) );
    SystemSettings->setValue( "Invalid section header", tr( "The section header is invalid." ) );
    SystemSettings->setValue( "Invalid GPS datum or not", tr( "Invalid GPS datum or not a WaypointPlus file." ) );
    SystemSettings->setValue( "Unknown feature", tr( "The feature is unknown." ) );
    SystemSettings->setValue( "Only one feature (route or track) is supported by STM", tr( "Only one of both features (route or track) is supported by STM." ) );
    SystemSettings->setValue( "This build excluded TEF support because expat was not installed", tr( "This build excluded TEF support because expat is not available." ) );
    SystemSettings->setValue( "error in source file", tr( "An error occured in the source file." ) );
    SystemSettings->setValue( "waypoint count differs to internal", tr( "The amount of waypoints differed to the internal item count." ) );
    SystemSettings->setValue( "requested to read", tr( "There was an attempt to read an unexpected amount of bytes." ) );
    SystemSettings->setValue( "is not recognized", tr( "The datum is not recognized." ) );
    SystemSettings->setValue( "input file does not appear to be a valid .TPG file", tr( "The input file does not appear to be a valid .TPG file." ) );
    SystemSettings->setValue( "attempt to output too many points", tr( "There was an attempt to output too many points." ) );
    SystemSettings->setValue( "The input file does not look like a valid .TPO file", tr( "The input file does not look like a valid .TPO file." ) );
    SystemSettings->setValue( "gpsbabel can only read TPO version 2.7.7 or below; this file is", tr( "This TPO file's format is to young (newer than 2.7.7) for gpsbabel." ) );
    SystemSettings->setValue( "this file format only supports tracks, not waypoints or routes", tr( "This file format only supports tracks, not waypoints or routes." ) );
    SystemSettings->setValue( "gpsbabel can only read TPO versions through 3.x.x", tr( "gpsbabel can only read TPO versions through 3.x.x." ) );
    SystemSettings->setValue( "writing ouput for state", tr( "Writing output for this state currently is not supported" ) );
    SystemSettings->setValue( "invalid character in time option", tr( "There was an invalid character in the time option." ) );
    SystemSettings->setValue( "invalid fix type", tr( "Invalid fix type." ) );
    SystemSettings->setValue( "init: Found track point without time", tr( "A track point without timestamp was found." ) );
    SystemSettings->setValue( "Track points badly ordered (timestamp)", tr( "Track points are not ordered by timestamp." ) );
    SystemSettings->setValue( "title: Missing your title", tr( "Your title was missing." ) );
    SystemSettings->setValue( "pack: Tracks overlap in time", tr( "The tracks overlap in time." ) );
    SystemSettings->setValue( "No time interval specified", tr( "There was no time interval specified." ) );
    SystemSettings->setValue( "invalid time interval specified, must be one a positive number", tr( "The time interval specified was invalid. It must be a positive number." ) );
    SystemSettings->setValue( "invalid time interval specified, must be one of [dhms]", tr( "The time interval specified was invalid. It must be one of [dhms]." ) );
    SystemSettings->setValue( "No distance specified", tr( "There was no distance specified." ) );
    SystemSettings->setValue( "invalid distance specified, must be one a positive number", tr( "The distance specified was invalid. It must be a positive number." ) );
    SystemSettings->setValue( "invalid distance specified, must be one of [km]", tr( "The distance specified was invalid. It must be one of [km]." ) );
    SystemSettings->setValue( "range: parameter too long", tr( "Parameter for range was to long." ) );
    SystemSettings->setValue( "range: invalid character", tr( "There was an invalid character for range." ) );
    SystemSettings->setValue( "range-check: Invalid time stamp (stopped at", tr( "There was an invalid time stamp for range-check." ) );
    SystemSettings->setValue( "Cannot split more than one track, please pack (or merge) before", tr( "Cannot split more than one track. Please pack or merge before processing." ) );
    SystemSettings->setValue( "Invalid option value", tr( "There was an invalid value for the option." ) );
    SystemSettings->setValue( "not done yet", tr( "Not available yet." ) );
    SystemSettings->setValue( "gpsbabel: Unable to allocate ", tr( "gpsbabel was unable to allocate memory." ) );
    SystemSettings->setValue( "must have a filename specified for", tr( "A filename needs to be specified." ) );
    SystemSettings->setValue( "cannot open", tr( "There was an error opening a file or a port." ) );
    SystemSettings->setValue( "writing output file.  Error was", tr( "Error writing the output file." ) );
    SystemSettings->setValue( "Invalid character", tr( "There was an invalid character in date or time format." ) );
    SystemSettings->setValue( "A format name is required", tr( "A format name needs to be specified." ) );
    SystemSettings->setValue( "reading file.  Unsupported version", tr( "Unsupported file format version in input file." ) );
    SystemSettings->setValue( "reading file. Invalid file header", tr( "There was an invalid header in the input file." ) );
    SystemSettings->setValue( "Can't allocate", tr( "There was an error allocating memory." ) );
    SystemSettings->setValue( "Comm error", tr( "A communication error occured." ) );
    SystemSettings->setValue( "Read error", tr( "A read error occured." ) );
    SystemSettings->setValue( "Write error", tr( "A write error occured." ) );
    SystemSettings->setValue( "Can't initialise port", tr( "There was an error initialising the port." ) );
    SystemSettings->setValue( "Read timout", tr( "A timout occured while attempting to read data." ) );
    SystemSettings->setValue( "Can't open file", tr( "There was an error opening the file." ) );
    SystemSettings->setValue( "Bad response from unit", tr( "There was a bad response from the unit." ) );
    SystemSettings->setValue( "Can't autodetect data format", tr( "There was an error autodetecting the data format." ) );
    SystemSettings->setValue( "Internal error: formats not ordered in ascending size order", tr( "An internal error occured: formats are not ordered in ascending size order." ) );
    SystemSettings->setValue( "This build excluded WFFF_XML support because expat was not installed", tr( "This build excluded WFFF_XML support because expat is not available." ) );
    SystemSettings->setValue( "XCSV input style not declared.  Use ... -i xcsv,style=path/to/file.style", tr( "XCSV input style not declared.  Use ... -i xcsv,style=path/to/style.file" ) );
    SystemSettings->setValue( "XCSV output style not declared.  Use ... -o xcsv,style=path/to/file.style", tr( "XCSV output style not declared.  Use ... -o xcsv,style=path/to/style.file" ) );
    SystemSettings->setValue( "Parse error at", tr( "A parse error occured." ) );
    SystemSettings->setValue( "Cannot create XML Parser", tr( "There was an error creating an XML Parser." ) );
    SystemSettings->setValue( "This format does not support reading XML files as libexpat was not present", tr( "This format does not support reading XML files as libexpat was not available." ) );
    SystemSettings->setValue( "Writing file of type", tr( "Generating such filetypes is not supported." ) );
    SystemSettings->setValue( "unknown road type for road changes", tr( "The road type for road changes is unknown." ) );
    SystemSettings->setValue( "invalid format for road changes", tr( "The format for road changes is invalid." ) );
    SystemSettings->setValue( "Invalid or unsupported file (filesize)", tr( "The file is invalid or unsupported due to its filesize." ) );
    SystemSettings->setValue( "structure error at", tr( "There was a coordinates structure error." ) );
    SystemSettings->setValue( "invalid route number", tr( "There was an invalid route number." ) );
    SystemSettings->setValue( "wpt_type must be", tr( "An error was detected in wpt_type." ) );
    SystemSettings->setValue( "type must be", tr( "The type doesn't seem to be correct." ) );

    SystemSettings->setValue( "usb_set_configuration failed:", tr( "The USB-Configuration failed." ) );
    SystemSettings->setValue( "usb_set_configuration failed, probably because kernel driver", tr( "The USB-Configuration failed. Probably a kernel module blocks the device.\nAs superuser root, try to execute the commands\nrmmod garmin_gps and chmod o+rwx /proc/bus/usb -R\n to solve the problem temporarily." ) );
    SystemSettings->setValue( "Found no Garmin USB devices", tr( "No Garmin USB device seems to be connected to the computer." ) );

    SystemSettings->endGroup();

    SystemSettings->sync();
}

