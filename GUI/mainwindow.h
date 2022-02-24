#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QGroupBox>
#include <QDoubleSpinBox>
#include <QMainWindow>
#include <QTimer>
#include "../si5351.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QLabel* m_labelCLK0Freq;
    QLabel* m_labelCLK1Freq;
    QLabel* m_labelCLK2Freq;
    QGroupBox* m_groupboxCLK0;
    QGroupBox* m_groupboxCLK1;
    QGroupBox* m_groupboxCLK2;
    QDoubleSpinBox* m_doublespinboxCLK0Freq;
    QDoubleSpinBox* m_doublespinboxCLK1Freq;
    QDoubleSpinBox* m_doublespinboxCLK2Freq;
    QLabel* m_labelSi5351Status;

    Si5351* m_si5351;

    QTimer* m_timerCheckStatus;

private slots:
    void enableCLK0(bool bEnable);
    void enableCLK1(bool bEnable);
    void enableCLK2(bool bEnable);

    void updateFreqCLK0(double dfFreq);
    void updateFreqCLK1(double dfFreq);
    void updateFreqCLK2(double dfFreq);

    void updateSi5351Status();
};

#endif // MAINWINDOW_H
