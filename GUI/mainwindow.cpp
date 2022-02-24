#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_si5351 = new Si5351("/dev/i2c-1");
    m_si5351->init(SI5351_CRYSTAL_LOAD_10PF, 0, 0);

    m_timerCheckStatus = new QTimer(this);
    m_timerCheckStatus->setInterval(500);
    m_timerCheckStatus->setSingleShot(true);
    connect(m_timerCheckStatus, &QTimer::timeout, this, &MainWindow::updateSi5351Status);

    //CLK0
    m_labelCLK0Freq = new QLabel("kHz");
    m_doublespinboxCLK0Freq = new QDoubleSpinBox;
    m_doublespinboxCLK0Freq->setRange(4, 225000);
    m_doublespinboxCLK0Freq->setSingleStep(10);
    m_doublespinboxCLK0Freq->setDecimals(3);
    m_doublespinboxCLK0Freq->setValue(10);
    m_doublespinboxCLK0Freq->setAlignment(Qt::AlignRight);
    QHBoxLayout* hboxLayout0 = new QHBoxLayout;
    hboxLayout0->addWidget(m_doublespinboxCLK0Freq, 100);
    hboxLayout0->addWidget(m_labelCLK0Freq, 0);
    m_groupboxCLK0 = new QGroupBox("CLK0");
    m_groupboxCLK0->setCheckable(true);
    m_groupboxCLK0->setChecked(false);
    m_groupboxCLK0->setLayout(hboxLayout0);
    //CLK1
    m_labelCLK1Freq = new QLabel("kHz");
    m_doublespinboxCLK1Freq = new QDoubleSpinBox;
    m_doublespinboxCLK1Freq->setRange(4, 225000);
    m_doublespinboxCLK1Freq->setSingleStep(100);
    m_doublespinboxCLK1Freq->setDecimals(3);
    m_doublespinboxCLK1Freq->setValue(1000);
    m_doublespinboxCLK1Freq->setAlignment(Qt::AlignRight);
    QHBoxLayout* hboxLayout1 = new QHBoxLayout;
    hboxLayout1->addWidget(m_doublespinboxCLK1Freq, 100);
    hboxLayout1->addWidget(m_labelCLK1Freq, 0);
    m_groupboxCLK1 = new QGroupBox("CLK1");
    m_groupboxCLK1->setCheckable(true);
    m_groupboxCLK1->setChecked(false);
    m_groupboxCLK1->setLayout(hboxLayout1);
    //CLK2
    m_labelCLK2Freq = new QLabel("kHz");
    m_doublespinboxCLK2Freq = new QDoubleSpinBox;
    m_doublespinboxCLK2Freq->setRange(4, 225000);
    m_doublespinboxCLK2Freq->setSingleStep(1000);
    m_doublespinboxCLK2Freq->setDecimals(3);
    m_doublespinboxCLK2Freq->setValue(100000);
    m_doublespinboxCLK2Freq->setAlignment(Qt::AlignRight);
    QHBoxLayout* hboxLayout2 = new QHBoxLayout;
    hboxLayout2->addWidget(m_doublespinboxCLK2Freq, 100);
    hboxLayout2->addWidget(m_labelCLK2Freq, 0);
    m_groupboxCLK2 = new QGroupBox("CLK2");
    m_groupboxCLK2->setCheckable(true);
    m_groupboxCLK2->setChecked(false);
    m_groupboxCLK2->setLayout(hboxLayout2);
    //Status
    m_labelSi5351Status = new QLabel;
    QGroupBox* groupboxStatus = new QGroupBox("Status");
    QHBoxLayout* hboxLayoutStatus = new QHBoxLayout;
    hboxLayoutStatus->addWidget(m_labelSi5351Status);
    groupboxStatus->setLayout(hboxLayoutStatus);

    QHBoxLayout* hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(m_groupboxCLK0);
    hboxLayout->addWidget(m_groupboxCLK1);
    hboxLayout->addWidget(m_groupboxCLK2);

    QVBoxLayout* vboxLayout = new QVBoxLayout;
    vboxLayout->addLayout(hboxLayout);
    vboxLayout->addWidget(groupboxStatus);

    QWidget* w = new QWidget;
    w->setLayout(vboxLayout);

    setCentralWidget(w);

    //Connect signals
    connect(m_groupboxCLK0, &QGroupBox::toggled, this, &MainWindow::enableCLK0);
    connect(m_groupboxCLK1, &QGroupBox::toggled, this, &MainWindow::enableCLK1);
    connect(m_groupboxCLK2, &QGroupBox::toggled, this, &MainWindow::enableCLK2);
    connect(m_doublespinboxCLK0Freq, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateFreqCLK0);
    connect(m_doublespinboxCLK1Freq, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateFreqCLK1);
    connect(m_doublespinboxCLK2Freq, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateFreqCLK2);
}

