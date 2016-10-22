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

#include <QFile>
#include <QStringList>
#include <QHttpHandler>
#include "qhttphandler_p.h"

QHttpHandlerPrivate::QHttpHandlerPrivate(QHttpHandler *handler) : QObject(handler), q(handler)
{
}

QHttpHandler::QHttpHandler(QObject *parent) : QObject(parent), d(new QHttpHandlerPrivate(this))
{
}

void QHttpHandler::addRedirect(const QRegExp &pattern, const QString &path)
{
	d->redirects.append(Redirect(pattern, path));
}

void QHttpHandler::addSubHandler(const QRegExp &pattern, QHttpHandler *handler)
{
	d->subHandlers.append(SubHandler(pattern, handler));
}

void QHttpHandler::setMaximumGetBytes(qint64 nbytes)
{
	d->maximumGetAllowed = nbytes;
}

void QHttpHandler::setMaximumPutBytes(qint64 nbytes)
{
	d->maximumPutAllowed = nbytes;
}

void QHttpHandler::route(QHttpSocket *socket, const QString &url)
{
	QString path( url );

	qDebug() << "route: PATH " << path;

	// Check each of the redirects for a match
	foreach(Redirect redirect, d->redirects) {

		int idx = redirect.first.indexIn( path );

		qDebug() << "route: regexp '" << redirect.first.pattern() << "'";
		qDebug() << "route: pattern '" << redirect.second << "'";
		qDebug() << "route: PATH '" << path << "' IDX=" << idx;

		if (idx != -1) {
			QString newPath( redirect.second );
			qDebug() << "route: found '" << newPath << "'";
			QStringList redirects = redirect.first.capturedTexts();

			//foreach(QString replacement, redirect.first.capturedTexts().mid(1)) {
			foreach(QString replacement, redirects) {
				QString newpath( path );
				newPath = newpath.replace( replacement, newPath );
				//newPath = newPath.arg( replacement );
				qDebug() << "route: REPL " << replacement << newPath;
			}
			socket->writeRedirect(newPath.toUtf8());
			qDebug() << "-------------------------------------------";
			return;
		}
		qDebug() << "-------------------------------------------";
	}

	// Check each of the sub-handlers for a match
	foreach(SubHandler subHandler, d->subHandlers) {
		if(subHandler.first.indexIn(path) != -1) {
			subHandler.second->route(socket, path.mid(subHandler.first.matchedLength()));
			return;
		}
	}
	qDebug() << "route: process PATH " << path;
	// If no match, invoke the process() method
	process( socket, path );
}

void QHttpHandler::process(QHttpSocket *socket, const QString &path)
{
	// The default response is simply a 404 error
	socket->writeError(QHttpSocket::NotFound);
}

#include "../include/moc_qhttphandler_p.cpp"
#include "../include/moc_qhttphandler.cpp"
