#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>




namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void showInfo(char *str, void *voidcpu, uint16_t pc, uint8_t A, uint8_t X, uint8_t Y);

private slots:
    void on_btnNext_clicked();

    void on_btnStop_toggled(bool checked);

    void on_btnNMI_clicked();

private:
    Ui::MainWindow *ui;

    bool stopRun;
    bool nextIntruction;
    bool NMIpressed;


};

#endif // MAINWINDOW_H
