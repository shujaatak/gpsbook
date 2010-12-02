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

#include "ListItem.h"

ListItem::ListItem( QTreeWidget * Parent )
{
    ListViewItem = new QTreeWidgetItem;
    Parent->addTopLevelItem( ListViewItem );
    FileExists = false;
    IsDevice = false;
    CharSet == "";
}


ListItem::~ListItem()
{
    delete ListViewItem;
}


void ListItem::setup( QString Data, QString WD )
{
    // First initializing some data.
    Type = "";
    Value = "";
    Options.clear();

    GpsBabelWorkingDir = WD;
    if ( Data.startsWith( " -i " ) )
    {
        DataType = "Input";
        Data.remove( " -i " );
    }
    else if ( Data.startsWith( " -o " ) )
    {
        DataType = "Output";
        Data.remove( " -o " );
    }
    else if ( Data.startsWith( " -x " ) )
    {
        DataType = "Filter";
        Data.remove( " -x " );
    }
    else
    {
        return;
    }
    // Removing possible whitespace
    Data = Data.trimmed();

    // Splitting Data
    QRegExp Separators;
    if ( DataType == "Input" || DataType == "Output" )
    {
        // Splitting the string from right to left
        // The rightmost part always is a file or an interface
        Separators.setPattern( " -[fF] " );
        Value = Data.split( Separators ).at( 1 );
        Data.remove( Value );
        Data.remove( Separators );
        Data = Data.trimmed();

        // Checking if there is a character set
        if ( Data.contains( " -c " ) )
        {
            Separators.setPattern( " -c " );
            CharSet = Data.split( Separators ).at( 1 );
            Data.remove( CharSet );
            Data.remove( Separators );
        }
        else
        {
            CharSet = "";
        }
    }
    // Removing possibly existing blanks at both ends
    Data = Data.trimmed();

    // That's it for IO specific stuff. The rest is equal for filters, inputs and outputs
    if ( Data.contains( "," ) )
    {
        Separators.setPattern( "," );
        Options = Data.split( Separators );
        // The first element contains the type
        Type = Options.at( 0 );
        // Removing Type from the options list
        Options.removeAt( 0 );
    }
    else
    {
        Type = Data;
    }
    // Data has done its job
    Data = "";

    // DisplayType is used to feed the first column of the GUI items and is equal for IOs and filters
    // Currently it equals Type, but future versions will convert DisplayType to something more human readable
    DisplayType = Type;

    // Determining if this object is a device, which only makes sense in case of IOs
    // Devices are read from the config file so users are able to add device types manually
    IsDevice = false;
    if ( DataType != "Filter" )
    {
        QStringList DeviceTypes = SettingsManager::instance()->deviceTypes();
        for ( int i = 0; i < DeviceTypes.count(); i++ )
        {
            if ( DeviceTypes.at( i ) == Type )
            {
                IsDevice = true;
            }
        }
    }

    // If "Value" doesn't contain an absolute file path (inputs and outputs only, not filters),
    // prepend the gpsbabel working dir before checking for the file's existence
    // TODO: Even in filters, file existence should be checked, but it's difficult to determine files => postponed
    if ( DataType != "Filter" )
    {
        QFileInfo FileInfo( Value );
        if ( !FileInfo.isAbsolute() && !IsDevice && !(Value == "gpsbook") )
        {
            // Even on Windows, paths use "/" instead of "\" inside QT
            Value.prepend( "/" );
            Value.prepend( GpsBabelWorkingDir );
            FileInfo.setFile( Value );
        }
        FileExists = FileInfo.exists();

        // Display value is used to feed the second column of the GUI items and is *not* equal for IOs and filters
        // For filters it will display the filter options
        // For files it will display the file name without its path
        if ( !IsDevice )
        {
            DisplayValue = FileInfo.fileName();
        }
        else
        {
            // This if statement mainly is necessary for Windows because fileName()
            // does treat a colon treat as a path delimiter there.
            // Thus usb:0 gets displayed as "0" on Windows
            DisplayValue = Value;
        }
    }
    else if ( DataType == "Filter" )
    {
        DisplayValue = Options.join( "," );
    }

    updateGuiItem();
}


