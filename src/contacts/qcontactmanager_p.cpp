/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qcontactmanager.h"
#include "qcontactmanager_p.h"
#include "qcontactmanagerengine.h"
#include "qcontactmanagerenginefactory.h"
#include "qcontactmanagerenginev2wrapper_p.h"

#include "qcontact_p.h"

#include "qcontactaction.h"
#include "qcontactactiondescriptor.h"
#ifdef QT_SIMULATOR
#include "qcontactsimulatorbackend_p.h"
#endif

#include <QSharedData>
#include <QtPlugin>
#include <QPluginLoader>
#include <QWeakPointer>

#include <QDebug>
#include <QDir>
#include <QFile>

#include <QApplication>


#if defined(Q_OS_SYMBIAN)
# include <f32file.h>
#endif

#include "qcontactmemorybackend_p.h"
#include "qcontactinvalidbackend_p.h"
#include "qmobilitypluginsearch.h"

QTM_BEGIN_NAMESPACE

/* Shared QContactManager stuff here, default engine stuff below */
QHash<QString, QContactManagerEngineFactory*> QContactManagerData::m_engines;
QSet<QContactManager*> QContactManagerData::m_aliveEngines;
QList<QContactActionManagerPlugin*> QContactManagerData::m_actionManagers;

bool QContactManagerData::m_discoveredStatic;
QStringList QContactManagerData::m_pluginPaths;

static void qContactsCleanEngines()
{
    // This is complicated by needing to remove any engines before we unload factories
    // guard pointers as one engine could be parent of another manager and cause doubledelete
    QList<QWeakPointer<QContactManager> > aliveManagers;
    foreach(QContactManager* manager, QContactManagerData::m_aliveEngines) {
        aliveManagers << QWeakPointer<QContactManager>(manager);
    }

    foreach(QWeakPointer<QContactManager> manager, aliveManagers) {
        if (!manager) {
            // deleting engine of one manager, could cause deleting next manager in list (aggregation case)
            continue;
        }
        // We don't delete the managers here, we just kill their engines
        // and replace it with an invalid engine (for safety :/)
        QContactManagerData* d = QContactManagerData::managerData(manager.data());

        delete d->m_engine;
        d->m_engine = new QContactInvalidEngine();
    }

    QList<QContactManagerEngineFactory*> factories = QContactManagerData::m_engines.values();

    for (int i=0; i < factories.count(); i++) {
        delete factories.at(i);
    }
    QContactManagerData::m_engines.clear();
    QContactManagerData::m_actionManagers.clear();
    QContactManagerData::m_aliveEngines.clear();
}

static int parameterValue(const QMap<QString, QString>& parameters, const char* key, int defaultValue)
{
    if (parameters.contains(QString::fromAscii(key))) {
        bool ok;
        int version = parameters.value(QString::fromAscii(key)).toInt(&ok);

        if (ok)
            return version;
    }
    return defaultValue;
}

