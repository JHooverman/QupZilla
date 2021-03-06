/* ============================================================
* QupZilla - WebKit based browser
* Copyright (C) 2010-2012  David Rosca <nowrep@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* ============================================================ */
#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QDebug>
#include <QUrl>
#include <QWebView>
#include <QWebFrame>
#include <QWebHistory>
#include <QtNetwork/QtNetwork>
#include <QDesktopServices>
#include <QMessageBox>
#include <QDesktopServices>
#include <QStyle>
#include <QFileDialog>
#include <QWebInspector>
#include <QFileSystemWatcher>

class QupZilla;
class TabbedWebView;
class SpeedDial;
class NetworkManagerProxy;
class WebPage : public QWebPage
{
    Q_OBJECT
public:
    struct AdBlockedEntry {
        QString rule;
        QUrl url;

        bool operator==(const AdBlockedEntry &other) const {
            return (this->rule == other.rule && this->url == other.url);
        }
    };

    WebPage(QupZilla* mainClass);
    ~WebPage();

    QUrl url() const;

    void setWebView(TabbedWebView* view);
    void populateNetworkRequest(QNetworkRequest &request);

    TabbedWebView* getView() { return m_view; }
    void setSSLCertificate(const QSslCertificate &cert);
    QSslCertificate sslCertificate();

    bool javaScriptPrompt(QWebFrame* originatingFrame, const QString &msg, const QString &defaultValue, QString* result);
    bool javaScriptConfirm(QWebFrame* originatingFrame, const QString &msg);
    void javaScriptAlert(QWebFrame* originatingFrame, const QString &msg);

    void addAdBlockRule(const QString &filter, const QUrl &url);
    QList<AdBlockedEntry> adBlockedEntries() { return m_adBlockedEntries; }

    void scheduleAdjustPage();
    bool isRunningLoop();

    static QString UserAgent;
    QString userAgentForUrl(const QUrl &url) const;

    void disconnectObjects();

signals:
    void privacyChanged(bool status);

protected slots:
    QWebPage* createWindow(QWebPage::WebWindowType type);
    void handleUnsupportedContent(QNetworkReply* url);

    void progress(int prog);
    void finished();

private slots:
    void cleanBlockedObjects();
    void urlChanged(const QUrl &url);
    void addJavaScriptObject();

    void watchedFileChanged(const QString &file);
    void printFrame(QWebFrame* frame);
    void downloadRequested(const QNetworkRequest &request);

private:
    virtual bool supportsExtension(Extension extension) const;
    virtual bool extension(Extension extension, const ExtensionOption* option, ExtensionReturn* output = 0);
    bool acceptNavigationRequest(QWebFrame* frame, const QNetworkRequest &request, NavigationType type);
    QString chooseFile(QWebFrame* originatingFrame, const QString &oldFile);

    static QString m_lastUploadLocation;

    QupZilla* p_QupZilla;
    NetworkManagerProxy* m_networkProxy;
    QNetworkRequest m_lastRequest;
    QWebPage::NavigationType m_lastRequestType;
    TabbedWebView* m_view;
    SpeedDial* m_speedDial;
    QSslCertificate m_SslCert;
    QList<QSslCertificate> m_SslCerts;
    QList<AdBlockedEntry> m_adBlockedEntries;
    QFileSystemWatcher* m_fileWatcher;

    QEventLoop* m_runningLoop;

    bool m_blockAlerts;
    bool m_secureStatus;
    bool m_adjustingScheduled;

    bool m_isClosing;
};

#endif // WEBPAGE_H
