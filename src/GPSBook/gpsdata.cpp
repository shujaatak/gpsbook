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
#include "gpsdata.h"
#include <QList>
#include <QObject>
#include <QDebug>

namespace GPSBook {

    /*------------------------------------------------------------------------------*
      Person
     *------------------------------------------------------------------------------*/
    /*!
        \brief Create person and object it contains
    */
    Person::Person()
    {
        email = new EMail();
        link  = new Link();
    }

    /*!
        \brief Clear data
    */
    void Person::clearData()
    {
        name = "";
        email->clearData();
        link->clearData();
    }

    /*------------------------------------------------------------------------------*
      Waypoint
     *------------------------------------------------------------------------------*/
    /*!
        \brief Clear data
    */
    WayPoint::WayPoint()
    {
        lat = 0;
        lon = 0;
        ele = 0;
        magvar = 0;
        geoidheight = 0;
        name = "";
        cmt = "";
        desc = "";
        src = "";
        sym = "";
        type = "";
        fix = "";
        sat = 0;
        hdop = 0;
        vdop = 0;
        pdop = 0;
        ageofdgpsdata = 0;
        dgpsid = 0;
    }

    /*!
        \brief Clear data and delete object of the list
    */
    void WayPoint::clearData()
    {
        while (!linkList.isEmpty())
        {
            if (!linkList.isEmpty())
                delete linkList.takeFirst();
        }
    }


    /*------------------------------------------------------------------------------*
      TrackSeg
     *------------------------------------------------------------------------------*/
    /*!
        \brief Clear data and delete object of the list
    */
    void TrackSeg::clearData()
    {
        while (!wayPointList.isEmpty())
        {
            wayPointList.takeFirst()->clearData();
            if (!wayPointList.isEmpty())
                delete wayPointList.takeFirst();
        }
    }

    /*------------------------------------------------------------------------------*
      Track
     *------------------------------------------------------------------------------*/
    /*!
        \brief Clear data and delete object of the list
    */
    void Track::clearData()
    {
        while (!linkList.isEmpty())
        {
            if (!linkList.isEmpty())
                delete linkList.takeFirst();
        }
        while (!trackSegList.isEmpty())
        {
            trackSegList.takeFirst()->clearData();

            if (!trackSegList.isEmpty())
                delete trackSegList.takeFirst();
        }
    }

    /*------------------------------------------------------------------------------*
      Route
     *------------------------------------------------------------------------------*/
    /*!
        \brief Clear data
    */
    void Route::clearData()
    {
        TrackSeg::clearData();
        while (!linkList.isEmpty())
        {
            if (!linkList.isEmpty())
                delete linkList.takeFirst();
        }
    }

    /*------------------------------------------------------------------------------*
      Metadata
     *------------------------------------------------------------------------------*/
    /*!
        \brief Create metadata and object it contains
    */
    MetaData::MetaData()
    {
        author = new Person();
        copyright = new Copyright();
        bounds = new Bounds();
    }

    /*!
        \brief Clear data
    */
    void MetaData::clearData()
    {
        name = "";
        desc = "";
        author->clearData();
        copyright->clearData();
        while (!linkList.isEmpty())
        {
            if (!linkList.isEmpty())
                delete linkList.takeFirst();
        }
        time = QDateTime(QDate(0,0,0));
        keywords = "";
        bounds->clearData();
    }

    /*------------------------------------------------------------------------------*
      Copyright
     *------------------------------------------------------------------------------*/
    /*!
        \brief Clear data
    */
    void Copyright::clearData()
    {
        author = "";
        year = 0;
        license = "";
    }

    /*------------------------------------------------------------------------------*
      EMail
     *------------------------------------------------------------------------------*/
    /*!
        \brief Clear data
    */
    void EMail::clearData()
    {
        id = "";
        domain = "";
    }

    /*------------------------------------------------------------------------------*
      Link
     *------------------------------------------------------------------------------*/
    /*!
        \brief Clear data
    */
    void Link::clearData()
    {
        href = "";
        text = "";
        type = "";
    }

    /*------------------------------------------------------------------------------*
      Bounds
     *------------------------------------------------------------------------------*/
    /*!
        \brief Clear data
    */
    void Bounds::clearData()
    {
        minlat = 0;
        minlon = 0;
        maxlat = 0;
        maxlon = 0;
    }

    /*------------------------------------------------------------------------------*
      GPSData
     *------------------------------------------------------------------------------*/
    /*!
        \brief Initialize the variable representing the instance of the singleton
    */
    //GPSData* GPSData::m_instance = 0;

    /*!
        \brief Define variable representing the instance of the singleton
    */
    GPSData::GPSData()
    {
        metadata = new MetaData();
        filename = "";
        readWriteLock = new QReadWriteLock(QReadWriteLock::NonRecursive);
        clearData();
    }

    GPSData::~GPSData(){
        delete metadata;
        delete readWriteLock;
    }


