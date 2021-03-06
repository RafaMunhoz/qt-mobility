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

#include "qbluetooth.h"

/*!
    \namespace QBluetooth
    \brief The QBluetooth namespace contains functions and definitions related to Bluetooth.
    \since 1.2

    \ingroup connectivity-bluetooth
    \inmodule QtConnectivity
*/

/*!
    \enum QBluetooth::Security

    This enum describe the security requirements of a Bluetooth service.

    \value NoSecurity       The service does not require any security.

    \value Authorization The service requires authorization. Device does not
    have to paired, the connection will be granted by prompting the user unless
    the device is Authorized-Paired where the connection will be made
    automatically.

    \value Authentication The service requires authentication. Device must
    paired, the user maybe prompted on connection unless the device is
    Authorized-Paired.

    \value Encryption The service requires that the communications link be
    encrypted.  This requires the device be paired.

    \value Secure The service requires that the communications link be secure.
    Legacy pairing is not permitted, Simple Pairing from Bluetooth 2.1 or
    greater is required.
*/
