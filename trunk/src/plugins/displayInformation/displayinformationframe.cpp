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
#include <QMessageBox>
#include <QFileInfo>
#include "dialoglinkedition.h"
#include "dialogselectsymbol.h"

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
                    m_ui->lineEditWaypointSymbol->setText(mGPSData->wayPointList[mGPSData->displayedWaypointIndex]->sym);
                    updateToolButtonWaypointSymbol();
                    m_ui->textEditWaypointComment->setText(mGPSData->wayPointList[mGPSData->displayedWaypointIndex]->cmt);
                    updateWaypointlinks();
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
                    updateRoutelinks();
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
                    updateTracklinks();
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
                m_ui->checkBoxUseDate->setChecked(mGPSData->metadata->time.isValid());
                if (m_ui->checkBoxUseDate->isChecked()) {
                    m_ui->dateTimeEditMetaDataDate->setDateTime(mGPSData->metadata->time);
                }
                else {
                    m_ui->dateTimeEditMetaDataDate->setDateTime(QDateTime::currentDateTime());
                }

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
                updateMetadatalinks();
            }
            else
            {
                m_ui->stackedWidget->setCurrentIndex(0);
            }
        }
        mGPSData->unlockGPSData();

        mGPSData->blockSignals(false);
        if (modified){
            //Send signal only if modified
            mGPSData->setModified(modified);
        }
    } //DisplayInformationFrame::updateDisplay

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::editLink(QList<Link*>* linkList, int id)
    {
        DialogLinkEdition* linkEdition = new DialogLinkEdition(this);
        Link* link;
        if(id < linkList->count())
        {
            link = linkList->at(id);
            linkEdition->init(link->text,link->href,link->type);
        }
        else
        {
            link = new Link();
        }
        if (linkEdition->exec() == QDialog::Accepted)
        {
            link->text = linkEdition->text;
            link->href = linkEdition->url;
            link->type = linkEdition->mimetype;
            if(id == linkList->count())
            {
                linkList->append(link);
            }
            mGPSData->setModified(true);
        }
        delete linkEdition;
    }

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

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::updateMetadatalinks()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        m_ui->listWidgetMetaDataLinks->clear();
        foreach (Link* link, mGPSData->metadata->linkList) {
            QListWidgetItem* item = new QListWidgetItem(link->text);
            m_ui->listWidgetMetaDataLinks->addItem(item);
        }
    } // DisplayInformationFrame::updateTracklinks

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

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::updateTracklinks()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        m_ui->listWidgetTrackLinks->clear();
        foreach (Link* link, mGPSData->trackList[mGPSData->displayedTrackIndex]->linkList) {
            QListWidgetItem* item = new QListWidgetItem(link->text);
            m_ui->listWidgetTrackLinks->addItem(item);
        }
    } // DisplayInformationFrame::updateTracklinks

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


    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::updateRoutelinks()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        m_ui->listWidgetRouteLinks->clear();
        foreach (Link* link, mGPSData->routeList[mGPSData->displayedRouteIndex]->linkList) {
            QListWidgetItem* item = new QListWidgetItem(link->text);
            m_ui->listWidgetRouteLinks->addItem(item);
        }
    } // DisplayInformationFrame::updateRoutelinks

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


    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_lineEditWaypointSymbol_editingFinished()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        if (isVisible()) {
            mGPSData->wayPointList[mGPSData->displayedWaypointIndex]->sym = m_ui->lineEditWaypointSymbol->text();
            updateToolButtonWaypointSymbol();
            mGPSData->setModified(true);
        }
    } // DisplayInformationFrame::on_textEditWaypointComment_textChanged


    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::updateWaypointlinks()
    {
        qDebug() << __FILE__ << __FUNCTION__;
        m_ui->listWidgetWaypointLinks->clear();
        foreach (Link* link, mGPSData->wayPointList[mGPSData->displayedWaypointIndex]->linkList) {
            QListWidgetItem* item = new QListWidgetItem(link->text);
            m_ui->listWidgetWaypointLinks->addItem(item);
        }
    } // DisplayInformationFrame::updateRoutelinks

    /////////////////////////////////////////////////////////////////////////////////////////////////


    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_toolButtonTrackAddLink_clicked()
    {
        QList<Link*> *linkList = &(mGPSData->trackList[mGPSData->displayedTrackIndex]->linkList);
        editLink(linkList,linkList->count());
        updateTracklinks();
    }// DisplayInformationFrame::on_toolButtonTrackAddLink_clicked

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_toolButtonTrackEditLink_clicked()
    {
        if (m_ui->listWidgetTrackLinks->selectedItems().count())
        {
            QList<Link*> *linkList = &(mGPSData->trackList[mGPSData->displayedTrackIndex]->linkList);
            editLink(linkList,m_ui->listWidgetTrackLinks->currentIndex().row());
            updateTracklinks();
        }
    }// DisplayInformationFrame::on_toolButtonTrackEditLink_clicked

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_toolButtonTrackDeleteLink_clicked()
    {
        if (m_ui->listWidgetTrackLinks->selectedItems().count())
        {
            if (QMessageBox::question(this,
                                      tr("Deleting link"),
                                      tr("Your are about to delete the selected link.") ,
                                      QMessageBox::Ok|QMessageBox::Cancel) == QMessageBox::Ok)
            {
                mGPSData->trackList[mGPSData->displayedTrackIndex]->linkList.removeAt(m_ui->listWidgetTrackLinks->currentIndex().row());
                mGPSData->setModified(true);
                updateTracklinks();
            }
        }

    }// DisplayInformationFrame::on_toolButtonTrackDeleteLink_clicked

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_listWidgetTrackLinks_itemSelectionChanged()
    {
        m_ui->toolButtonTrackEditLink->setEnabled(m_ui->listWidgetTrackLinks->selectedItems().count() != 0);
        m_ui->toolButtonTrackDeleteLink->setEnabled(m_ui->listWidgetTrackLinks->selectedItems().count() != 0);
    }// DisplayInformationFrame::on_listWidgetTrackLinks_itemSelectionChanged


    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_toolButtonMetadataAddLink_clicked()
    {
        QList<Link*> *linkList = &(mGPSData->metadata->linkList);
        editLink(linkList,linkList->count());
        updateMetadatalinks();
    }// DisplayInformationFrame::on_toolButtonMetadataAddLink_clicked()

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_toolButtonMetadataEditLink_clicked()
    {
        if (m_ui->listWidgetMetaDataLinks->selectedItems().count())
        {
            QList<Link*> *linkList = &(mGPSData->metadata->linkList);
            editLink(linkList,m_ui->listWidgetMetaDataLinks->currentIndex().row());
            updateMetadatalinks();
        }
    }// DisplayInformationFrame::on_toolButtonMetadataEditLink_clicked()

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_toolButtonMetadataDeleteLink_clicked()
    {
        if (m_ui->listWidgetMetaDataLinks->selectedItems().count())
        {
            if (QMessageBox::question(this,
                                      tr("Deleting link"),
                                      tr("Your are about to delete the selected link.") ,
                                      QMessageBox::Ok|QMessageBox::Cancel) == QMessageBox::Ok)
            {
                mGPSData->metadata->linkList.removeAt(m_ui->listWidgetMetaDataLinks->currentIndex().row());
                mGPSData->setModified(true);
                updateTracklinks();
            }
        }
    }// DisplayInformationFrame::on_toolButtonMetadataDeleteLink_clicked()

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_listWidgetMetaDataLinks_itemSelectionChanged()
    {
        m_ui->toolButtonMetadataEditLink->setEnabled(m_ui->listWidgetMetaDataLinks->selectedItems().count() != 0);
        m_ui->toolButtonMetadataDeleteLink->setEnabled(m_ui->listWidgetMetaDataLinks->selectedItems().count() != 0);
    }// DisplayInformationFrame::on_listWidgetMetaDataLinks_itemSelectionChanged()

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_toolButtonWaypointAddLink_clicked()
    {
        QList<Link*> *linkList = &(mGPSData->wayPointList[mGPSData->displayedWaypointIndex]->linkList);
        editLink(linkList,linkList->count());
        updateWaypointlinks();
    }// DisplayInformationFrame::on_toolButtonWaypointAddLink_clicked()

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_toolButtonWaypointEditLink_clicked()
    {
        if (m_ui->listWidgetWaypointLinks->selectedItems().count())
        {
            QList<Link*> *linkList = &(mGPSData->wayPointList[mGPSData->displayedWaypointIndex]->linkList);
            editLink(linkList,m_ui->listWidgetWaypointLinks->currentIndex().row());
            updateWaypointlinks();
        }
    }// DisplayInformationFrame::on_toolButtonWaypointEditLink_clicked()

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_toolButtonWaypointDeleteLink_clicked()
    {
        if (m_ui->listWidgetWaypointLinks->selectedItems().count())
        {
            if (QMessageBox::question(this,
                                      tr("Deleting link"),
                                      tr("Your are about to delete the selected link.") ,
                                      QMessageBox::Ok|QMessageBox::Cancel) == QMessageBox::Ok)
            {
                mGPSData->wayPointList[mGPSData->displayedWaypointIndex]->linkList.removeAt(m_ui->listWidgetWaypointLinks->currentIndex().row());
                mGPSData->setModified(true);
                updateWaypointlinks();
            }
        }
    }// DisplayInformationFrame::on_toolButtonWaypointDeleteLink_clicked()

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_listWidgetWaypointLinks_itemSelectionChanged()
    {
        m_ui->toolButtonWaypointEditLink->setEnabled(m_ui->listWidgetWaypointLinks->selectedItems().count() != 0);
        m_ui->toolButtonWaypointDeleteLink->setEnabled(m_ui->listWidgetWaypointLinks->selectedItems().count() != 0);
    }// DisplayInformationFrame::on_listWidgetWaypointLinks_itemSelectionChanged()

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_toolButtonRouteAddLink_clicked()
    {
        QList<Link*> *linkList = &(mGPSData->routeList[mGPSData->displayedRouteIndex]->linkList);
        editLink(linkList,linkList->count());
        updateRoutelinks();
    }// DisplayInformationFrame::on_toolButtonRouteAddLink_clicked()

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_toolButtonRouteEditLink_clicked()
    {
        if (m_ui->listWidgetRouteLinks->selectedItems().count())
        {
            QList<Link*> *linkList = &(mGPSData->routeList[mGPSData->displayedRouteIndex]->linkList);
            editLink(linkList,m_ui->listWidgetRouteLinks->currentIndex().row());
            updateRoutelinks();
        }
    }// DisplayInformationFrame::on_toolButtonRouteEditLink_clicked()

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_toolButtonRouteDeleteLink_clicked()
    {
        if (m_ui->listWidgetRouteLinks->selectedItems().count())
        {
            if (QMessageBox::question(this,
                                      tr("Deleting link"),
                                      tr("Your are about to delete the selected link.") ,
                                      QMessageBox::Ok|QMessageBox::Cancel) == QMessageBox::Ok)
            {
                mGPSData->routeList[mGPSData->displayedRouteIndex]->linkList.removeAt(m_ui->listWidgetRouteLinks->currentIndex().row());
                mGPSData->setModified(true);
                updateRoutelinks();
            }
        }
    }// DisplayInformationFrame::on_toolButtonRouteDeleteLink_clicked()

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_listWidgetRouteLinks_itemSelectionChanged()
    {
        m_ui->toolButtonRouteEditLink->setEnabled(m_ui->listWidgetRouteLinks->selectedItems().count() != 0);
        m_ui->toolButtonRouteDeleteLink->setEnabled(m_ui->listWidgetRouteLinks->selectedItems().count() != 0);
    }// DisplayInformationFrame::on_listWidgetRouteLinks_itemSelectionChanged()

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::on_toolButtonWaypointSymbol_clicked()
    {
        DialogSelectSymbol* selectSymbol = new DialogSelectSymbol(this);
        selectSymbol->setSelectedIcon(m_ui->lineEditWaypointSymbol->text());
        if (selectSymbol->exec() == QDialog::Accepted)
        {
            m_ui->lineEditWaypointSymbol->setText(selectSymbol->getSelectedSymbolName());
            on_lineEditWaypointSymbol_editingFinished();
        }
        delete selectSymbol;
    }// DisplayInformationFrame::on_toolButtonWaypointSymbol_clicked()

    /*------------------------------------------------------------------------------*

     *------------------------------------------------------------------------------*/
    void DisplayInformationFrame::updateToolButtonWaypointSymbol()
    {
        if (QFileInfo( ":/icons/icons/" +  m_ui->lineEditWaypointSymbol->text() + ".png" ).exists()) {
            m_ui->toolButtonWaypointSymbol->setIcon(QIcon(":/icons/icons/" +  m_ui->lineEditWaypointSymbol->text() + ".png"));
        }
        else if (QFileInfo( ":/icons/google-marker/" +  m_ui->lineEditWaypointSymbol->text() + ".png" ).exists()) {
            m_ui->toolButtonWaypointSymbol->setIcon(QIcon(":/icons/google-marker/" +  m_ui->lineEditWaypointSymbol->text() + ".png"));
        }
        else if (QFileInfo( ":/icons/numeric/" +  m_ui->lineEditWaypointSymbol->text() + ".png" ).exists()) {
            m_ui->toolButtonWaypointSymbol->setIcon(QIcon(":/icons/numeric/" +  m_ui->lineEditWaypointSymbol->text() + ".png"));
        }
        else
        {
            m_ui->toolButtonWaypointSymbol->setIcon(QIcon(":/icons/icons/cluster4.png"));
        }
    }// DisplayInformationFrame::updateToolButtonWaypointSymbol()
}

void PluginDisplayInformation::DisplayInformationFrame::on_checkBoxUseDate_toggled(bool checked)
{
    if (checked) {
        mGPSData->metadata->time = m_ui->dateTimeEditMetaDataDate->dateTime();
    }
    else {
        mGPSData->metadata->time = QDateTime(QDate(0,0,0));
    }
    mGPSData->setModified(true);
}
