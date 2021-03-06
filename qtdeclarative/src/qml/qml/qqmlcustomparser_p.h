/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtQml module of the Qt Toolkit.
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

#ifndef QQMLCUSTOMPARSER_H
#define QQMLCUSTOMPARSER_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qqmlmetatype_p.h"
#include "qqmlerror.h"
#include "qqmlscript_p.h"
#include "qqmlbinding_p.h"

#include <QtCore/qbytearray.h>
#include <QtCore/qxmlstream.h>

QT_BEGIN_NAMESPACE


class QQmlCompiler;

class QQmlCustomParserPropertyPrivate;
class Q_QML_PRIVATE_EXPORT QQmlCustomParserProperty
{
public:
    QQmlCustomParserProperty();
    QQmlCustomParserProperty(const QQmlCustomParserProperty &);
    QQmlCustomParserProperty &operator=(const QQmlCustomParserProperty &);
    ~QQmlCustomParserProperty();

    QString name() const;
    QQmlScript::Location location() const;

    bool isList() const;
    // Will be one of QQmlScript::Variant, QQmlCustomParserProperty or 
    // QQmlCustomParserNode
    QList<QVariant> assignedValues() const;

private:
    friend class QQmlCustomParserNodePrivate;
    friend class QQmlCustomParserPropertyPrivate;
    QQmlCustomParserPropertyPrivate *d;
};

class QQmlCustomParserNodePrivate;
class Q_QML_PRIVATE_EXPORT QQmlCustomParserNode
{
public:
    QQmlCustomParserNode();
    QQmlCustomParserNode(const QQmlCustomParserNode &);
    QQmlCustomParserNode &operator=(const QQmlCustomParserNode &);
    ~QQmlCustomParserNode();

    QString name() const;
    QQmlScript::Location location() const;

    QList<QQmlCustomParserProperty> properties() const;

private:
    friend class QQmlCustomParserNodePrivate;
    QQmlCustomParserNodePrivate *d;
};

class Q_QML_PRIVATE_EXPORT QQmlCustomParser
{
public:
    enum Flag {
        NoFlag                    = 0x00000000,
        AcceptsAttachedProperties = 0x00000001,
        AcceptsSignalHandlers     = 0x00000002
    };
    Q_DECLARE_FLAGS(Flags, Flag)

    QQmlCustomParser() : compiler(0), object(0), m_flags(NoFlag) {}
    QQmlCustomParser(Flags f) : compiler(0), object(0), m_flags(f) {}
    virtual ~QQmlCustomParser() {}

    void clearErrors();
    Flags flags() const { return m_flags; }

    virtual QByteArray compile(const QList<QQmlCustomParserProperty> &)=0;
    virtual void setCustomData(QObject *, const QByteArray &)=0;

    QList<QQmlError> errors() const { return exceptions; }

protected:
    void error(const QString& description);
    void error(const QQmlCustomParserProperty&, const QString& description);
    void error(const QQmlCustomParserNode&, const QString& description);

    int evaluateEnum(const QByteArray&, bool *ok) const;

    const QMetaObject *resolveType(const QString&) const;

    QQmlBinding::Identifier rewriteBinding(const QQmlScript::Variant&, const QString&);
    QString rewriteSignalHandler(const QQmlScript::Variant&, const QString&);

private:
    QList<QQmlError> exceptions;
    QQmlCompiler *compiler;
    QQmlScript::Object *object;
    Flags m_flags;
    friend class QQmlCompiler;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QQmlCustomParser::Flags)

#if 0
#define QML_REGISTER_CUSTOM_TYPE(URI, VERSION_MAJ, VERSION_MIN, NAME, TYPE, CUSTOMTYPE) \
            qmlRegisterCustomType<TYPE>(#URI, VERSION_MAJ, VERSION_MIN, #NAME, #TYPE, new CUSTOMTYPE)
#endif

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QQmlCustomParserProperty)
Q_DECLARE_METATYPE(QQmlCustomParserNode)

#endif
