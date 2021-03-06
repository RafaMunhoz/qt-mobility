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

#ifndef QORGANIZERRECURRENCETRANSFORM_H
#define QORGANIZERRECURRENCETRANSFORM_H

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

#include "qtorganizer.h"
#include <CMulticalendar.h>
#include <CRecurrenceRule.h>

QTM_USE_NAMESPACE

class OrganizerRecurrenceTransform
{
public:
    OrganizerRecurrenceTransform();
    ~OrganizerRecurrenceTransform();

public: // Conversion from QRecurrence to CRecurrence
    // init
    void beginTransformToCrecurrence();

    // set rules and dates
    void addQOrganizerRecurrenceRule(const QOrganizerRecurrenceRule &rule);
    void addQOrganizerItemExceptionRule(const QOrganizerRecurrenceRule &rule);
    void addQOrganizerItemRecurrenceDate(const QDate &date);
    void addQOrganizerItemExceptionDate(const QDate &date);

    // get recurrence
    CRecurrence* crecurrence(bool *success = 0) const;

public: // Conversion from CRecurrence to QRecurrence
    // init and set recurrence
    void transformToQrecurrence(CRecurrence *crecurrence);

    // get rules and dates
    QSet<QOrganizerRecurrenceRule> recurrenceRules() const;
    QSet<QOrganizerRecurrenceRule> exceptionRules() const;
    QSet<QDate> recurrenceDates() const;
    QSet<QDate> exceptionDates() const;

private:
    // qrule -> crule conversions
    QString qrecurrenceRuleToIcalRecurrenceRule(const QOrganizerRecurrenceRule &rule) const;
    QString qfrequencyToIcalFrequency(QOrganizerRecurrenceRule::Frequency frequency) const;
    QString qcountToIcalCount(int count) const;
    QString qintervalToIcalInterval(int interval) const;
    QString qendDateToIcalUntil(QDate endDate) const;
    QString qdaysOfWeekToIcalByDay(const QSet<Qt::DayOfWeek> &daysOfWeek) const;
    QString qweekStartToIcalWkst(Qt::DayOfWeek dayOfWeek) const;
    QString qweekdayToIcalWeekday(Qt::DayOfWeek dayOfWeek) const;
    QString qdaysOfMonthToIcalByMonthDay(const QSet<int> &daysOfMonth) const;
    QString qdaysOfYearToIcalByYearDay(const QSet<int> &daysOfYear) const;
    QString qmonthsToIcalByMonth(const QSet<QOrganizerRecurrenceRule::Month> &months) const;
    QString qweeksOfYearToIcalByWeekNo(const QSet<int> &weeksOfYear) const;
    QString qpositionsToIcalBySetPos(const QSet<int> &positions) const;
    QString listOfNumbers(const QSet<int> &list) const;
    int qfrequencyToRtype(QOrganizerRecurrenceRule::Frequency frequency) const;

    // crule -> qrule conversions
    QOrganizerRecurrenceRule icalRecurrenceRuleToQrecurrenceRule(CRecurrenceRule *rule) const;
    QOrganizerRecurrenceRule::Frequency icalFrequencyToQfrequency(FREQUENCY frequency) const;
    Qt::DayOfWeek icalWeekdayToQdayOfWeek(short weekday, bool *status = 0) const;
    Qt::DayOfWeek icalRecurrenceTypeWeekdayToQdayOfWeek(const QString& weekday) const;

    // common methods
    QMap<QString, Qt::DayOfWeek> icalRecurrenceWeekDayQdayOfWeekMapping() const;

private:
    // QRecurrence -> CRecurrence
    int m_rtype; // Recursion type parameter for the native calendar backend
    std::vector< CRecurrenceRule* > m_vRRuleList; // recurrence and exception rules
    std::vector< std::string > m_vRecDateList; // recursion dates
    std::vector< std::string > m_vExceptionDateList; // exception dates

    // CRecurrence -> QRecurrence
    QSet<QOrganizerRecurrenceRule> m_lRecurrenceRules;
    QSet<QOrganizerRecurrenceRule> m_lExceptionRules;
    QSet<QDate> m_lRecurrenceDates;
    QSet<QDate> m_lExceptionDates;
};

#endif // QORGANIZERRECURRENCETRANSFORM_H
