/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef LANDMARKFILTERDIALOG_H
#define LANDMARKFILTERDIALOG_H

#include <QListWidget>
#include <QWidget>

#include <qlandmark.h>
#include <qlandmarkfilter.h>
#include <qlandmarkfetchrequest.h>
#include <qlandmarkmanager.h>

#include "ui_landmarkfilterdialog.h"

QTM_USE_NAMESPACE

class LandmarkFilterDialog : public QDialog, public Ui_LandmarkFilterDialog
{
Q_OBJECT
public:
    LandmarkFilterDialog(QLandmarkFetchRequest *fetchRequest, QLandmarkManager * manager, QWidget *parent =0, Qt::WindowFlags flags =0);
    ~LandmarkFilterDialog();

signals:
    void doFetchAll();

public slots:
    virtual void accept();
    virtual void reject();

    void filterAllCheckBoxStateChanged(int state);
    void otherFiltersCheckBoxStateChanged(int state);
    void categoryRemoved(const QList<QLandmarkCategoryId> &categoryIds);

protected:
    void showEvent(QShowEvent *showEvent);

private:
    bool setupFetchRequest();
    bool isValidLatitude(const QString &latitude);
    bool isValidLongitude(const QString &longitude);
    QLandmarkCategoryId oldCategoryId;
    QLandmarkFetchRequest *fetchRequest;
    QLandmarkManager *manager;
};

#endif
