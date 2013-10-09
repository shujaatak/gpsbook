/* Copyright (c) 2009 Hermann Kraus
 * This software is available under a "MIT Style" license
 * (see COPYING).
 * QuaZip usage by X@v
 */
/** \file
    Read SRTM zip files. */
#include "zip.h"

#include <QFileInfo>
#include <QDebug>
#include <quazip/quazip.h>
#include <quazip/quazipfile.h>
#include <math.h>
#include <QIODevice>


/** Creates a buffer of all the data in the SRTM file. The uncompressed version is used if available.
  *
  * \returns The length of the side of the data (e.g. 1201 or 3601)
  * \note The buffer returned is owned by the caller of this function and _must_ be freed after usage.
  */
int SrtmZipFile::getData(QString filename, qint16 **buffer)
{
    qint64 size = 0;

    QuaZip zip(filename);
    zip.open(QuaZip::mdUnzip);

    QuaZipFile fz(&zip);

    for(bool f=zip.goToFirstFile(); f; f=zip.goToNextFile()) {

        fz.open(QIODevice::ReadOnly);

        size = sqrt(fz.size()/2);
        if (size*size*2 != fz.size()) {
            qCritical() << "ZIP(Uncompressed): Invalid data: Not a square!";
        }

        *buffer = new qint16[fz.size()/2];
        if (!*buffer) {
            qCritical() << "ZIP(Uncompressed): Could not allocate buffer.";
            return 0;
        }
        qint64 read = fz.read((char *)*buffer,fz.size());

        if ( read != fz.size()) {
            qCritical() << "ZIP(Uncompressed): Could not read all bytes.";
        }


        fz.close();
    }

    zip.close();


    return size;
}
