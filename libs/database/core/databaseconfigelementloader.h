/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2009-06-27
 * Description : Database element configuration
 *
 * Copyright (C) 2009-2010 by Holger Foerster <hamsi2k at freenet dot de>
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

#include "databaseconfig.h"

// Qt includes

#include <QString>
#include <QMap>
#include <QDomElement>

// Local includes

#include "digikam_export.h"

namespace Digikam
{
class DatabaseConfig;

class DIGIKAM_EXPORT DatabaseConfigElementLoader
{
public:

    DatabaseConfigElementLoader(const QString& filepath, int xmlVersion);

    bool           readConfig(const QString& filepath, int xmlVersion);
    DatabaseConfig readDatabase(QDomElement& databaseElement);
    void           readDBActions(QDomElement& sqlStatementElements, DatabaseConfig& configElement);

public:

    bool                          isValid;
    QString                       errorMessage;
    QMap<QString, DatabaseConfig> databaseConfigs;
};

} // namespace Digikam
