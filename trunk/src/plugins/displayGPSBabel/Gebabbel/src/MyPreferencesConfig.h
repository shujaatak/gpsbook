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

#ifndef MYPREFERENCESCONFIG_H
#define MYPREFERENCESCONFIG_H

#include <iostream>

#include "ui_PreferencesConfig.h"
#include <QtGui>

#include "SettingsManager.h"


using namespace std;

class MyPreferencesConfig: public QDialog, private Ui::PreferencesConfig
{
    Q_OBJECT

private:
    void setupConnections();

public:
    void getSettings();
    MyPreferencesConfig( QWidget* parent = 0L );

public slots:
    void saveSettings();
    void selectGpsbabel();
};




#endif

