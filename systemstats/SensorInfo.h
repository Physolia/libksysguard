/*
    Copyright (c) 2019 David Edmundson <davidedmundson@kde.org>

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

#pragma once

#include <QDBusArgument>
#include <QDateTime>
#include <QDebug>
#include <QObject>
#include <QVariant>

#include "formatter/Unit.h"

#include "systemstats_export.h"

namespace KSysGuard
{

constexpr uint BackendUpdateInterval = 500;

// Data that is static for the lifespan of the sensor
class SYSTEMSTATS_EXPORT SensorInfo
{
public:
    SensorInfo() = default;

    QString name;        // Translated name of the sensor.
    QString shortName;   // Shorter translated name of the sensor, to use when space is constrained.
    QString description; // Translated description of the sensor.
    QVariant::Type variantType = QVariant::Invalid;
    KSysGuard::Unit unit = KSysGuard::UnitInvalid; // Both a format hint and implies data type (i.e double/string)
    qreal min = 0;
    qreal max = 0;
};

class SYSTEMSTATS_EXPORT SensorData
{
public:
    SensorData() = default;
    SensorData(const QString &_sensorProperty, const QVariant &_payload)
        : sensorProperty(_sensorProperty)
        , payload(_payload)
    {
    }
    QString sensorProperty;
    QVariant payload;
};

typedef QHash<QString, SensorInfo> SensorInfoMap;
typedef QVector<SensorData> SensorDataList;

inline QDBusArgument &operator<<(QDBusArgument &argument, const SensorInfo &s)
{
    argument.beginStructure();
    argument << s.name;
    argument << s.shortName;
    argument << s.description;
    argument << s.variantType;
    argument << s.unit;
    argument << s.min;
    argument << s.max;
    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, SensorInfo &s)
{
    argument.beginStructure();
    argument >> s.name;
    argument >> s.shortName;
    argument >> s.description;
    uint32_t t;
    argument >> t;
    s.variantType = static_cast<QVariant::Type>(t);
    argument >> t;
    s.unit = static_cast<KSysGuard::Unit>(t);
    argument >> s.min;
    argument >> s.max;
    argument.endStructure();
    return argument;
}

inline QDBusArgument &operator<<(QDBusArgument &argument, const SensorData &s)
{
    argument.beginStructure();
    argument << s.sensorProperty;
    argument << QDBusVariant(s.payload);
    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, SensorData &s)
{
    argument.beginStructure();
    argument >> s.sensorProperty;
    argument >> s.payload;
    argument.endStructure();
    return argument;
}

} // namespace KSysGuard

Q_DECLARE_METATYPE(KSysGuard::SensorInfo);
Q_DECLARE_METATYPE(KSysGuard::SensorData);
Q_DECLARE_METATYPE(KSysGuard::SensorDataList);
