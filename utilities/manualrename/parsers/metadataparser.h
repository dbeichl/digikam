/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2009-08-08
 * Description : a metadata parser class
 *
 * Copyright (C) 2009 by Andi Clemens <andi dot clemens at gmx dot net>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef METADATAPARSER_H
#define METADATAPARSER_H

// Qt includes

#include <QString>
#include <QStringList>

// KDE includes

#include <kdialog.h>

// Local includes

#include "parser.h"

class KLineEdit;

namespace Digikam
{

class MetadataPanel;

namespace ManualRename
{

class MetadataParserDialog : public KDialog
{
    Q_OBJECT

public:

    MetadataParserDialog();
    ~MetadataParserDialog();

    QStringList checkedTags() const;
    QString     separator()   const;

private:

    MetadataPanel* m_metadataPanel;
    KLineEdit*     m_separatorLineEdit;
};

// --------------------------------------------------------

class MetadataParser : public Parser
{
    Q_OBJECT

public:

    MetadataParser();
    ~MetadataParser() {};

protected:

    virtual void parseOperation(QString& parseString, const ParseInformation& info);

private Q_SLOTS:

    void slotTokenTriggered(const QString& token);

private:

    QString parseMetadata(const QString& token, const ParseInformation& info);
};

} // namespace ManualRename
} // namespace Digikam

#endif /* METADATAPARSER_H */