MainWindow::~MainWindow()
{
    delete m_si5351;
}

void MainWindow::enableCLK0(bool bEnable)
{
    if(bEnable){
        updateFreqCLK0(m_doublespinboxCLK0Freq->value());
        m_si5351->output_enable(SI5351_CLK0, true);
    } else {
        m_si5351->output_enable(SI5351_CLK0, false);
    }
}

void MainWindow::enableCLK1(bool bEnable)
{
    if(bEnable){
        updateFreqCLK1(m_doublespinboxCLK1Freq->value());
        m_si5351->output_enable(SI5351_CLK1, true);
    } else {
        m_si5351->output_enable(SI5351_CLK1, false);
    }
}

void MainWindow::enableCLK2(bool bEnable)
{
    if(bEnable){
        updateFreqCLK2(m_doublespinboxCLK2Freq->value());
        m_si5351->output_enable(SI5351_CLK2, true);
    } else {
        m_si5351->output_enable(SI5351_CLK2, false);
    }
}

void MainWindow::updateFreqCLK0(double dfFreq)
{
    uint64_t freq = dfFreq*100000;
    uint8_t setOK = m_si5351->set_freq(freq, SI5351_CLK0);
    if(setOK == 0){
        //OK
        m_timerCheckStatus->start();
        m_labelCLK0Freq->setStyleSheet("QLabel { color: black; }");
        m_labelSi5351Status->clear();
    } else {
        //Error set_freq
        m_labelCLK0Freq->setStyleSheet("QLabel { color: red; }");
    }
}

void MainWindow::updateFreqCLK1(double dfFreq)
{
    uint64_t freq = dfFreq*100000;
    uint8_t setOK = m_si5351->set_freq(freq, SI5351_CLK1);
    if(setOK == 0){
        //OK
        m_timerCheckStatus->start();
        m_labelCLK1Freq->setStyleSheet("QLabel { color: black; }");
        m_labelSi5351Status->clear();
    } else {
        //Error set_freq
        m_labelCLK1Freq->setStyleSheet("QLabel { color: red; }");
    }
}

void MainWindow::updateFreqCLK2(double dfFreq)
{
    uint64_t freq = dfFreq*100000;
    uint8_t setOK = m_si5351->set_freq(freq, SI5351_CLK2);
    if(setOK == 0){
        //OK
        m_timerCheckStatus->start();
        m_labelCLK2Freq->setStyleSheet("QLabel { color: black; }");
        m_labelSi5351Status->clear();
    } else {
        //Error set_freq
        m_labelCLK2Freq->setStyleSheet("QLabel { color: red; }");
    }
}

void MainWindow::updateSi5351Status()
{
    m_si5351->update_status();
    m_labelSi5351Status->setText(QString("SYS_INT: %1    LOL_A: %2    LOL_B: %3    LOS: %4    REVID: %5")
                                 .arg(m_si5351->dev_status.SYS_INIT)
                                 .arg(m_si5351->dev_status.LOL_A)
                                 .arg(m_si5351->dev_status.LOL_B)
                                 .arg(m_si5351->dev_status.LOS)
                                 .arg(m_si5351->dev_status.REVID));
}
