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

#include <qmobilityglobal.h>
#include "qradiotunercontrol.h"
#include "qmediacontrol_p.h"

QT_BEGIN_NAMESPACE


/*!
    \class QRadioTunerControl
    \inmodule QtMultimediaKit
    \ingroup multimedia-serv
    \since 1.0


    \brief The QRadioTunerControl class provides access to the radio tuning
    functionality of a QMediaService.

    If a QMediaService can tune an analog radio device it will implement
    QRadioTunerControl.  This control provides a means to tune a radio device
    to a specific \l {setFrequency()}{frequency} as well as search \l
    {searchForward()}{forwards} and \l {searchBackward()}{backwards} for a
    signal.

    The functionality provided by this control is exposed to application code
    through the QRadioTuner class.

    The interface name of QRadioTunerControl is \c com.nokia.Qt.QRadioTunerControl/1.0 as
    defined in QRadioTunerControl_iid.

    \sa QMediaService::requestControl(), QRadioTuner
*/

/*!
    \macro QRadioTunerControl_iid

    \c com.nokia.Qt.QRadioTunerControl/1.0

    Defines the interface name of the QRadioTunerControl class.

    \relates QRadioTunerControl
*/

/*!
    Constructs a radio tuner control with the given \a parent.
*/

QRadioTunerControl::QRadioTunerControl(QObject *parent):
    QMediaControl(*new QMediaControlPrivate, parent)
{
}

/*!
    Destroys a radio tuner control.
*/

QRadioTunerControl::~QRadioTunerControl()
{
}

/*!
    \fn bool QRadioTunerControl::isAvailable() const

    Returns true if the radio service is ready to use.
    \since 1.0
*/

/*!
    \fn QtMultimediaKit::AvailabilityError QRadioTunerControl::availabilityError() const

    Returns the error state of the radio service.
    \since 1.0
*/

/*!
    \fn QRadioTuner::State QRadioTunerControl::state() const

    Returns the current radio tuner state.
    \since 1.0
*/

/*!
    \fn QRadioTuner::Band QRadioTunerControl::band() const

    Returns the frequency band a radio tuner is tuned to.
    \since 1.0
*/

/*!
    \fn void QRadioTunerControl::bandChanged(QRadioTuner::Band band)

    Signals that the frequency \a band a radio tuner is tuned to has changed.
    \since 1.0
*/

/*!
    \fn void QRadioTunerControl::setBand(QRadioTuner::Band band)

    Sets the frequecy \a band a radio tuner is tuned to.

    Changing the frequency band will reset the frequency to the minimum frequency of the new band.
    \since 1.0
*/

/*!
    \fn bool QRadioTunerControl::isBandSupported(QRadioTuner::Band band) const

    Identifies if a frequency \a band is supported.

    Returns true if the band is supported, and false if it is not.
    \since 1.0
*/

/*!
    \fn int QRadioTunerControl::frequency() const

    Returns the frequency a radio tuner is tuned to.
    \since 1.0
*/

/*!
    \fn int QRadioTunerControl::frequencyStep(QRadioTuner::Band band) const

    Returns the number of Hertz to increment the frequency by when stepping through frequencies
    within a given \a band.
    \since 1.0
*/

/*!
    \fn QPair<int,int> QRadioTunerControl::frequencyRange(QRadioTuner::Band band) const

    Returns a frequency \a band's minimum and maximum frequency.
    \since 1.0
*/

/*!
    \fn void QRadioTunerControl::setFrequency(int frequency)

    Sets the \a frequency a radio tuner is tuned to.
    \since 1.0
*/

/*!
    \fn bool QRadioTunerControl::isStereo() const

    Identifies if a radio tuner is receiving a stereo signal.

    Returns true if the tuner is receiving a stereo signal, and false if it is not.
    \since 1.0
*/

/*!
    \fn QRadioTuner::StereoMode QRadioTunerControl::stereoMode() const

    Returns a radio tuner's stereo mode.

    \since 1.0
    \sa QRadioTuner::StereoMode
*/

/*!
    \fn void QRadioTunerControl::setStereoMode(QRadioTuner::StereoMode mode)

    Sets a radio tuner's stereo \a mode.

    \since 1.0
    \sa QRadioTuner::StereoMode
*/

/*!
    \fn int QRadioTunerControl::signalStrength() const

    Return a radio tuner's current signal strength as a percentage.
    \since 1.0
*/

/*!
    \fn int QRadioTunerControl::volume() const

    Returns the volume of a radio tuner's audio output as a percentage.
    \since 1.0
*/

/*!
    \fn void QRadioTunerControl::setVolume(int volume)

    Sets the percentage \a volume of a radio tuner's audio output.
    \since 1.0
*/

/*!
    \fn bool QRadioTunerControl::isMuted() const

    Identifies if a radio tuner's audio output is muted.

    Returns true if the audio is muted, and false if it is not.
    \since 1.0
*/

/*!
    \fn void QRadioTunerControl::setMuted(bool muted)

    Sets the \a muted state of a radio tuner's audio output.
    \since 1.0
*/

/*!
    \fn bool QRadioTunerControl::isSearching() const

    Identifies if a radio tuner is currently scanning for signal.

    Returns true if the tuner is scanning, and false if it is not.
    \since 1.0
*/

/*!
    \fn  void QRadioTunerControl::searchForward()

    Starts a forward scan for a signal, starting from the current \l frequency().
    \since 1.0
*/

/*!
    \fn void QRadioTunerControl::searchBackward()

    Starts a backwards scan for a signal, starting from the current \l frequency().
    \since 1.0
*/

/*!
    \fn void QRadioTunerControl::cancelSearch()

    Stops scanning for a signal.
    \since 1.0
*/

/*!
    \fn void QRadioTunerControl::start()

    Activate the radio device.
    \since 1.0
*/

/*!
    \fn QRadioTunerControl::stop()

    Deactivate the radio device.
    \since 1.0
*/

/*!
    \fn QRadioTuner::Error QRadioTunerControl::error() const

    Returns the error state of a radio tuner.
    \since 1.0
*/

/*!
    \fn QString QRadioTunerControl::errorString() const

    Returns a string describing a radio tuner's error state.
    \since 1.0
*/

/*!
    \fn void QRadioTunerControl::stateChanged(QRadioTuner::State state)

    Signals that the \a state of a radio tuner has changed.
    \since 1.0
*/


/*!
    \fn void QRadioTunerControl::frequencyChanged(int frequency)

    Signals that the \a frequency a radio tuner is tuned to has changed.
    \since 1.0
*/

/*!
    \fn void QRadioTunerControl::stereoStatusChanged(bool stereo)

    Signals that the \a stereo state of a radio tuner has changed.
    \since 1.0
*/

/*!
    \fn void QRadioTunerControl::searchingChanged(bool searching)

    Signals that the \a searching state of a radio tuner has changed.
    \since 1.0
*/

/*!
    \fn void QRadioTunerControl::signalStrengthChanged(int strength)

    Signals that the percentage \a strength of the signal received by a radio tuner has changed.
    \since 1.0
*/

/*!
    \fn void QRadioTunerControl::volumeChanged(int volume)

    Signals that the percentage \a volume of radio tuner's audio output has changed.
    \since 1.0
*/

/*!
    \fn void QRadioTunerControl::mutedChanged(bool muted)

    Signals that the \a muted state of a radio tuner's audio output has changed.
    \since 1.0
*/

/*!
    \fn void QRadioTunerControl::error(QRadioTuner::Error error)

    Signals that an \a error has occurred.
    \since 1.0
*/

#include "moc_qradiotunercontrol.cpp"
QT_END_NAMESPACE

