/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2008-05-19
 * Description : Fuzzy search sidebar tab contents.
 *
 * Copyright (C) 2008 by Gilles Caulier <caulier dot gilles at gmail dot com>
 * Copyright (C) 2008 by Marcel Wiesweg <marcel dot wiesweg at gmx dot de>
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

#ifndef FUZZYSEARCHVIEW_H
#define FUZZYSEARCHVIEW_H

// Qt includes.

#include <QWidget>

class QDragEnterEvent;
class QDropEvent;
class QPixmap;

namespace Digikam
{

class SAlbum;
class ImageInfo;
class SearchTextBar;
class LoadingDescription;
class FuzzySearchFolderView;
class FuzzySearchViewPriv;

class FuzzySearchView : public QWidget
{
    Q_OBJECT

public:

    FuzzySearchView(QWidget *parent=0);
    ~FuzzySearchView();

    FuzzySearchFolderView* folderView() const;
    SearchTextBar* searchBar() const;

    void setActive(bool val);
    void setImageInfo(const ImageInfo& info);

signals:

    void signalUpdateFingerPrints();

protected:

    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);

private:

    void setImageId(qlonglong imageid);

    void readConfig();
    void writeConfig();

    void createNewFuzzySearchAlbumFromSketch(const QString& name);
    void createNewFuzzySearchAlbumFromImage(const QString& name);

    bool checkName(QString& name);
    bool checkAlbum(const QString& name) const;

    void populateTreeView();

private slots:

    void slotTabChanged(int);

    void slotHSChanged(int h, int s);
    void slotVChanged();
    void slotClearSketch();
    void slotSaveSketchSAlbum();
    void slotCheckNameEditSketchConditions();

    void slotAlbumSelected(SAlbum*);
    void slotRenameAlbum(SAlbum*);

    void slotSaveImageSAlbum();
    void slotCheckNameEditImageConditions();
    void slotThumbnailLoaded(const LoadingDescription&, const QPixmap&);

    void slotFindDuplicates();

    void slotDirtySketch();
    void slotTimerSketchDone();
    void slotUndoRedoStateChanged(bool, bool);

    void slotLevelImageChanged();
    void slotTimerImageDone();

    void slotDuplicatesSearchTotalAmount(KJob*, KJob::Unit, qulonglong);
    void slotDuplicatesSearchProcessedAmount(KJob*, KJob::Unit, qulonglong);
    void slotDuplicatesSearchResult(KJob*);

private:

    FuzzySearchViewPriv *d;
};

}  // NameSpace Digikam

#endif /* FUZZYSEARCHVIEW_H */
