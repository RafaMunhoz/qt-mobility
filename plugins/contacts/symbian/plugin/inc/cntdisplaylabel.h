/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtCore module of the Qt Toolkit.
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

#ifndef CNTDISPLAYLABEL_H_
#define CNTDISPLAYLABEL_H_

#include <QObject>
#include <QLatin1String>
#include <e32base.h>

#include <qcontactmanager.h>
#include <qcontact.h>
#include <qcontactdetail.h>


#include <qmobilityglobal.h>

QTM_USE_NAMESPACE

class CRepository;
class CntDisplayLabel;

#include <centralrepository.h>

// CONSTANTS
// Please keep this in sync with the values in the system header file <cntuids.h>
// We cannot include this system header file because it is owned by an app layer
// package (Contacts package). This being a mw layer, the dependency is not allowed
// Cenrep UIDs used in Avkon & QML phonebook apps. Name ordering is checked in either
// app depending on the one available. Priority is given to QML. Synchronizing of the
// key values is done by the applications
// QML phonebook UIDs
const TUid KCRCntSettings = {0x2002FF54};
const TUint32 KCntNameOrdering = 0x00000001;

const TUid KCRUidPhonebook = {0x101f8794};
const TUint32 KPhonebookNameOrdering = 0x00000001;

// name order enumerations
// Please keep this in sync with <cntuids.h> See above comments
enum NameOrder {
    CntOrderLastFirst = 0x0,
    CntOrderLastCommaFirst = 0x1,
    CntOrderFirstLast = 0x2
};

class MDisplayLabel
{
public:
    virtual void updateNameOrdering() = 0;
};

/**
 * This class is used to listen to central repository value changes
 * of Contacts app settings. It is also used to get key values for
 * name ordering
 */
class CntCenrep : public CActive
{
public:
    CntCenrep(MDisplayLabel& aDisplayLabel );
    ~CntCenrep();
 
public:
    int getValue();

private:
    void RunL();
    TInt RunError(TInt aError);
    void DoCancel();

private:
    CRepository*                iCenrep;
    MDisplayLabel&              iDisplayLabel;
    TUint32                     iKey;
    int                         iValue;
};

class CntDisplayLabel : public QObject, public MDisplayLabel
{
    Q_OBJECT
    
public:
    CntDisplayLabel();
    virtual ~CntDisplayLabel();

    QString synthesizedDisplayLabel( const QContact& contact, QContactManager::Error* error);
    QList<QPair<QLatin1String, QLatin1String> > contactFilterDetails() const;
    QList<QPair<QLatin1String, QLatin1String> > groupFilterDetails() const;
    void updateNameOrdering();

signals:
    void displayLabelChanged();
    
private:
    void setDisplayLabelDetails();
    QString generateDisplayLabel( const QContact &contact, const QList<QList<QPair<QLatin1String, QLatin1String> > > detailList) const;
    QString delimiter() const;
    QString comma() const;
    
private:
    QList<QList<QPair<QLatin1String, QLatin1String> > > m_contactDisplayLabelDetails;
    QList<QList<QPair<QLatin1String, QLatin1String> > > m_groupDisplayLabelDetails;
    CntCenrep* m_settings;
    int m_nameOrder;
};

#endif /* CNTDISPLAYLABEL_H_ */
