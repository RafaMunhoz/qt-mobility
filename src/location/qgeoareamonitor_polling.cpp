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

#include "qgeoareamonitor_polling_p.h"
#include "qgeocoordinate.h"

QTM_BEGIN_NAMESPACE

#define UPDATE_INTERVAL_5S  5000

QGeoAreaMonitorPolling::QGeoAreaMonitorPolling(QObject *parent) : QGeoAreaMonitor(parent)
{
    insideArea = false;
    location = QGeoPositionInfoSource::createDefaultSource(this);
    if (location) {
        location->setUpdateInterval(UPDATE_INTERVAL_5S);
        connect(location, SIGNAL(positionUpdated(QGeoPositionInfo)),
                this, SLOT(positionUpdated(QGeoPositionInfo)));
    }
}

QGeoAreaMonitorPolling::~QGeoAreaMonitorPolling()
{
    if (location)
        location->stopUpdates();
}

void QGeoAreaMonitorPolling::setCenter(const QGeoCoordinate& coordinate)
{
    if (coordinate.isValid()) {
        QGeoAreaMonitor::setCenter(coordinate);
        checkStartStop();
    }
}

void QGeoAreaMonitorPolling::setRadius(qreal radius)
{
    QGeoAreaMonitor::setRadius(radius);
    checkStartStop();
}

void QGeoAreaMonitorPolling::connectNotify(const char *signal)
{
    if (signal == SIGNAL(areaEntered(QGeoPositionInfo)) ||
            signal == SIGNAL(areaExited(QGeoPositionInfo)))
        checkStartStop();
}

void QGeoAreaMonitorPolling::disconnectNotify(const char *signal)
{
    if (signal == SIGNAL(areaEntered(QGeoPositionInfo)) ||
            signal == SIGNAL(areaExited(QGeoPositionInfo)))
        checkStartStop();
}

void QGeoAreaMonitorPolling::checkStartStop()
{
    if (!location) return;

    if ((QObject::receivers(SIGNAL(areaEntered(QGeoPositionInfo))) > 0 ||
            QObject::receivers(SIGNAL(areaExited(QGeoPositionInfo))) > 0) &&
            QGeoAreaMonitor::center().isValid() &&
            QGeoAreaMonitor::radius() > qreal(0.0)) {
        location->startUpdates();
    } else {
        location->stopUpdates();
    }
}

void QGeoAreaMonitorPolling::positionUpdated(const QGeoPositionInfo &info)
{
    double distance = info.coordinate().distanceTo(QGeoAreaMonitor::center());

    if (distance <= QGeoAreaMonitor::radius()) {
        if (!insideArea)
            emit areaEntered(info);
        insideArea = true;
    } else if (insideArea) {
        emit areaExited(info);
        insideArea = false;
    }
}

#include "moc_qgeoareamonitor_polling_p.cpp"
QTM_END_NAMESPACE
