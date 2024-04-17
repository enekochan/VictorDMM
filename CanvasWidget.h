#ifndef VICTOR_DMM_CANVASWIDGET_H
#define VICTOR_DMM_CANVASWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QFont>
#include <QFontDatabase>
#include <QFontMetrics>
#include <QString>
#include <QTimer>
#include <QDateTime>
#include <QColor>

class CanvasWidget : public QWidget {
public:
    explicit CanvasWidget(QWidget* parent = nullptr);
    void updateSerialData(QByteArray buffer);
    void resetDisplay(bool status);
    void setConnected(bool value);
protected:
    void paintEvent(QPaintEvent*) override;
private slots:
    void updateText();
private:
    QPixmap battery;
    QPixmap beep;
    QPixmap diode;
    QFont textFont;
    QFont digitsFont;
    QFont scaleFont;
    QTimer* timer;
    QString firstDigit = "8";
    QString secondDigit = "8";
    QString thirdDigit = "8";
    QString fourthDigit = "8";
    qreal fontDpi = 72;
    int textFontSize = 20;
    int textFontStretch = 80;
    int digitsFontSize = 150;
    int digitsFontStretch = 60;
    int digitsPositionX = 5;
    int digitsPositionY = 200;
    int scaleFontSize = 8;
    int scaleFontStretch = 80;
    int dotSize = 5;
    int dotPositionX = 140;
    int dotsPositionY = digitsPositionY - dotSize;
    bool showNegative = true;
    bool showFirstDot = true;
    bool showSecondDot = true;
    bool showThirdDot = true;
    bool showBar = true;
    bool showHold = true;
    bool showRel = true;
    bool showRs232 = true;
    bool showAc = true;
    bool showDc = true;
    bool showAuto = true;
    bool showBat = true;
    bool showApo = true;
    bool showMin = true;
    bool showMax = true;
    bool showPercent = true;
    bool showDiode = true;
    bool showBeep = true;
    bool showMega = true;
    bool showKilo = true;
    bool showMili = true;
    bool showMicro = true;
    bool showNano = true;
    bool showTempF = true;
    bool showTempC = true;
    bool showFarad = true;
    bool showHz = true;
    bool showhFE = true;
    bool showOhm = true;
    bool showAmp = true;
    bool showVolt = true;
    bool showBarNegative = true;
    int barValue = 42;
    QByteArray serialData;
    bool connected = false;
    long lastUpdateDateTime;
};

#endif //VICTOR_DMM_CANVASWIDGET_H
