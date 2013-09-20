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
** Copyright (c) 2010, gpsbook-team
**
****************************************************************************/
#include "displayraweditorframe.h"
#include "ui_displayraweditorframe.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>


DisplayRawEditorFrame::DisplayRawEditorFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::DisplayRawEditorFrame)
{
    qDebug() << __FILE__ << __FUNCTION__;
    ui->setupUi(this);
    scintilla = new QsciScintilla;
    QsciLexerXML *lexerXML = new QsciLexerXML;
    scintilla->setLexer(lexerXML);
    ui->verticalLayout->addWidget(scintilla);
    scintilla->setAutoIndent(true);
    scintilla->setMarginLineNumbers(1,true);
    scintilla->setMarginWidth(1, 45);
    scintilla->setFolding(QsciScintilla::BoxedTreeFoldStyle);
}

DisplayRawEditorFrame::~DisplayRawEditorFrame()
{
    delete ui;
}

void DisplayRawEditorFrame::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DisplayRawEditorFrame::loadfile(QString filename)
{
    qDebug() << __FILE__ << __FUNCTION__ << " Loading...";
    QFile file(filename);
    if (!file.open(QFile::ReadOnly)) {
        /*QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(filename)
                             .arg(file.errorString()));*/
        return;
    }
    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    scintilla->setText(in.readAll());
    QApplication::restoreOverrideCursor();
    file.close();
    qDebug() << __FILE__ << __FUNCTION__ << " ...Loaded.";
}



void DisplayRawEditorFrame::on_commandLinkButton_clicked()
{
    qDebug() << __FILE__ << __FUNCTION__;
    loadfile(mGPSData->filename);
}

void DisplayRawEditorFrame::init(GPSData* gpsdata)
{
    mGPSData = gpsdata;
}
