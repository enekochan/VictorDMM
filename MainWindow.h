#ifndef VICTOR_DMM_MAINWINDOW_H
#define VICTOR_DMM_MAINWINDOW_H

#include <iostream>
#include <iomanip>
#include <utility>
#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "CanvasWidget.h"

class MainWindow : public QWidget {
public:
    MainWindow();
    ~MainWindow() override;
    void closeEvent(QCloseEvent* event) override;
private slots:
    void onComboBoxSelected(int index);
    void refreshDeviceComboBox();
    void onRefreshButtonClicked();
    void onConnectButtonClicked();
    void onDisconnectButtonClicked();
    void readSerialPortData();
    void loadUSBDevices();
private:
    std::vector<QString> deviceNames;
    unsigned short vendorId = 0x10c4;
    unsigned short productId = 0xea60;
    QSerialPort *serialPort;
    QByteArray receivedBuffer;
    CanvasWidget* canvasWidget;
    QComboBox* devicesComboBox;
    QPushButton* refreshButton;
    QPushButton* connectButton;
    QPushButton* disconnectButton;
};

#endif //VICTOR_DMM_MAINWINDOW_H
