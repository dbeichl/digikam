/* ============================================================
 * Authors: Gilles Caulier <caulier dot gilles at kdemail dot net>
 * Date   : 2006-21-12
 * Description : a embeded view to show the image preview widget.
 * 
 * Copyright 2006-2007 Gilles Caulier
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

// Qt includes.

#include <qstring.h>
#include <qvaluevector.h>
#include <qpainter.h>
#include <qpixmap.h>

// KDE includes.

#include <kdialogbase.h>
#include <klocale.h>
#include <kservice.h>
#include <krun.h>
#include <ktrader.h>
#include <kmimetype.h>
#include <kiconloader.h>

// Local includes.

#include "ddebug.h"
#include "albumdb.h"
#include "albummanager.h"
#include "albumsettings.h"
#include "imageinfo.h"
#include "dmetadata.h"
#include "dpopupmenu.h"
#include "tagspopupmenu.h"
#include "ratingpopupmenu.h"
#include "themeengine.h"
#include "imagepreviewview.h"
#include "imagepreviewview.moc"

namespace Digikam
{

class ImagePreviewViewPriv
{
public:

    ImagePreviewViewPriv()
    {
        hasPrev            = false;
        hasNext            = false;
        imagePreviewWidget = 0;
        imageInfo          = 0;
    }

    bool                hasPrev;
    bool                hasNext;

    ImageInfo          *imageInfo;

    ImagePreviewWidget *imagePreviewWidget;
};
    
ImagePreviewView::ImagePreviewView(QWidget *parent)
                : QVBox(parent)
{
    d = new ImagePreviewViewPriv;
    d->imagePreviewWidget = new ImagePreviewWidget(this);

    setFrameStyle(QFrame::GroupBoxPanel|QFrame::Plain); 
    setMargin(0); 
    setLineWidth(1); 

    // ----------------------------------------------------------------

    connect(ThemeEngine::instance(), SIGNAL(signalThemeChanged()),
            this, SLOT(slotThemeChanged()));  

    connect(d->imagePreviewWidget, SIGNAL( previewComplete() ),
            this, SIGNAL( previewLoadedSignal() ) );          
    
    connect(d->imagePreviewWidget, SIGNAL( previewFailed() ),
            this, SIGNAL( previewLoadedSignal() ) );    

    connect(d->imagePreviewWidget, SIGNAL(signalNextItem()),
            this, SIGNAL(signalNextItem()));

    connect(d->imagePreviewWidget, SIGNAL(signalPrevItem()),
            this, SIGNAL(signalPrevItem()));
}

ImagePreviewView::~ImagePreviewView()
{
    delete d;
}

void ImagePreviewView::slotThemeChanged()
{
    setPaletteBackgroundColor(ThemeEngine::instance()->baseColor());
}

void ImagePreviewView::setImageInfo(ImageInfo* info, bool hasPrev, bool hasNext)
{
    d->imageInfo = info;
    d->hasPrev   = hasPrev;
    d->hasNext   = hasNext;

    if (d->imageInfo)
        d->imagePreviewWidget->setImagePath(info->kurl().path()); 
    else 
        d->imagePreviewWidget->setImagePath(); 
}

ImageInfo* ImagePreviewView::getImageInfo()
{
    return d->imageInfo;
}

void ImagePreviewView::reload()
{
    d->imagePreviewWidget->reload();
}

void ImagePreviewView::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::RightButton)
    {
        RatingPopupMenu *ratingMenu     = 0;
        TagsPopupMenu   *assignTagsMenu = 0;
        TagsPopupMenu   *removeTagsMenu = 0;

        if (!d->imageInfo)
            return;

        // --------------------------------------------------------
    
        KURL url(d->imageInfo->kurl().path());
        KMimeType::Ptr mimePtr = KMimeType::findByURL(url, 0, true, true);
    
        QValueVector<KService::Ptr> serviceVector;
        KTrader::OfferList offers = KTrader::self()->query(mimePtr->name(), "Type == 'Application'");

        QPopupMenu openWithMenu;
    
        KTrader::OfferList::Iterator iter;
        KService::Ptr ptr;
        int index = 100;
    
        for( iter = offers.begin(); iter != offers.end(); ++iter )
        {
            ptr = *iter;
            openWithMenu.insertItem( ptr->pixmap(KIcon::Small), ptr->name(), index++);
            serviceVector.push_back(ptr);
        }

        // --------------------------------------------------------

        DPopupMenu popmenu(this);
        popmenu.insertItem(SmallIcon("back"), i18n("Back"), 10);
        if (!d->hasPrev) popmenu.setItemEnabled(10, false);

        popmenu.insertItem(SmallIcon("forward"), i18n("Forward"), 11);
        if (!d->hasNext) popmenu.setItemEnabled(11, false);
 
        popmenu.insertItem(SmallIcon("editimage"), i18n("Edit..."), 12);
        popmenu.insertItem(i18n("Open With"), &openWithMenu, 13);

        popmenu.insertSeparator();
        popmenu.insertItem(SmallIcon("edittrash"), i18n("Move to Trash"), 14);

        // Bulk assignment/removal of tags --------------------------

        Q_LLONG id = d->imageInfo->id();
        QValueList<Q_LLONG> idList;
        idList.append(id);

        assignTagsMenu = new TagsPopupMenu(idList, 1000, TagsPopupMenu::ASSIGN);
        removeTagsMenu = new TagsPopupMenu(idList, 2000, TagsPopupMenu::REMOVE);

        popmenu.insertSeparator();

        popmenu.insertItem(i18n("Assign Tag"), assignTagsMenu);
        int i = popmenu.insertItem(i18n("Remove Tag"), removeTagsMenu);

        connect(assignTagsMenu, SIGNAL(signalTagActivated(int)),
                this, SLOT(slotAssignTag(int)));

        connect(removeTagsMenu, SIGNAL(signalTagActivated(int)),
                this, SLOT(slotRemoveTag(int)));

        AlbumDB* db = AlbumManager::instance()->albumDB();
        if (!db->hasTags( idList ))
            popmenu.setItemEnabled(i, false);

        popmenu.insertSeparator();

        // Assign Star Rating -------------------------------------------
    
        ratingMenu = new RatingPopupMenu();
        
        connect(ratingMenu, SIGNAL(activated(int)),
                this, SLOT(slotAssignRating(int)));
    
        popmenu.insertItem(i18n("Assign Rating"), ratingMenu);

        // --------------------------------------------------------

        int idm = popmenu.exec(e->globalPos());
    
        switch(idm) 
        {
            case 10:     // Back
            {
                emit signalPrevItem();
                break;
            }

            case 11:     // Forward
            {
                emit signalNextItem();
                break;
            }

            case 12:     // Edit...
            {
                emit editImageSignal();
                break;
            }
  
            case 14:     // Move to trash
            {
                emit signalDeleteItem();
                break;
            }

            default:
                break;
        }

        // Open With...
        if (idm >= 100 && idm < 1000) 
        {
            KService::Ptr imageServicePtr = serviceVector[idm-100];
            KRun::run(*imageServicePtr, url);
        }
    
        serviceVector.clear();
        delete assignTagsMenu;
        delete removeTagsMenu;
        delete ratingMenu;
    }
}

void ImagePreviewView::slotAssignTag(int tagID)
{
    if (d->imageInfo)
    {
        QStringList oldKeywords = d->imageInfo->tagNames();

        d->imageInfo->setTag(tagID);

        // TODO MetadataHub: fix this part to use it instead.

        // Update Image Tags like Iptc keywords tags.

        if (AlbumSettings::instance())
        {
            if (AlbumSettings::instance()->getSaveIptcRating())
            {
                DMetadata metadata(d->imageInfo->filePath());
                metadata.setImageKeywords(oldKeywords, d->imageInfo->tagNames());
                metadata.applyChanges();
            }
        }
    }
}

void ImagePreviewView::slotRemoveTag(int tagID)
{
    if (d->imageInfo)
    {
        QStringList oldKeywords = d->imageInfo->tagNames();

        d->imageInfo->removeTag(tagID);

        // TODO MetadataHub: fix this part to use it instead.

        // Update Image Tags like Iptc keywords tags.

        if (AlbumSettings::instance())
        {
            if (AlbumSettings::instance()->getSaveIptcRating())
            {
                DMetadata metadata(d->imageInfo->filePath());
                metadata.setImageKeywords(oldKeywords, d->imageInfo->tagNames());
                metadata.applyChanges();
            }
        }
    }
}

void ImagePreviewView::slotAssignRating(int rating)
{
    rating = QMIN(5, QMAX(0, rating));
    
    if (d->imageInfo)
    {
        d->imageInfo->setRating(rating);

        // TODO MetadataHub: fix this part to use it instead.
        
        // Store Image rating as Iptc tag.
    
        if (AlbumSettings::instance())
        {
            if (AlbumSettings::instance()->getSaveIptcRating())
            {
                DMetadata metadata(d->imageInfo->filePath());
                metadata.setImageRating(rating);
                metadata.applyChanges();
            }
        }
    }
}

}  // NameSpace Digikam

