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

#include "nfctestserviceprovider2.h"

#include <QtGui>
#include <QApplication>
#include <qremoteserviceregister.h>
#include <qservicemanager.h>

#include <QDebug>
#include <qnearfieldmanager.h>
#include <qndefmessage.h>
#include <qnearfieldtarget.h>

#include <iostream>
#include <fstream>

using namespace std;

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void MyOutputHandler(QtMsgType type, const char *msg) {
    static int fd = -1;
      if (fd == -1)
          fd = ::open("E:\\nfctestserviceprovider2.log", O_WRONLY | O_CREAT);

      ::write(fd, msg, strlen(msg));
      ::write(fd, "\n", 1);
      ::fsync(fd);

      switch (type) {
      case QtFatalMsg:
          abort();
      }
}

QTM_USE_NAMESPACE

nfctestserviceprovider2* w;

class MyContentHandler : public QObject
{
    Q_OBJECT

    
signals:
        void userHandleMessage(const QNdefMessage& msg, QNearFieldTarget* target);
        
public slots:
        void handleMessage(const QNdefMessage& msg, QNearFieldTarget* target)
        {
        QFile m_file("E:\\testserviceprovider2.dat");
        m_file.open(QIODevice::ReadWrite | QIODevice::Append);
        QDataStream m_dataStream(&m_file);
        QByteArray msgArray = msg.toByteArray();
        m_dataStream << msgArray;
                         
        w->close();
        }
        
public:
    MyContentHandler(QObject* parent = 0)
        : QObject(parent)
    {
    qDebug() << " MyContentHandler constructed !!!!!" << endl;
      connect(this, SIGNAL(userHandleMessage(const QNdefMessage& , QNearFieldTarget* )),
            this, SLOT(handleMessage(const QNdefMessage& , QNearFieldTarget* )));
    }

};

void unregisterExampleService()
{
    QServiceManager m;
    m.removeService("nfctestserviceprovider2");
}

void registerExampleService()
{
    unregisterExampleService();
    QServiceManager m;
    const QString path = QCoreApplication::applicationDirPath() + "/xmldata/nfctestserviceprovider2.xml";
    qWarning() << "xml path:" << path;
    if (!m.addService(path))
        {
        qWarning() << "Cannot register service provider" << path;
        }
    else
        {
        qDebug() << " Register ok" << endl;
        }
}

Q_DECLARE_METATYPE(QMetaType::Type);



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    w = new nfctestserviceprovider2();
    w->setWindowTitle( "nfc test service provider 2" );
    w->showMaximized();
    
    qInstallMsgHandler(MyOutputHandler);
    
    qRegisterMetaType<QNearFieldTarget*>("QNearFieldTarget*"); 
    qRegisterMetaType<QNdefMessage>("QNdefMessage"); 

    registerExampleService();
    
    MyContentHandler handler;
    QNearFieldManager manager;
    
    int handle = manager.registerNdefMessageHandler(&handler, SIGNAL(userHandleMessage(QNdefMessage, QNearFieldTarget*)));
    
    QFile m_file("E:\\testserviceprovider2.dat");
    m_file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream m_textStream(&m_file);
    m_textStream << "register handle return " << QString::number(handle);
            
    int ret = app.exec();
    
    manager.unregisterNdefMessageHandler(handle);
    delete w;
    return ret;
}

#include "main.moc"
