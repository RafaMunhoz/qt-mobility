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
// Copyright (c) 2008 Roberto Raggi <roberto.raggi@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "Token.h"
#include "Literals.h"

using namespace CPlusPlus;

static const char *token_names[] = {
    (""), ("<error>"),

    ("<C++ comment>"), ("<C++ doxy comment>"),
    ("<comment>"), ("<doxy comment>"),

    ("<identifier>"), ("<numeric literal>"), ("<char literal>"),
    ("<wide char literal>"), ("<string literal>"), ("<wide char literal>"),
    ("<@string literal>"), ("<angle string literal>"),

    ("&"), ("&&"), ("&="), ("->"), ("->*"), ("^"), ("^="), (":"), ("::"),
    (","), ("/"), ("/="), ("."), ("..."), (".*"), ("="), ("=="), ("!"),
    ("!="), (">"), (">="), (">>"), (">>="), ("{"), ("["), ("<"), ("<="),
    ("<<"), ("<<="), ("("), ("-"), ("-="), ("--"), ("%"), ("%="), ("|"),
    ("|="), ("||"), ("+"), ("+="), ("++"), ("#"), ("##"), ("?"), ("}"),
    ("]"), (")"), (";"), ("*"), ("*="), ("~"), ("~="),

    ("asm"), ("auto"), ("bool"), ("break"), ("case"), ("catch"), ("char"),
    ("class"), ("const"), ("const_cast"), ("continue"), ("default"),
    ("delete"), ("do"), ("double"), ("dynamic_cast"), ("else"), ("enum"),
    ("explicit"), ("export"), ("extern"), ("false"), ("float"), ("for"),
    ("friend"), ("goto"), ("if"), ("inline"), ("int"), ("long"),
    ("mutable"), ("namespace"), ("new"), ("operator"), ("private"),
    ("protected"), ("public"), ("register"), ("reinterpret_cast"),
    ("return"), ("short"), ("signed"), ("sizeof"), ("static"),
    ("static_cast"), ("struct"), ("switch"), ("template"), ("this"),
    ("throw"), ("true"), ("try"), ("typedef"), ("typeid"), ("typename"),
    ("union"), ("unsigned"), ("using"), ("virtual"), ("void"),
    ("volatile"), ("wchar_t"), ("while"),

    // gnu
    ("__attribute__"), ("__typeof__"),

    // objc @keywords
    ("@catch"), ("@class"), ("@compatibility_alias"), ("@defs"), ("@dynamic"),
    ("@encode"), ("@end"), ("@finally"), ("@implementation"), ("@interface"),
    ("@not_keyword"), ("@optional"), ("@package"), ("@private"), ("@property"),
    ("@protected"), ("@protocol"), ("@public"), ("@required"), ("@selector"),
    ("@synchronized"), ("@synthesize"), ("@throw"), ("@try"),

    ("SIGNAL"), ("SLOT"), ("Q_SIGNAL"), ("Q_SLOT"), ("signals"), ("slots"),
    ("Q_FOREACH"), ("Q_D"), ("Q_Q"),
#ifdef ICHECK_BUILD
    ("Q_INVOKABLE"), ("Q_PROPERTY"), ("Q_ENUMS"), ("Q_FLAGS"), ("Q_DECLARE_FLAGS")
#endif
};

Token::Token() :
    flags(0), offset(0), ptr(0)
{
}

Token::~Token()
{
}

void Token::reset()
{
    flags = 0;
    offset = 0;
    ptr = 0;
}

const char *Token::name(int kind)
{ return token_names[kind]; }

const char *Token::spell() const
{
    switch (f.kind) {
    case T_IDENTIFIER:
        return identifier->chars();

    case T_NUMERIC_LITERAL:
    case T_CHAR_LITERAL:
    case T_STRING_LITERAL:
    case T_AT_STRING_LITERAL:
    case T_ANGLE_STRING_LITERAL:
    case T_WIDE_CHAR_LITERAL:
    case T_WIDE_STRING_LITERAL:
        return literal->chars();

    default:
        return token_names[f.kind];
    } // switch
}


