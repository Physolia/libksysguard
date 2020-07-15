/*
    Copyright (C) 2020 Marco Martin <mart@kde.org>

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

#include <QString>
#include <QHash>

namespace KSysGuard
{

class SensorGroup
{
public:
    SensorGroup();
    ~SensorGroup();

    void retranslate();

    QString groupRegexForId(const QString &key);
    QString sensorNameForRegEx(const QString &expr);
    QString segmentNameForRegEx(const QString &expr);

private:
    QHash <QString, QString> m_sensorNames;
    QHash <QString, QString> m_segmentNames;
};
} // namespace KSysGuard