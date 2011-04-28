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
** Copyright (c) 2009-2010, gpsbook-team
**
****************************************************************************/
#ifndef GPSDATA_H
#define GPSDATA_H

#include <QObject>
#include <QList>
#include <QString>
#include <QDateTime>
#include <QHash>
#include <QVariant>
#include <QReadWriteLock>

namespace GPSBook {

    enum {
      GPX = 0,
      METADATA = 1,
      WAYPOINTLIST = 2,
      WAYPOINT = 3,
      ROUTELIST = 4,
      ROUTE = 5,
      TRACKLIST = 6,
      TRACK = 7,
      TRACKSEG = 8
    };

    /*!
      \brief Object reflecting GPX bounds xml object

      <bounds minlat="latitudeType [1]" minlon="longitudeType [1]"
              maxlat="latitudeType [1]" maxlon="longitudeType [1]"/>
    */
    class Bounds : public QObject
    {
        Q_OBJECT

        public:
            void clearData();
            double minlat;
            double minlon;
            double maxlat;
            double maxlon;
    };

    /*!
      \brief Object reflecting GPX email's xml object

      <email id="xsd:string [1]" domain="xsd:string [1]"/>
    */
    class EMail : public QObject
    {
        Q_OBJECT

        public:
            void clearData();
            QString id;
            QString domain;
    };

    /*!
      \brief Object reflecting GPX link's xml object

      <link href="xsd:anyURI [1]">
        <text> xsd:string </text> [0..1]
        <type> xsd:string </type> [0..1]
      </link>
    */
    class Link : public QObject
    {
        Q_OBJECT

        public:
            void clearData();
            QString href;
            QString text;
            QString type;
    };

    /*!
      \brief Object reflecting GPX person's xml object

      <person>
        <name> xsd:string </name> [0..1]
        <email> emailType </email> [0..1]
        <link> linkType </link> [0..1]
      </person>
    */
    class Person : public QObject
    {
        Q_OBJECT

        public:
            Person();
            void clearData();
            QString name;
            EMail* email;
            Link* link;
    };

    /*!
      \brief Object reflecting GPX copyright's xml object

      <copyright author="xsd:string [1]">
        <year> xsd:gYear </year> [0..1]
        <license> xsd:anyURI </license> [0..1]
      </copyright>
    */
    class Copyright : public QObject
    {
        Q_OBJECT

        public:
            void clearData();
            QString author;
            int year;
            QString license;
    };

    /*!
      \brief Object reflecting GPX wpt's xml object

      <wpt lat="latitudeType [1]" lon="longitudeType [1]">
        <ele> xsd:decimal </ele> [0..1]
        <time> xsd:dateTime </time> [0..1]
        <magvar> degreesType </magvar> [0..1]
        <geoidheight> xsd:decimal </geoidheight> [0..1]
        <name> xsd:string </name> [0..1]
        <cmt> xsd:string </cmt> [0..1]
        <desc> xsd:string </desc> [0..1]
        <src> xsd:string </src> [0..1]
        <link> linkType </link> [0..*]
        <sym> xsd:string </sym> [0..1]
        <type> xsd:string </type> [0..1]
        <fix> fixType </fix> [0..1]
        <sat> xsd:nonNegativeInteger </sat> [0..1]
        <hdop> xsd:decimal </hdop> [0..1]
        <vdop> xsd:decimal </vdop> [0..1]
        <pdop> xsd:decimal </pdop> [0..1]
        <ageofdgpsdata> xsd:decimal </ageofdgpsdata> [0..1]
        <dgpsid> dgpsStationType </dgpsid> [0..1]
        <extensions> extensionsType </extensions> [0..1]
      </wpt>
    */
    class WayPoint : public QObject
    {
        Q_OBJECT

        public:
            WayPoint();
            void clearData();
            double lat;
            double lon;
            double ele;
            QDateTime time;
            double magvar;
            double geoidheight;
            QString name;
            QString cmt;
            QString desc;
            QString src;
            QList<Link*> linkList;
            QString sym;
            QString type;
            QString fix;
            int sat;
            double hdop;
            double vdop;
            double pdop;
            double ageofdgpsdata;
            int dgpsid;
            QVariantHash extensions;
    };

    /*!
      \brief Object reflecting GPX trkseg's xml object

      <trkseg>
        <trkpt> wptType </trkpt> [0..*]
        <extensions> extensionsType </extensions> [0..1]
      </trkseg>
    */
    class TrackSeg : public QObject
    {
        Q_OBJECT

        public:
            void clearData();
            QList<WayPoint*> wayPointList;
            QVariantHash extensions;
    };

