/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QTMEDIANAMESPACE_H
#define QTMEDIANAMESPACE_H

#include <QtCore/qpair.h>
#include <QtCore/qmetatype.h>

QTM_BEGIN_NAMESPACE

namespace QtMedia
{
    enum MetaData
    {
        // Common
        Title,
        SubTitle,
        Author,
        Comment,
        Description,
        Category,
        Genre,
        Year,
        Date,
        UserRating,
        Keywords,
        Language,
        Publisher,
        Copyright,
        ParentalRating,
        RatingOrganisation,

        // Media
        Size,
        MediaType,
        Duration,

        // Audio
        AudioBitrate,
        AudioCodec,
        AverageLevel,
        Channels,
        PeakValue,
        Frequency,

        // Music
        AlbumTitle,
        AlbumArtist,
        ContributingArtist,
        Composer,
        Conductor,
        Lyrics,
        Mood,
        TrackNumber,
        TrackCount,

        CoverArtUriSmall,
        CoverArtUriLarge,

        // Image/Video
        Resolution,
        PixelAspectRatio,

        // Video
        VideoFrameRate,
        VideoBitRate,
        VideoCodec,

        PosterUri,

        // Movie
        ChapterNumber,
        Director,
        LeadPerformer,
        Writer,

        // Photos
        CameraManufacturer,
        CameraModel,
        Event,
        Subject,
        Orientation,
        ExposureTime,
        FNumber,
        ExposureProgram,
        ISOSpeedRatings,
        ExposureBiasValue,
        DateTimeOriginal,
        DateTimeDigitized,
        SubjectDistance,
        MeteringMode,
        LightSource,
        Flash,
        FocalLength,
        ExposureMode,
        WhiteBalance,
        DigitalZoomRatio,
        FocalLengthIn35mmFilm,
        SceneCaptureType,
        GainControl,
        Contrast,
        Saturation,
        Sharpness,
        DeviceSettingDescription
    };

    enum SupportEstimate
    {
        NotSupported,
        MaybeSupported,
        ProbablySupported,
        PreferedService
    };

    enum EncodingQuality
    {
        VeryLowQuality,
        LowQuality,
        NormalQuality,
        HighQuality,
        VeryHighQuality
    };

    enum EncodingMode
    {
        ConstantQualityEncoding,
        ConstantBitrateEncoding,
        AverageBitrateEncoding,
        TwoPassEncoding
    };

}

QTM_END_NAMESPACE

#endif