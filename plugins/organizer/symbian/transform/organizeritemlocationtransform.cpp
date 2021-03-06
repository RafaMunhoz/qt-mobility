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
#include "organizeritemlocationtransform.h"
#include "qorganizeritemlocation.h"

void OrganizerItemLocationTransform::modifyBaseSchemaDefinitions(QMap<QString, QMap<QString, QOrganizerItemDetailDefinition> > &schemaDefs) const
{
    // Create a detail definition
    QOrganizerItemDetailDefinition d;
    d.setName(QOrganizerItemLocation::DefinitionName);
    QOrganizerItemDetailFieldDefinition f;
    f.setDataType(QVariant::String);
    f.setAllowableValues(QVariantList());
    QMap<QString, QOrganizerItemDetailFieldDefinition> fields;

    fields.insert(QOrganizerItemLocation::FieldLabel, f);
#ifdef SYMBIAN_CALENDAR_V2
    f.setDataType(QVariant::Double);
    fields.insert(QOrganizerItemLocation::FieldLatitude, f);
    fields.insert(QOrganizerItemLocation::FieldLongitude, f);
#endif
    d.setFields(fields);
    d.setUnique(true);

    // Replace default detail definition with our own
    foreach (QString itemTypeName, schemaDefs.keys()) {
        QMap<QString, QOrganizerItemDetailDefinition> details = schemaDefs.value(itemTypeName);
        if (details.contains(d.name()))
            schemaDefs[itemTypeName].insert(d.name(), d);
    }    
}

void OrganizerItemLocationTransform::transformToDetailL(const CCalEntry& entry, QOrganizerItem *item)
{
    QString label = toQString(entry.LocationL());
	QOrganizerItemLocation location;
    if (!label.isEmpty())
        location.setLabel(label);

#ifdef SYMBIAN_CALENDAR_V2
    double latitude;
    double longitude;
    CCalGeoValue *geoValue = entry.GeoValueL();
    if (geoValue) {
        if (geoValue->GetLatLong(latitude, longitude)) {
            location.setLatitude(latitude);
            location.setLongitude(longitude);
        }
    }
#endif
    if (!location.isEmpty())
        item->saveDetail(&location);
}

void OrganizerItemLocationTransform::transformToEntryL(const QOrganizerItem& item, CCalEntry* entry)
{
    QOrganizerItemLocation loc = item.detail<QOrganizerItemLocation>();
    if (!loc.label().isEmpty()) {
        entry->SetLocationL(toPtrC16(loc.label()));
    }
    if(loc.hasValue(QOrganizerItemLocation::FieldLatitude) && loc.hasValue(QOrganizerItemLocation::FieldLongitude))
    {
#ifdef SYMBIAN_CALENDAR_V2
        CCalGeoValue* geoValue = CCalGeoValue::NewL();
        CleanupStack::PushL(geoValue);
        geoValue->SetLatLongL(loc.latitude(), loc.longitude());
        entry->SetGeoValueL(*geoValue);
        CleanupStack::PopAndDestroy(geoValue);
#else
        // We do not support saving of latitude/longitude.
        User::Leave(KErrNotSupported);
#endif
    }
}

QString OrganizerItemLocationTransform::detailDefinitionName()
{
    return QOrganizerItemLocation::DefinitionName;    
}