void QContactManagerData::createEngine(const QString& managerName, const QMap<QString, QString>& parameters)
{
    m_engine = 0;

    QString builtManagerName = managerName.isEmpty() ? QContactManager::availableManagers().value(0) : managerName;
    if (builtManagerName == QLatin1String("memory")) {
        QContactManagerEngine* engine = QContactMemoryEngine::createMemoryEngine(parameters);
        m_engine = new QContactManagerEngineV2Wrapper(engine);
        m_signalSource = engine;
#ifdef QT_SIMULATOR
    } else if (builtManagerName == QLatin1String("simulator")) {
        QContactManagerEngine* engine = QContactSimulatorEngine::createSimulatorEngine(parameters);
        m_engine = new QContactManagerEngineV2Wrapper(engine);
        m_signalSource = engine;
#endif
    } else {
        int implementationVersion = parameterValue(parameters, QTCONTACTS_IMPLEMENTATION_VERSION_NAME, -1);

        bool found = false;
        bool loadedDynamic = false;

        /* First check static factories */
        loadStaticFactories();

        /* See if we got a fast hit */
        QList<QContactManagerEngineFactory*> factories = m_engines.values(builtManagerName);
        m_lastError = QContactManager::NoError;

        while(!found) {
            foreach (QContactManagerEngineFactory* f, factories) {
                QList<int> versions = f->supportedImplementationVersions();
                if (implementationVersion == -1 ||//no given implementation version required
                        versions.isEmpty() || //the manager engine factory does not report any version
                        versions.contains(implementationVersion)) {
                    QContactManagerEngine* engine = f->engine(parameters, &m_lastError);
                    // if it's a V2, use it
                    // qobject_cast for QContactManagerEngineV2 broken, see QTMOBILITY-1798
                    // Workaround: use code behind general qobject_cast explicitly:
                    //m_engine = qobject_cast<QContactManagerEngineV2*>(engine);
                    m_engine = static_cast<QContactManagerEngineV2*>(reinterpret_cast<QContactManagerEngineV2*>(0)->staticMetaObject.cast(engine));
                    if (!m_engine && engine) {
                        // Nope, v1, so wrap it
                        m_engine = new QContactManagerEngineV2Wrapper(engine);
                        m_signalSource = engine;
                    } else {
                        m_signalSource = m_engine; // use the v2 engine directly
                    }
                    found = true;
                    break;
                }
            }

            // Break if found or if this is the second time through
            if (loadedDynamic || found)
                break;

            // otherwise load dynamic factories and reloop
            loadFactories();
            factories = m_engines.values(builtManagerName);
            loadedDynamic = true;
        }

        // XXX remove this
        // the engine factory could lie to us, so check the real implementation version
        if (m_engine && (implementationVersion != -1 && m_engine->managerVersion() != implementationVersion)) {
            m_lastError = QContactManager::VersionMismatchError;
            m_signalSource = m_engine = 0;
        }

        if (!m_engine) {
            if (m_lastError == QContactManager::NoError)
                m_lastError = QContactManager::DoesNotExistError;
            m_signalSource = m_engine = new QContactInvalidEngine();
        }
    }
}


void QContactManagerData::loadStaticFactories()
{
    if (!m_discoveredStatic) {
#if !defined QT_NO_DEBUG
        const bool showDebug = qgetenv("QT_DEBUG_PLUGINS").toInt() > 0;
#endif

        m_discoveredStatic = true;

        /* Clean stuff up at the end */
        qAddPostRoutine(qContactsCleanEngines);

        /* Loop over all the static plugins */
        QObjectList staticPlugins = QPluginLoader::staticInstances();
        for (int i=0; i < staticPlugins.count(); i++ ){
            QContactManagerEngineFactory *f = qobject_cast<QContactManagerEngineFactory*>(staticPlugins.at(i));
            if (f) {
                QString name = f->managerName();
#if !defined QT_NO_DEBUG
                if (showDebug)
                    qDebug() << "Static: found an engine plugin" << f << "with name" << name;
#endif
                if (name != QLatin1String("memory") && name != QLatin1String("invalid") && !name.isEmpty()) {
                    // we also need to ensure that we haven't already loaded this factory.
                    if (m_engines.keys().contains(name)) {
                        qWarning() << "Static contacts plugin" << name << "has the same name as a currently loaded plugin; ignored";
                    } else {
                        m_engines.insertMulti(name, f);
                    }
                } else {
                    qWarning() << "Static contacts plugin with reserved name" << name << "ignored";
                }
            }
        }
    }
}


