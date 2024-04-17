#include "MainWindow.h"

MainWindow::MainWindow() {
    serialPort = new QSerialPort(this);

    canvasWidget = new CanvasWidget(this);

    // Create a QComboBox with the USB devices
    devicesComboBox = new QComboBox(this);
    refreshDeviceComboBox();
    connect(devicesComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::onComboBoxSelected);

    // Create buttons with actions
    refreshButton = new QPushButton(this);
    refreshButton->setIcon(QIcon(":/images/refresh.png"));
    //refreshButton->setIconSize(QSize(15, 15));
    refreshButton->setFixedSize(25, 25);
    connectButton = new QPushButton("Connect", this);
    disconnectButton = new QPushButton("Disconnect", this);
    connectButton->setDisabled(true);
    disconnectButton->setDisabled(true);

    connect(refreshButton, &QPushButton::clicked, this, &MainWindow::onRefreshButtonClicked);
    connect(connectButton, &QPushButton::clicked, this, &MainWindow::onConnectButtonClicked);
    connect(disconnectButton, &QPushButton::clicked, this, &MainWindow::onDisconnectButtonClicked);
    connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::readSerialPortData);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->setContentsMargins(5, 0, 5, 5);
    buttonLayout->addWidget(devicesComboBox);
    buttonLayout->addWidget(refreshButton);
    buttonLayout->addWidget(connectButton);
    buttonLayout->addWidget(disconnectButton);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 0, 5, 0);
    mainLayout->addWidget(canvasWidget);
    mainLayout->addLayout(buttonLayout);

    // Disable window resizing
    //setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint);
    setWindowTitle("Victor 86C DMM");
    setFixedSize(445, 310);
    setContentsMargins(5, 0, 5, 0);
}

MainWindow::~MainWindow() {
    if (serialPort->isOpen()) {
        serialPort->close();
        //qDebug() << "Serial port closed";
    }
}

void MainWindow::closeEvent(QCloseEvent* event) {
    // Code to execute when the application is closed
    if (serialPort->isOpen()) {
        serialPort->close();
        //qDebug() << "Serial port closed";
    }
    QWidget::closeEvent(event);
}

void MainWindow::onComboBoxSelected(int index) {
    // Handle QComboBox selection change
    //qDebug() << "Selected option: " << devicesComboBox->itemText(index);
    //qDebug() << deviceNames[index-1].toStdString();
    if (index == 0) {
        connectButton->setDisabled(true);
    } else {
        connectButton->setDisabled(false);
    }
    disconnectButton->setDisabled(true);
}

void MainWindow::refreshDeviceComboBox() {
    loadUSBDevices();
    devicesComboBox->clear();
    devicesComboBox->addItem("Select device...");
    for (const QString& deviceName: deviceNames) {
        devicesComboBox->addItem(deviceName);
    }
}

void MainWindow::onRefreshButtonClicked() {
    refreshDeviceComboBox();
}

void MainWindow::onConnectButtonClicked() {
    // Open the USB port
    int selectedDevice = devicesComboBox->currentIndex();
    QString serialPortPath = devicesComboBox->itemText(selectedDevice);
    //qDebug() << serialPortPath.toStdString();

    serialPort->setPortName(serialPortPath);
    serialPort->setBaudRate(QSerialPort::Baud2400);
    serialPort->setReadBufferSize(256);
    if (serialPort->open(QIODevice::ReadOnly)) {
        //qDebug() << "Serial port opened successfully";
        refreshButton->setDisabled(true);
        connectButton->setDisabled(true);
        disconnectButton->setDisabled(false);
        devicesComboBox->setDisabled(true);
        canvasWidget->updateSerialData(QByteArray());
        canvasWidget->setConnected(true);
    } else {
        //qDebug() << "Error opening serial port: " << serialPort->errorString();
        QMessageBox *q = new QMessageBox(this);
        q->setText("Error opening serial port.");
        q->exec();
    }
}

void MainWindow::onDisconnectButtonClicked() {
    if (serialPort->isOpen()) {
        serialPort->close();
        //qDebug() << "Serial port closed";
    }
    refreshButton->setDisabled(false);
    connectButton->setDisabled(false);
    disconnectButton->setDisabled(true);
    devicesComboBox->setDisabled(false);
    canvasWidget->setConnected(false);
    canvasWidget->resetDisplay(true);
}

void MainWindow::readSerialPortData() {
    QByteArray data = serialPort->readAll();
    receivedBuffer.append(data);

//    if (receivedBuffer.length() == 14) {
//        for (int i = 0; i < receivedBuffer.length(); ++i) {
//            std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0')
//                      << (static_cast<char>(receivedBuffer[i]) & 0xFF) << " ";
//        }
//        std::cout << std::endl;
//    }

    // Check if a newline character is received
    if (receivedBuffer.contains('\n')) {
        //QString receivedLine = QString::fromLocal8Bit(receivedBuffer.trimmed());
        //qDebug() << receivedLine;
        canvasWidget->updateSerialData(receivedBuffer);

        // Clear the buffer for the next line
        receivedBuffer.clear();
    }
}

void MainWindow::loadUSBDevices() {
    //QString d = "/dev/ttyUSB0";
    //deviceNames.push_back(d);
    deviceNames.clear();
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &portInfo : serialPortInfos) {
        if ((portInfo.hasVendorIdentifier() && portInfo.vendorIdentifier() == vendorId) &&
            (portInfo.hasProductIdentifier() && portInfo.productIdentifier() == productId)) {
            deviceNames.push_back(portInfo.systemLocation());
        }
    }
}
