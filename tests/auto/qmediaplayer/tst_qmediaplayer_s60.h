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
#ifndef TST_QMEDIAPLAYER_S60_H
#define TST_QMEDIAPLAYER_S60_H

#include <QtTest/QtTest>
#include <QtCore>
#include <QtGui>
#include <QFile>

#include <QMediaPlayer>
#include <QMediaPlayerControl>
#include <QMediaPlaylist>
#include <QMediaService>
#include <QMediaStreamsControl>
#include <QVideoWidget>

QT_USE_NAMESPACE

#define WAIT_FOR_CONDITION(a,e)            \
    for (int _i = 0; _i < 500; _i += 1) {  \
        if ((a) == (e)) break;             \
        QTest::qWait(10);}


#define WAIT_LONG_FOR_CONDITION(a,e)        \
    for (int _i = 0; _i < 1800; _i += 1) {  \
        if ((a) == (e)) break;              \
        QTest::qWait(100);}

class mediaStatusList : public QObject, public QList<QMediaPlayer::MediaStatus>
{
    Q_OBJECT
public slots:
    void mediaStatus(QMediaPlayer::MediaStatus status) {
        append(status);
    }

public:
    mediaStatusList(QObject *obj, const char *aSignal)
    : QObject()
    {
        connect(obj, aSignal, this, SLOT(mediaStatus(QMediaPlayer::MediaStatus)));
    }
};

class tst_QMediaPlayer_s60: public QObject
{
    Q_OBJECT

public slots:
    void initTestCase_data();
    void initTestCase_data_default_winscw();
    void initTestCase_data_default_armv5();
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    void testNullService();
    void testMedia();
    void testDuration();
    void testPosition();
    void testPositionWhilePlaying();
    void testVolume();
    void testVolumeWhilePlaying();
    void testMuted();
    void testMutedWhilePlaying();
    void testVideoAndAudioAvailability();
    void testError();
    void testPlay();
    void testPause();
    void testStop();
    void testMediaStatus();
    void testPlaylist();
    void testStreamControl();

private:
    QMediaPlayer *m_player;
    QVideoWidget *m_widget;
    bool runonce;
};

#endif // TST_QMEDIAPLAYER_S60_H
