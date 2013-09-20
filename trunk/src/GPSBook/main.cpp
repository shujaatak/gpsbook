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
#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>
#include <QTranslator>
#include <QSettings>
#include <QMessageBox>
#include <QDir>
#include <QLocale>
#include <QPlastiqueStyle>
#include <QSqlError>
#include <QDebug>
#include <iostream>
#include "mainwindow.h"
#include "dialogfirststart.h"
#include "database.h"
#include "gpsbookapplication.h"


using namespace GPSBook;

int main(int argc, char *argv[])
{
    GPSBookApplication application(argc, argv);

    //Define application name and version
    QCoreApplication::setApplicationName("GPSBook");
    QCoreApplication::setApplicationVersion(QString(BUILD_VERSION) + "-" + QString(SVN_VERSION));

    //Open settings
    QSettings settings("GPSBook","GPSBook");
    QString lang=settings.value("Translation","").toString();

    //Parse command line
    QStringList args = QCoreApplication::arguments();
    for ( int i = 0; i < args.count(); i++ ) {
        QString s = args[i] ;
        if ( s.startsWith( "--firststart" ) ) {
            settings.setValue("FirstStart",true);
        } else if ( s.startsWith( "--lang" ) ) {
            lang=args[i++];
        } else if (s.startsWith("--version")){
            std::cout << "gpsbook version " << qApp->applicationVersion().toStdString() << std::endl;
            std::cout << "copyright (c) 2009-2010, gpsbook-team" << std::endl;
            return 0;
        }
    }

    //Prepare splash screen
    QPixmap pixmap(":/icons/gpsbook/splash.gif");
    QSplashScreen splash(pixmap);
    qApp->setActiveWindow(&splash);
    splash.show();
    splash.showMessage(qApp->applicationName()+ " " + qApp->applicationVersion());
    qApp->processEvents();

    //Apply translation
    application.loadTranslations(QDir("../translations"));
    GPSBookApplication::setLanguage(lang);

    //First start of application: initialize the application
    if (settings.value("FirstStart",true).toBool())
    {
        //Open the preference dialog to choose language and storage location
        DialogFirstStart dialogFirstStart;

        dialogFirstStart.setModal(true);
        dialogFirstStart.show();
        if ( dialogFirstStart.exec() == QDialog::Accepted )
        {
            QString storagePath = settings.value("StorageLocation","").toString();
            //Check if directory choosen exists and create it if necesary
            QDir *dir = new QDir();
            if ( ( !storagePath.isEmpty() ) && ( !dir->exists(storagePath) ) )
            {
                if ( QMessageBox::question(NULL,qApp->applicationName(),qApp->tr("Directory is not existing.")+"\n"+storagePath+"\n"+qApp->tr("Do you want to create it?"),QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes )
                {
                    dir->mkdir(storagePath);
                }
                else
                {
                    return false;
                }
            }
            else
            {
                if ( QMessageBox::question(NULL,qApp->applicationName(),qApp->tr("Directory exists.\nCreation of subdirectories if needed"),QMessageBox::Ok,QMessageBox::Cancel) == QDialog::Rejected )
                {
                    //User doesn't want to create needed directories
                    return false;
                }

            }
            if ( !(
                //Location where database will be stored
                dir->mkpath(storagePath+"/db/") &&
                //Location where gpx data will be stored
                dir->mkpath(storagePath+"/data/") &&
                //The 2 following lines will be used in future to store plugins and translation for user only
                //Note: a specific process will have to be written to resolve conflicts
                dir->mkpath(storagePath+"/translation/") &&
                dir->mkpath(storagePath+"/plugins/") ) )
            {
                QMessageBox::critical(0, qApp->applicationName(),qApp->tr("An error occurs during directories creation. Aborting."));
                return false;
            }

        }
        else
        {
            //User did click on Cancel
            return false;
        }
        //First start is a success
        settings.setValue("FirstStart",false);
    }


    //Force style to clean look
    #if defined(Q_OS_LINUX)
      //GPSBookApplication::setStyle(new QPlastiqueStyle);
    #endif

    //Check is SQLITE is available
    if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
        QMessageBox::critical(NULL,qApp->applicationName(),"<b>" + qApp->tr("Cannot load sqlite driver.") + "</b><br>" + qApp->applicationName() + " " + qApp->tr("Cannot work with SQLite without this driver, that's why you have to install it to run this application."));
    }
    else
    {
        //Check if DB is existing and create
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(QDir::homePath()+"/gpsbook/db/database.sqlite");
        Database::initializeDatabase();

        //Open database
        if (!db.open())
        {
            //It is not possible to open database
            QMessageBox::critical(0, qApp->applicationName(),qApp->tr("Unable to establish a database connection.\n--> ")+db.lastError().text()+"\n--> "+db.databaseName()+"\n\n"+qApp->tr("Application will be reinitialized at next start up. You should try again."));
            //Reset first start
            settings.setValue("FirstStart",true);
            return false;
        }
    }


    //Create main window and start application
    MainWindow mainWindow;
    mainWindow.setSplashScreen(&splash);
    mainWindow.init();
    //QPixmap icon(":/icons/gpsbook/boussole.gif");
    //mainWindow.setWindowIcon(QIcon(icon));
    mainWindow.show();
    splash.finish(&mainWindow);
    return application.exec();

} //main
