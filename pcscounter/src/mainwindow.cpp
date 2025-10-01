#include "mainwindow.h"
#include "src/ui_mainwindow.h"

#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new QStringListModel(ui->listView);
    ui->listView->setModel(model);
    ui->splitter->setStretchFactor(1, 1);
    connect(ui->increaseButton, &QPushButton::clicked, this, &MainWindow::increase);
    connect(ui->decreaseButton, &QPushButton::clicked, this, &MainWindow::decrease);
    connect(ui->resetButton, &QPushButton::clicked, this, &MainWindow::reset);
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::addCounter);
    connect(ui->removeButton, &QPushButton::clicked, this, &MainWindow::removeCounter);
    connect(ui->listView, &QListView::clicked, this, &MainWindow::openCounter);
    counters.append(Counter(tr("Default")));
    updateModel();
}

MainWindow::~MainWindow()
{
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
    if(ui->listView->selectionModel()->hasSelection())
    {
        const QModelIndex index = ui->listView->selectionModel()->selectedIndexes()[0];
        if(index.row() >= 0 && index.row() < counters.length())
        {
            if(QMessageBox::question(this, tr("Remove Counter"), tr("Remove counter %1?").arg(index.data().toString()), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
            {
                counters.removeAt(index.row());
                updateModel();
            }
        }
    }
}

void MainWindow::openCounter(const QModelIndex &index)
{
    currentCounter = index.row();
    ui->counterLabel->setText(QString::number(counters[currentCounter].currentValue));
}
