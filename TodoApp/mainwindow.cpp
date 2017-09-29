#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QInputDialog>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mTasks()
{
    ui->setupUi(this);
    connect(ui->addTaskButton, &QPushButton::clicked,this,&MainWindow::addTask);
    updateStatus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addTask()
{
    qDebug()<<"Adding new task";
    bool ok;
    QString name=QInputDialog::getText(this,tr("Add task"),tr("Task name"),QLineEdit::Normal,tr("Untitled Task"),&ok);
    if(ok && !name.isEmpty()){
        Task* task=new Task(name);
        mTasks.append(task);
        connect(task, &Task::removed,
        this, &MainWindow::removeTask);
        connect(task, &Task::statusChanged, this,
        &MainWindow::taskStatusChanged);
        ui->tasksLayout->addWidget(task);
        updateStatus();
    }
}

void MainWindow::removeTask(Task* task)
{
    mTasks.removeOne(task);
    ui->tasksLayout->removeWidget(task);
    task->setParent(0);
    updateStatus();
    delete task;
}

void MainWindow::taskStatusChanged(Task *task)
{
    updateStatus();
}
void MainWindow::updateStatus()
{
    int completedCount=0;
    for(auto t:mTasks)completedCount+=(t->isCompleted());
    int todoCount=mTasks.size()-completedCount;
    ui->statusLabel->setText(QString("Status: %1 todo / %2 completed").arg(todoCount).arg(completedCount));
}
