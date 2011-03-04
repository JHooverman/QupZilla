/* ============================================================
* QupZilla - WebKit based browser
* Copyright (C) 2010-2011  nowrep
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
#include "sslmanager.h"
#include "ui_sslmanager.h"
#include "networkmanager.h"
#include "mainapplication.h"

SSLManager::SSLManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SSLManager)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    QList<QSslCertificate> certs = mApp->networkManager()->getCertExceptions();
    foreach (QSslCertificate cert, certs) {
        QListWidgetItem* item = new QListWidgetItem(ui->list);
        item->setText(cert.serialNumber());
        item->setData(0, certs.indexOf(cert));
        ui->list->addItem(item);
    }
}

SSLManager::~SSLManager()
{
    delete ui;
}
