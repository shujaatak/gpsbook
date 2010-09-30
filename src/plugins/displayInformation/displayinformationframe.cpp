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
#include "displayinformationframe.h"
#include "ui_displayinformationframe.h"
#include <QDebug>

namespace PluginDisplayInformation {

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DisplayInformationFrame::DisplayInformationFrame(QWidget *parent) :
        QFrame(parent),
        m_ui(new Ui::DisplayInformationFrame)
    {
        m_ui->setupUi(this);
        settings = new QSettings("GPSBook","GPSBook");

        m_ui->stackedWidget->setCurrentIndex(0);
    } //DisplayInformationFrame::DisplayInformationFrame

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    DisplayInformationFrame::~DisplayInformationFrame()
    {
        delete m_ui;
    } //DisplayInformationFrame::~DisplayInformationFrame

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::changeEvent(QEvent *e)
    {
        switch (e->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            break;
        default:
            break;
        }
    } //DisplayInformationFrame::changeEvent

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::autoSetEnabled()
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        bool enabled = (!mGPSData->trackList.isEmpty() ||
                        !mGPSData->routeList.isEmpty() ||
                        !mGPSData->wayPointList.isEmpty());
        if (enabled != isEnabled()) {
            setEnabled(enabled);
        }
    } //DisplayInformationFrame::autoSetEnabled

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::init(GPSData* mGPSData)
    {
        qDebug() << __FILE__ << __FUNCTION__ ;
        this->mGPSData = mGPSData;
    } //DisplayInformationFrame::init

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::updateDisplay()
    {
        qDebug() << __FILE__ << __FUNCTION__;

        bool modified = false;
        mGPSData->blockSignals(true);

        autoFillDefault = settings->value("displayInformationDefaultFilling",false).toBool();

        mGPSData->lockGPSDataForRead();
        if (mGPSData->trackList.isEmpty()) {
             m_ui->stackedWidget->setCurrentIndex(0);
         }
        else
        {
            if (mGPSData->displayedWaypointIndex >= 0)
            {
                m_ui->stackedWidget->setCurrentIndex(2);
                if (mGPSData->displayedWaypointIndex >= 0 ) {
                    m_ui->lineEditWaypointName->setText(mGPSData->wayPointList[mGPSData->displayedWaypointIndex]->name);
                    m_ui->lineEditWaypointDescription->setText(mGPSData->wayPointList[mGPSData->displayedWaypointIndex]->desc);
                    m_ui->lineEditWaypointSource->setText(mGPSData->wayPointList[mGPSData->displayedWaypointIndex]->src);
                    m_ui->lineEditWaypointType->setText(mGPSData->wayPointList[mGPSData->displayedWaypointIndex]->type);
                    m_ui->lineEditWaypointSymbole->setText(mGPSData->wayPointList[mGPSData->displayedWaypointIndex]->sym);
                    m_ui->textEditWaypointComment->setText(mGPSData->wayPointList[mGPSData->displayedWaypointIndex]->cmt);
                    //listViewWaypointLinks
                }
            }
            else if (mGPSData->displayedRouteIndex >= 0)
            {
                m_ui->stackedWidget->setCurrentIndex(3);
                if (mGPSData->displayedRouteIndex >= 0 ) {
                    m_ui->lineEditRouteName->setText(mGPSData->routeList[mGPSData->displayedRouteIndex]->name);
                    m_ui->lineEditRouteDescription->setText(mGPSData->routeList[mGPSData->displayedRouteIndex]->desc);
                    m_ui->lineEditRouteSource->setText(mGPSData->routeList[mGPSData->displayedRouteIndex]->src);
                    m_ui->lineEditRouteType->setText(mGPSData->routeList[mGPSData->displayedRouteIndex]->type);
                    m_ui->textEditRouteComment->setText(mGPSData->routeList[mGPSData->displayedRouteIndex]->cmt);
                    //listViewRouteLinks
                }
            }
            else if (mGPSData->displayedTrackIndex >= 0)
            {
                m_ui->stackedWidget->setCurrentIndex(4);
                if (mGPSData->displayedTrackIndex >= 0) {
                    m_ui->lineEditTrackName->setText(mGPSData->trackList[mGPSData->displayedTrackIndex]->name);
                    m_ui->lineEditTrackDescription->setText(mGPSData->trackList[mGPSData->displayedTrackIndex]->desc);
                    m_ui->lineEditTrackSource->setText(mGPSData->trackList[mGPSData->displayedTrackIndex]->src);
                    m_ui->lineEditTrackType->setText(mGPSData->trackList[mGPSData->displayedTrackIndex]->type);
                    m_ui->textEditTrackComment->setText(mGPSData->trackList[mGPSData->displayedTrackIndex]->cmt);
                    //m_ui->listViewTrackLinks
                }
            }
            else if (mGPSData->displayedWaypointIndex == -2 &&
                     mGPSData->displayedRouteIndex    == -2 &&
                     mGPSData->displayedTrackIndex    == -2 &&
                     mGPSData->displayedSegmentIndex  == -2 )
            {
                m_ui->stackedWidget->setCurrentIndex(1);
                //Metadata
                m_ui->lineEditMetaDataName->setText(mGPSData->metadata->name);
                m_ui->lineEditMetaDataDescription->setText(mGPSData->metadata->desc);
                m_ui->dateTimeEditMetaDataDate->setDateTime(mGPSData->metadata->time);
                m_ui->lineEditMetaDataKeywords->setText(mGPSData->metadata->keywords);
                //-----------------
                if (autoFillDefault &&  (mGPSData->metadata->author->name == ""))
                {
                    mGPSData->metadata->author->name = settings->value("displayInformationDefaultAuthorName","").toString();
                    modified = true;
                }
                m_ui->lineEditMetaDataAuthorName->setText(mGPSData->metadata->author->name);
                //-----------------
                if (autoFillDefault &&  (mGPSData->metadata->author->email->id == ""))
                {
                    mGPSData->metadata->author->email->id = settings->value("displayInformationDefaultEMailId","").toString();
                    modified = true;
                }
                m_ui->lineEditMetaDataEMailId->setText(mGPSData->metadata->author->email->id);
                //-----------------
                if (autoFillDefault &&  (mGPSData->metadata->author->email->domain == ""))
                {
                    mGPSData->metadata->author->email->domain = settings->value("displayInformationDefaultEMailDomain","").toString();
                    modified = true;
                }
                m_ui->lineEditMetaDataEMailDomain->setText(mGPSData->metadata->author->email->domain);
                //-----------------
                if (autoFillDefault &&  (mGPSData->metadata->copyright->author == ""))
                {
                    mGPSData->metadata->copyright->author = settings->value("displayInformationDefaultOwner","").toString();
                    modified = true;
                }
                m_ui->lineEditMetaDataCopyrightOwner->setText(mGPSData->metadata->copyright->author);
                //-----------------
                if (autoFillDefault &&  (mGPSData->metadata->copyright->license == ""))
                {
                    mGPSData->metadata->copyright->license = settings->value("displayInformationDefaultLicense","").toString();
                    modified = true;
                }
                m_ui->lineEditMetaDataCopyrightLicense->setText(mGPSData->metadata->copyright->license);
                //-----------------
                if (autoFillDefault &&  (mGPSData->metadata->copyright->year == 0))
                {
                    mGPSData->metadata->copyright->year = 2010;
                    modified = true;
                }
                m_ui->lineEditMetaDataLicenseYear->setText(QString::number(mGPSData->metadata->copyright->year));
            }
            else
            {
                m_ui->stackedWidget->setCurrentIndex(0);
            }
        }
        mGPSData->unlockGPSData();

        mGPSData->blockSignals(false);
        mGPSData->setModified(modified);

    } //DisplayInformationFrame::updateDisplay


    /////////////////////////////////////////////////////////////////////////////////////////////////


    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_lineEditMetaDataName_editingFinished()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        mGPSData->metadata->name = m_ui->lineEditMetaDataName->text();
        mGPSData->setModified(true);
    } //DisplayInformationFrame::on_lineEditGPSDataName_editingFinished

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_lineEditMetaDataDescription_editingFinished()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        mGPSData->metadata->desc = m_ui->lineEditMetaDataDescription->text();
        mGPSData->setModified(true);
    } //DisplayInformationFrame::on_lineEditDescription_editingFinished

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_dateTimeEditMetaDataDate_editingFinished()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        mGPSData->metadata->time = m_ui->dateTimeEditMetaDataDate->dateTime();
        mGPSData->setModified(true);
    } //DisplayInformationFrame::on_dateTimeEdit_editingFinished

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_lineEditMetaDataKeywords_editingFinished()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        mGPSData->metadata->keywords = m_ui->lineEditMetaDataKeywords->text();
        mGPSData->setModified(true);
    } //DisplayInformationFrame::on_lineEditKeywords_editingFinished

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_lineEditMetaDataAuthorName_editingFinished()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        mGPSData->metadata->author->name = m_ui->lineEditMetaDataAuthorName->text();
        mGPSData->setModified(true);
    } //DisplayInformationFrame::on_lineEditAuthorName_editingFinished

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_lineEditMetaDataEMailId_editingFinished()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        mGPSData->metadata->author->email->id = m_ui->lineEditMetaDataEMailId->text();
        mGPSData->setModified(true);
    } //DisplayInformationFrame::on_lineEditEMailId_editingFinished

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_lineEditMetaDataEMailDomain_editingFinished()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        mGPSData->metadata->author->email->domain = m_ui->lineEditMetaDataEMailDomain->text();
        mGPSData->setModified(true);
    } //DisplayInformationFrame::on_lineEditEMailDomain_editingFinished

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_lineEditMetaDataCopyrightOwner_editingFinished()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        mGPSData->metadata->copyright->author = m_ui->lineEditMetaDataCopyrightOwner->text();
        mGPSData->setModified(true);
    } //DisplayInformationFrame::on_lineEditCopyrightOwner_editingFinished

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_lineEditMetaDataLicenseYear_editingFinished()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        mGPSData->metadata->copyright->year = m_ui->lineEditMetaDataLicenseYear->text().toInt();
        mGPSData->setModified(true);
    } //DisplayInformationFrame::on_lineEditLicenseYear_editingFinished

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_lineEditMetaDataCopyrightLicense_editingFinished()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        mGPSData->metadata->copyright->license = m_ui->lineEditMetaDataCopyrightLicense->text();
        mGPSData->setModified(true);
    } //DisplayInformationFrame::on_lineEditCopyrightLicense_editingFinished



    /////////////////////////////////////////////////////////////////////////////////////////////////


    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_lineEditTrackName_editingFinished()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        mGPSData->trackList[mGPSData->displayedTrackIndex]->name = m_ui->lineEditTrackName->text();
        mGPSData->setModified(true);
    } // DisplayInformationFrame::on_lineEditTrackName_editingFinished

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_lineEditTrackDescription_editingFinished()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        mGPSData->trackList[mGPSData->displayedTrackIndex]->desc = m_ui->lineEditTrackDescription->text();
        mGPSData->setModified(true);
    } // DisplayInformationFrame::on_lineEditTrackDescription_editingFinished

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_lineEditTrackSource_editingFinished()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        mGPSData->trackList[mGPSData->displayedTrackIndex]->src = m_ui->lineEditTrackSource->text();
        mGPSData->setModified(true);
    } // DisplayInformationFrame::on_lineEditTrackSource_editingFinished

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_textEditTrackComment_textChanged()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        if (isVisible()) {
            mGPSData->trackList[mGPSData->displayedTrackIndex]->cmt = m_ui->textEditTrackComment->toPlainText();
            mGPSData->setModified(true);
        }

    } // DisplayInformationFrame::on_textEditTrackComment_textChange

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_lineEditTrackType_editingFinished()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        mGPSData->trackList[mGPSData->displayedTrackIndex]->type = m_ui->lineEditTrackType->text();
        mGPSData->setModified(true);
    } // DisplayInformationFrame::on_lineEditTrackType_editingFinished

    /////////////////////////////////////////////////////////////////////////////////////////////////


    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_lineEditRouteName_cursorPositionChanged(int , int )
    {
        qDebug() << __FILE__ << __FUNCTION__;
        mGPSData->routeList[mGPSData->displayedRouteIndex]->name = m_ui->lineEditRouteName->text();
        mGPSData->setModified(true);
    } // DisplayInformationFrame::on_lineEditRouteName_cursorPositionChanged

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_lineEditRouteDescription_editingFinished()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        mGPSData->routeList[mGPSData->displayedRouteIndex]->desc = m_ui->lineEditRouteDescription->text();
        mGPSData->setModified(true);
    } // DisplayInformationFrame::on_lineEditRouteDescription_editingFinished

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_lineEditRouteSource_editingFinished()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        mGPSData->routeList[mGPSData->displayedRouteIndex]->src = m_ui->lineEditRouteSource->text();
        mGPSData->setModified(true);
    } // DisplayInformationFrame::on_lineEditRouteSource_editingFinished

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_textEditRouteComment_textChanged()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        if (isVisible()) {
            mGPSData->routeList[mGPSData->displayedRouteIndex]->cmt = m_ui->textEditRouteComment->toPlainText();
            mGPSData->setModified(true);
        }
    } // DisplayInformationFrame::on_textEditRouteComment_textChanged


    /////////////////////////////////////////////////////////////////////////////////////////////////


    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_lineEditWaypointName_editingFinished()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        mGPSData->wayPointList[mGPSData->displayedWaypointIndex]->name = m_ui->lineEditWaypointName->text();
        mGPSData->setModified(true);
    } // DisplayInformationFrame::on_lineEditWaypointName_editingFinished

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_lineEditWaypointDescription_editingFinished()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        mGPSData->wayPointList[mGPSData->displayedWaypointIndex]->desc = m_ui->lineEditWaypointDescription->text();
        mGPSData->setModified(true);
    } // DisplayInformationFrame::on_lineEditWaypointDescription_editingFinished

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_lineEditWaypointSource_editingFinished()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        mGPSData->wayPointList[mGPSData->displayedWaypointIndex]->src = m_ui->lineEditWaypointSource->text();
        mGPSData->setModified(true);
    } // DisplayInformationFrame::on_lineEditWaypointSource_editingFinished

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_textEditWaypointComment_textChanged()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        if (isVisible()) {
            mGPSData->wayPointList[mGPSData->displayedWaypointIndex]->cmt = m_ui->textEditWaypointComment->toPlainText();
            mGPSData->setModified(true);
        }
    } // DisplayInformationFrame::on_textEditWaypointComment_textChanged
}
