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

#ifndef CPPMODELMANAGERINTERFACE_H
#define CPPMODELMANAGERINTERFACE_H

#include <cpptools/cpptools_global.h>
#include <cplusplus/CppDocument.h>
#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QPointer>
#include <QtCore/QStringList>

namespace ProjectExplorer {
    class Project;
}

namespace CppTools {

class AbstractEditorSupport;

class CPPTOOLS_EXPORT CppModelManagerInterface : public QObject
{
    Q_OBJECT

public:
    class ProjectInfo
    {
    public:
        ProjectInfo()
        { }

        ProjectInfo(QPointer<ProjectExplorer::Project> project)
            : project(project)
        { }

        operator bool() const
        { return ! project.isNull(); }

        bool isValid() const
        { return ! project.isNull(); }

        bool isNull() const
        { return project.isNull(); }

    public: // attributes
        QPointer<ProjectExplorer::Project> project;
        QString projectPath;
        QByteArray defines;
        QStringList sourceFiles;
        QStringList includePaths;
        QStringList frameworkPaths;
        QStringList precompiledHeaders;
    };

    class WorkingCopy
    {
    public:
        void insert(const QString &fileName, const QString &source, unsigned revision = 0)
        { _elements.insert(fileName, qMakePair(source, revision)); }

        bool contains(const QString &fileName) const
        { return _elements.contains(fileName); }

        QString source(const QString &fileName) const
        { return _elements.value(fileName).first; }

        QPair<QString, unsigned> get(const QString &fileName) const
        { return _elements.value(fileName); }

    private:
        typedef QHash<QString, QPair<QString, unsigned> > Table;
        Table _elements;
    };

public:
    CppModelManagerInterface(QObject *parent = 0) : QObject(parent) {}
    virtual ~CppModelManagerInterface() {}

    static CppModelManagerInterface *instance();

    virtual WorkingCopy workingCopy() const = 0;
    virtual CPlusPlus::Snapshot snapshot() const = 0;

    virtual QList<ProjectInfo> projectInfos() const = 0;
    virtual ProjectInfo projectInfo(ProjectExplorer::Project *project) const = 0;
    virtual void updateProjectInfo(const ProjectInfo &pinfo) = 0;

    virtual QStringList includesInPath(const QString &path) const = 0;

    virtual void addEditorSupport(AbstractEditorSupport *editorSupport) = 0;
    virtual void removeEditorSupport(AbstractEditorSupport *editorSupport) = 0;

    virtual QList<int> references(CPlusPlus::Symbol *symbol,
                                  CPlusPlus::Document::Ptr doc,
                                  const CPlusPlus::Snapshot &snapshot) = 0;

    virtual void renameUsages(CPlusPlus::Symbol *symbol) = 0;
    virtual void findUsages(CPlusPlus::Symbol *symbol) = 0;

    virtual void findMacroUsages(const CPlusPlus::Macro &macro) = 0;

public Q_SLOTS:
    void updateModifiedSourceFiles();
    virtual void updateSourceFiles(const QStringList &sourceFiles) = 0;
    virtual void GC() = 0;
};

class CPPTOOLS_EXPORT AbstractEditorSupport
{
public:
    explicit AbstractEditorSupport(CppModelManagerInterface *modelmanager);
    virtual ~AbstractEditorSupport();

    virtual QByteArray contents() const = 0;
    virtual QString fileName() const = 0;

    void updateDocument();

    // TODO: find a better place for common utility functions
    static QString functionAt(const CppModelManagerInterface *mm,
                              const QString &fileName,
                              int line, int column);

    static QString licenseTemplate();

private:
    CppModelManagerInterface *m_modelmanager;
};

} // namespace CppTools

#endif // CPPMODELMANAGERINTERFACE_H
