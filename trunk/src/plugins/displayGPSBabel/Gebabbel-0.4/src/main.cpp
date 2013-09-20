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

#include <QtGui/QtGui>
#include "MyMainWindow.h"
#include "SettingsManager.h"

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );
    app.setQuitOnLastWindowClosed( true );

    // See http://web.mit.edu/qt/www/i18n.html#internationalization
    // The translation .qm files are included in the application binary
    // Maybe the locale should be configurable via the GUI. Anyway, at least it's possible via the config file
    QTranslator Translator( &app );
    // qDebug() << SettingsManager::instance()->language();
    Translator.load( SettingsManager::instance()->language() );
    app.installTranslator( &Translator );

    QFrame * MainForm = new MyMainWindow();

    if ( argc > 1 )
    {
        QString FileName ( argv[1] );
        QFileInfo FileInfo( FileName );
        cerr << "\n";
        cerr << APPNAME << " " << "doesn't accept commandline arguments.";
        cerr << "\n\n";
        cerr << APPNAME << " " << VERSION << ", (c) by " << AUTHOR << " " << YEARS << ".";
        cerr << "\n";
        cerr << APPNAME << " was released under terms and conditions of the GPL license, V2.";
        cerr << "\n";
        cerr << "For further information visit http://gebabbel.sourceforge.net";
        cerr << "\n\n";
    }
    MainForm->show();
    return app.exec();
}


