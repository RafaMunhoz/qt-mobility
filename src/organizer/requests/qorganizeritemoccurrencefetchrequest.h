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

#ifndef QORGANIZERITEMOCCURRENCEFETCHREQUEST_H
#define QORGANIZERITEMOCCURRENCEFETCHREQUEST_H

#include "qtorganizerglobal.h"
#include "qorganizerabstractrequest.h"
#include "qorganizeritemsortorder.h"
#include "qorganizeritemfilter.h"
#include "qorganizeritem.h"
#include "qorganizeritemfetchhint.h"

#include <QList>
#include <QStringList>

QTM_BEGIN_NAMESPACE

class QOrganizerItemOccurrenceFetchRequestPrivate;
class Q_ORGANIZER_EXPORT QOrganizerItemOccurrenceFetchRequest : public QOrganizerAbstractRequest
{
    Q_OBJECT

public:
    QOrganizerItemOccurrenceFetchRequest(QObject* parent = 0);
    ~QOrganizerItemOccurrenceFetchRequest();

    void setParentItem(const QOrganizerItem& item);
    void setStartDate(const QDateTime& date);
    void setEndDate(const QDateTime& date);
    void setMaxOccurrences(int maxCount);
    void setFetchHint(const QOrganizerItemFetchHint& hint);

    QOrganizerItem parentItem() const;
    QDateTime startDate() const;
    QDateTime endDate() const;
    int maxOccurrences() const;
    QOrganizerItemFetchHint fetchHint() const;

    /* Results */
    QList<QOrganizerItem> itemOccurrences() const;

private:
    Q_DISABLE_COPY(QOrganizerItemOccurrenceFetchRequest)
    friend class QOrganizerManagerEngine;
    Q_DECLARE_PRIVATE_D(d_ptr, QOrganizerItemOccurrenceFetchRequest)
};

QTM_END_NAMESPACE

#endif
