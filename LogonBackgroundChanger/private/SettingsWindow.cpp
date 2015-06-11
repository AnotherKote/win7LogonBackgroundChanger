#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QFileDialog>
#include <QListWidget>
#include <QDir>
#include <QMessageBox>
#include <QSettings>

#include "SettingsWindow.hpp"
#include <QDebug>

SettingsWindow::SettingsWindow(QWidget *parent)
: QWidget(parent)
, m_pcurrentDir(nullptr)
, m_pbrowseButton(nullptr)
, m_paddDir(nullptr)
, m_premDir(nullptr)
, m_psave(nullptr)
, m_pcancel(nullptr)
, m_pdirectoryList(nullptr)
, m_psettings(nullptr)
{
    this->setWindowFlags(Qt::Dialog);

    m_pcurrentDir = new QLineEdit(this);
    m_pbrowseButton = new QPushButton(this); ///<@todo set folder icon for this button
    m_paddDir = new QPushButton(this);
    m_premDir = new QPushButton(this);
    m_psave = new QPushButton(this);
    m_pcancel = new QPushButton(this);
    m_pdirectoryList = new QListWidget(this);
    m_psettings = new QSettings(this);

    m_paddDir->setText("&Add");
    m_premDir->setText("&Remove");
    m_psave->setText("&Save");
    m_pcancel->setText("&Cancel");

    m_paddDir->setEnabled(false);
    m_premDir->setEnabled(false);

    connect(m_pcurrentDir, SIGNAL(textChanged(QString)), this, SLOT(setAddButtonEnabled(QString)));
    connect(m_pdirectoryList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(listSelectionChanged(QListWidgetItem*,QListWidgetItem*)));
    connect(m_pbrowseButton, SIGNAL(clicked(bool)), this, SLOT(fileDialogOpen()));
    connect(m_paddDir, SIGNAL(clicked(bool)), this, SLOT(addDirToList()));
    connect(m_premDir, SIGNAL(clicked(bool)), this, SLOT(remDirFromList()));
    connect(m_psave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
    connect(m_pcancel, SIGNAL(clicked(bool)), this, SLOT(close()));

    QHBoxLayout *browseLayout = new QHBoxLayout();
    browseLayout->addWidget(m_pcurrentDir);
    browseLayout->addWidget(m_pbrowseButton);

    QHBoxLayout *addRemoveLayout = new QHBoxLayout();
    addRemoveLayout->addWidget(m_paddDir);
    addRemoveLayout->addWidget(m_premDir);

    QHBoxLayout *cancelSaveLayout = new QHBoxLayout();
    cancelSaveLayout->addWidget(m_pcancel);
    cancelSaveLayout->addStretch(1);
    cancelSaveLayout->addWidget(m_psave);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(browseLayout);
    mainLayout->addLayout(addRemoveLayout);
    mainLayout->addWidget(m_pdirectoryList);
    mainLayout->addLayout(cancelSaveLayout);

    QSize winSize(800, 400);
    resize(winSize);
}

void SettingsWindow::fileDialogOpen()
{
    m_pdirectoryList->addItem(QFileDialog::getExistingDirectory());
}

void SettingsWindow::addDirToList()
{
    QString currentDir = m_pcurrentDir->text();
    QDir curDir(currentDir);
    if(curDir.exists()){
        if(m_pdirectoryList->findItems(currentDir, Qt::MatchFixedString).isEmpty()){
            m_pdirectoryList->addItem(curDir.path());
        } else {
            QMessageBox::information(this, "Info", "Already in list.");
        }
    } else {
        QMessageBox::warning(this, "Path error", "Path doesn't exist!");
    }
}

void SettingsWindow::remDirFromList()
{
   if(m_pdirectoryList->currentItem() != nullptr){
      delete m_pdirectoryList->currentItem();
   }
}

void SettingsWindow::setAddButtonEnabled(QString currentDir)
{
   m_paddDir->setDisabled(currentDir.isEmpty());
}

void SettingsWindow::listSelectionChanged(QListWidgetItem* currentItem, QListWidgetItem*)
{
   m_premDir->setEnabled(currentItem != nullptr); //disable-enable button "remove from list"

   if(currentItem != nullptr){
      m_pcurrentDir->setText(currentItem->text());
   }
}

void SettingsWindow::saveSettings()
{
   int index = 0;
   m_psettings->remove("/Dirs/");
   for(auto dir: m_pdirectoryList->findItems(".*", Qt::MatchRegExp)){
      m_psettings->setValue("/Dirs/" + QString::number(index++), dir->text());
   }
   this->hide();
   emit settingsChanged();
}


void SettingsWindow::showEvent(QShowEvent *)
{
   m_pdirectoryList->clear();
   for(int index = 0; !m_psettings->value("/Dirs/" + QString::number(index)).toString().isEmpty(); ++index){
      m_pdirectoryList->addItem(m_psettings->value("/Dirs/" + QString::number(index)).toString());
   }
}