    /*!
      \brief Object reflecting GPX trk's xml object

      <trk>
        <name> xsd:string </name> [0..1]
        <cmt> xsd:string </cmt> [0..1]
        <desc> xsd:string </desc> [0..1]
        <src> xsd:string </src> [0..1]
        <link> linkType </link> [0..*]
        <number> xsd:nonNegativeInteger </number> [0..1]
        <type> xsd:string </type> [0..1]
        <extensions> extensionsType </extensions> [0..1]
        <trkseg> trksegType </trkseg> [0..*]
      </trk>
    */
    class Track : public QObject
    {
        Q_OBJECT

        public:
            void clearData();
            QString name;
            QString cmt;
            QString desc;
            QString src;
            QList<Link*> linkList;
            int number;
            QString type;
            QList<TrackSeg*> trackSegList;
            QVariantHash extensions;
    };

    /*!
      \brief Object reflecting GPX rte's xml object

      <rte>
        <name> xsd:string </name> [0..1]
        <cmt> xsd:string </cmt> [0..1]
        <desc> xsd:string </desc> [0..1]
        <src> xsd:string </src> [0..1]
        <link> linkType </link> [0..*]
        <number> xsd:nonNegativeInteger </number> [0..1]
        <type> xsd:string </type> [0..1]
        <extensions> extensionsType </extensions> [0..1]
        <rtept> wptType </rtept> [0..*]
      </rte>
    */
    class Route : public TrackSeg
    {
        Q_OBJECT

        public:
            void clearData();
            QString name;
            QString cmt;
            QString desc;
            QString src;
            QList<Link*> linkList;
            int number;
            QString type;

            //inherited from trackSegment
            //QList<WayPoint*> trackPointList;
            //QVariantHash extensions;
    };



    /*!
      \brief Object reflecting GPX metadata's xml object

      <metadata>
        <name> xsd:string </name> [0..1]
        <desc> xsd:string </desc> [0..1]
        <author> personType </author> [0..1]
        <copyright> copyrightType </copyright> [0..1]
        <link> linkType </link> [0..*]
        <time> xsd:dateTime </time> [0..1]
        <keywords> xsd:string </keywords> [0..1]
        <bounds> boundsType </bounds> [0..1]
        <extensions> extensionsType </extensions> [0..1]
      </metadata>
    */
    class MetaData : public QObject
    {
        Q_OBJECT

        public:
            MetaData();
            void clearData();
            QString name;
            QString desc;
            Person* author;
            Copyright* copyright;
            QList<Link*> linkList;
            QDateTime time;
            QString keywords;
            Bounds* bounds;
            QVariantHash extensions;
    };

    /*!
      \brief Object reflecting GPX's xml object

      <gpx version="1.1 [1]" creator="xsd:string [1]">
          <metadata> metadataType </metadata> [0..1]
          <wpt> wptType </wpt> [0..*]
          <rte> rteType </rte> [0..*]
          <trk> trkType </trk> [0..*]
          <extensions> extensionsType </extensions> [0..1]
      </gpx>
    */
    class GPSData : public QObject
    {
        Q_OBJECT

        private:
            //static GPSData* m_instance;
            QReadWriteLock* readWriteLock;

        public:
            GPSData();
            ~GPSData ();
            QString filename;
            bool isFromCatalog;
            bool isModified;
            QString version;
            QString creator;
            MetaData* metadata;
            QList<WayPoint*> wayPointList;
            QList<Route*> routeList;
            QList<Track*> trackList;
            QVariantHash extensions;

            /*static GPSData *instance ()
            {
                if (m_instance == 0)
                    m_instance = new GPSData ();
                return (m_instance);
            }

            ~GPSData ()
            {
                m_instance = 0;
            }*/

            void lockGPSDataForWrite();
            void lockGPSDataForRead();
            void unlockGPSData();
            void clearData();
            void setGPXModified(bool value);
            void setRoutesModified(bool value);
            void setTracksModified(bool value);
            void setWaypointsModified(bool value);
            void setFromCatalog(bool value);
            QVariant getExtensionData(QVariantHash extensions, QString extensionName, QString key);
            QVariantHash setExtensionData(QVariantHash extensions, QString extensionName, QString key, QVariant value);

            //Displayed is typically the element selected into the catalog (left side)
            int displayedWaypointIndex;
            int displayedRouteIndex;
            int displayedTrackIndex;
            int displayedSegmentIndex;

            //Some time, the element displayed can have many sub element.
            //This sub element could be selected individualy by a plugin
            int selectedWaypointIndex;
            int selectedRouteIndex;
            int selectedTrackIndex;
            int selectedSegmentIndex;

        signals:
            void signalGPSDataLocked();
            void signalGPSDataUnlocked();
            void signalGPSDataGPXUpdated();
            void signalGPSDataTracksUpdated();
            void signalGPSDataRoutesUpdated();
            void signalGPSDataWaypointsUpdated();
    };
}

#endif // GPSDATA_H
