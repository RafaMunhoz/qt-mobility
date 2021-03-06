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

#include "qgeosearchreply.h"
#include "qgeosearchreply_p.h"

QTM_BEGIN_NAMESPACE
/*!
    \class QGeoSearchReply

    \brief The QGeoSearchReply class manages an operation started by an
    instance of QGeoSearchManager.


    \inmodule QtLocation
    \since 1.1

    \ingroup maps-places

    Instances of QGeoSearchReply manage the state and results of these
    operations.

    The isFinished(), error() and errorString() methods provide information
    on whether the operation has completed and if it completed successfully.

    The finished() and error(QGeoSearchReply::Error,QString)
    signals can be used to monitor the progress of the operation.

    It is possible that a newly created QGeoSearchReply may be in a finished
    state, most commonly because an error has occurred. Since such an instance
    will never emit the finished() or
    error(QGeoSearchReply::Error,QString) signals, it is
    important to check the result of isFinished() before making the connections
    to the signals. The documentation for QGeoSearchManager demonstrates how
    this might be carried out.

    If the operation completes successfully the results will be able to be
    accessed with places().
*/

/*!
    \enum QGeoSearchReply::Error

    Describes an error which prevented the completion of the operation.

    \value NoError
        No error has occurred.
    \value EngineNotSetError
        The search manager that was used did not have a QGeoSearchManagerEngine instance associated with it.
    \value CommunicationError
        An error occurred while communicating with the service provider.
    \value ParseError
        The response from the service provider was in an unrecognizable format.
    \value UnsupportedOptionError
        The requested operation or one of the options for the operation are not
        supported by the service provider.
    \value CombinationError
        An error occurred while results where being combined from multiple sources.
    \value UnknownError
        An error occurred which does not fit into any of the other categories.
*/

/*!
    Constructs a search reply with the specified \a parent.
*/
QGeoSearchReply::QGeoSearchReply(QObject *parent)
    : QObject(parent),
      d_ptr(new QGeoSearchReplyPrivate()) {}

/*!
    Constructs a search reply with a given \a error and \a errorString and the specified \a parent.
    \since 1.1
*/
QGeoSearchReply::QGeoSearchReply(Error error, const QString &errorString, QObject *parent)
    : QObject(parent),
      d_ptr(new QGeoSearchReplyPrivate(error, errorString)) {}

/*!
    Destroys this search reply object.
*/
QGeoSearchReply::~QGeoSearchReply()
{
    delete d_ptr;
}

/*!
    Sets whether or not this reply has finished to \a finished.

    If \a finished is true, this will cause the finished() signal to be
    emitted.

    If the operation completed successfully, QGeoSearchReply::setPlaces()
    should be called before this function. If an error occurred,
    QGeoSearchReply::setError() should be used instead.
    \since 1.1
*/
void QGeoSearchReply::setFinished(bool finished)
{
    d_ptr->isFinished = finished;
    if (d_ptr->isFinished)
        emit this->finished();
}

/*!
    Return true if the operation completed successfully or encountered an
    error which cause the operation to come to a halt.
    \since 1.1
*/
bool QGeoSearchReply::isFinished() const
{
    return d_ptr->isFinished;
}

/*!
    Sets the error state of this reply to \a error and the textual
    representation of the error to \a errorString.

    This wil also cause error() and finished() signals to be emitted, in that
    order.
    \since 1.1
*/
void QGeoSearchReply::setError(QGeoSearchReply::Error error, const QString &errorString)
{
    d_ptr->error = error;
    d_ptr->errorString = errorString;
    emit this->error(error, errorString);
    setFinished(true);
}

/*!
    Returns the error state of this reply.

    If the result is QGeoSearchReply::NoError then no error has occurred.
    \since 1.1
*/
QGeoSearchReply::Error QGeoSearchReply::error() const
{
    return d_ptr->error;
}

/*!
    Returns the textual representation of the error state of this reply.

    If no error has occurred this will return an empty string.  It is possible
    that an error occurred which has no associated textual representation, in
    which case this will also return an empty string.

    To determine whether an error has occurred, check to see if
    QGeoSearchReply::error() is equal to QGeoSearchReply::NoError.
    \since 1.1
*/
QString QGeoSearchReply::errorString() const
{
    return d_ptr->errorString;
}

