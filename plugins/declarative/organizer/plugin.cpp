/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
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


#include <QApplication>
#include <QtDeclarative>
#include <QDeclarativeExtensionPlugin>

#include "qdeclarativeorganizermodel_p.h"
#include "qdeclarativeorganizeritem_p.h"
#include "qdeclarativeorganizeritemdetail_p.h"
#include "qdeclarativeorganizeritemfilter_p.h"
#include "qdeclarativeorganizerrecurrencerule_p.h"
#include "qdeclarativeorganizeritemfetchhint_p.h"
#include "qdeclarativeorganizeritemsortorder_p.h"
#include "qdeclarativeorganizercollection_p.h"

QT_USE_NAMESPACE


class QOrganizerQmlPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT
public:
    void registerTypes(const char *uri)
    {
        Q_ASSERT(uri == QLatin1String("QtMobility.organizer"));
        qmlRegisterType<QDeclarativeOrganizerModel>(uri, 1, 1, "OrganizerModel");
        qmlRegisterType<QDeclarativeOrganizerRecurrenceRule>(uri, 1, 1, "RecurrenceRule");
        qmlRegisterType<QDeclarativeOrganizerItemFetchHint>(uri, 1, 1, "FetchHint");
        qmlRegisterType<QDeclarativeOrganizerItemSortOrder>(uri, 1, 1, "SortOrder");
        qmlRegisterType<QDeclarativeOrganizerCollection>(uri, 1, 1, "Collection");

        //items
        qmlRegisterType<QDeclarativeOrganizerItem>(uri, 1, 1, "OrganizerItem");
        qmlRegisterType<QDeclarativeOrganizerEvent>(uri, 1, 1, "Event");
        qmlRegisterType<QDeclarativeOrganizerEventOccurrence>(uri, 1, 1, "EventOccurrence");
        qmlRegisterType<QDeclarativeOrganizerJournal>(uri, 1, 1, "Journal");
        qmlRegisterType<QDeclarativeOrganizerNote>(uri, 1, 1, "Note");
        qmlRegisterType<QDeclarativeOrganizerTodo>(uri, 1, 1, "Todo");
        qmlRegisterType<QDeclarativeOrganizerTodoOccurrence>(uri, 1, 1, "TodoOccurrence");

        //details
        qmlRegisterType<QDeclarativeOrganizerItemDetail>(uri, 1, 1, "Detail");
        qmlRegisterType<QDeclarativeOrganizerEventTime>(uri, 1, 1, "EventTime");
        qmlRegisterType<QDeclarativeOrganizerItemDescription>(uri, 1, 1, "Description");
        qmlRegisterType<QDeclarativeOrganizerItemDisplayLabel>(uri, 1, 1, "DisplayLabel");
        qmlRegisterType<QDeclarativeOrganizerItemGuid>(uri, 1, 1, "Guid");
        qmlRegisterType<QDeclarativeOrganizerItemParent>(uri, 1, 1, "ItemParent");
        qmlRegisterType<QDeclarativeOrganizerItemLocation>(uri, 1, 1, "Location");
        qmlRegisterType<QDeclarativeOrganizerItemPriority>(uri, 1, 1, "Priority");
        qmlRegisterType<QDeclarativeOrganizerItemRecurrence>(uri, 1, 1, "Recurrence");
        qmlRegisterType<QDeclarativeOrganizerItemReminder>(uri, 1, 1, "Reminder");
        qmlRegisterType<QDeclarativeOrganizerItemAudibleReminder>(uri, 1, 1, "AudibleReminder");
        qmlRegisterType<QDeclarativeOrganizerItemVisualReminder>(uri, 1, 1, "VisualReminder");
        qmlRegisterType<QDeclarativeOrganizerItemEmailReminder>(uri, 1, 1, "EmailReminder");
        qmlRegisterType<QDeclarativeOrganizerItemTimestamp>(uri, 1, 1, "Timestamp");
        qmlRegisterType<QDeclarativeOrganizerItemType>(uri, 1, 1, "ItemType");
        qmlRegisterType<QDeclarativeOrganizerJournalTime>(uri, 1, 1, "JournalTime");
        qmlRegisterType<QDeclarativeOrganizerTodoProgress>(uri, 1, 1, "TodoProgress");
        qmlRegisterType<QDeclarativeOrganizerTodoTime>(uri, 1, 1, "TodoTime");

        //filters
        qmlRegisterType<QDeclarativeOrganizerItemFilter>(uri, 1, 1, "Filter");
        qmlRegisterType<QDeclarativeOrganizerItemChangeLogFilter>(uri, 1, 1, "ChangeLogFilter");
        qmlRegisterType<QDeclarativeOrganizerItemCollectionFilter>(uri, 1, 1, "CollectionFilter");
        qmlRegisterType<QDeclarativeOrganizerItemDetailFilter>(uri, 1, 1, "DetailFilter");
        qmlRegisterType<QDeclarativeOrganizerItemDetailRangeFilter>(uri, 1, 1, "DetailRangeFilter");
        qmlRegisterType<QDeclarativeOrganizerItemIdFilter>(uri, 1, 1, "IdFilter");
        qmlRegisterType<QDeclarativeOrganizerItemIntersectionFilter>(uri, 1, 1, "IntersectionFilter");
        qmlRegisterType<QDeclarativeOrganizerItemUnionFilter>(uri, 1, 1, "UnionFilter");
        qmlRegisterType<QDeclarativeOrganizerItemInvalidFilter>(uri, 1, 1, "InvalidFilter");
        qmlRegisterType<QDeclarativeOrganizerItemCompoundFilter>();
    }
};

#include "plugin.moc"

Q_EXPORT_PLUGIN2(qorganizerqmlplugin, QOrganizerQmlPlugin);
