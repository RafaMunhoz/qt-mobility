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

#include "qgeotiledmappingmanagerengine.h"
#include "qgeotiledmappingmanagerengine_p.h"

QTM_BEGIN_NAMESPACE

/*!
    \class QGeoTiledMappingManagerEngine

    \brief The QGeoTiledMappingManagerEngine class is provided to make writing
    Qt Maps and Navigation API plugins for tiled based mapping services easier.

    \inmodule QtLocation

    \ingroup maps-impl-tiled

    \since 1.1

    Subclasses of QGeoTiledMappingManagerEngine need to provide an implementation
    of getTileImage().

    It is important that the function setTileSize() is called before
    getTileImage() to ensure that the tile is dealt with correctly after it is
    fetched.  If setTileSize() is not calle dfirst the behaviour is undefined.

    A subclass of QGeoTiledMappingManagerEngine will often make use of a subclass
    fo QGeoTiledMapReply internally, in order to add any engine-specific
    data (such as a QNetworkReply object for network-based services) to the
    QGeoTiledMapReply instances used by the engine.

    QGeoTiledMappingManagerEngine will report that custom map objects are supported
    by default.
*/

/*!
    Constructs a new tiled mapping manager using the parameters \a parameters
    and the parent \a parent.
*/
QGeoTiledMappingManagerEngine::QGeoTiledMappingManagerEngine(const QMap<QString, QVariant> &parameters, QObject *parent)
    : QGeoMappingManagerEngine(new QGeoTiledMappingManagerEnginePrivate(), parent)
{
    Q_UNUSED(parameters)
    setSupportsCustomMapObjects(true);
}

/*!
    Destroys this tiled mapping manager.
*/
QGeoTiledMappingManagerEngine::~QGeoTiledMappingManagerEngine()
{
    // deleted in superclass destructor
}

/*
    Requests the map tiled specified by \a request.

    A QGeoTiledMapReply object will be returned, which can be used to manage the
    fetching of the tile and to return the tile data.

    This manager and the returned QGeoTiledMapReply object will emit signals
    indicating if the operation completes or if errors occur.

    Once the operation has completed, QGeoTiledMapReply::mapImageData() and
    QGeoTiledMapReply::mapImageFormat() can be used to generate the tile image.

    The user is responsible for deleting the returned reply object, although
    this can be done in the slot connected to QGeoTiledMappingManagerEngine::finished(),
    QGeoTiledMappingManagerEngine::error(), QGeoTiledMapReply::finished() or
    QGeoTiledMapReply::error() with deleteLater().
*/

/*!
\fn virtual QGeoTiledMapReply* QGeoTiledMappingManagerEngine::getTileImage(const QGeoTiledMapRequest &request)
    Requests the map tiled specified by \a request.

    A QGeoTiledMapReply object will be returned, which can be used to manage the
    fetching of the tile and to return the tile data.

    The returned QGeoTiledMapReply object will emit signals
    indicating if the operation completes or if errors occur.

    Once the operation has completed, QGeoTiledMapReply::mapImageData() and
    QGeoTiledMapReply::mapImageFormat() can be used to generate the tile image.

    The user is responsible for deleting the returned reply object, although
    this can be done in the slot connected to QGeoTiledMapReply::finished() or
    QGeoTiledMapReply::error() with deleteLater().

    \since 1.1
*/

/*!
    Returns a new QGeoMapData instance which will be managed by
    this manager.

    A QGeoMapData instance contains and manages the information about
    what a map widget is looking at.  A  single manager can be used by several
    QGraphcisGeoMap instances since each instance has an associated QGeoMapData instance.

    The QGeoMapData instance can be treated as a kind of session object, or
    as a model in a model-view-controller architecture, with QGraphicsGeoMap
    as the view and QGeoMappingManagerEngine as the controller.

    The instance returned by the default implementation will be a
    QGeoTiledMapData instance. Subclasses of QGeoTiledMappingManagerEngine are
    free to override this function to return subclasses of QGeoTiledMapData in
    order to customize the map.
    \since 1.1
*/
QGeoMapData* QGeoTiledMappingManagerEngine::createMapData()
{
    return new QGeoTiledMapData(this);
}

/*!
    Returns the size of the tiles returned by this tiled mapping manager.
    \since 1.1
*/
QSize QGeoTiledMappingManagerEngine::tileSize() const
{
    Q_D(const QGeoTiledMappingManagerEngine);
    return d->tileSize;
}

/*!
    Sets the size of the tiles returned by this tiled mapping manager to \a
    tileSize.

    Subclasses of QGeoTiledMappingManagerEngine should use this function to
    ensure tileSize() provides accurate information.
    \since 1.1
*/
void QGeoTiledMappingManagerEngine::setTileSize(const QSize &tileSize)
{
    Q_D(QGeoTiledMappingManagerEngine);
    d->tileSize = tileSize;
}

/*******************************************************************************
*******************************************************************************/

QGeoTiledMappingManagerEnginePrivate::QGeoTiledMappingManagerEnginePrivate()
    : QGeoMappingManagerEnginePrivate() {}

QGeoTiledMappingManagerEnginePrivate::~QGeoTiledMappingManagerEnginePrivate() {}

/*******************************************************************************
*******************************************************************************/

#include "moc_qgeotiledmappingmanagerengine.cpp"

QTM_END_NAMESPACE
