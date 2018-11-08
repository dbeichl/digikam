/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2016-08-14
 * Description : An unit test to read or write metadata through multi-core threads.
 *
 * Copyright (C) 2016-2018 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef DIGIKAM_META_READER_THREAD_TEST_H
#define DIGIKAM_META_READER_THREAD_TEST_H

// Qt includes

#include <QList>
#include <QUrl>
#include <QMap>

// Local includes

#include "abstractunittest.h"
#include "metaenginesettingscontainer.h"
#include "dmetadatasettingscontainer.h"
#include "actionthreadbase.h"

using namespace Digikam;


class MetaReaderThread : public ActionThreadBase
{
    Q_OBJECT

public:

    enum Direction
    {
        NOT_DEFINED    = -1,
        READ_FROM_FILE = 0,
        WRITE_TO_FILE  = 1
    };

public:

    explicit MetaReaderThread(QObject* const parent);
    ~MetaReaderThread();

    void readMetadata(const QList<QUrl>& list,
                      Direction direction,
                      const MetaEngineSettingsContainer& settings);

    QString stats(const QStringList& mimeTypes);

    static QString directionToString(Direction direction);

protected Q_SLOTS:

    void slotJobFinished();
    void slotStats(const QString& mt, bool p);

Q_SIGNALS:

    void done();

private:

    QMap<QString, bool> m_stats;     // Map of type-mime, and file processed.
};

// -------------------------------------------------------------------------

class Mytask : public ActionJob
{
    Q_OBJECT

public:

    Mytask();

public:

    QUrl                        url;
    MetaReaderThread::Direction direction;
    MetaEngineSettingsContainer settings;

protected:

    void run();

Q_SIGNALS:

    void signalStats(const QString&, bool);
};

// -------------------------------------------------------------------------

class MetaReaderThreadTest : public AbstractUnitTest
{
    Q_OBJECT

private:

    void runMetaReader(const QString& path,
                       const QStringList& mimeTypes,
                       MetaReaderThread::Direction direction,
                       const MetaEngineSettingsContainer& settings,
                       int threadsToUse);

private Q_SLOTS:

    void testMetaReaderThread();
};

#endif // DIGIKAM_META_READER_THREAD_TEST_H