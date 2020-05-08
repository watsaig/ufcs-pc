#include "testcommunicator.h"

void noMessageOutput(QtMsgType, const QMessageLogContext&, const QString&)
{}

void TestCommunicator::initTestCase()
{
    // Suppress all debug output
    //qInstallMessageHandler(noMessageOutput);

    // Instantiate serialCommunicator
    c = new SerialCommunicator();

}

void TestCommunicator::init()
{
    c->mDecoderIndex = 0;
    c->mDecoderRecording  = false;
    c->mDecoderEscaped = false;
    c->mBuffer.clear();
    c->mDecodedBuffer.clear();
}

void TestCommunicator::cleanup()
{

}

void TestCommunicator::decodeSimpleValidSequence()
{
    // Valid message, no escape sequence, no junk
    QByteArray cleanMessage;
    cleanMessage.append(VALVE);
    cleanMessage.append(1);
    cleanMessage.append(true);

    c->mBuffer.append(START_BYTE);
    c->mBuffer.append(cleanMessage);
    c->mBuffer.append(STOP_BYTE);

    QByteArray b = c->decodeBuffer();
    QCOMPARE(b, cleanMessage);
}


void TestCommunicator::decodeValidSequenceWithLeadingJunk()
{
    // Valid message with junk before it (that includes stop and/or escape)

    QByteArray cleanMessage;
    cleanMessage.append(VALVE);
    cleanMessage.append(1);
    cleanMessage.append(true);

    c->mBuffer.append(ESCAPE_BYTE);
    c->mBuffer.append(3);
    c->mBuffer.append(STOP_BYTE);
    c->mBuffer.append(7);
    c->mBuffer.append(55);

    c->mBuffer.append(START_BYTE);
    c->mBuffer.append(cleanMessage);
    c->mBuffer.append(STOP_BYTE);

    QByteArray b = c->decodeBuffer();
    QCOMPARE(b, cleanMessage);
}

void TestCommunicator::decodeValidSequenceWithEscapes()
{
    // Valid message with escape sequences in it

    QByteArray cleanMessage;
    cleanMessage.append(VALVE);
    cleanMessage.append(1);
    cleanMessage.append(STOP_BYTE);
    cleanMessage.append(true);

    c->mBuffer.append(START_BYTE);
    c->mBuffer.append(cleanMessage);
    c->mBuffer.insert(3, ESCAPE_BYTE); // just before the stop byte
    c->mBuffer.append(STOP_BYTE);

    QByteArray b = c->decodeBuffer();
    QCOMPARE(b, cleanMessage);
}

void TestCommunicator::decodeJunk()
{
    // Pure junk (no start sequence)

    // Note: START_BYTE is 250 (xFA)

    c->mBuffer =  QByteArrayLiteral("\x00\xf0\x96\x72\x37\x55\x0c\x3f");

    QByteArray b = c->decodeBuffer();
    QCOMPARE(b, QByteArray());
}


void TestCommunicator::decodeFragmentedMessage()
{
    // Decode one message split into 3 (so successive calls to decode are necessary)
    QByteArray cleanMessage = QByteArrayLiteral("\x00\xf0\x96\xf0\x23\x72\x37\x55\x0c\x3f");
    // 10 characters + start and stop. Split 4+3+3.

    c->mBuffer.append(START_BYTE);
    c->mBuffer.append(cleanMessage.left(4));

    QByteArray b = c->decodeBuffer();
    QCOMPARE(b, QByteArray());

    c->mBuffer.append(cleanMessage.mid(4, 3));

    b = c->decodeBuffer();
    QCOMPARE(b, QByteArray());

    c->mBuffer.append(cleanMessage.mid(7, -1));
    c->mBuffer.append(STOP_BYTE);

    b = c->decodeBuffer();
    QCOMPARE(b, cleanMessage);
}

void TestCommunicator::decodeSeveralMessages()
{
    // The rest of these tests reset the internal states (mDecoderEscaped, etc)
    // to make sure that the tests themselves don't put the decoder in a bad state
    // (via the init() function).

    // This test, on the other hand, makes sure that in normal operation,
    // the decoder can handle several messages in a row with no issue.

    QByteArray m1 = QByteArrayLiteral("\x00\xf0\x96\x72\x37\x55\x0c\x3f");
    QByteArray m2 = QByteArrayLiteral("\x01\xf1\x95\x71\x35\x52\x0b\x3e");
    QByteArray m3 = QByteArrayLiteral("\x03\xe0\x16\x82\x27\x45\x0d\x2f");

    c->mBuffer.append(START_BYTE);
    c->mBuffer.append(m1);
    c->mBuffer.append(STOP_BYTE);


    c->mBuffer.append(START_BYTE);
    c->mBuffer.append(m2);
    c->mBuffer.append(STOP_BYTE);


    c->mBuffer.append(START_BYTE);
    c->mBuffer.append(m3);
    c->mBuffer.append(STOP_BYTE);

    QCOMPARE(c->decodeBuffer(), m1);
    QCOMPARE(c->decodeBuffer(), m2);
    QCOMPARE(c->decodeBuffer(), m3);
}

