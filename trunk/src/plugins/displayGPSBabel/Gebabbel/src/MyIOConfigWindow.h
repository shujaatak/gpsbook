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

#ifndef MYIOCONFIGWINDOW_H
#define MYIOCONFIGWINDOW_H

#include <iostream>

#include "ui_IOConfig.h"
#include <QtGui>
#include "ListItem.h"
#include "SettingsManager.h"


using namespace std;

class MyIOConfigWindow: public QDialog, private Ui::IOConfig
{
    Q_OBJECT

private:
    QDir * RequestDir;
    void setupConnections();
    bool isDevice;

    QString DataType;
    QString TempOptions;

public:
    MyIOConfigWindow( QWidget* parent = 0L );

public slots:
    void setData( QString, QStringList, QString, QString );
    void setDataType( QString );
    void fillCombos();
    void fillOptionsCombo( QString );

    void updateOptionsEdit( QString );
    void preserveOptionsEdit();

    void setupHelp();
    void setHelpText();

    QString data();

    // Those are not obsolete as used to check if one of the two combos is empty
    QString Type();
    QString Value();

    void transmitValues();
    void selectFile();

signals:



private slots:
    void on_BtnSelectFile_2_clicked();
};



#endif

