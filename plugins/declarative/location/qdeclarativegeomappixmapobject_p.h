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

#ifndef QDECLARATIVEGEOMAPPIXMAPOBJECT_H
#define QDECLARATIVEGEOMAPPIXMAPOBJECT_H

#include "qdeclarativegeomapobject_p.h"
#include "qdeclarativecoordinate_p.h"
#include "qgeomappixmapobject.h"

#include <QColor>
#include <QUrl>
#include <QNetworkReply>

QTM_BEGIN_NAMESPACE

class QDeclarativeGeoMapPixmapObject : public QDeclarativeGeoMapObject
{
    Q_OBJECT
    Q_ENUMS(Status)

    Q_PROPERTY(QDeclarativeCoordinate* coordinate READ coordinate WRITE setCoordinate NOTIFY coordinateChanged)
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QPoint offset READ offset WRITE setOffset NOTIFY offsetChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)

public:
    enum Status {
        Null,
        Ready,
        Loading,
        Error
    };

    QDeclarativeGeoMapPixmapObject(QDeclarativeItem *parent = 0);
    ~QDeclarativeGeoMapPixmapObject();

    QDeclarativeCoordinate* coordinate();
    void setCoordinate(QDeclarativeCoordinate *coordinate);

    QUrl source() const;
    void setSource(const QUrl &source);

    QPoint offset() const;
    void setOffset(const QPoint &offset);

    Status status() const;

Q_SIGNALS:
    void coordinateChanged(const QDeclarativeCoordinate *coordinate);
    void sourceChanged(const QUrl &source);
    void offsetChanged(const QPoint &offset);
    void statusChanged(QDeclarativeGeoMapPixmapObject::Status status);

private Q_SLOTS:
    void coordinateLatitudeChanged(double latitude);
    void coordinateLongitudeChanged(double longitude);
    void coordinateAltitudeChanged(double altitude);
    void finished();
    void error(QNetworkReply::NetworkError error);

private:
    void setStatus(const QDeclarativeGeoMapPixmapObject::Status status);
    void load();

    QGeoMapPixmapObject* pixmap_;
    QDeclarativeCoordinate *coordinate_;
    QUrl source_;
    QNetworkReply *reply_;
    Status status_;

    Q_DISABLE_COPY(QDeclarativeGeoMapPixmapObject)
};

QTM_END_NAMESPACE

QML_DECLARE_TYPE(QTM_PREPEND_NAMESPACE(QDeclarativeGeoMapPixmapObject));

#endif
