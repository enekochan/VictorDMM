#include "CanvasWidget.h"

CanvasWidget::CanvasWidget(QWidget* parent) : QWidget(parent) {
    battery.load(":/images/battery.png");
    battery = battery.scaled(battery.size() * 0.4, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    beep.load(":/images/beep.png");
    beep = beep.scaled(beep.size() * 0.4, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    diode.load(":/images/diode.png");
    diode = diode.scaled(diode.size() * 0.4, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QFont systemFont = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
    fontDpi = QFontMetrics(systemFont).fontDpi();
    if (fontDpi == 72) {
        textFontSize += 4;
        textFontStretch += 15;
        digitsFontSize += 50;
        digitsFontStretch += 17;
        scaleFontStretch += 20;
    } else if (fontDpi == 96) {

    }

    textFont.setPointSize(textFontSize);
    textFont.setFamily("Arial");
    textFont.setBold(true);
    textFont.setStretch(textFontStretch);

    digitsFont.setPointSize(digitsFontSize);
    digitsFont.setStretch(digitsFontStretch);
    QString fontFilePath = ":/fonts/Segment7.ttf";
    QFontDatabase::addApplicationFont(fontFilePath);
    digitsFont.setFamily("Segment7");

    scaleFont.setPointSize(scaleFontSize);
    scaleFont.setFamily("Arial");
    scaleFont.setBold(true);
    scaleFont.setStretch(scaleFontStretch);

    lastUpdateDateTime = QDateTime::currentMSecsSinceEpoch();

    // Call updateText every second
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &CanvasWidget::updateText);
    timer->start(100);

    setFixedSize(425, 265);
}

void CanvasWidget::updateSerialData(QByteArray buffer) {
    serialData = std::move(buffer);
    lastUpdateDateTime = QDateTime::currentMSecsSinceEpoch();
}

void CanvasWidget::resetDisplay(bool status) {
    firstDigit =
    secondDigit =
    thirdDigit =
    fourthDigit = status ? "8" : " ";

    showNegative =
    showFirstDot =
    showSecondDot =
    showThirdDot =
    showBar =
    showHold =
    showRel =
    showRs232 =
    showAc =
    showDc =
    showAuto =
    showBat =
    showApo =
    showMin =
    showMax =
    showPercent =
    showDiode =
    showBeep =
    showMega =
    showKilo =
    showMili =
    showMicro =
    showNano =
    showTempF =
    showTempC =
    showFarad =
    showHz =
    showhFE =
    showOhm =
    showAmp =
    showVolt =
    showBarNegative = status;

    barValue = 42;

    serialData = QByteArray();

    update();
}

void CanvasWidget::setConnected(bool value) {
    connected = value;
}

void CanvasWidget::paintEvent(QPaintEvent*) {
    QPainter painter(this);

    // Fill background color
    painter.fillRect(rect(), QColor("#788770")); // #4c533f

    // Set paint color
    painter.setPen(QColor(Qt::black));

    // Set text font
    painter.setFont(textFont);

    if (showMax)
        painter.drawText(60, 28, "MAX");
    if (showMin)
        painter.drawText(115, 28, "MIN");
    if (showHold)
        painter.drawText(160, 28, "HOLD");
    if (showRel)
        painter.drawText(230, 28, "REL");
    if (showRs232)
        painter.drawText(280, 28, "RS232");

    QFont biggerFont = textFont;
    biggerFont.setPointSize(textFontSize + 5); // Set a bigger font size
    painter.setFont(biggerFont);
    if (showDc)
        painter.drawText(15, 70, "DC");
    if (showAc)
        painter.drawText(15, 100, "AC");
    if (showTempC)
        painter.drawText(357, 155, "ºC");
    if (showTempF)
        painter.drawText(387, 155, "ºF");
    painter.setFont(textFont);
    if (showAuto)
        painter.drawText(15, 160, "AUTO");
    if (showApo)
        painter.drawText(15, 190, "APO");

    if (showPercent)
        painter.drawText(357, 65, "%");
    if (showhFE) {
        painter.drawText(382, 65, "h");
        QFont smallerFont = textFont;
        smallerFont.setPointSize(textFontSize - (fontDpi == 72?3:6)); // Set a smaller font size
        painter.setFont(smallerFont);
        painter.drawText(395, 65, "FE");
    }
    painter.setFont(textFont);

    if (showNano)
        painter.drawText(355, 108, "n");
    if (showMili)
        painter.drawText(367, 108, "m");
    if (showMicro)
        painter.drawText(370, 113, "μ");

    if (showVolt)
        painter.drawText(385, 108, "V");
    if (showAmp)
        painter.drawText(395, 108, "A");
    if (showFarad)
        painter.drawText(410, 108, "F");

    if (showMega)
        painter.drawText(353, 190, "M");
    if (showKilo)
        painter.drawText(370, 190, "k");
    if (showOhm)
        painter.drawText(382, 190, "Ω");

    if (showHz)
        painter.drawText(397, 190, "Hz");

    if (showBar) {
        // Set scale font
        painter.setFont(scaleFont);
        painter.drawText(20, 250, "0");
        painter.drawText(108, 250, "10");
        painter.drawText(198, 250, "20");
        painter.drawText(288, 250, "30");
        painter.drawText(378, 250, "40");

        painter.setBrush(Qt::black);
        painter.setPen(Qt::black);
        for (int i = 0; i <= 42; i++) {
            int positionX = 20 + i * 9;
            // Draw scale dots
            painter.drawEllipse(positionX, 255, 4, 4);
            // Draw scale small rectangles
            if (barValue >= i)
                painter.drawRect(positionX, 220, 5, 18);
            // Draw scale triangles every 5 rectangles
            if (barValue >= i && i % 5 == 0) {
                QPolygonF triangle;
                triangle << QPointF(positionX, 220) << QPointF(positionX + (fontDpi==72?2.7:2.8), 216) << QPointF(positionX + (fontDpi==72?5:6), 220);
                painter.drawPolygon(triangle);
            }
            // Draw bigger scale rectangles and higher triangles every 10 rectangles
            if (barValue >= i && i % 10 == 0) {
                painter.drawRect(positionX, 215, 5, 20);
                QPolygonF triangle;
                triangle << QPointF(positionX, 216) << QPointF(positionX + (fontDpi==72?2.7:2.8), 212) << QPointF(positionX + (fontDpi==72?5:6), 216);
                painter.drawPolygon(triangle);
            }
            // Draw last arrow triangle
            if (barValue >= i && i != 0 && i % 42 == 0) {
                QPolygonF triangle;
                triangle << QPointF(positionX + 5, 220)
                         << QPointF(positionX + 11, 229)
                         << QPointF(positionX + 5, 238);
                painter.drawPolygon(triangle);
            }
        }
        // Draw every 10 dots
        painter.drawEllipse(19, 254, 6, 6);
        painter.drawEllipse(109, 254, 6, 6);
        painter.drawEllipse(199, 254, 6, 6);
        painter.drawEllipse(289, 254, 6, 6);
        painter.drawEllipse(379, 254, 6, 6);
        // Draw scale negative
        if (showBarNegative)
            painter.drawRect(3, 225, 12, 5);
    }

    // Set digits font
    painter.setFont(digitsFont);
    // Paint digits
    painter.drawText(digitsPositionX + 70,digitsPositionY, firstDigit);
    painter.drawText(digitsPositionX + 140,digitsPositionY, secondDigit);
    painter.drawText(digitsPositionX + 210,digitsPositionY, thirdDigit);
    painter.drawText(digitsPositionX + 280,digitsPositionY, fourthDigit);

    painter.setBrush(Qt::black);
    painter.setPen(Qt::black);
    // Draw negative with a rectangle
    if (showNegative) {
        painter.drawRect(digitsPositionX + 20, digitsPositionY - 88, 35, 15);
        // Paint negative with text
        //painter.drawText(digitsPositionX + 0,digitsPositionY, "-");
    }
    // Paint dots
    if (showFirstDot) {
        painter.drawRect(dotPositionX - dotSize, dotsPositionY - dotSize, dotSize * 2, dotSize * 2);
    }
    if (showSecondDot) {
        painter.drawRect(dotPositionX + 70 - dotSize, dotsPositionY - dotSize, dotSize * 2, dotSize * 2);
    }
    if (showThirdDot) {
        painter.drawRect(dotPositionX + 140 - dotSize, dotsPositionY - dotSize, dotSize * 2, dotSize * 2);
    }
    // Draw parallelogram to simulate an italic dot
    //QPolygonF point1;
    //point1 << QPointF(dotPositionX - 6, dotsPositionY + 6) << QPointF(dotPositionX - 4, dotsPositionY - 6) << QPointF(dotPositionX + 8, dotsPositionY - 6) << QPointF(dotPositionX + 6, dotsPositionY + 6);
    //point1 << QPointF(dotPositionX - dotSize, dotsPositionY + dotSize) << QPointF(dotPositionX - dotSize, dotsPositionY - dotSize) << QPointF(dotPositionX + dotSize, dotsPositionY - dotSize) << QPointF(dotPositionX + dotSize, dotsPositionY + dotSize);
    //painter.drawPolygon(point1);
    // Use font dot character
    //painter.drawText(dotPositionX + 25, dotsPositionY, firstDot);
    //painter.drawText(dotPositionX + 95, dotsPositionY, secondDot);
    //painter.drawText(dotPositionX + 165, dotsPositionY, thirdDot);

    // Paint icons
    if (showBat)
        painter.drawPixmap(12, 7, battery);
    if (showBeep)
        painter.drawPixmap(350, 5, beep);
    if (showDiode)
        painter.drawPixmap(380, 5, diode);
}


void CanvasWidget::updateText() {
    // The DMM is connected to the USB port but turned off or not in RS232 mode so blank the screen
    if (connected && QDateTime::currentMSecsSinceEpoch() - lastUpdateDateTime > 1500) {
        resetDisplay(false);
        update();
        return;
    }

    if (serialData.length() != 14)
        return;

    if (serialData[0] == (char)0x2B || serialData[0] == (char)0x2D) {
        // RS232
        showRs232 = true;

        // Negative
        showNegative = serialData[0] == (char)0x2D;

        // Numbers and Overload
        if (serialData[1] == (char)0x3F && serialData[2] == (char)0x30 && serialData[3] == (char)0x3A && serialData[4] == (char)0x3F) {
            firstDigit = " ";
            secondDigit = "O";
            thirdDigit = "L";
            fourthDigit = " ";
        } else {
            firstDigit = serialData[1];
            secondDigit = serialData[2];
            thirdDigit = serialData[3];
            fourthDigit = serialData[4];
        }

        // Dots
        showFirstDot = serialData[6] == (char)0x31;
        showSecondDot = serialData[6] == (char)0x32;
        showThirdDot = serialData[6] == (char)0x34;

        // SB1 byte
        showBar = (serialData[7] & (char)0x01) != 0x00;
        showHold = (serialData[7] & (char)0x02) != 0x00;
        showRel = (serialData[7] & (char)0x04) != 0x00;
        showAc = (serialData[7] & (char)0x08) != 0x00;
        showDc = (serialData[7] & (char)0x10) != 0x00;
        showAuto = (serialData[7] & (char)0x20) != 0x00;

        // SB2 byte
        showNano = (serialData[8] & (char)0x02) != 0x00;
        showBat = (serialData[8] & (char)0x04) != 0x00;
        showApo = (serialData[8] & (char)0x08) != 0x00;
        showMin = (serialData[8] & (char)0x10) != 0x00;
        showMax = (serialData[8] & (char)0x20) != 0x00;

        // SB3
        showPercent = (serialData[9] & (char)0x02) != 0x00;
        showDiode = (serialData[9] & (char)0x04) != 0x00;
        showBeep = (serialData[9] & (char)0x08) != 0x00;
        showMega = (serialData[9] & (char)0x10) != 0x00;
        showKilo = (serialData[9] & (char)0x20) != 0x00;
        showMili = (serialData[9] & (char)0x40) != 0x00;
        showMicro = (serialData[9] & (char)0x80) != 0x00;

        // SB4
        showTempF = (serialData[10] & (char)0x01) != 0x00;
        showTempC = (serialData[10] & (char)0x02) != 0x00;
        showFarad = (serialData[10] & (char)0x04) != 0x00;
        showHz = (serialData[10] & (char)0x08) != 0x00;
        showhFE = (serialData[10] & (char)0x10) != 0x00;
        showOhm = (serialData[10] & (char)0x20) != 0x00;
        showAmp = (serialData[10] & (char)0x40) != 0x00;
        showVolt = (serialData[10] & (char)0x80) != 0x00;

        // Bar
        showBarNegative = (serialData[11] & (char)0x80) != 0x00;
        barValue = (toascii(QChar(serialData[11]).unicode()) & 0x7F);
        // In overload if the bar is shown set it to maximum value
        if (thirdDigit == "L" && showBar) {
            barValue = 42;
        }
        //qDebug() << barValue;

        if (showBeep)
            showOhm = true;
    }

    update(); // Trigger a repaint
}
