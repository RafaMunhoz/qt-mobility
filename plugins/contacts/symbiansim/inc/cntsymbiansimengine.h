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

#ifndef CNTSYMBIANSIMENGINE_H
#define CNTSYMBIANSIMENGINE_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//
#include "qtcontactsglobal.h"
#include "qcontactmanagerengine.h"
#include "qcontactmanagerenginefactory.h"
#include <flogger.h>
#include <f32file.h>

//#define SYMBIANSIM_BACKEND_DEBUG

namespace {
#ifdef SYMBIANSIM_BACKEND_DEBUG
    void PbkPrintToLog( TRefByValue<const TDesC> aFormat, ... )
    {
        _LIT( KLogDir, "Sim" );
        _LIT( KLogName, "sim.log" );

        VA_LIST args;
        VA_START( args, aFormat );
        RFileLogger::WriteFormat(KLogDir, KLogName, EFileLoggingModeAppend, aFormat, args);
        VA_END( args );
    }
#else
    void PbkPrintToLog( TRefByValue<const TDesC> aFormat, ... ) { Q_UNUSED(aFormat); }
#endif    
}  // namespace

QTM_USE_NAMESPACE

#define CNT_SYMBIANSIM_MANAGER_NAME "symbiansim"

Q_DEFINE_LATIN1_CONSTANT(KSimSyncTarget, "SIM");
Q_DEFINE_LATIN1_CONSTANT(KParameterKeySimStoreName, "store");
Q_DEFINE_LATIN1_CONSTANT(KParameterValueSimStoreNameAdn, "ADN");
Q_DEFINE_LATIN1_CONSTANT(KParameterValueSimStoreNameSdn, "SDN");
Q_DEFINE_LATIN1_CONSTANT(KParameterValueSimStoreNameFdn, "FDN");
Q_DEFINE_LATIN1_CONSTANT(KParameterValueSimStoreNameOn, "ON");

class CntSimStore;
class CntAbstractSimRequest;

class CntSymbianSimPhoneNumberMatching
{
public:
    static TBool formatAndCompareL(const QString& numberToMatch, const QString& matchingNumber);
    static TBool compareNumbersL(const TDesC& numberA, const TDesC& numberB);
    static void formatNumber(TDes& number);
    static TInt getLenExeludeLeadingPlusAndZeros(const TDesC& numberA);
};

class CntSymbianSimEngineData : public QSharedData
{
public:
    CntSymbianSimEngineData();
    ~CntSymbianSimEngineData();
    
    CntSimStore *m_simStore;
    QMap<QContactAbstractRequest *, CntAbstractSimRequest *> m_asyncRequests;
};

class CntSymbianSimEngine : public QContactManagerEngine
{
    Q_OBJECT

public:
    CntSymbianSimEngine(const QMap<QString, QString>& parameters, QContactManager::Error* error);
    CntSymbianSimEngine(const CntSymbianSimEngine &other);
    ~CntSymbianSimEngine();

    QString managerName() const;

    int managerVersion() const { return 1;}

    /* Defaulted functions - XXX check*/
    QList<QVariant::Type> supportedDataTypes() const
    {
        return QContactManagerEngine::supportedDataTypes();
    }

    /* Contacts - Accessors and Mutators */
    QList<QContactLocalId> contactIds(const QContactFilter& filter, const QList<QContactSortOrder>& sortOrders, QContactManager::Error* error) const;
    QList<QContact> contacts(const QContactFilter& filter, const QList<QContactSortOrder>& sortOrders, const QContactFetchHint& fetchHint, QContactManager::Error* error) const;
    QContact contact(const QContactLocalId& contactId, const QContactFetchHint& fetchHint, QContactManager::Error* error) const;
   
    bool saveContacts(QList<QContact>* contacts, QMap<int, QContactManager::Error>* errorMap, QContactManager::Error* error);
    bool removeContacts(const QList<QContactLocalId>& contactIds, QMap<int, QContactManager::Error>* errorMap, QContactManager::Error* error);

    /* Synthesize the display label of a contact */
    QString synthesizedDisplayLabel(const QContact& contact, QContactManager::Error* error) const;
    
    /* Definitions - Accessors and Mutators */
    QMap<QString, QContactDetailDefinition> detailDefinitions(const QString& contactType, QContactManager::Error* error) const;

    /* Asynchronous Request Support */
    void requestDestroyed(QContactAbstractRequest* req);
    bool startRequest(QContactAbstractRequest* req);
    bool cancelRequest(QContactAbstractRequest* req);
    bool waitForRequestFinished(QContactAbstractRequest* req, int msecs);    
    
    /* Capabilities reporting */
    bool hasFeature(QContactManager::ManagerFeature feature, const QString& contactType = QContactType::TypeContact) const;
    bool isFilterSupported(const QContactFilter& filter) const;
    QStringList supportedContactTypes() const;

public:
    void updateDisplayLabel(QContact& contact) const;
    CntSimStore* simStore() { return d->m_simStore; }
    void setReadOnlyAccessConstraint(QContactDetail* detail) const;
    bool filter(const QContactFilter &filter, const QContact &contact);

private:
    bool executeRequest(QContactAbstractRequest *req, QContactManager::Error* qtError) const;

private:
    QExplicitlySharedDataPointer<CntSymbianSimEngineData> d;
};

class CntSymbianSimFactory : public QObject, public QContactManagerEngineFactory
{
    Q_OBJECT
    Q_INTERFACES(QtMobility::QContactManagerEngineFactory)
    public:
        QContactManagerEngine* engine(const QMap<QString, QString>& parameters, QContactManager::Error* error);
        QString managerName() const;
};

#endif  //CNTSYMBIANSIMENGINE_H
