/* ============================================================
 * File  : syncjob.h
 * Author: Renchi Raju <renchi@pooh.tam.uiuc.edu>
 * Date  : 2004-10-04
 * Description : 
 * 
 * Copyright 2004 by Renchi Raju
 *
 * Concept copied from kdelibs/kio/kio/netaccess.h/cpp
 *   This file is part of the KDE libraries
 *   Copyright (C) 1997 Torben Weis (weis@kde.org)
 *   Copyright (C) 1998 Matthias Ettrich (ettrich@kde.org)
 *   Copyright (C) 1999 David Faure (faure@kde.org)
 * 
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published bythe Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * ============================================================ */

#ifndef SYNCJOB_H
#define SYNCJOB_H

#include <qobject.h>
#include <kurl.h>

namespace KIO
{
class Job;
}

class SyncJob : public QObject
{
    Q_OBJECT

public:

    /* this will either trash or delete the urls depending
       on the user settings */
    static bool userDelete(const KURL::List& urls);

    /* this will delete the urls. use only userDelete unless
       there is a specific need for this */
    static bool del(const KURL::List& urls);
    
    /* moves the urls to trash. use only userDelete unless
       there is a specific need for this */
    static bool trash(const KURL::List& urls);

    static QString lastErrorMsg();
    static int     lastErrorCode();
    
private:

    SyncJob();
    ~SyncJob();

    bool delPriv(const KURL::List& urls);
    bool trashPriv(const KURL::List& urls);

    void enter_loop();
    
    static int      lastErrorCode_;
    static QString* lastErrorMsg_;
    bool            success_;

private slots:

    void slotResult( KIO::Job * job );
    
};

#endif /* SYNCJOB_H */