/*!
    Sets the viewport which contains the results to \a viewport.
    \since 1.1
*/
void QGeoSearchReply::setViewport(QGeoBoundingArea *viewport)
{
    d_ptr->viewport = viewport;
}

/*!
    Returns the viewport which contains the results.

    This function will return 0 if no viewport bias
    was specified in the QGeoSearchManager function which created this reply.
    \since 1.1
*/
QGeoBoundingArea* QGeoSearchReply::viewport() const
{
    return d_ptr->viewport;
}

/*!
    Returns a list of places.

    The places are the results of the operation corresponding to the
    QGeoSearchManager function which created this reply.
    \since 1.1
*/
QList<QGeoPlace> QGeoSearchReply::places() const
{
    return d_ptr->places;
}

/*!
    Adds \a place to the list of places in this reply.
    \since 1.1
*/
void QGeoSearchReply::addPlace(const QGeoPlace &place)
{
    d_ptr->places.append(place);
}

/*!
    Sets the list of \a places in the reply.
    \since 1.1
*/
void QGeoSearchReply::setPlaces(const QList<QGeoPlace> &places)
{
    d_ptr->places = places;
}

/*!
    Cancels the operation immediately.

    This will do nothing if the reply is finished.
    \since 1.1
*/
void QGeoSearchReply::abort()
{
    if (!isFinished())
        setFinished(true);
}

/*!
    Returns the limit on the number of responses from each data source.

    If no limit was set this function will return -1.

    This may be more than places().length() if the number of responses
    was less than the number requested.

    If QGeoSearchManager::search() is used along with
    QGeoSearchManager::setAdditionalLandmarkManagers the number of results can
    be as high as limit * (1 + number of additional landmark managers).
    \since 1.1
*/
int QGeoSearchReply::limit() const
{
    return d_ptr->limit;
}

/*!
    Returns the offset into the entire result set at which to start
    fetching results.
    \since 1.1
*/
int QGeoSearchReply::offset() const
{
    return d_ptr->offset;
}

/*!
    Sets the limit on the number of responses from each data source to \a limit.

    If \a limit is -1 then all available responses will be returned.
    \since 1.1
*/
void QGeoSearchReply::setLimit(int limit)
{
    d_ptr->limit = limit;
}

/*!
    Sets the offset in the entire result set at which to start
    fetching result to \a offset.
    \since 1.1
*/
void QGeoSearchReply::setOffset(int offset)
{
    d_ptr->offset = offset;
}

/*!
    \fn void QGeoSearchReply::finished()

    This signal is emitted when this reply has finished processing.

    If error() equals QGeoSearchReply::NoError then the processing
    finished successfully.

    This signal and QGeoSearchManager::finished() will be
    emitted at the same time.

    \note Do no delete this reply object in the slot connected to this
    signal. Use deleteLater() instead.
    \since 1.1
*/
/*!
    \fn void QGeoSearchReply::error(QGeoSearchReply::Error error, const QString &errorString)

    This signal is emitted when an error has been detected in the processing of
    this reply. The finished() signal will probably follow.

    The error will be described by the error code \a error. If \a errorString is
    not empty it will contain a textual description of the error.

    This signal and QGeoSearchManager::error() will be emitted at the same time.

    \note Do no delete this reply object in the slot connected to this
    signal. Use deleteLater() instead.
    \since 1.1
*/

/*******************************************************************************
*******************************************************************************/

QGeoSearchReplyPrivate::QGeoSearchReplyPrivate()
    : error(QGeoSearchReply::NoError),
      errorString(""),
      isFinished(false),
      viewport(0),
      limit(-1),
      offset(0) {}

QGeoSearchReplyPrivate::QGeoSearchReplyPrivate(QGeoSearchReply::Error error, const QString &errorString)
    : error(error),
      errorString(errorString),
      isFinished(true),
      viewport(0),
      limit(-1),
      offset(0) {}

QGeoSearchReplyPrivate::~QGeoSearchReplyPrivate() {}


#include "moc_qgeosearchreply.cpp"

QTM_END_NAMESPACE
