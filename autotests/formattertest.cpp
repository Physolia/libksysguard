/*
 * Copyright 2020  Arjen Hiemstra <ahiemstra@heimr.nl>
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <QTest>
#include <QLocale>

#include "Formatter.h"
#include "Unit.h"

#include <unistd.h>
#ifdef Q_OS_OSX
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#define QSL QStringLiteral

class FormatterTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase()
    {
        // Ensure we use a known locale for the test.
        QLocale::setDefault(QLocale{QLocale::English, QLocale::UnitedStates});
    }

    void testDouble_data()
    {
        QTest::addColumn<double>("input");
        QTest::addColumn<KSysGuard::Unit>("unit");
        QTest::addColumn<QString>("output");

        QTest::newRow("1.0, B") << 1.0 << KSysGuard::UnitByte << QSL("1 B");
        QTest::newRow("1.0, KiB") << 1.0 << KSysGuard::UnitKiloByte << QSL("1.0 KiB");
        QTest::newRow("1.0, KiB/s") << 1.0 << KSysGuard::UnitKiloByteRate << QSL("1.0 KiB/s");
        QTest::newRow("1.0, %") << 1.0 << KSysGuard::UnitPercent << QSL("1.0%");

        QTest::newRow("0.213, B") << 0.213 << KSysGuard::UnitByte << QString::number(0) + QSL(" B");
        QTest::newRow("0.213, kHZ") << 0.213 << KSysGuard::UnitKiloHertz << QString::number(213.0, 'f', 1) + QSL(" Hz");
        QTest::newRow("5.647, KiB") << 5.647 << KSysGuard::UnitKiloByte << QString::number(5.6) + QSL(" KiB");
        QTest::newRow("99.99, KiB/s") << 99.99 << KSysGuard::UnitKiloByteRate << QString::number(100.0, 'f', 1) + QSL(" KiB/s");
        QTest::newRow("0.2567, %") << 0.2567 << KSysGuard::UnitPercent << QString::number(0.3) + QSL("%");
    }

    void testDouble()
    {
        QFETCH(double, input);
        QFETCH(KSysGuard::Unit, unit);
        QFETCH(QString, output);

        auto formatted = KSysGuard::Formatter::formatValue(input, unit);
        QCOMPARE(formatted, output);
    }

    void testFormatTime_data()
    {
        QTest::addColumn<int>("input");
        QTest::addColumn<QString>("output");
        QTest::newRow("1 s") << 1 << QSL("0:01");
        QTest::newRow("10 s") << 10 << QSL("0:10");
        QTest::newRow("1 m") << 60 << QSL("1:00");
        QTest::newRow("10m") << 60 * 10 << QSL("10:00");
        QTest::newRow("1h") << 60 * 60 << QSL("60:00");
        QTest::newRow("1h 1 m 1s") << (60 * 60) + 60 + 1 << QSL("61:01");
    }

    void testFormatTime()
    {
        QFETCH(int, input);
        QFETCH(QString, output);
        auto formatted =  KSysGuard::Formatter::formatValue(input, KSysGuard::UnitTime);
        QCOMPARE(formatted, output);
    }

};

QTEST_MAIN(FormatterTest);

#include "formattertest.moc"
