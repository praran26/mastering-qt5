#include "task.h"
#include "ui_task.h"
#include <QDebug>
#include <QInputDialog>
Task::Task(const QString &name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Task)
{
    ui->setupUi(this);
    setName(name);
    connect(ui->editButton,&QPushButton::clicked,this,&Task::rename);
    connect(ui->removeButton,&QPushButton::clicked,[this]{
    emit removed(this);
    });
    connect(ui->checkbox, &QCheckBox::toggled,
    this, &Task::checked);
}

Task::~Task()
{
    delete ui;
}

void Task::setName(const QString &name)
{
    ui->checkbox->setText(name);
}

bool Task::isCompleted() const{
qDebug()<< ui->checkbox->checkState();
return ui->checkbox->isChecked();
}

QString Task::name()const{
return ui->checkbox->text();
}

void Task::rename()
{
bool ok;
QString value = QInputDialog::getText(this, tr("Edit task"),
tr("Task name"),
QLineEdit::Normal,
this->name(), &ok);
if (ok && !value.isEmpty()) {
setName(value);
}
}

void Task::checked(bool checked)
{
QFont font(ui->checkbox->font());
font.setStrikeOut(checked);
ui->checkbox->setFont(font);
emit statusChanged(this);
}
