/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "qmessagestore_symbian_p.h"
#include "qmtmengine_symbian_p.h"

#include <QString>
#include <QEventLoop>


QTM_BEGIN_NAMESPACE

Q_GLOBAL_STATIC(QMessageStorePrivate,messageStorePrivate);

QMessageStorePrivate::QMessageStorePrivate()
  : q_ptr(0),
    _mtmEngine(0),
    _error(QMessageStore::NoError)
{
}

QMessageStorePrivate::~QMessageStorePrivate()
{
}

void QMessageStorePrivate::initialize(QMessageStore *store)
{
    q_ptr = store;
    _mtmEngine = CMTMEngine::instance();
}

QMessageIdList QMessageStorePrivate::queryMessages(const QMessageFilter &filter, const QMessageOrdering &ordering, uint limit, uint offset) const
{
    QMessageIdList ids;
    QMessageServiceAction serviceaction;
    connect(&serviceaction, SIGNAL(messagesFound(const QMessageIdList&)), this, SLOT(messagesFound(const QMessageIdList&)));
    if (serviceaction.queryMessages(filter, ordering, limit, offset)) {
        QEventLoop loop;
        QObject::connect(&serviceaction, SIGNAL(messagesFound(const QMessageIdList&)), &loop, SLOT(quit()));
        loop.exec();
        ids = _ids;
        _ids.clear();
    }
    return ids;
}

QMessageIdList QMessageStorePrivate::queryMessages(const QMessageFilter &filter, const QString &body, QMessageDataComparator::Options options, const QMessageOrdering &ordering, uint limit, uint offset) const
{
    QMessageIdList ids;
    QMessageServiceAction serviceaction;
    connect(&serviceaction, SIGNAL(messagesFound(const QMessageIdList&)), this, SLOT(messagesFound(const QMessageIdList&)));
    if (serviceaction.queryMessages(filter, body, options, ordering, limit, offset)) {
        QObject::connect(&serviceaction, SIGNAL(messagesFound(const QMessageIdList&)), &loop, SLOT(quit()));
        loop.exec();
        ids = _ids;
        _ids.clear();
    }
    return ids;
}

int QMessageStorePrivate::countMessages(const QMessageFilter& filter) const
{
    int count = 0;
    QMessageServiceAction serviceaction;
    connect(&serviceaction, SIGNAL(messagesCounted(int)), this, SLOT(messagesCounted(int)));
    if (serviceaction.countMessages(filter)) {
        QObject::connect(&serviceaction, SIGNAL(messagesCounted(int)), &loop, SLOT(quit()));
        loop.exec();
        count = _count;
        _count = 0;
    }
    return count;
}

void QMessageStorePrivate::stateChanged(QMessageServiceAction::State a)
{
    if (a == QMessageServiceAction::Failed) {
        loop.quit();
    }
}

void QMessageStorePrivate::messagesFound(const QMessageIdList &ids)
{
    _ids = ids;
    loop.quit();
}

void QMessageStorePrivate::messagesCounted(int count)
{
    _count = count;
    loop.quit();
}

QMessageAccountIdList QMessageStorePrivate::queryAccounts(const QMessageAccountFilter &filter, const QMessageAccountOrdering &ordering, uint limit, uint offset) const
{
    return _mtmEngine->queryAccounts(filter, ordering, limit, offset);
}

int QMessageStorePrivate::countAccounts(const QMessageAccountFilter &filter) const
{
    return _mtmEngine->countAccounts(filter);
}

QMessageFolderIdList QMessageStorePrivate::queryFolders(const QMessageFolderFilter &filter, const QMessageFolderOrdering &ordering, uint limit, uint offset) const
{
    return _mtmEngine->queryFolders(filter, ordering, limit, offset);
}

int QMessageStorePrivate::countFolders(const QMessageFolderFilter& filter) const
{
    return _mtmEngine->countFolders(filter);
}

QMessageFolder QMessageStorePrivate::folder(const QMessageFolderId& id) const
{
    return _mtmEngine->folder(id);
}


bool QMessageStorePrivate::addMessage(QMessage *m)
{
    return _mtmEngine->addMessage(m);
}

bool QMessageStorePrivate::updateMessage(QMessage *m)
{
    return _mtmEngine->updateMessage(m);
}

bool QMessageStorePrivate::removeMessage(const QMessageId &id, QMessageStore::RemovalOption option)
{
    return _mtmEngine->removeMessage(id, option);
}

bool QMessageStorePrivate::removeMessages(const QMessageFilter &filter, QMessageStore::RemovalOption option)
{
    bool retVal = true;
    
    QMessageIdList ids;
    QMessageServiceAction serviceaction;
    connect(&serviceaction, SIGNAL(messagesFound(const QMessageIdList&)), this, SLOT(messagesFound(const QMessageIdList&)));
    if (serviceaction.queryMessages(filter)) {
        QEventLoop loop;
        QObject::connect(&serviceaction, SIGNAL(messagesFound(const QMessageIdList&)), &loop, SLOT(quit()));
        loop.exec();
        ids = _ids;
        _ids.clear();
        for (int i=0; i < ids.count(); i++) {
            if (!_mtmEngine->removeMessage(ids[i], option)) {
                retVal = false;
            }
        }
    } else {
        retVal = false;
    }
    
    return retVal;
}

