/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
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
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QDebug>

#include "qgstappsrc.h"
#include <QtNetwork>

QGstAppSrc::QGstAppSrc(QObject *parent)
    :QObject(parent)
    ,m_stream(0)
    ,m_appSrc(0)
    ,m_sequential(false)
    ,m_maxBytes(0)
    ,m_setup(false)
    ,m_dataRequestSize(-1)
    ,m_dataRequested(false)
    ,m_enoughData(false)
    ,m_forceData(false)
{
}

QGstAppSrc::~QGstAppSrc()
{
    if (m_appSrc)
        gst_object_unref(G_OBJECT(m_appSrc));
}

bool QGstAppSrc::setup(GstElement* appsrc)
{
    if (m_setup || m_stream == 0 || appsrc == 0)
        return false;

    m_appSrc = GST_APP_SRC(appsrc);
    m_callbacks.need_data   = &QGstAppSrc::on_need_data;
    m_callbacks.enough_data = &QGstAppSrc::on_enough_data;
    m_callbacks.seek_data   = &QGstAppSrc::on_seek_data;
    gst_app_src_set_callbacks(m_appSrc, (GstAppSrcCallbacks*)&m_callbacks, this, (GDestroyNotify)&QGstAppSrc::destroy_notify);

    g_object_get(G_OBJECT(m_appSrc), "max-bytes", &m_maxBytes, NULL);

    if (m_sequential)
        m_streamType = GST_APP_STREAM_TYPE_STREAM;
    else
        m_streamType = GST_APP_STREAM_TYPE_RANDOM_ACCESS;
    gst_app_src_set_stream_type(m_appSrc, m_streamType);
    gst_app_src_set_size(m_appSrc, (m_sequential) ? -1 : m_stream->size());

    return  m_setup = true;
}

void QGstAppSrc::setStream(QIODevice *stream)
{
    if (m_stream)
        disconnect(m_stream, SIGNAL(readyRead()), this, SLOT(onDataReady()));
    if (m_appSrc)
        gst_object_unref(G_OBJECT(m_appSrc));

    m_dataRequestSize = -1;
    m_dataRequested = false;
    m_enoughData = false;
    m_forceData = false;

    m_appSrc = 0;
    m_stream = stream;
    connect(m_stream, SIGNAL(readyRead()), this, SLOT(onDataReady()));
    m_sequential = m_stream->isSequential();
    m_setup = false;
}

QIODevice *QGstAppSrc::stream() const
{
    return m_stream;
}

GstAppSrc *QGstAppSrc::element()
{
    return m_appSrc;
}

void QGstAppSrc::onDataReady()
{
    if (!m_enoughData) {
        m_dataRequested = true;
        pushDataToAppSrc();
    }
}

void QGstAppSrc::pushDataToAppSrc()
{
    if (!m_stream || !m_setup)
        return;

    if (m_dataRequested && !m_enoughData) {
        qint64 size;
        if (m_dataRequestSize < 0)
            size = qMin(m_stream->bytesAvailable(), queueSize());
        else
            size = qMin(m_stream->bytesAvailable(), (qint64)m_dataRequestSize);
        void *data = g_malloc(size);
        GstBuffer* buffer = gst_app_buffer_new(data, size, g_free, data);
        buffer->offset = m_stream->pos();
        qint64 bytesRead = m_stream->read((char*)GST_BUFFER_DATA(buffer), size);
        buffer->offset_end =  buffer->offset + bytesRead - 1;

        if (bytesRead > 0) {
            m_dataRequested = false;
            m_enoughData = false;
            GstFlowReturn ret = gst_app_src_push_buffer (GST_APP_SRC (element()), buffer);
            if (ret == GST_FLOW_ERROR) {
                qWarning()<<"appsrc: push buffer error";
            } else if (ret == GST_FLOW_WRONG_STATE) {
                qWarning()<<"appsrc: push buffer wrong state";
            } else if (ret == GST_FLOW_RESEND) {
                qWarning()<<"appsrc: push buffer resend";
            }
        }
    } else if (m_stream->atEnd()) {
        sendEOS();
    }
}

bool QGstAppSrc::doSeek(qint64 value)
{
     return stream()->seek(value);
}


gboolean QGstAppSrc::on_seek_data(GstAppSrc *element, guint64 arg0, gpointer userdata)
{
    QGstAppSrc *self = reinterpret_cast<QGstAppSrc*>(userdata);
    if (self && self->stream() ) {
        if (!self->stream()->isSequential())
            QMetaObject::invokeMethod(self, "doSeek", Qt::AutoConnection, Q_ARG(qint64, arg0));
    }

    return true;
}

void QGstAppSrc::on_enough_data(GstAppSrc *element, gpointer userdata)
{
    QGstAppSrc *self = reinterpret_cast<QGstAppSrc*>(userdata);
    if (self)
        self->enoughData() = true;
}

void QGstAppSrc::on_need_data(GstAppSrc *element, guint arg0, gpointer userdata)
{
    QGstAppSrc *self = reinterpret_cast<QGstAppSrc*>(userdata);
    if (self) {
        self->dataRequested() = true;
        self->enoughData() = false;
        self->dataRequestSize()= arg0;
        QMetaObject::invokeMethod(self, "pushDataToAppSrc", Qt::AutoConnection);
    }
}

void QGstAppSrc::destroy_notify(gpointer data)
{
    Q_UNUSED(data);
}

void QGstAppSrc::sendEOS()
{
    gst_app_src_end_of_stream(GST_APP_SRC(m_appSrc));
    if (!stream()->isSequential())
        stream()->reset();
}