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
#ifndef DISPLAYTEMPLATEOPTIONSFRAME_H
#define DISPLAYTEMPLATEOPTIONSFRAME_H

#include <QFrame>
#include <QAbstractButton>


namespace Ui {
    class DisplayStatisticOptionsFrame;
}

namespace PluginDisplayStatistic {

    class DisplayStatisticOptionsFrame : public QFrame
    {
        Q_OBJECT

    public:
        explicit DisplayStatisticOptionsFrame(QWidget *parent = 0);
        ~DisplayStatisticOptionsFrame();

    protected:
        void changeEvent(QEvent *e);

    private:
        Ui::DisplayStatisticOptionsFrame *ui;

private slots:
    void on_buttonBox_clicked(QAbstractButton* );
    void on_checkBoxOtherUpdateDistances_toggled(bool);
    void on_checkBoxOtherUpdateAcceleration_toggled(bool);
    void on_checkBoxOtherUpdateSpeed_toggled(bool);
    void on_checkBoxCatalogUpdateDistance_toggled(bool);
    void on_checkBoxCatalogUpdateAcceleration_toggled(bool);
    void on_checkBoxCatalogUpdateSpeed_toggled(bool);
};

}

#endif // DISPLAYTEMPLATEOPTIONSFRAME_H