QMessage QMessageStorePrivate::message(const QMessageId& id) const
{
    return _mtmEngine->message(id);
}

QMessageAccount QMessageStorePrivate::account(const QMessageAccountId &id) const
{
    return _mtmEngine->account(id);
}

QMessageStore::NotificationFilterId QMessageStorePrivate::registerNotificationFilter(const QMessageFilter &filter)
{
    return _mtmEngine->registerNotificationFilter(*this, filter);
}

void QMessageStorePrivate::unregisterNotificationFilter(QMessageStore::NotificationFilterId notificationFilterId)
{
    _mtmEngine->unregisterNotificationFilter(notificationFilterId);    
}

void QMessageStorePrivate::messageNotification(QMessageStorePrivate::NotificationType type, const QMessageId& id,
                                               const QMessageStore::NotificationFilterIdSet &matchingFilters)
{
    switch (type) {
        case Added:
            emit q_ptr->messageAdded(id, matchingFilters);
            break;
        case Updated:
            emit q_ptr->messageUpdated(id, matchingFilters);
            break;
        case Removed:
            emit q_ptr->messageRemoved(id, matchingFilters);
            break;
    }
}

QMessageStore::QMessageStore(QObject *parent)
    : QObject(parent),
      d_ptr(messageStorePrivate())
{
    Q_ASSERT(d_ptr != 0);
    Q_ASSERT(d_ptr->q_ptr == 0); // QMessageStore should be singleton
    d_ptr->initialize(this);
}

QMessageStore::~QMessageStore()
{
    d_ptr = 0; // should be cleaned up by automatically
}

QMessageStore* QMessageStore::instance()
{
    QMessageStorePrivate *d = messageStorePrivate();
    Q_ASSERT(d != 0);
    if (!d->q_ptr)
        new QMessageStore();
    return d->q_ptr;
}

QMessageStore::ErrorCode QMessageStore::lastError() const
{
    return NoError;
}

QMessageIdList QMessageStore::queryMessages(const QMessageFilter &filter, const QMessageOrdering &ordering, uint limit, uint offset) const
{
    return messageStorePrivate()->queryMessages(filter, ordering, limit, offset);
}

QMessageIdList QMessageStore::queryMessages(const QMessageFilter &filter, const QString &body, QMessageDataComparator::Options options, const QMessageOrdering &ordering, uint limit, uint offset) const
{
    return messageStorePrivate()->queryMessages(filter, body, options, ordering, limit, offset);
}

QMessageFolderIdList QMessageStore::queryFolders(const QMessageFolderFilter &filter, const QMessageFolderOrdering &ordering, uint limit, uint offset) const
{
    return messageStorePrivate()->queryFolders(filter, ordering, limit, offset);
}

QMessageAccountIdList QMessageStore::queryAccounts(const QMessageAccountFilter &filter, const QMessageAccountOrdering &ordering, uint limit, uint offset) const
{
    return messageStorePrivate()->queryAccounts(filter, ordering, limit, offset);
}

int QMessageStore::countMessages(const QMessageFilter& filter) const
{
    return messageStorePrivate()->countMessages(filter);
}

int QMessageStore::countFolders(const QMessageFolderFilter& filter) const
{
    return messageStorePrivate()->countFolders(filter);
}

int QMessageStore::countAccounts(const QMessageAccountFilter& filter) const
{
    return messageStorePrivate()->countAccounts(filter);
}

bool QMessageStore::removeMessage(const QMessageId& id, RemovalOption option)
{
    return messageStorePrivate()->removeMessage(id, option);
}

bool QMessageStore::removeMessages(const QMessageFilter& filter, QMessageStore::RemovalOption option)
{
    return messageStorePrivate()->removeMessages(filter, option);
}

bool QMessageStore::addMessage(QMessage *m)
{
    return messageStorePrivate()->addMessage(m);   	
}

bool QMessageStore::updateMessage(QMessage *m)
{
	return messageStorePrivate()->updateMessage(m);
}

QMessage QMessageStore::message(const QMessageId& id) const
{
    return messageStorePrivate()->message(id);
}

QMessageFolder QMessageStore::folder(const QMessageFolderId& id) const
{
    return messageStorePrivate()->folder(id);
}

QMessageAccount QMessageStore::account(const QMessageAccountId& id) const
{
    return messageStorePrivate()->account(id);
}

QMessageStore::NotificationFilterId QMessageStore::registerNotificationFilter(const QMessageFilter &filter)
{
    return messageStorePrivate()->registerNotificationFilter(filter);
}

void QMessageStore::unregisterNotificationFilter(QMessageStore::NotificationFilterId notificationFilterId)
{
    messageStorePrivate()->unregisterNotificationFilter(notificationFilterId);
}

#include "moc_qmessagestore_symbian_p.cpp"
QTM_END_NAMESPACE