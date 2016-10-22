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

#include <stdio.h>

#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QByteArray>
#include <QProcess>

#include "qiodevicecopier.h"
#include "qfilesystemhandler_p.h"
#include "qlocatehandler.h"

QLocateHandler::QLocateHandler(QObject *parent) : QFilesystemHandler(parent)
{
	setDocumentRoot( QString("/var/lib/slocate/") );
}

QLocateHandler::QLocateHandler(const QString &documentRoot, QObject *parent)
	: QFilesystemHandler(documentRoot, parent)
{
	//setDocumentRoot(documentRoot);
}

QFileInfoList QLocateHandler::retrHits(const QString &command)
{
	std::string cmd( command.toStdString() );
	QFileInfoList flist;

	// Found that the following code works better than
	// the Qt version, using QProcess, below.
	//
	::FILE *proc = ::popen( cmd.c_str(), "r" );

	if ( proc ) {
		char *buffer = new char[ 4096 ];
		size_t chrs = 0;

		while ( !::feof(proc) && !::ferror(proc) ) {
			if (::getline( &buffer, &chrs, proc ) > 0) {
				QByteArray path( buffer, (int) chrs );
				flist.append( QFileInfo( path ));
			}
		}
		delete buffer;
		::pclose( proc );
	}

#if 0
	QProcess process( this );
	QByteArray line;

	process.setReadChannel( QProcess::StandardOutput );
	process.start( command, QIODevice::ReadOnly|QIODevice::Text );

	qDebug() << "retrHits: STATE=" << process.state();

	if (process.waitForStarted( 1000 )) {

		process.waitForReadyRead( 5000 );
		qDebug() << "retrHits: waitForReadyRead !";

		while (process.state() == QProcess::Running) {

			process.waitForReadyRead( 1 );

			if ( process.bytesAvailable() ) {

				line = process.readLine( 1024 );

				if ( !line.isEmpty() ) {
					QString path( line.constData() );
					flist.append( QFileInfo( path ));
				}
			}

			if ( process.atEnd() ) {
				break;
			}
		}
	}

	process.waitForFinished(1000);
	process.close();
#endif

	qDebug() << "retrHits: Exiting with " << flist.size() << " entries !";
	return flist;
}

void QLocateHandler::process(QHttpSocket *socket, const QString &path)
{
	QHttpHeaderMap headers = socket->headers();
	QString username( headers.value( "X-UserName" ));
	QString command, listing;

	qDebug() << "Process:" << path;

	if ( username.isEmpty() )
		username = "slocate";

	// If a document root is not set, an error has occurred
	if (d->documentRoot.path().isNull()) {
		socket->writeError(QHttpSocket::InternalServerError);
		return;
	}

	// URL-decode the path
	QString decodedPath( QUrl::fromPercentEncoding(path.toUtf8()));

	QString locatedb( d->documentRoot.path() );

	locatedb.append( "/" );
	locatedb.append( username );
	locatedb.append( ".db" );

	qDebug() << "process:" << locatedb;

	/*
	if(QFileInfo(absolutePath).isDir()) {
		d->processDirectory(socket, decodedPath, absolutePath);
	} else {
		d->processFile(socket, absolutePath);
	}
	*/

	command.append(
		QString( "slocate -q -i -d %1 -n 2000 \"%2\"" )
		.arg( locatedb )
		.arg( decodedPath )
	);

	qDebug() << "Process:" << command;

	// Add entries for each of the files
	foreach(QFileInfo info, retrHits( command )) {
		listing.append(QString("<li><a href=\"%1/%2\">%3</a></li>")
			.arg( info.absolutePath() )
			.arg( info.fileName() )
			.arg( info.fileName() )
		);
			//.arg( info.fileName().toHtmlEscaped() )
			//.arg( info.isDir() ? "/" : "") );
	}

	// Build the response and convert the string to UTF-8
	QByteArray data = d->ListTemplate
			.arg("/" + path)
			//.arg("/" + path.toHtmlEscaped())
			.arg(listing)
			.arg(QHTTPENGINE_VERSION)
			.toUtf8();

	// Set the headers and write the content
	socket->setHeader( "Content-Type", "text/html" );
	socket->setHeader( "Content-Length", QByteArray::number( data.length() ));
	socket->write( data );
	socket->close();
}

