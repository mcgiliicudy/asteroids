#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mos6502.h"

#include <QGraphicsTextItem>
#include <QGraphicsScene>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    stopRun = false;
    nextIntruction = false;
    NMIpressed = false;


    QGraphicsTextItem *textItem= new QGraphicsTextItem("fefefefefefeefe\nrtioiotrioter\nrwerew");
    textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
    textItem->setDefaultTextColor(QColor(255,255,100));
    textItem->setFont(ui->graphicsView->font());

    QGraphicsTextItem *textItem2= new QGraphicsTextItem("A: 00\nX: FF\nY: AF\n\nPC: 0A18");
//    textItem2->setTextInteractionFlags(Qt::TextEditorInteraction);
    textItem2->setDefaultTextColor(QColor(255,255,245));
    textItem2->setFont(ui->graphicsView->font());
    textItem2->setPos(QPointF(-100,-100));


    QGraphicsScene *scene = new QGraphicsScene();

    scene->addItem(textItem);
    scene->addItem(textItem2);

    ui->graphicsView->setScene(scene);

    auto scene2 = ui->graphicsView->scene();





}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showInfo(char *str, void *voidcpu, uint16_t pc, uint8_t A, uint8_t X, uint8_t Y)
{
    // Breakpoint.
//    if (pc==0x6811) stopRun=true;
//    if (pc==0x7B7B) stopRun=true;
//    if (pc==0x7B7A) stopRun=true;
//    if (pc==0x7B79) stopRun=true;
//    if (pc==0x7BC0) stopRun=true;
//    if (pc>=0x6822 && pc<=0x6827) stopRun=true;



    mos6502 *cpu = (mos6502 *)voidcpu;


    while (ui->listWidget->count()>100) ui->listWidget->takeItem(0);
    ui->listWidget->addItem(QString(str));
    ui->listWidget->scrollToItem(ui->listWidget->item(ui->listWidget->count()-1));

    ui->edtA->setText(QString::number(A));
    ui->edtX->setText(QString::number(X));
    ui->edtY->setText(QString::number(Y));
    QCoreApplication::processEvents();
    QCoreApplication::processEvents();
    QCoreApplication::processEvents();

    if (NMIpressed)
    {
        cpu->NMI();
        NMIpressed = false;
    }

    if (stopRun)
    {
        while (!nextIntruction)  QCoreApplication::processEvents();
        nextIntruction = false;
    }
}

void MainWindow::on_btnNext_clicked()
{
    nextIntruction = true;
}

void MainWindow::on_btnStop_toggled(bool checked)
{
    stopRun = checked;
}

void MainWindow::on_btnNMI_clicked()
{
    NMIpressed = true;
}
