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

#include <QtTest/QtTest>
#include <qsysteminfo.h>
#include <qsystemdeviceinfo.h>

QTM_USE_NAMESPACE

//#define SHOWDEBUGS

QSemaphore MainThreadRunningSemaphore;
QSemaphore OtherThreadRunningSemaphore;

class Thread : public QThread
{
    Q_OBJECT
public:
    Thread(QSystemDeviceInfo *deviceInfo, QSystemNetworkInfo *networkInfo)
        : mSysDeviceInfo(deviceInfo), mSysNetworkInfo(networkInfo) {}

    void run() {
#ifdef SHOWDEBUGS
        qDebug()<<"Thread::run - running in thread: "<<thread()->currentThreadId();
        qDebug()<<"Thread::run - read something directly from object in different thread: "
                <<mSysDeviceInfo->manufacturer();
        qDebug()<<"Thread::run - create a signal&slot connection";
#endif//SHOWDEBUGS
        connect( mSysDeviceInfo, SIGNAL(batteryLevelChanged(int)),
                this, SLOT(testSlot()));
        MainThreadRunningSemaphore.release();
        OtherThreadRunningSemaphore.acquire();
#ifdef SHOWDEBUGS
        qDebug()<<"ThreadU::run - read something directly from object moved to this thread: "
                <<mSysNetworkInfo->currentMode();
#endif//SHOWDEBUGS
        MainThreadRunningSemaphore.release();
    }

public slots:
    void testSlot() {
#ifdef SHOWDEBUGS
        qDebug()<<"Thread::testSlot called - running in thread: "<<thread()->currentThreadId();
#endif//SHOWDEBUGS
    }

public:
    QSystemDeviceInfo *mSysDeviceInfo;
    QSystemNetworkInfo *mSysNetworkInfo;
};

class tst_QSystemInfo_MoveInfosToDifferentThreads : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testCase() {
#ifdef SHOWDEBUGS
        qDebug()<<"tst_QSystemInfo_MoveInfosToDifferentThreads::testCase - create objects - running in thread: "<<thread()->currentThreadId();
#endif//SHOWDEBUGS
        QSystemDeviceInfo deviceInfo;
        QSystemNetworkInfo networkInfo;
        Thread testThread(&deviceInfo, &networkInfo);
        testThread.start();
        MainThreadRunningSemaphore.acquire();
#ifdef SHOWDEBUGS
        qDebug()<<"tst_QSystemInfo_MoveInfosToDifferentThreads::testCase - move objects to other thread";
#endif//SHOWDEBUGS
        deviceInfo.moveToThread(&testThread);
        networkInfo.moveToThread(&testThread);
        OtherThreadRunningSemaphore.release();
        MainThreadRunningSemaphore.acquire();
#ifdef SHOWDEBUGS
        qDebug()<<"tst_QSystemInfo_MoveInfosToDifferentThreads::testCase - read something directly from object moved to other thread: "
                <<deviceInfo.productName();
#endif//SHOWDEBUGS
        OtherThreadRunningSemaphore.release();
        testThread.wait();
    }
};

QTEST_MAIN(tst_QSystemInfo_MoveInfosToDifferentThreads);

#include "tst_MoveInfosToDifferentThreads.moc"
