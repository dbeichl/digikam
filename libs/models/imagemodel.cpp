/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2009-03-05
 * Description : Qt item model for database entries
 *
 * Copyright (C) 2009 by Marcel Wiesweg <marcel dot wiesweg at gmx dot de>
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

#include "imagemodel.h"
#include "imagemodel.moc"

// Qt includes

#include <QSet>

// KDE includes

#include <kdebug.h>

// Local includes

#include "imageinfo.h"
#include "imageinfolist.h"

namespace Digikam
{

class ImageModelPriv
{
public:

    ImageModelPriv()
    {
        preprocessor      = 0;
        keepFilePathCache = false;
    }

    ImageInfoList   infos;
    QSet<qlonglong> ids;

    bool            keepFilePathCache;
    QHash<QString, int> filePathHash;

    QObject        *preprocessor;
};

ImageModel::ImageModel(QObject *parent)
          : QAbstractListModel(parent),
            d(new ImageModelPriv)
{
}

ImageModel::~ImageModel()
{
    delete d;
}

void ImageModel::setKeepsFilePathCache(bool keepCache)
{
    d->keepFilePathCache = keepCache;
}

bool ImageModel::keepsFilePathCache() const
{
    return d->keepFilePathCache;
}

bool ImageModel::isEmpty() const
{
    return d->infos.isEmpty();
}

ImageInfo ImageModel::imageInfo(const QModelIndex &index) const
{
    return d->infos[index.row()];
}

ImageInfo &ImageModel::imageInfoRef(const QModelIndex &index) const
{
    return d->infos[index.row()];
}

qlonglong ImageModel::imageId(const QModelIndex &index) const
{
    return d->infos[index.row()].id();
}

ImageInfo ImageModel::imageInfo(int row) const
{
    if (row >= d->infos.size())
        return ImageInfo();
    return d->infos[row];
}

ImageInfo &ImageModel::imageInfoRef(int row) const
{
    return d->infos[row];
}

qlonglong ImageModel::imageId(int row) const
{
    if (row >= d->infos.size())
        return -1;
    return d->infos[row].id();
}

// static method
ImageInfo ImageModel::retrieveImageInfo(const QModelIndex &index)
{
    ImageModel *model = index.data(ImageModelPointerRole).value<ImageModel*>();
    Q_ASSERT(model);
    int row = index.data(ImageModelInternalId).toInt();
    return model->imageInfo(row);
}

QModelIndex ImageModel::indexForPath(const QString &filePath) const
{
    if (d->keepFilePathCache)
    {
        int index = d->filePathHash.value(filePath, -1);
        if (index != -1)
            return createIndex(index, 0);
    }
    else
    {
        const int size = d->infos.size();
        for (int i=0; i<size; i++)
            if (d->infos[i].filePath() == filePath)
                return createIndex(i, 0);
    }
    return QModelIndex();
}

ImageInfo ImageModel::imageInfo(const QString &filePath) const
{
    if (d->keepFilePathCache)
    {
        int index = d->filePathHash.value(filePath, -1);
        if (index != -1)
            return d->infos[index];
    }
    else
    {
        foreach (const ImageInfo &info, d->infos)
            if (info.filePath() == filePath)
                return info;
    }
    return ImageInfo();
}

void ImageModel::addImageInfos(const QList<ImageInfo> &infos)
{
    if (infos.isEmpty())
        return;

    if (d->preprocessor)
        emit preprocess(infos);
    else
        appendInfos(infos);
}

void ImageModel::clearImageInfos()
{
    reset();
    d->infos.clear();
    d->ids.clear();
    d->filePathHash.clear();
    imageInfosCleared();
}

QList<ImageInfo> ImageModel::imageInfos() const
{
    return d->infos;
}

QSet<qlonglong> ImageModel::imageIds() const
{
    return d->ids;
}

bool ImageModel::hasImage(qlonglong id) const
{
    return d->ids.contains(id);
}

bool ImageModel::hasImage(const ImageInfo &info) const
{
    return d->ids.contains(info.id());
}

void ImageModel::setPreprocessor(QObject *preprocessor)
{
    unsetPreprocessor(d->preprocessor);
    d->preprocessor = preprocessor;
}

void ImageModel::unsetPreprocessor(QObject *preprocessor)
{
    if (preprocessor && d->preprocessor == preprocessor)
    {
        disconnect(this, SIGNAL(preprocess(const QList<ImageInfo> &)), 0, 0);
        disconnect(d->preprocessor, 0, this, SLOT(appendInfos(const QList<ImageInfo> &)));
    }
}

void ImageModel::reAddImageInfos(const QList<ImageInfo> &infos)
{
    appendInfos(infos);
}

void ImageModel::appendInfos(const QList<ImageInfo> &infos)
{
    emit imageInfosAboutToBeAdded(infos);
    int firstNewIndex = d->infos.size();
    int lastNewIndex = d->infos.size() + infos.size() - 1;
    beginInsertRows(QModelIndex(), firstNewIndex, lastNewIndex);
    d->infos << infos;
    foreach (const ImageInfo &info, infos)
        d->ids << info.id();
    if (d->keepFilePathCache)
    {
        for (int i=firstNewIndex; i<=lastNewIndex; i++)
            d->filePathHash[d->infos[i].filePath()] = i;
    }
    endInsertRows();
    emit imageInfosAdded(infos);
}


QVariant ImageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
        case Qt::DisplayRole:
        case Qt::ToolTipRole:
            return d->infos[index.row()].name();
        case ImageModelPointerRole:
            return QVariant::fromValue(const_cast<ImageModel*>(this));
        case ImageModelInternalId:
            return index.row();
    }
    return QVariant();
}

QVariant ImageModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section)
    Q_UNUSED(orientation)
    Q_UNUSED(role)
    return QVariant();
}

int ImageModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return d->infos.size();
}

Qt::ItemFlags ImageModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsSelectable;
}

QModelIndex ImageModel::index(int row, int column, const QModelIndex &parent) const
{
    if (column != 0 || parent.isValid() || row >= d->infos.size())
        return QModelIndex();

    return createIndex(row, 0);
}

/*
Qt::DropActions ImageModel::supportedDropActions() const
{
    //TODO
    return QAbstractItemModel::supportedDropActions();
}

QStringList ImageModel::mimeTypes() const
{
    //TODO
    return QAbstractItemModel::mimeTypes();
}

bool ImageModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    //TODO
    return QAbstractItemModel::dropMimeData(data, action, row, column, parent);
}

QMimeData *ImageModel::mimeData(const QModelIndexList &indexes) const
{
    //TODO
    return QAbstractItemModel::mimeData(indexes);
}
*/

} // namespace Digikam
