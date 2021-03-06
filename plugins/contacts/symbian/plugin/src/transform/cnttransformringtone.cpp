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
#include "cnttransformringtone.h"
#include "cntmodelextuids.h"

#include "qcontactringtone.h"

QList<CContactItemField *> CntTransformRingtone::transformDetailL(const QContactDetail &detail)
{
    if(detail.definitionName() != QContactRingtone::DefinitionName)
        User::Leave(KErrArgument);

    QList<CContactItemField *> fieldList;

    const QContactRingtone &ringtone(static_cast<const QContactRingtone&>(detail));

    if (ringtone.audioRingtoneUrl().isValid()) {
        QUrl ringtoneUrl = ringtone.audioRingtoneUrl();
        QString ringtoneString;
        if (ringtoneUrl.scheme() == QLatin1String("file")) {
            ringtoneString = ringtoneUrl.toLocalFile();
            ringtoneString.replace("/", "\\");  // Must be stored with backslashes in Symbian
        } else {
            ringtoneString = ringtoneUrl.toString();
        }
        transformToTextFieldL(detail, fieldList,
            ringtoneString,
            KUidContactFieldRingTone,
            KUidContactFieldVCardMapUnknown, false);
	}

    if (ringtone.videoRingtoneUrl().isValid()) {
        QUrl ringtoneUrl = ringtone.videoRingtoneUrl();
        QString ringtoneString;
        if (ringtoneUrl.scheme() == QLatin1String("file")) {
            ringtoneString = ringtoneUrl.toLocalFile();
            ringtoneString.replace("/", "\\");  // Must be stored with backslashes in Symbian
        } else {
            ringtoneString = ringtoneUrl.toString();
        }
        transformToTextFieldL(detail, fieldList,
            ringtoneString,
            KUidContactFieldVideoRingTone,
            KUidContactFieldVCardMapUnknown, false);
    }

	return fieldList;
}

QContactDetail *CntTransformRingtone::transformItemField(const CContactItemField& field, const QContact &contact)
{
    QContactRingtone *ringtone = new QContactRingtone(contact.detail<QContactRingtone>());

	CContactTextField* storage = field.TextStorage();
    QString ringtoneString = QString::fromUtf16(storage->Text().Ptr(), storage->Text().Length());
    	
    QUrl ringtoneUrl = QUrl::fromUserInput(ringtoneString);
    
    if (field.ContentType().ContainsFieldType(KUidContactFieldRingTone)) {
        ringtone->setAudioRingtoneUrl(ringtoneUrl);
	}
    else if (field.ContentType().ContainsFieldType(KUidContactFieldVideoRingTone)) {
        ringtone->setVideoRingtoneUrl(ringtoneUrl);
    }

    return ringtone;
}

bool CntTransformRingtone::supportsDetail(QString detailName) const
{
    bool ret = false;
    if (detailName == QContactRingtone::DefinitionName) {
        ret = true;
    }
    return ret;
}

QList<TUid> CntTransformRingtone::supportedFields() const
{
    return QList<TUid>()
        << KUidContactFieldRingTone
        << KUidContactFieldVideoRingTone;
}

QList<TUid> CntTransformRingtone::supportedSortingFieldTypes(QString /*detailFieldName*/) const
{
    // Sorting not supported
    return QList<TUid>();
}


/*!
 * Checks whether the subtype is supported
 *
 * \a subType The subtype to be checked
 * \return True if this subtype is supported
 */
bool CntTransformRingtone::supportsSubType(const QString& /*subType*/) const
{
    return false;
}

/*!
 * Returns the filed id corresponding to a field
 *
 * \a fieldName The name of the supported field
 * \return fieldId for the fieldName, 0  if not supported
 */
quint32 CntTransformRingtone::getIdForField(const QString& fieldName) const
{
    if (QContactRingtone::FieldAudioRingtoneUrl == fieldName)
        return 0;
    else if (QContactRingtone::FieldVideoRingtoneUrl == fieldName)
        return 0;
    else
        return 0;
}

/*!
 * Modifies the detail definitions. The default detail definitions are
 * queried from QContactManagerEngine::schemaDefinitions and then modified
 * with this function in the transform leaf classes.
 *
 * \a definitions The detail definitions to modify.
 * \a contactType The contact type the definitions apply for.
 */
void CntTransformRingtone::detailDefinitions(QMap<QString, QContactDetailDefinition> &definitions, const QString& contactType) const
{
    Q_UNUSED(contactType);

    if(definitions.contains(QContactRingtone::DefinitionName)) {
        QContactDetailDefinition d = definitions.value(QContactRingtone::DefinitionName);
        QMap<QString, QContactDetailFieldDefinition> fields = d.fields();

        // Context not supported in symbian back-end, remove
        fields.remove(QContactRingtone::FieldContext);
        // nor vibe thingy
        fields.remove(QContactRingtone::FieldVibrationRingtoneUrl);

        d.setFields(fields);
        d.setUnique(true);

        // Replace original definitions
        definitions.insert(d.name(), d);
    }
}
