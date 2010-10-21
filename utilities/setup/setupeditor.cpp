/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2004-08-03
 * Description : setup Image Editor tab.
 *
 * Copyright (C) 2004-2010 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#include "setupeditor.moc"

// Qt includes

#include <QCheckBox>
#include <QColor>
#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>

// KDE includes

#include <kaboutdata.h>
#include <kapplication.h>
#include <kcolorbutton.h>
#include <kconfig.h>
#include <kdialog.h>
#include <kglobal.h>
#include <klocale.h>
#include <knuminput.h>
#include <kvbox.h>

// LibKDcraw includes

#include <libkdcraw/rnuminput.h>

using namespace KDcrawIface;

namespace Digikam
{

class SetupEditor::SetupEditorPriv
{
public:

    SetupEditorPriv() :
        configGroupName("ImageViewer Settings"),
        configUseThemeBackgroundColorEntry("UseThemeBackgroundColor"),
        configBackgroundColorEntry("BackgroundColor"),
        configFullScreenHideToolBarEntry("FullScreen Hide ToolBar"),
        configFullScreenHideThumbBarEntry("FullScreenHideThumbBar"),
        configUseRawImportToolEntry("UseRawImportTool"),
        configUnderExposureColorEntry("UnderExposureColor"),
        configOverExposureColorEntry("OverExposureColor"),
        configUnderExposurePercentsEntry("UnderExposurePercentsEntry"),
        configOverExposurePercentsEntry("OverExposurePercentsEntry"),
        configExpoIndicatorModeEntry("ExpoIndicatorMode"),

        themebackgroundColor(0),
        hideToolBar(0),
        hideThumbBar(0),
        useRawImportTool(0),
        expoIndicatorMode(0),
        colorBox(0),
        backgroundColor(0),
        underExposureColor(0),
        overExposureColor(0),
        underExposurePcents(0),
        overExposurePcents(0)
    {}

    const QString    configGroupName;
    const QString    configUseThemeBackgroundColorEntry;
    const QString    configBackgroundColorEntry;
    const QString    configFullScreenHideToolBarEntry;
    const QString    configFullScreenHideThumbBarEntry;
    const QString    configUseRawImportToolEntry;
    const QString    configUnderExposureColorEntry;
    const QString    configOverExposureColorEntry;
    const QString    configUnderExposurePercentsEntry;
    const QString    configOverExposurePercentsEntry;
    const QString    configExpoIndicatorModeEntry;

    QCheckBox*       themebackgroundColor;
    QCheckBox*       hideToolBar;
    QCheckBox*       hideThumbBar;
    QCheckBox*       useRawImportTool;
    QCheckBox*       expoIndicatorMode;

    KHBox*           colorBox;
    KColorButton*    backgroundColor;
    KColorButton*    underExposureColor;
    KColorButton*    overExposureColor;

