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
#ifndef DISPLAYRAWEDITORFRAME_H
#define DISPLAYRAWEDITORFRAME_H

#include <QFrame>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerxml.h>
#include "gpsdata.h"

using namespace GPSBook;

namespace Ui {
    class DisplayRawEditorFrame;
}

class DisplayRawEditorFrame : public QFrame
{
    Q_OBJECT

public:
    explicit DisplayRawEditorFrame(QWidget *parent = 0);
    ~DisplayRawEditorFrame();
    void loadfile(QString filename);
    void init(GPSData* gpsdata);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DisplayRawEditorFrame *ui;
    QsciScintilla *scintilla;
    GPSData* mGPSData;

private slots:
    void on_commandLinkButton_clicked();
};

#endif // DISPLAYRAWEDITORFRAME_H
