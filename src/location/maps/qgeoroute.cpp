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

#include "qgeoroute.h"
#include "qgeoroute_p.h"

#include "qgeoboundingbox.h"
#include "qgeoroutesegment.h"

#include <QDateTime>

QTM_BEGIN_NAMESPACE

/*!
    \class QGeoRoute
    \brief The QGeoRoute class represents a route between two points.

    \inmodule QtLocation
    \since 1.1

    \ingroup maps-routing

    A QGeoRoute object contains high level information about a route, such
    as the length the route, the estimated travel time for the route,
    and enough information to render a basic image of the route on a map.

    The QGeoRoute object also contains a list of QGeoRouteSegment objecs which
    describe subsections of the route in greater detail.

    Routing information is normally requested using
    QGeoRoutingManager::calculateRoute(), which returns a QGeoRouteReply
    instance. If the operation is completed successfully the routing
    information can be accessed with QGeoRouteReply::routes()

    \sa QGeoRoutingManager
*/

/*!
    Constructs a route object.
*/
QGeoRoute::QGeoRoute()
    : d_ptr(new QGeoRoutePrivate()) {}

/*!
    Constructs a route object from the contents of \a other.
*/
QGeoRoute::QGeoRoute(const QGeoRoute &other)
    : d_ptr(other.d_ptr) {}

/*!
    Destroys this route object.
*/
QGeoRoute::~QGeoRoute()
{
}

/*!
    Assigns the contents of \a other to this route and returns a reference to
    this route.
    \since 1.1
*/
QGeoRoute& QGeoRoute::operator= (const QGeoRoute & other)
{
    d_ptr = other.d_ptr;
    return *this;
}

/*!
    Returns whether this route and \a other are equal.
    \since 1.1
*/
bool QGeoRoute::operator ==(const QGeoRoute &other) const
{
    return (d_ptr.constData() == other.d_ptr.constData());
}

/*!
    Returns whether this route and \a other are not equal.
    \since 1.1
*/
bool QGeoRoute::operator !=(const QGeoRoute &other) const
{
    return (d_ptr.constData() != other.d_ptr.constData());
}

/*!
    Sets the ID of this route to \a id.

    Service providers which support the updating of routes commonly assign
    IDs to routes.  If this route came from such a service provider changing
    the ID will probably cause route updates to stop working.
    \since 1.1
*/
void QGeoRoute::setRouteId(const QString &id)
{
    d_ptr->id = id;
}

/*!
    Returns the ID of this route.

    Service providers which support the updating of routes commonly assign
    IDs to routes.  If this route did not come from such a service provider
    the function will return an empty string.
    \since 1.1
*/
QString QGeoRoute::routeId() const
{
    return d_ptr->id;
}

/*!
    Sets the route request which describes the criteria used in the
    calculcation of this route to \a request.
*/
void QGeoRoute::setRequest(const QGeoRouteRequest &request)
{
    d_ptr->request = request;
}

/*!
    Returns the route request which describes the criteria used in
    the calculation of this route.
    \since 1.1
*/
QGeoRouteRequest QGeoRoute::request() const
{
    return d_ptr->request;
}

/*!
    Sets the bounding box which encompasses the entire route to \a bounds.
    \since 1.1
*/
void QGeoRoute::setBounds(const QGeoBoundingBox &bounds)
{
    d_ptr->bounds = bounds;
}

/*!
    Returns a bounding box which encompasses the entire route.
    \since 1.1
*/
QGeoBoundingBox QGeoRoute::bounds() const
{
    return d_ptr->bounds;
}

/*!
    Sets the first route segment in the route to \a routeSegment.
    \since 1.1
*/
void QGeoRoute::setFirstRouteSegment(const QGeoRouteSegment &routeSegment)
{
    d_ptr->firstSegment = routeSegment;
}

/*!
    Returns the first route segment in the route.

    Will return an invalid route segment if there are no route segments
    associated with the route.

    The remaining route segments can be accessed sequentially with
    QGeoRouteSegment::nextRouteSegment.
    \since 1.1
*/
QGeoRouteSegment QGeoRoute::firstRouteSegment() const
{
    return d_ptr->firstSegment;
}

/*!
    Sets the estimated amount of time it will take to traverse this route,
    in seconds, to \a secs.
    \since 1.1
*/
void QGeoRoute::setTravelTime(int secs)
{
    d_ptr->travelTime = secs;
}

/*!
    Returns the estimated amount of time it will take to traverse this route,
    in seconds.
    \since 1.1
*/
int QGeoRoute::travelTime() const
{
    return d_ptr->travelTime;
}

/*!
    Sets the distance covered by this route, in metres, to \a distance.
    \since 1.1
*/
void QGeoRoute::setDistance(qreal distance)
{
    d_ptr->distance = distance;
}

/*!
    Returns the distance covered by this route, in metres.
    \since 1.1
*/
qreal QGeoRoute::distance() const
{
    return d_ptr->distance;
}

/*!
    Sets the travel mode for this route to \a mode.

    This should be one of the travel modes returned by request().travelModes().
    \since 1.1
*/
void QGeoRoute::setTravelMode(QGeoRouteRequest::TravelMode mode)
{
    d_ptr->travelMode = mode;
}

/*!
    Returns the travel mode for the this route.

    This should be one of the travel modes returned by request().travelModes().
    \since 1.1
*/
QGeoRouteRequest::TravelMode QGeoRoute::travelMode() const
{
    return d_ptr->travelMode;
}

/*!
    Sets the geometric shape of the route to \a path.

    The coordinates in \a path should be listed in the order in which they
    would be traversed by someone traveling along this segment of the route.
    \since 1.1
*/
void QGeoRoute::setPath(const QList<QGeoCoordinate> &path)
{
    d_ptr->path = path;
}

/*!
    Returns the geometric shape of the route.

    The coordinates should be listed in the order in which they
    would be traversed by someone traveling along this segment of the route.
    \since 1.1
*/
QList<QGeoCoordinate> QGeoRoute::path() const
{
    return d_ptr->path;
}

/*******************************************************************************
*******************************************************************************/

QGeoRoutePrivate::QGeoRoutePrivate()
    : travelTime(0),
      distance(0.0),
      travelMode(QGeoRouteRequest::CarTravel) {}

QGeoRoutePrivate::QGeoRoutePrivate(const QGeoRoutePrivate &other)
    : QSharedData(other),
      id(other.id),
      request(other.request),
      bounds(other.bounds),
      travelTime(other.travelTime),
      distance(other.distance),
      travelMode(other.travelMode),
      path(other.path),
      firstSegment(other.firstSegment) {}

QGeoRoutePrivate::~QGeoRoutePrivate() {}

bool QGeoRoutePrivate::operator ==(const QGeoRoutePrivate &other) const
{
    QGeoRouteSegment s1 = firstSegment;
    QGeoRouteSegment s2 = other.firstSegment;

    while (true) {
        if (s1.isValid() != s2.isValid())
            return false;
        if (!s1.isValid())
            break;
        if (s1 != s2)
            return false;
        s1 = s1.nextRouteSegment();
        s2 = s2.nextRouteSegment();
    }

    return ((id == other.id)
            && (request == other.request)
            && (bounds == other.bounds)
            && (travelTime == other.travelTime)
            && (distance == other.distance)
            && (travelMode == other.travelMode)
            && (path == other.path));
}

QTM_END_NAMESPACE
