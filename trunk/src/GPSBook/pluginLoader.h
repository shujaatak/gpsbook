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
** Copyright (c) 2009, gpsbook-team
**
****************************************************************************/

#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <QList>

class IPluginInterface;
class QString;
class QStringList;

/*!
 * \brief Gestionnaire d'opérations
 * Il est utilisé pour le chargement des différents plugins. Il garde les
 * plugins en mémoire après le chargement.
 * Cette classe est un singleton.
 */
class PluginLoader
{
public:
    ~PluginLoader();

    QStringList operationList();

    static PluginLoader * instance();

private:
    PluginLoader();
    IPluginInterface * getPluginByName( QString pluginName );

private:
    QList<IPluginInterface *> m_operationList;
    static PluginLoader * m_instance;
};

#endif // PLUGINLOADER_H