void TestCommunicator::valveChange()
{
    // Construct a command to toggle a valve.
    // Valve commands contain two parameters, each one byte (number and state)
    // A few different valve numbers and states are tested

    uint8_t command = VALVE;

    const uint n = 5;

    uint8_t numbers[n] {10, 5, 2, 3, 4};
    bool states[n] {true, false, false, true, false};

    for (uint i(0); i < n; ++i) {
        QByteArray p1, p2;
        p1.push_back(numbers[i]);
        p2.push_back(states[i]);

        QList<QByteArray> params { p1, p2 };

        QSignalSpy spy(c, SIGNAL(valveStateChanged(uint, bool)));

        c->handleCommand(command, params);

        QCOMPARE(spy.count(), 1);
        QList<QVariant> arguments = spy.takeFirst();
        QCOMPARE(arguments[0].toInt(), numbers[i]);
        QCOMPARE(arguments[1].toBool(), states[i]);
    }
}

void TestCommunicator::pumpChange()
{
    // Construct a command to toggle a pump.
    // Pump commands contain two parameters, each one byte (number and state)
    // A few different pump numbers and states are tested

    uint8_t command = PUMP;

    const uint n = 5;

    uint8_t numbers[n] {1, 0, 2, 10, 25};
    bool states[n] {true, false, false, true, false};

    for (uint i(0); i < n; ++i) {
        QByteArray p1, p2;
        p1.push_back(numbers[i]);
        p2.push_back(states[i]);

        QList<QByteArray> params { p1, p2 };

        QSignalSpy spy(c, SIGNAL(pumpStateChanged(uint, bool)));

        c->handleCommand(command, params);

        QCOMPARE(spy.count(), 1);
        QList<QVariant> arguments = spy.takeFirst();
        QCOMPARE(arguments[0].toInt(), numbers[i]);
        QCOMPARE(arguments[1].toBool(), states[i]);
    }
}


void TestCommunicator::pressureChange()
{
    // Construct a command to communicate pressure.
    // PRESSURE commands contain 3 parameters (each one byte):
    // controller number, setpoint (0-255), and current pressure (0-255).
    // Two signals are emitted: one for setpoint and one for current pressure.
    // The signals emitted rescale the pressures to doubles between 0 and 1.

    uint8_t command = PRESSURE;
    const uint n = 5;
    uint8_t numbers[n] {0, 1, 10, 5, 3};
    uint8_t sp[n] {23, 203, 10, 73, 33};
    uint8_t pv[n] {10, 255, 3, 67, 105};

    for (uint i(0); i < n; ++i) {
        QByteArray p1, p2, p3;
        p1.push_back(numbers[i]);
        p2.push_back(sp[i]);
        p3.push_back(pv[i]);

        QList<QByteArray> params { p1, p2, p3 };

        QSignalSpy spSpy(c, SIGNAL(pressureSetpointChanged(uint, double)));
        QSignalSpy pvSpy(c, SIGNAL(pressureChanged(uint, double)));

        c->handleCommand(command, params);
        QCOMPARE(spSpy.count(), 1);
        QCOMPARE(pvSpy.count(), 1);

        QList<QVariant> spArgs = spSpy.takeFirst();
        QCOMPARE(spArgs[0].toDouble(), numbers[i]);
        QCOMPARE(spArgs[1].toDouble(), sp[i]/255.);

        QList<QVariant> pvArgs = pvSpy.takeFirst();
        QCOMPARE(pvArgs[0].toDouble(), numbers[i]);
        QCOMPARE(pvArgs[1].toDouble(), pv[i]/255.);
    }
}

void TestCommunicator::frameMessage()
{
    // Messages need to be framed by a start and end byte, and any special characters
    // (stop byte or escape byte) need to be escaped.

    // START_BYTE: 250 (xFA)
    // STOP_BYTE: 251 (xFB)
    // ESCAPE_BYTE: 252 (xFC)

    QByteArray m = QByteArrayLiteral("\x23\x51\x88\xf0\xFC\x9c\x99\xfb\x40\x09");
    QByteArray mFramed = QByteArrayLiteral("\xFA\x23\x51\x88\xf0\xFC\xFC\x9c\x99\xFC\xFB\x40\x09\xFB");

    QByteArray result = c->frameMessage(m);
    QCOMPARE(result, mFramed);
}

void TestCommunicator::uptime()
{
    // Uptime is 4-byte parameter giving the time since last boot of the microcontroller
    // in seconds (unsigned 32 bit type)

    uint32_t time = 4582;
    uint8_t command = UPTIME;
    QList<QByteArray> params;
    params.push_back(QByteArray::number(time));

    QSignalSpy spy(c, SIGNAL(uptimeChanged(ulong)));
    c->handleCommand(command, params);

    QCOMPARE(spy.count(), 1);
    QVariantList args = spy.takeFirst();
    QCOMPARE(args[0].toUInt(), time);

}

void TestCommunicator::parseDecodedBuffer()
{
    // Parse a valid buffer, and execute the command in it.

    uint8_t command(VALVE);
    uint8_t number(25);
    uint8_t state(true);
    QByteArray b;

    b.push_back(command);
    b.push_back(uint8_t(1));
    b.push_back(number);
    b.push_back(uint8_t(1));
    b.push_back(state);


    QSignalSpy spy(c, SIGNAL(valveStateChanged(uint, bool)));

    c->parseDecodedBuffer(b);

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments[0].toUInt(), number);
    QCOMPARE(arguments[1].toBool(), state);
}

void TestCommunicator::cleanupTestCase()
{
    delete c;
}




QTEST_MAIN(TestCommunicator)
