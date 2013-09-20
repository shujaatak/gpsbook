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
#ifndef INTERFACE_H
#define INTERFACE_H

#include <QObject>
#include <QThread>
#include <QtPlugin>
#include <QStringList>
#include <gpsdata.h>

using namespace GPSBook;

class QIcon;
class QString;
class QWidget;
class ServiceInterface;


/*!
  \brief Service provider
  The service provider is gathering all information about service provided by plugins
  and allow other plugins to use the services.

  This is a singleton owned by the main application and sent to plugins once all plugins
  are loaded.
*/
class ServiceProvider
{
    private:
        QStringList mServiceList;
        QHash<QString,QStringList> mServicePropertyList;
        static ServiceProvider* mInstance;
        ServiceProvider() {}

    public:
        ~ServiceProvider(){
          delete mInstance;
          mInstance = 0;
        }

        static ServiceProvider* getServiceProvider(){
            if (mInstance == 0)
                mInstance = new ServiceProvider();
            return (mInstance);
        }

        QStringList getServiceList(){
            return mServiceList;
        }
        QStringList getServicesPropertiList( const char * serviceName ){
            return mServicePropertyList[serviceName];
        }

        ServiceInterface* getService(const char * serviceName ){
            //To remove warning at compilation
            serviceName = serviceName;
            return NULL;
        }
        //QStringList* getServicePropertydescription(const char * serviceName,
        //                                         const char * serviceId,
        //                                         const char * property ){ return NULL; }

        ServiceProvider(const ServiceProvider &);             // hide copy constructor
        ServiceProvider& operator=(const ServiceProvider &);  // hide assign operator
                                                            // we leave just the declarations, so the compiler will warn us
                                                            // if we try to use those two functions by accident


};

/*!
   \brief Interface for services
   Service can be visual or not.

   Visual service will provide a widget that could be used by a plugin
   Non visual service are just providing a functionnality based on properties.

   Configuration of service is done using QObject::property
   Service property are defined by service's developper.
   The following keyword are reserved:
     GPSDATA to assign an instance os GPSData to the service

 */
class ServiceInterface : public QObject
{
    Q_OBJECT
    public:
      virtual void        run()                           = 0;
      virtual QWidget*    getWidget()                     = 0;
      virtual QStringList getServicePropertyList()        = 0;
      virtual QStringList getServicePropertyDescription() = 0;
};



/*!
   \brief Interface for plugins

   \warning Do not use this interface directly. GPSBook is not designed to load such a plugin.
   \sa InputOutputPluginInterface FilterPluginInterface DisplayPluginInterface

 */
class PluginInterface : public QObject
{
    Q_OBJECT
    protected:
        QStringList mServicesList;
        QHash<QString,QStringList> mServicesPropertiesList;
    public:
        virtual QString  getName()    = 0;
        virtual QIcon    getIcon()    = 0;
        virtual QWidget* getHelp()    = 0;
        virtual QWidget* getOptions() = 0;
        virtual void     init(QWidget* parent, GPSData* gpsdata) = 0;

        virtual ServiceInterface* getService(const char * serviceName ) = 0;
        virtual QStringList getServiceList() = 0;
        virtual QStringList getServicePropertyList(const char * serviceName ) = 0;


     public slots:
        virtual void on_about() = 0;
        virtual void on_all_plugins_loaded() = 0;

 };

/*!
   \brief Interface for IO plugins

   \warning Interface used by GPSBook to load and save GPS data from/to file.
   \sa PluginInterface FilterPluginInterface DisplayPluginInterface

 */
class InputOutputPluginInterface : public PluginInterface
{
    Q_OBJECT
    public:
        virtual ~InputOutputPluginInterface() {}
        virtual QString getOpenFilter() = 0;
        virtual QString getSaveFilter() = 0;
        virtual void open(QString filename, GPSData* gpsdata) = 0;
        virtual void save(QString filename, GPSData* gpsdata) = 0;
};

/*!
   \brief Interface for filters plugins

   \warning
   \sa InputOutputPluginInterface PluginInterface DisplayPluginInterface

 */
class FilterPluginInterface : public PluginInterface
{
    Q_OBJECT
    public:
        virtual ~FilterPluginInterface() {}

        virtual void on_showPlugin() = 0;

};

/*!
   \brief Interface for display plugins

   \warning
   \sa InputOutputPluginInterface FilterPluginInterface PluginInterface

 */
class DisplayPluginInterface : public PluginInterface
{
    Q_OBJECT
    public:
        virtual ~DisplayPluginInterface() {}

        virtual QWidget* getWidget() = 0;

    public slots:
        virtual void on_gpsdataGPXChanged()       = 0;
        virtual void on_gpsdataTracksChanged()    = 0;
        virtual void on_gpsdataRoutesChanged()    = 0;
        virtual void on_gpsdataWaypointsChanged() = 0;
        virtual void on_selectionChanged()        = 0;
        virtual void on_fileLoaded()              = 0;
        virtual void on_showPlugin()              = 0;
        virtual void on_hidePlugin()              = 0;

    signals:
        void signalLoadFile(QString,bool);
        void signalFileLoaded();
        //Filters may not be applicables for all type of data.
        //This signal define what kind of data should be available for the active filter
        //signalSetTrackSelection(GPX, Waypoint, Route, Tracks, Segments);
        void signalSetTrackSelection(bool, bool, bool, bool, bool);
        void signalShowCatalog(bool);
};

Q_DECLARE_INTERFACE(PluginInterface,            "org.gpsbook.PluginInterface/1.0")
Q_DECLARE_INTERFACE(InputOutputPluginInterface, "org.gpsbook.InputOutputPluginInterface/1.0")
Q_DECLARE_INTERFACE(FilterPluginInterface,      "org.gpsbook.FilterPluginInterface/1.0")
Q_DECLARE_INTERFACE(DisplayPluginInterface,     "org.gpsbook.DisplayPluginInterface/1.0")

#endif // INTERFACE_H
