/*
 * Copyright (c) 2015 Nathan Osman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef QHTTPENGINE_QLOCATEHANDLER_H
#define QHTTPENGINE_QLOCATEHANDLER_H

#include <QFileInfoList>

#include "qhttpengine.h"
#include "qhttphandler.h"
#include "qiodevicecopier.h"
#include "qfilesystemhandler.h"

/**
 * @brief Handler for locate requests
 * @headerfile qlocatehandler.h QHttpEngine/QLocateHandler
 *
 * This handler responds to requests for resources on a locate database. The
 * constructor is provided with a path to the root directory, which will be
 * used to resolve all paths. The following example creates a handler that
 * serves files from the /var/www directory:
 *
 * @code
 * QLocateHandler handler("/var/www");
 * @endcode
 *
 * Requests for resources outside the root will be ignored. The document root
 * can be modified after initialization. It is possible to use a resource
 * directory for the document root.
 */
class QHTTPENGINE_EXPORT QLocateHandler : public QFilesystemHandler
{
//   Q_OBJECT

public:
    /**
     * @brief Create a new locate handler
     */
    explicit QLocateHandler(QObject *parent = 0);

    /**
     * @brief Create a new locate handler from the specified directory
     */
    QLocateHandler(const QString &documentRoot, QObject *parent = 0);

protected:

    /**
     * @brief Returns hits from slocate database
     */
	QFileInfoList retrHits(const QString &command);

    /**
     * @brief Reimplementation of QHttpHandler::process()
     */
    virtual void process(QHttpSocket *socket, const QString &path);

};

#endif // QHTTPENGINE_QLOCATEHANDLER_H
