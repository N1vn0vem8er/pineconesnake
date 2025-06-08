#include "createeventdialog.h"
#include "mainwindow.h"
#include "eventwidget.h"
#include <QMessageBox>
#include "repeatedeventwidget.h"
#include "resourcesmanager.h"
#include "ui_mainwindow.h"

#define VERSION "0.0.1"
#define LICENSELINK "https://www.gnu.org/licenses/gpl-3.0.html"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->addReminderButton, &QPushButton::clicked, this, &MainWindow::addReminderPressed);
    connect(ui->actionAbout_Qt, &QAction::triggered, this, [this]{QMessageBox::aboutQt(this, tr("About Qt"));});
    connect(ui->actionAbout_PCS_Reminder, &QAction::triggered, this, &MainWindow::openAbout);
    connect(ui->actionExit, &QAction::triggered, qApp, &QApplication::quit);
    connect(ui->actionClose_Window, &QAction::triggered, this, &MainWindow::close);
    refresh();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refresh()
{
    if(layout!=nullptr)
    {
        delete layout;
        for(const auto& i : std::as_const(widgets)) delete i;
        widgets.clear();
    }
    layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    ResourcesManager* rm = ResourcesManager::getInstance();
    events = rm->getAllEvents();
    repeatedEvents = rm->getAllRepeating();
    for(const auto& i : std::as_const(repeatedEvents))
    {
        RepeatedEventWidget* widget = new RepeatedEventWidget(i, this);
        connect(widget, &RepeatedEventWidget::requestRefresh, this, &MainWindow::refresh);
        connect(ui->actionEnable_all, &QAction::triggered, widget, &RepeatedEventWidget::enable);
        connect(ui->actionDisable_all, &QAction::triggered, widget, &RepeatedEventWidget::disable);
        layout->addWidget(widget);
        widgets.append(widget);
    }
    for(const auto& i : std::as_const(events))
    {
        EventWidget* widget = new EventWidget(i, this);
        connect(widget, &EventWidget::requestRefresh, this, &MainWindow::refresh);
        connect(ui->actionEnable_all, &QAction::triggered, widget, &EventWidget::enable);
        connect(ui->actionDisable_all, &QAction::triggered, widget, &EventWidget::disable);
        layout->addWidget(widget);
        widgets.append(widget);
    }
    layout->addStretch();
    ui->scrollAreaWidgetContents->setLayout(layout);
    emit requestRefresh();
}

void MainWindow::addReminderPressed()
{
    CreateEventDialog* dialog = new CreateEventDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dialog, &CreateEventDialog::addEvent, this, &MainWindow::addReminder);
    connect(dialog, &CreateEventDialog::addRepeating, this, &MainWindow::addRepeating);
    dialog->show();
}

void MainWindow::addReminder(const EventManager::Event &event)
{
    ResourcesManager::getInstance()->saveEvent(event);
    refresh();
    emit requestRefresh();
}

void MainWindow::addRepeating(const EventManager::RepeatedEvent &event)
{
    ResourcesManager::getInstance()->saveRepeating(event);
    refresh();
    emit requestRefresh();
}

void MainWindow::openAbout()
{
    QMessageBox::about(this, tr("About PCS Reminder"), tr("<html><body><h3>PCS Reminder</h3><p>PCS Reminder is a simple reminder application. It is a part of Pinecone Snake project.</p><p>Version: %1</p><p>License: <a href=\"%2\">GPL 3</a></p></body></html>")
                                                           .arg(VERSION, LICENSELINK));
}
