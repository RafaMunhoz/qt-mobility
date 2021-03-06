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
#include <QMainWindow>
#include <QItemDelegate>

QT_BEGIN_NAMESPACE
class QTableWidget;
class QLabel;
class QLineEdit;
class QTableWidget;
class QPushButton;
class QAbstractButton;
class QRadioButton;
QT_END_NAMESPACE

class EditorWidget;
class QCrmlGenerator : public QMainWindow
{
    Q_OBJECT
public:
    QCrmlGenerator();
private slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();

protected:
    void closeEvent(QCloseEvent *event);

private:
    bool safeToClear();
    EditorWidget *m_editorWidget;

    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *exitAction;

    QString m_saveFile;
};


//#define INCL_TYPE 1

#ifdef INCL_TYPE
class TypeDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    TypeDelegate(QTableWidget *parent);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
                            const QModelIndex &) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *,
                        const QModelIndex &index) const;
private:
    QTableWidget *m_parentTable;

};
#endif

class KeyIdDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    KeyIdDelegate(QTableWidget *parent);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
                            const QModelIndex &) const;
    void setModelData(QWidget *editor, QAbstractItemModel *,
                        const QModelIndex &index) const;
private:
    QTableWidget *m_parentTable;

};

class PathDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    PathDelegate(QTableWidget *parent);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &,
                            const QModelIndex &) const;
    void setEditorData(QWidget *editor, QAbstractItemModel *,
                        const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *,
                        const QModelIndex &index) const;
private:
    QTableWidget *m_parentTable;
};

class EditorWidget: public QWidget
{
    Q_OBJECT
public:
    enum Column{KeyId, Path, Type};
    EditorWidget();

public slots:
    void addRow();
    void removeRow();

    void initNew();
    void save(const QString &);
    void open(const QString &);

    bool isModified();
    bool verifyContents();

private slots:
    void setModified(bool b = true);
    void moveRowDown();
    void moveRowUp();
    void targetChanged(QAbstractButton *);

private:
    QLabel *m_repoLabel;
    QLineEdit *m_repoUID;//repository or category ID
    QTableWidget *m_tableWidget;
    QPushButton *m_addRowButton;
    QPushButton *m_removeRowButton;
    QRadioButton *m_CRepRadio;
    QRadioButton *m_RPropRadio;
#ifdef INCL_TYPE
    TypeDelegate *m_typeDelegate;
#endif
    KeyIdDelegate *m_keyIdDelegate;
    PathDelegate *m_pathDelegate;

    bool m_isModified;
};
