#ifndef WEBKITSERVERWINDOW_H
#define WEBKITSERVERWINDOW_H

#include <QtNetwork>

#include <QApplication>
#include <QDialog>
#include <QImage>

#include "EventWebKitHeartbeat.h"

namespace Ui
{
    class WebKitServerWindow;
}

class WebKitHeart : public QThread
{
public:
    bool alive;
    int rate;
    WebKitHeart(QObject *parent = 0 ) : QThread(parent), alive(true), rate(2) {}
    void run();
};

class WebKitServerWindow : public QDialog
{
    Q_OBJECT

public:
    WebKitServerWindow(QWidget *parent = 0);
    ~WebKitServerWindow();

    // void timerEvent(QTimerEvent *);
    void closeEvent(QCloseEvent *);
    QNetworkAccessManager *manager() { return manager_; }

    bool event(QEvent *);
    bool paused() const;
    void updateRequests();

public slots:
    void finished(QNetworkReply*);
    void newConnection();
    void refreshList();

protected:
    void changeEvent(QEvent *e);

private:
    QNetworkAccessManager *manager_;
    QLocalServer *server_;
    QTimer timer_;

private:
    Ui::WebKitServerWindow *ui;
    int rps_;
    WebKitHeart *heart_;
    QTime lastbeat_;

private slots:
    void on_pushButton_2_clicked(bool checked);
    void on_listWidget_currentRowChanged(int currentRow);
    void on_pushButton_clicked();
    void on_spinBoxMaxFPS_valueChanged(int );
};

#endif // WEBKITSERVERWINDOW_H
