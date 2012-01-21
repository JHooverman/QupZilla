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
#include "webtab.h"
#include "qupzilla.h"
#include "tabbedwebview.h"
#include "webpage.h"
#include "tabbar.h"
#include "locationbar.h"

WebTab::WebTab(QupZilla* mainClass, LocationBar* locationBar)
    : QWidget()
    , p_QupZilla(mainClass)
    , m_locationBar(locationBar)
    , m_pinned(false)
    , m_inspectorVisible(false)
{
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);

    m_view = new TabbedWebView(p_QupZilla, this);
    WebPage* page = new WebPage(p_QupZilla);
    m_view->setWebPage(page);
    m_layout->addWidget(m_view);

    setLayout(m_layout);
    setAutoFillBackground(true); // We don't want this transparent

    connect(m_view, SIGNAL(showNotification(QWidget*)), this, SLOT(showNotification(QWidget*)));
    connect(m_view, SIGNAL(iconChanged()), m_locationBar.data(), SLOT(siteIconChanged()));
    connect(m_view, SIGNAL(loadStarted()), m_locationBar.data(), SLOT(clearIcon()));
    connect(m_view, SIGNAL(loadFinished(bool)), m_locationBar.data(), SLOT(siteIconChanged()));
    connect(m_view, SIGNAL(urlChanged(QUrl)), m_locationBar.data(), SLOT(showUrl(QUrl)));
    connect(m_view, SIGNAL(rssChanged(bool)), m_locationBar.data(), SLOT(showRSSIcon(bool)));
    connect(m_view->webPage(), SIGNAL(privacyChanged(bool)), m_locationBar.data(), SLOT(setPrivacy(bool)));
    connect(m_locationBar.data(), SIGNAL(loadUrl(QUrl)), m_view, SLOT(load(QUrl)));
}

TabbedWebView* WebTab::view()
{
    return m_view;
}

bool WebTab::isPinned()
{
    return m_pinned;
}

void WebTab::setPinned(bool state)
{
    m_pinned = state;
}

void WebTab::setLocationBar(LocationBar* bar)
{
    m_locationBar = bar;
}

LocationBar* WebTab::locationBar()
{
    return m_locationBar.data();
}

bool WebTab::inspectorVisible()
{
    return m_inspectorVisible;
}

void WebTab::setInspectorVisible(bool v)
{
    m_inspectorVisible = v;
}

void WebTab::showNotification(QWidget* notif)
{
    if (m_layout->count() > 1) {
        delete m_layout->itemAt(0)->widget();
    }

    m_layout->insertWidget(0, notif);
    notif->show();
}

int WebTab::tabIndex()
{
    return m_view->tabIndex();
}

void WebTab::pinTab(int index)
{
    TabWidget* tabWidget = p_QupZilla->tabWidget();
    if (!tabWidget) {
        return;
    }

    if (m_pinned) { //Unpin tab
        m_pinned = false;
        tabWidget->setTabText(index, m_view->title());
        tabWidget->getTabBar()->updateCloseButton(index);
    }
    else {   // Pin tab
        m_pinned = true;
        tabWidget->setCurrentIndex(0); //             <<-- those 2 lines fixes
        tabWidget->getTabBar()->moveTab(index, 0);    // | weird behavior with bad
        tabWidget->setTabText(0, "");                 // | tabwidget update if we
        tabWidget->setCurrentIndex(0); //             <<-- are moving current tab
        tabWidget->getTabBar()->updateCloseButton(0);
    }
}

void WebTab::disconnectObjects()
{
    disconnect(this);
    disconnect(m_locationBar.data());
    disconnect(m_view);
}

WebTab::~WebTab()
{
    if (m_locationBar) {
        delete m_locationBar.data();
    }
}
