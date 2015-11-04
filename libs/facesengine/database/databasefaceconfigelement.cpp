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

#include "databasefaceconfigelement.h"

// Qt includes

#include <QtGlobal>
#include <QStandardPaths>

// Local includes

#include "digikam_dbconfig.h"
#include "databaseconfigelementloader.h"

using namespace Digikam;

namespace FacesEngine
{

Q_GLOBAL_STATIC_WITH_ARGS(DatabaseConfigElementLoader,
                          loader,
                          (QStandardPaths::locate(QStandardPaths::GenericDataLocation, QLatin1String("digikam/facesengine/dbfaceconfig.xml")),
                           dbfaceconfig_xml_version)
                         )

DatabaseConfig DatabaseFaceConfigElement::element(const QString& databaseType)
{
    // Unprotected read-only access? Usually accessed under DatabaseAccess protection anyway
    return loader()->databaseConfigs.value(databaseType);
}

bool DatabaseFaceConfigElement::checkReadyForUse()
{
    return loader()->isValid;
}

QString DatabaseFaceConfigElement::errorMessage()
{
    return loader()->errorMessage;
}

} // namespace FacesEngine
