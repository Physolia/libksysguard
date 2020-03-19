/*
    Copyright (C) 2020 Arjen Hiemstra <ahiemstra@heimr.nl>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "SensorQuery.h"

#include <QDBusPendingCallWatcher>
#include <QDBusReply>
#include <QRegularExpression>

#include "SensorDaemonInterface_p.h"
#include "sensors_logging.h"

using namespace KSysGuard;

class SensorQuery::Private
{
public:
    enum class State {
        Initial,
        Running,
        Finished
    };

    void updateResult(const QDBusPendingReply<SensorInfoMap> &reply);

    QString path;
    State state = State::Initial;
    QVector<QPair<QString, SensorInfo>> result;

    QDBusPendingCallWatcher *watcher = nullptr;
};

KSysGuard::SensorQuery::SensorQuery(const QString& path, QObject* parent)
    : QObject(parent), d(std::make_unique<Private>())
{
    d->path = path;
}

KSysGuard::SensorQuery::~SensorQuery()
{
}

QString KSysGuard::SensorQuery::path() const
{
    return d->path;
}

void KSysGuard::SensorQuery::setPath(const QString& path)
{
    if (path == d->path) {
        return;
    }

    if (d->state != Private::State::Initial) {
        qCWarning(LIBKSYSGUARD_SENSORS) << "Cannot modify a running or finished query";
        return;
    }

    d->path = path;
}

bool KSysGuard::SensorQuery::execute()
{
    if (d->state != Private::State::Initial) {
        return false;
    }

    d->state = Private::State::Running;


    auto watcher = SensorDaemonInterface::instance()->allSensors();
    d->watcher = watcher;
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [watcher, this]() {
        watcher->deleteLater();
        d->watcher = nullptr;
        d->state = Private::State::Finished;
        d->updateResult(QDBusPendingReply<SensorInfoMap>(*watcher));
        Q_EMIT finished(this);
    });

    return true;
}

bool KSysGuard::SensorQuery::waitForFinished()
{
    if (!d->watcher) {
        return false;
    }

    d->watcher->waitForFinished();
    return true;
}

QStringList KSysGuard::SensorQuery::sensorIds() const
{
    QStringList ids;
    std::transform(d->result.cbegin(), d->result.cend(), std::back_inserter(ids), [](auto entry) { return entry.first; });
    return ids;
}

QVector<QPair<QString, SensorInfo>> KSysGuard::SensorQuery::result() const
{
    return d->result;
}

void KSysGuard::SensorQuery::Private::updateResult(const QDBusPendingReply<SensorInfoMap> &reply)
{
    if (path.isEmpty()) { // add everything
        const SensorInfoMap response = reply.value();
        for (auto it = response.constBegin(); it != response.constEnd(); it++) {
            result.append(qMakePair(it.key(), it.value()));
        }
        return;
    }
    auto regexp = QRegularExpression{QRegularExpression::wildcardToRegularExpression(path)};

    const auto sensorIds = reply.value().keys();
    for (auto id : sensorIds) {
        if (regexp.match(id).hasMatch()) {
            result.append(qMakePair(id, reply.value().value(id)));
        }
    }
}