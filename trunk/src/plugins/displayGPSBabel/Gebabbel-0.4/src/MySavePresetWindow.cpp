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

#include "MySavePresetWindow.h"

MySavePresetWindow::MySavePresetWindow( QWidget* parent )
    : QDialog( parent )
{
    setupUi( this );
    setupConnections();
    setupGui();
}


void MySavePresetWindow::setupConnections()
{
    connect( ButtonCancel, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( ButtonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( ComboName, SIGNAL( activated( QString ) ), this, SLOT( setCommentText( QString ) ) );
}


void MySavePresetWindow::setupGui()
{
    // As this window is a persistent window, the last saved preset name reappeared when saving a preset
    // Meanwhile seems to be gone
    setWindowTitle( tr( "Save Preset" ) );
    TextLabel->setText( tr( "Please enter a name for the new preset.\nChoosing the name of an existing preset will overwrite the existing preset.\nThe comment is optional:" ) );

    ComboName->addItems( SettingsManager::instance()->presetNames() );

    QStringList AllCommentNames = SettingsManager::instance()->commentNames();
    for ( int i = 0; i < AllCommentNames.count(); i++ )
    {
        if ( SettingsManager::instance()->comment( AllCommentNames.at( i ) ) != "" )
        {
            ComboComment->addItem( SettingsManager::instance()->comment( AllCommentNames.at( i ) ) );
        }
    }
    ComboName->setEditText( tr( "Enter new name here" ) );
    ComboComment->setEditText( "" );
}


void MySavePresetWindow::setCommentText( QString PresetName )
{
    if ( SettingsManager::instance()->comment( PresetName ) != "" )
    {
        ComboComment->setEditText( SettingsManager::instance()->comment( PresetName ) );
    }
    else
    {
        ComboComment->setEditText( "" );
    }
}


QString MySavePresetWindow::tellName()
{
    return ComboName->currentText();
}


QString MySavePresetWindow::tellComment()
{
    return ComboComment->currentText();
}