    RDoubleNumInput* underExposurePcents;
    RDoubleNumInput* overExposurePcents;
};

SetupEditor::SetupEditor(QWidget* parent)
           : QScrollArea(parent), d(new SetupEditorPriv)
{
    QWidget* panel = new QWidget(viewport());
    setWidget(panel);
    setWidgetResizable(true);

    QVBoxLayout* layout = new QVBoxLayout(panel);

    // --------------------------------------------------------

    QGroupBox* interfaceOptionsGroup = new QGroupBox(i18n("Interface Options"), panel);
    QVBoxLayout* gLayout1            = new QVBoxLayout(interfaceOptionsGroup);

    d->themebackgroundColor          = new QCheckBox(i18n("&Use theme background color"),
                                                     interfaceOptionsGroup);

    d->themebackgroundColor->setWhatsThis( i18n("Enable this option to use the background theme "
                                                "color in the image editor area.") );

    d->colorBox                  = new KHBox(interfaceOptionsGroup);
    QLabel* backgroundColorlabel = new QLabel( i18n("&Background color:"), d->colorBox );
    d->backgroundColor           = new KColorButton(d->colorBox);
    backgroundColorlabel->setBuddy(d->backgroundColor);
    d->backgroundColor->setWhatsThis( i18n("Customize the background color to use "
                                           "in the image editor area.") );

    d->hideToolBar      = new QCheckBox(i18n("H&ide toolbar in fullscreen mode"), interfaceOptionsGroup);
    d->hideThumbBar     = new QCheckBox(i18n("Hide &thumbbar in fullscreen mode"), interfaceOptionsGroup);

    d->useRawImportTool = new QCheckBox(i18n("Use Raw Import Tool to handle Raw images"), interfaceOptionsGroup);
    d->useRawImportTool->setWhatsThis(i18n("Set this option to use the Raw Import "
                                           "tool to load RAW images. "
                                           "With this tool you are able to customize indeep decoding settings."));

    gLayout1->addWidget(d->themebackgroundColor);
    gLayout1->addWidget(d->colorBox);
    gLayout1->addWidget(d->hideToolBar);
    gLayout1->addWidget(d->hideThumbBar);
    gLayout1->addWidget(d->useRawImportTool);
    gLayout1->setMargin(KDialog::spacingHint());
    gLayout1->setSpacing(KDialog::spacingHint());

    // --------------------------------------------------------

    QGroupBox* exposureOptionsGroup = new QGroupBox(i18n("Exposure Indicators"), panel);
    QVBoxLayout* gLayout2           = new QVBoxLayout(exposureOptionsGroup);

    KHBox* underExpoBox         = new KHBox(exposureOptionsGroup);
    QLabel* underExpoColorlabel = new QLabel( i18n("&Under-exposure color:"), underExpoBox);
    d->underExposureColor       = new KColorButton(underExpoBox);
    underExpoColorlabel->setBuddy(d->underExposureColor);
    d->underExposureColor->setWhatsThis( i18n("Customize color used in image editor to identify "
                                              "under-exposed pixels.") );

    KHBox* underPcentBox        = new KHBox(exposureOptionsGroup);
    QLabel* underExpoPcentlabel = new QLabel( i18n("Under-exposure percents:"), underPcentBox);
    d->underExposurePcents      = new RDoubleNumInput(underPcentBox);
    d->underExposurePcents->setDecimals(1);
    d->underExposurePcents->input()->setRange(0.1, 5.0, 0.1, true);
    d->underExposurePcents->setDefaultValue(1.0);
    underExpoPcentlabel->setBuddy(d->underExposurePcents);
    d->underExposurePcents->setWhatsThis( i18n("Adjust the percents of the bottom of image histogram "
                                               "which will be used to check under exposed pixels.") );

    KHBox* overExpoBox         = new KHBox(exposureOptionsGroup);
    QLabel* overExpoColorlabel = new QLabel( i18n("&Over-exposure color:"), overExpoBox);
    d->overExposureColor       = new KColorButton(overExpoBox);
    overExpoColorlabel->setBuddy(d->overExposureColor);
    d->overExposureColor->setWhatsThis( i18n("Customize color used in image editor to identify "
                                             "over-exposed pixels.") );

    KHBox* overPcentBox        = new KHBox(exposureOptionsGroup);
    QLabel* overExpoPcentlabel = new QLabel( i18n("Over-exposure percents:"), overPcentBox);
    d->overExposurePcents      = new RDoubleNumInput(overPcentBox);
    d->overExposurePcents->setDecimals(1);
    d->overExposurePcents->input()->setRange(0.1, 5.0, 0.1, true);
    d->overExposurePcents->setDefaultValue(1.0);
    overExpoPcentlabel->setBuddy(d->underExposurePcents);
    d->overExposurePcents->setWhatsThis( i18n("Adjust the percents of the top of image histogram "
                                              "which will be used to check over exposed pixels.") );

    d->expoIndicatorMode       = new QCheckBox(i18n("Indicate exposure as pure color"), exposureOptionsGroup);
    d->overExposureColor->setWhatsThis( i18n("If this option is enabled, over and under exposure indicators will be displayed "
                                             "only when pure white and pure black color matches, as all color components match "
                                             "the condition in the same time. "
                                             "Else indicators are turn on when one of color components match the condition.") );

    gLayout2->addWidget(underExpoBox);
    gLayout2->addWidget(underPcentBox);
    gLayout2->addWidget(overExpoBox);
    gLayout2->addWidget(overPcentBox);
    gLayout2->addWidget(d->expoIndicatorMode);
    gLayout2->setMargin(KDialog::spacingHint());
    gLayout2->setSpacing(KDialog::spacingHint());

    // --------------------------------------------------------

    layout->addWidget(interfaceOptionsGroup);
    layout->addWidget(exposureOptionsGroup);
    layout->addStretch();
    layout->setMargin(0);
    layout->setSpacing(KDialog::spacingHint());

    // --------------------------------------------------------

    connect(d->themebackgroundColor, SIGNAL(toggled(bool)),
            this, SLOT(slotThemeBackgroundColor(bool)));

    readSettings();

    // --------------------------------------------------------

    setAutoFillBackground(false);
    viewport()->setAutoFillBackground(false);
    panel->setAutoFillBackground(false);
}

SetupEditor::~SetupEditor()
{
    delete d;
}

void SetupEditor::slotThemeBackgroundColor(bool e)
{
    d->colorBox->setEnabled(!e);
}

void SetupEditor::readSettings()
{
    KSharedConfig::Ptr config = KGlobal::config();
    KConfigGroup group        = config->group(d->configGroupName);
    QColor Black(Qt::black);
    QColor White(Qt::white);
    d->themebackgroundColor->setChecked(group.readEntry(d->configUseThemeBackgroundColorEntry, true));
    d->backgroundColor->setColor(group.readEntry(d->configBackgroundColorEntry,                Black));
    d->hideToolBar->setChecked(group.readEntry(d->configFullScreenHideToolBarEntry,            false));
    d->hideThumbBar->setChecked(group.readEntry(d->configFullScreenHideThumbBarEntry,          true));
    d->underExposureColor->setColor(group.readEntry(d->configUnderExposureColorEntry,          White));
    d->overExposureColor->setColor(group.readEntry(d->configOverExposureColorEntry,            Black));
    d->useRawImportTool->setChecked(group.readEntry(d->configUseRawImportToolEntry,            false));
    d->expoIndicatorMode->setChecked(group.readEntry(d->configExpoIndicatorModeEntry,          true));
    d->underExposurePcents->setValue(group.readEntry(d->configUnderExposurePercentsEntry,      1.0));
    d->overExposurePcents->setValue(group.readEntry(d->configOverExposurePercentsEntry,        1.0));
}

void SetupEditor::applySettings()
{
    KSharedConfig::Ptr config = KGlobal::config();
    KConfigGroup group        = config->group(d->configGroupName);
    group.writeEntry(d->configUseThemeBackgroundColorEntry, d->themebackgroundColor->isChecked());
    group.writeEntry(d->configBackgroundColorEntry,         d->backgroundColor->color());
    group.writeEntry(d->configFullScreenHideToolBarEntry,   d->hideToolBar->isChecked());
    group.writeEntry(d->configFullScreenHideThumbBarEntry,  d->hideThumbBar->isChecked());
    group.writeEntry(d->configUnderExposureColorEntry,      d->underExposureColor->color());
    group.writeEntry(d->configOverExposureColorEntry,       d->overExposureColor->color());
    group.writeEntry(d->configUseRawImportToolEntry,        d->useRawImportTool->isChecked());
    group.writeEntry(d->configExpoIndicatorModeEntry,       d->expoIndicatorMode->isChecked());
    group.writeEntry(d->configUnderExposurePercentsEntry,   d->underExposurePcents->value());
    group.writeEntry(d->configOverExposurePercentsEntry,    d->overExposurePcents->value());
    group.sync();
}

}  // namespace Digikam