    /*!
        \brief Clear data
    */
    void GPSData::clearData()
    {
        displayedWaypointIndex = -2;
        displayedRouteIndex = -2;
        displayedTrackIndex = -2;
        displayedSegmentIndex = -2;
        selectedWaypointIndex = -1;
        selectedRouteIndex = -1;
        selectedTrackIndex = -1;
        selectedSegmentIndex = -1;
        filename = "";
        isModified = false;
        isFromCatalog = false;
        version = "";
        creator = "";

        metadata->clearData();

        while (!wayPointList.isEmpty())
        {
            wayPointList.takeFirst()->clearData();
            if (!wayPointList.isEmpty())
                delete wayPointList.takeFirst();
        }

        while (!routeList.isEmpty())
        {
            routeList.takeFirst()->clearData();
            if (!routeList.isEmpty())
                delete routeList.takeFirst();
        }

        while (!trackList.isEmpty())
        {
            trackList.takeFirst()->clearData();
            if (!trackList.isEmpty())
                delete trackList.takeFirst();
        }
    }

    /*!
        \brief Lock gps data for write and emit signal to inform that data are locked.
        It is mandatory to lock GPSData before using it to avoid access conflict.
    */
    void GPSData::lockGPSDataForWrite()
    {
        //qDebug() << "GPSData::lockGPSDataForWrite - readWriteLock->tryLockForWrite()";
        readWriteLock->lockForWrite();
        emit signalGPSDataLocked();
    }

    /*!
        \brief Lock gps data for read
        It is mandatory to lock GPSData before using it to avoid access conflict.

    */
    void GPSData::lockGPSDataForRead()
    {
        //qDebug() << "GPSData::lockGPSDataForRea - readWriteLock->tryLockForRead()";
        readWriteLock->lockForRead();
    }
    /*!
        \brief Unlock gps data and emit signal to inform that data have been updated
        It is mandatory to unlock GPSData after usage to let other part of the application access to the data.
    */
    void GPSData::unlockGPSData()
    {

        //qDebug() << "GPSData::unlockGPSData - readWriteLock->unlock()";
        readWriteLock->unlock();
        emit signalGPSDataUnlocked();
    }

    /*!
        \brief Define data has modified and emit signal to inform that data have been updated

        \param value
    */
    void GPSData::setGPXModified(bool value)
    {
        qDebug() << __FILE__ << __FUNCTION__;
        isModified = value;
        emit signalGPSDataGPXUpdated();
    }

    /*!
        \brief Define tracks has modified and emit signal to inform that data have been updated

        \param value
    */
    void GPSData::setTracksModified(bool value)
    {
        qDebug() << __FILE__ << __FUNCTION__;
        isModified = value;
        emit signalGPSDataTracksUpdated();
    }

    /*!
        \brief Define routes has modified and emit signal to inform that data have been updated

        \param value
    */
    void GPSData::setRoutesModified(bool value)
    {
        qDebug() << __FILE__ << __FUNCTION__;
        isModified = value;
        emit signalGPSDataRoutesUpdated();
    }

    /*!
        \brief Define waypoints has modified and emit signal to inform that data have been updated

        \param value
    */
    void GPSData::setWaypointsModified(bool value)
    {
        qDebug() << __FILE__ << __FUNCTION__;
        isModified = value;
        emit signalGPSDataWaypointsUpdated();
    }

    /*!
        \brief Define data has been loaded from catalog

        \param value
    */
    void GPSData::setFromCatalog(bool value)
    {
        isFromCatalog = value;
        emit signalGPSDataGPXUpdated();
    }

    /*!
        \brief Get a data from extension

        \param extensions Extension where data has to be extracted
        \param extensionName Extension name to get data
        \param key Key of data to exctract
        \return Data extracted or NULL is ExtensionName or key is not existing
    */
    QVariant GPSData::getExtensionData(QVariantHash extensions, QString extensionName, QString key)
    {
        if ( ! extensions.contains( extensionName ) )
        {
            return QVariant( (int) NULL);
        }
        else
        {
           return extensions.value( extensionName ).toHash().value(key,QVariant( (int) NULL));
        }
    }


    /*!
        \brief Set a data extension

        \param extensions Extension where data has to be added
        \param extensionName Extension name to set data
        \param key Key of data to add
        \param value Value to add
        \return Extension reference to be stored by the caller
    */
    QVariantHash GPSData::setExtensionData(QVariantHash extensions, QString extensionName, QString key, QVariant value)
    {
        //qDebug() << "Adding extension [ " << ExtensionName << " | " << key << " | " << value << " ]";
        QVariantHash ext;
        if ( extensions.contains( extensionName ) )
        {
           ext = extensions.value( extensionName ).toHash();
           //qDebug() << "extension is existing: QHash(" << ext.count() << ")";
        }
        ext.insert(key,value);
        extensions.insert(extensionName, ext);
        return extensions;
    }
}
