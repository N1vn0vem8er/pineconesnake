#include "mainwindow.h"
#include "src/ui_mainwindow.h"
#include <QInputDialog>
#include <QMessageBox>
#include <limits>
#include <QSettings>

#define VERSION "0.1.0"
#define LICENSELINK "https://www.gnu.org/licenses/gpl-3.0.html"
#define PROJECTLINK "https://github.com/N1vn0vem8er/pineconesnake"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSettings settings("pcscounter");
    restoreState(settings.value("state").toByteArray());
    restoreGeometry(settings.value("geometry").toByteArray());
    ui->actionSide_Panel->setChecked(settings.value("showSidePanel", true).toBool());
    ui->sidePanel->setVisible(ui->actionSide_Panel->isChecked());
    model = new QStringListModel(ui->listView);
    ui->listView->setModel(model);
    ui->splitter->setStretchFactor(1, 1);
    counterNameLabel = new QLabel(ui->statusbar);
    ui->statusbar->addPermanentWidget(counterNameLabel);
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
    connect(ui->actionNext, &QAction::triggered, this, &MainWindow::next);
    connect(ui->actionPrevious, &QAction::triggered, this, &MainWindow::previous);
    counters.append(Counter(tr("Default")));
    updateModel();
    updateCounter();
}

MainWindow::~MainWindow()
{
    QSettings settings("pcscounter");
    settings.setValue("state", saveState());
    settings.setValue("geometry", saveGeometry());
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

void MainWindow::updateCounter()
{
    counterNameLabel->setText(tr("Counter: %1").arg(counters[currentCounter].name));
    ui->counterLabel->setText(QString::number(counters[currentCounter].currentValue));
}

void MainWindow::increase()
{
    counters[currentCounter].currentValue += counters[currentCounter].step;
    updateCounter();
}

void MainWindow::decrease()
{
    counters[currentCounter].currentValue -= counters[currentCounter].step;
    updateCounter();
}

void MainWindow::reset()
{
    counters[currentCounter].currentValue = 0;
    updateCounter();
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
        updateCounter();
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
                updateCounter();
            }
        }
    }
}

void MainWindow::openCounter(const QModelIndex &index)
{
    currentCounter = index.row();
    updateCounter();
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
    QMessageBox::about(this, tr("About"), tr(R"(
    <html>
        <body>
            <h3>PCS Counter</h3>
            <p>PCS Counter is just a counter. It is a part of the <a href="%1">Pinecone Snake</a> project.</p>
            <p>Version: %2</p>
            <p>License: <a href="%3">GPL 3</a></p>
        </body
    </html>
    )").arg(PROJECTLINK, VERSION, LICENSELINK));
}

void MainWindow::next()
{
    if(currentCounter < counters.length() - 1)
        currentCounter++;
    else
        currentCounter = 0;
    updateCounter();
}

void MainWindow::previous()
{
    if(currentCounter > 0)
        currentCounter--;
    else
        currentCounter = counters.length() - 1;
    updateCounter();
}
