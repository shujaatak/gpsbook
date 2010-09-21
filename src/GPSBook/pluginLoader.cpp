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

#include <QtGui>

#include "pluginLoader.h"
#include "pluginInterface.h"

PluginLoader * PluginLoader::m_instance = 0;


PluginLoader::PluginLoader()
{
    QDir pluginsDir = QDir(qApp->applicationDirPath());
    pluginsDir.cd("plugins");

    foreach (QString fileName, pluginsDir.entryList(QDir::Files))
    {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin)
        {
            IPluginInterface * op = qobject_cast<IPluginInterface *>(plugin);
            if (op)
            {
                m_operationList << op;
            }
        }
    }
}

PluginLoader::~PluginLoader()
{
    /* Pas besoin de supprimer les plugins en mémoire,
    ils sont supprimés automatiquement �  la fin. */
}

PluginLoader * PluginLoader::instance()
{
    if (m_instance==0) {
        m_instance = new PluginLoader();
    }
    return m_instance;
}

IPluginInterface * PluginLoader::getPluginByName( QString pluginName )
{
    foreach(IPluginInterface * plugin, m_operationList) {
        if (plugin->getName() == pluginName) {
            return plugin;
        }
    }
    return 0;
}