QTreeWidgetItem * ListItem::tellListViewItem()
{
    return ListViewItem;
}


QStringList ListItem::content()
{
    QStringList Contents;
    QString TempString = Type;

    if ( Options.size() != 0 && Options.at( 0 ) != "" )
    {
        TempString.append( "," );
        TempString.append( Options.join( "," ) );
    }
    Contents.append( TempString );
    TempString = "";

    if ( DataType != "Filter" && CharSet != "" )
    {
        Contents.append( "-c" );
        Contents.append( CharSet );
    }

    if ( DataType == "Input" )
    {
        Contents.append( "-f" );
    }
    else if ( DataType == "Output" )
    {
        Contents.append( "-F" );
    }

    if ( DataType != "Filter" )
    {
        Contents.append( Value );
    }
    return Contents;
}


QString ListItem::tellType()
{
    return Type;
}


QStringList ListItem::tellOptions()
{
    return Options;
}


QString ListItem::tellCharSet()
{
    return CharSet;
}


QString ListItem::tellValue()
{
    return Value;
}


void ListItem::updateGuiItem()
{
    ListViewItem->setText( 0, DisplayType );
    ListViewItem->setText( 1, DisplayValue );
    ListViewItem->setToolTip( 0, SettingsManager::instance()->TypeName( DataType, Type ) );

    if ( DataType == "Filter" )
    {
        ListViewItem->setIcon( 0, QIcon( ":/icons/Filter.png" ) );
        ListViewItem->setTextColor( 1, QColor( "gray" ) );
        if ( Options.count() > 0 )
        {
            ListViewItem->setToolTip( 1, tr( "Values of this filter item") );
        }
        else
        {
            ListViewItem->setToolTip( 1, "" );
        }
        return;
    }
    else if ( Value == "gpsbook" )
    {
        ListViewItem->setIcon( 0, QIcon( ":/icons/gpsbook/boussole.gif" ) );
        ListViewItem->setTextColor( 1, QColor( "blue" ) );
        ListViewItem->setToolTip( 1, Value );
        return;
    }
    else if ( IsDevice == true )
    {
        ListViewItem->setIcon( 0, QIcon( ":/icons/GpsDevice.png" ) );
        ListViewItem->setTextColor( 1, QColor( "blue" ) );
        ListViewItem->setToolTip( 1, Value );
        return;
    }
    else if ( DataType == "Output" )
    {
        ListViewItem->setIcon( 0, QIcon( ":/icons/Folder.png" ) );
        ListViewItem->setTextColor( 1, QColor( "black" ) );
        ListViewItem->setToolTip( 1, Value );
        return;
    }
    else if ( FileExists == false && DataType == "Input" )
    {
        ListViewItem->setIcon( 0, QIcon( ":/icons/FolderWarning.png" ) );
        ListViewItem->setTextColor( 1, QColor( "red" ) );
        ListViewItem->setToolTip( 1, tr( "Sorry, but the file %1 was not found" ).arg( Value ) );
        return;
    }
    else if ( DataType == "Input" )
    {
        ListViewItem->setIcon( 0, QIcon( ":/icons/Folder.png" ) );
        ListViewItem->setTextColor( 1, QColor( "black" ) );
        ListViewItem->setToolTip( 1, Value );
        return;
    }
    else
    {
        ListViewItem->setIcon( 0, QIcon( ":/icons/Empty.png" ) );
        ListViewItem->setTextColor( 1, QColor( "red" ) );
        ListViewItem->setToolTip( 1, tr( "Sorry, Gebabbel does not know what this item is all about." ) );
        return;
    }

}


bool ListItem::isAlright()
{
    // TODO: Consider if there are more occasions than this ones
    if ( IsDevice == false && DataType == "Input" && FileExists == false )
    {
        return false;
    }
    else
    {
        return true;
    }
}


