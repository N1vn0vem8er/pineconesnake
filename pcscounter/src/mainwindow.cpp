#include "mainwindow.h"
#include "src/ui_mainwindow.h"
#include <QInputDialog>
#include <QMessageBox>
#include <limits>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSettings settings("pcscounter");
    ui->actionSide_Panel->setChecked(settings.value("showSidePanel", true).toBool());
    ui->sidePanel->setVisible(ui->actionSide_Panel->isChecked());
    model = new QStringListModel(ui->listView);
    ui->listView->setModel(model);
    ui->splitter->setStretchFactor(1, 1);
    connect(ui->increaseButton, &QPushButton::clicked, this, &MainWindow::increase);
    connect(ui->decreaseButton, &QPushButton::clicked, this, &MainWindow::decrease);
    connect(ui->resetButton, &QPushButton::clicked, this, &MainWindow::reset);
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::addCounter);
    connect(ui->removeButton, &QPushButton::clicked, this, &MainWindow::removeCounter);
    connect(ui->listView, &QListView::clicked, this, &MainWindow::openCounter);
    connect(ui->actionSet_Step, &QAction::triggered, this, &MainWindow::setStep);
    connect(ui->actionIncrease, &QAction::triggered, this, &MainWindow::increase);
    connect(ui->actionDecrease, &QAction::triggered, this, &MainWindow::decrease);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::addCounter);
    connect(ui->actionRemove, &QAction::triggered, this, &MainWindow::removeCounter);
    connect(ui->actionReset, &QAction::triggered, this, &MainWindow::reset);
    connect(ui->actionExit, &QAction::triggered, qApp, &QApplication::closeAllWindows);
    connect(ui->actionSide_Panel, &QAction::triggered, ui->sidePanel, &QWidget::setVisible);
    connect(ui->actionAbout_Qt, &QAction::triggered, this, [&]{QMessageBox::aboutQt(this, tr("About Qt"));});
    connect(ui->actionAbout_PCS_Counter, &QAction::triggered, this, &MainWindow::showAbout);
    counters.append(Counter(tr("Default")));
    updateModel();
    ui->counterLabel->setText(QString::number(counters[currentCounter].currentValue));
}

MainWindow::~MainWindow()
{
    QSettings settings("pcscounter");
    settings.setValue("showSidePanel", ui->actionSide_Panel->isChecked());
    delete ui;
}

void MainWindow::updateModel()
{
    QStringList list;
    for(const Counter& i : std::as_const(counters))
    {
        list.append(i.name);
    }
    model->setStringList(list);
}

void MainWindow::increase()
{
    counters[currentCounter].currentValue += counters[currentCounter].step;
    ui->counterLabel->setText(QString::number(counters[currentCounter].currentValue));
}

void MainWindow::decrease()
{
    counters[currentCounter].currentValue -= counters[currentCounter].step;
    ui->counterLabel->setText(QString::number(counters[currentCounter].currentValue));
}

void MainWindow::reset()
{
    counters[currentCounter].currentValue = 0;
    ui->counterLabel->setText(QString::number(counters[currentCounter].currentValue));
}

void MainWindow::addCounter()
{
    bool ok;
    const QString name = QInputDialog::getText(this, tr("Add Counter"), tr("Name for counter"), QLineEdit::Normal, "", &ok);
    if(ok && !name.isEmpty())
    {
        Counter counter(name);
        counters.append(counter);
        updateModel();
        currentCounter = counters.length() - 1;
        ui->counterLabel->setText(QString::number(counters[currentCounter].currentValue));
    }
}

void MainWindow::removeCounter()
{
    if(counters.length() > 1 && ui->listView->selectionModel()->hasSelection())
    {
        const QModelIndex index = ui->listView->selectionModel()->selectedIndexes()[0];
        if(index.row() > 0 && index.row() < counters.length())
        {
            if(QMessageBox::question(this, tr("Remove Counter"), tr("Remove counter %1?").arg(index.data().toString()), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
            {
                counters.removeAt(index.row());
                currentCounter = index.row() - 1;
                updateModel();
                ui->counterLabel->setText(QString::number(counters[currentCounter].currentValue));
            }
        }
    }
}

void MainWindow::openCounter(const QModelIndex &index)
{
    currentCounter = index.row();
    ui->counterLabel->setText(QString::number(counters[currentCounter].currentValue));
}

void MainWindow::setStep()
{
    bool ok;
    const int step = QInputDialog::getInt(this, tr("Set Step"), tr("Step"), counters[currentCounter].step, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), 1, &ok);
    if(ok)
        counters[currentCounter].step = step;
}

void MainWindow::showAbout()
{

}