/* Plugin loader */
void QContactManagerData::loadFactories()
{
#if !defined QT_NO_DEBUG
    const bool showDebug = qgetenv("QT_DEBUG_PLUGINS").toInt() > 0;
#endif

    // Always do this..
    loadStaticFactories();

    // But only load dynamic plugins when the paths change
    QStringList paths = QCoreApplication::libraryPaths();
#ifdef QTM_PLUGIN_PATH
    paths << QLatin1String(QTM_PLUGIN_PATH);
#endif

    if (paths != m_pluginPaths) {
        m_pluginPaths = paths;

        QStringList plugins = mobilityPlugins(QLatin1String("contacts"));

        /* Now discover the dynamic plugins */
        for (int i=0; i < plugins.count(); i++) {
            QPluginLoader qpl(plugins.at(i));

#if !defined QT_NO_DEBUG
            if (showDebug)
                qDebug() << "Loading plugin" << plugins.at(i);
#endif

            QContactManagerEngineFactory *f = qobject_cast<QContactManagerEngineFactory*>(qpl.instance());
            QContactActionManagerPlugin *m = qobject_cast<QContactActionManagerPlugin*>(qpl.instance());

            if (f) {
                QString name = f->managerName();
#if !defined QT_NO_DEBUG
                if (showDebug)
                    qDebug() << "Dynamic: found a contact engine plugin" << f << "with name" << name;
#endif
                if (name != QLatin1String("memory") && name != QLatin1String("invalid") && !name.isEmpty()) {
                    // we also need to ensure that we haven't already loaded this factory.
                    if (m_engines.keys().contains(name)) {
                        qWarning() << "Contacts plugin" << plugins.at(i) << "has the same name as currently loaded plugin" << name << "; ignored";
                    } else {
                        m_engines.insertMulti(name, f);
                    }
                } else {
                    qWarning() << "Contacts plugin" << plugins.at(i) << "with reserved name" << name << "ignored";
                }
            }

            if (m) {
                m_actionManagers.append(m);
            }

            /* Debugging */
#if !defined QT_NO_DEBUG
            if (showDebug && !f && !m) {
                qDebug() << "Unknown plugin:" << qpl.errorString();
                if (qpl.instance()) {
                    qDebug() << "[qobject:" << qpl.instance() << "]";
                }
            }
#endif
        }
        
        QStringList engineNames;
        foreach (QContactManagerEngineFactory* f, m_engines.values()) {
            QStringList versions;
            foreach (int v, f->supportedImplementationVersions()) {
                versions << QString::fromAscii("%1").arg(v);
            }
            engineNames << QString::fromAscii("%1[%2]").arg(f->managerName()).arg(versions.join(QString::fromAscii(",")));
        }
#if !defined QT_NO_DEBUG
        if (showDebug) {
            qDebug() << "Found engines:" << engineNames;
            qDebug() << "Found action engines:" << m_actionManagers;
        }
#endif
    }
}

// Observer stuff

void QContactManagerData::registerObserver(QContactManager* manager, QContactObserver* observer)
{
    if (!manager)
        return;

    QContactManagerData* d = QContactManagerData::get(manager);

    d->m_observerForContact.insert(observer->contactLocalId(), observer);

    // If this is the first observer, connect to the engine too
    if (d->m_observerForContact.size() == 1) {
        // This takes advantage of the manager connectNotify code
        QObject::connect(manager, SIGNAL(contactsChanged(QList<QContactLocalId>)),
                manager, SLOT(_q_contactsUpdated(QList<QContactLocalId>)));
        QObject::connect(manager, SIGNAL(contactsRemoved(QList<QContactLocalId>)),
                manager, SLOT(_q_contactsDeleted(QList<QContactLocalId>)));
    }
}

void QContactManagerData::unregisterObserver(QContactManager* manager, QContactObserver* observer)
{
    Q_ASSERT(manager);

    QContactManagerData* d = QContactManagerData::get(manager);

    QContactLocalId key = d->m_observerForContact.key(observer);
    if (key != 0) {
        d->m_observerForContact.remove(key, observer);

        // If there are now no more observers, disconnect from the engine
        if (d->m_observerForContact.size() == 0) {
            // This takes advantage of the manager disconnectNotify code
            QObject::disconnect(manager, SIGNAL(contactsChanged(QList<QContactLocalId>)),
                    manager, SLOT(_q_contactsUpdated(QList<QContactLocalId>)));
            QObject::disconnect(manager, SIGNAL(contactsRemoved(QList<QContactLocalId>)),
                    manager, SLOT(_q_contactsDeleted(QList<QContactLocalId>)));
        }
    }
}

void QContactManagerData::_q_contactsUpdated(const QList<QContactLocalId>& ids)
{
    foreach (QContactLocalId id, ids) {
        QList<QContactObserver*> observers = m_observerForContact.values(id);
        foreach (QContactObserver* observer, observers) {
            QMetaObject::invokeMethod(observer, "contactChanged");
        }
    }
}

void QContactManagerData::_q_contactsDeleted(const QList<QContactLocalId>& ids)
{
    foreach (QContactLocalId id, ids) {
        QList<QContactObserver*> observers = m_observerForContact.values(id);
        foreach (QContactObserver* observer, observers) {
            QMetaObject::invokeMethod(observer, "contactRemoved");
        }
    }
}

// trampolines for private classes
QContactManagerData* QContactManagerData::get(const QContactManager* manager)
{
    return manager->d;
}

QContactManagerEngineV2* QContactManagerData::engine(const QContactManager* manager)
{
    if (manager)
        return manager->d->m_engine;
    return 0;
}

QTM_END_NAMESPACE

