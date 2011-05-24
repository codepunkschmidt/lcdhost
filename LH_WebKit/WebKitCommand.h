#ifndef WEBKITCOMMAND_H
#define WEBKITCOMMAND_H

#include <QString>
#include <QUrl>
#include <QSize>
#include <QByteArray>
#include <QImage>
#include <QIODevice>

class WebKitCommand
{
public:
    WebKitCommand( int c, QSize s = QSize(), QUrl u = QUrl(), QString h = QString() );
    WebKitCommand( QByteArray a ) : command_(0), size_(QSize()), url_(QUrl()), html_(QString()) {decode(a);}

    int command() const { return command_; }
    QSize size() const { return size_; }
    QUrl url() const { return url_; }
    QString html() const { return html_; }

    QByteArray encode();
    bool decode( const QByteArray& );
    bool write( QIODevice * );

private:
    int command_; // 'HURQ'
    QSize size_; // size to render into
    QUrl url_; // if html isEmpty(), this is URL to load, otherwise it's baseURL
    QString html_; // leave empty if URL request
};

typedef struct WebKitData_
{
    qint32 bytecount; // number of bytes of image data that follows
    int progress;
    int w, h;
} WebKitData;

class WebKitResult
{
public:
    WebKitResult( int p = 0, QImage i = QImage() ) : progress_(p), image_(i) {}

    int progress() const { return progress_; }
    QImage image() const { return image_; }

    QByteArray encode();
    bool decode( const QByteArray& );

private:
    int progress_;
    QImage image_;
};

#endif // WEBKITCOMMAND_H
