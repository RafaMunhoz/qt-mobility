/****************************************************************************
**
** Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** If you have questions regarding the use of this file, please
** contact Nokia at http://qt.nokia.com/contact.
** $QT_END_LICENSE$
**
****************************************************************************/
#ifndef QSYSTEMINFO_LINUX_P_H
#define QSYSTEMINFO_LINUX_P_H


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


#include <QObject>
#include <QSize>
#include <QHash>

#include "qsysteminfo.h"
#include <qsysinfoglobal.h>
#if !defined(QT_NO_DBUS)
#include <qhalservice_linux_p.h>
#endif

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QStringList;
class QSystemNetworkInfo;
class QTimer;

class QSystemInfoPrivate : public QObject
{
    Q_OBJECT

public:

    QSystemInfoPrivate(QObject *parent = 0);
    virtual ~QSystemInfoPrivate();
// general
    QString currentLanguage() const; // 2 letter ISO 639-1
    QStringList availableLanguages() const;	 // 2 letter ISO 639-1

    QString version(QSystemInfo::Version,  const QString &parameter = QString());

    QString currentCountryCode() const; //2 letter ISO 3166-1
//features
    bool hasFeatureSupported(QSystemInfo::Feature feature);
Q_SIGNALS:
    void currentLanguageChanged(const QString &);

private:
#if !defined(QT_NO_DBUS)
    bool hasHalDeviceFeature(const QString &param);
    bool hasHalUsbFeature(qint32 usbClass);
    QHalInterface halIface;
#endif
    bool hasSysFeature(const QString &featureStr);
    QTimer *langTimer;
    QString langCached;

private Q_SLOTS:
    void startLangaugePolling();


};

class QSystemNetworkInfoPrivate : public QObject
{
    Q_OBJECT

public:

    QSystemNetworkInfoPrivate(QObject *parent = 0);
    virtual ~QSystemNetworkInfoPrivate();

    QSystemNetworkInfo::NetworkStatus networkStatus(QSystemNetworkInfo::NetworkMode mode);
    qint32 networkSignalStrength(QSystemNetworkInfo::NetworkMode mode);
    int cellId();
    int locationAreaCode();

    QString currentMobileCountryCode(); // Mobile Country Code
    QString currentMobileNetworkCode(); // Mobile Network Code

    QString homeMobileCountryCode();
    QString homeMobileNetworkCode();

    QString networkName(QSystemNetworkInfo::NetworkMode mode);
    QString macAddress(QSystemNetworkInfo::NetworkMode mode);

    QNetworkInterface interfaceForMode(QSystemNetworkInfo::NetworkMode mode);

Q_SIGNALS:
   void networkStatusChanged(QSystemNetworkInfo::NetworkMode, QSystemNetworkInfo::NetworkStatus);
   void networkSignalStrengthChanged(QSystemNetworkInfo::NetworkMode,int);
   void currentMobileCountryCodeChanged(const QString &);
   void currentMobileNetworkCodeChanged(const QString &);
   void networkNameChanged(QSystemNetworkInfo::NetworkMode, const QString &);
   void networkModeChanged(QSystemNetworkInfo::NetworkMode);

};

class QSystemDisplayInfoPrivate : public QObject
{
    Q_OBJECT

public:

    QSystemDisplayInfoPrivate(QObject *parent = 0);
    virtual ~QSystemDisplayInfoPrivate();


// display
    int displayBrightness(int screen);
    int colorDepth(int screen);
};

class QSystemMemoryInfoPrivate : public QObject
{
    Q_OBJECT

public:

    QSystemMemoryInfoPrivate(QObject *parent = 0);
    virtual ~QSystemMemoryInfoPrivate();

    // memory
    qint64 availableDiskSpace(const QString &driveVolume);
    qint64 totalDiskSpace(const QString &driveVolume);
    QStringList listOfVolumes();
    QSystemMemoryInfo::VolumeType volumeType(const QString &driveVolume); //returns enum

private:
    QHash<QString, QString> mountEntriesHash;
    void mountEntries();

};

class QSystemDeviceInfoPrivate : public QObject
{
    Q_OBJECT

public:

    QSystemDeviceInfoPrivate(QObject *parent = 0);
    ~QSystemDeviceInfoPrivate();

// device

    static QString imei();
    static QString imsi();
    static QString manufacturer();
    static QString model();
    static QString productName();

    QSystemDeviceInfo::InputMethodFlags inputMethodType();

    int  batteryLevel() const;
    QSystemDeviceInfo::BatteryStatus batteryStatus();

    QSystemDeviceInfo::SimStatus simStatus();
    bool isDeviceLocked();
    QSystemDeviceInfo::Profile currentProfile();
    bool isBatteryCharging();

    QSystemDeviceInfo::PowerState currentPowerState();
    void setConnection();

Q_SIGNALS:
    void batteryLevelChanged(int);
    void batteryStatusChanged(QSystemDeviceInfo::BatteryStatus );

    void powerStateChanged(QSystemDeviceInfo::PowerState);
    void currentProfileChanged(QSystemDeviceInfo::Profile);
    void bluetoothStateChanged(bool);

private:
#if !defined(QT_NO_DBUS)
    QHalInterface *halIface;
    QHalDeviceInterface *halIfaceDevice;

private Q_SLOTS:
    void halChanged(int,QVariantList);
#endif
};


class QSystemScreenSaverPrivate : public QObject
{
    Q_OBJECT

public:
    QSystemScreenSaverPrivate(QObject *parent = 0);
    ~QSystemScreenSaverPrivate();

    bool screenSaverEnabled();
    bool screenBlankingEnabled();
    bool setScreenSaverEnabled(bool b);
    bool setScreenBlankingEnabled(bool b);
    bool isScreenLockOn();

private:
    QString screenPath;
    QString settingsPath;
    bool screenSaverSecure;

    uint currentPid;
    bool kdeIsRunning;
    bool gnomeIsRunning;
    void whichWMRunning();

};

QT_END_NAMESPACE

QT_END_HEADER

#endif /*QSYSTEMSINFO_LINUX_P_H*/

// End of file
