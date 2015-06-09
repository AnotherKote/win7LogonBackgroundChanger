#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>

class QLineEdit;
class QPushButton;
class QListWidget;
class QListWidgetItem;
class QSettings;

class SettingsWindow : public QWidget
{
    Q_OBJECT

private:
    QLineEdit *m_pcurrentDir;
    QPushButton *m_pbrowseButton;
    QPushButton *m_paddDir;
    QPushButton *m_premDir;
    QPushButton *m_psave;
    QPushButton *m_pcancel;
    QListWidget *m_pdirectoryList;
    QSettings *m_psettings;

public:
    explicit SettingsWindow(QWidget *parent = 0);

signals:
    void settingsChanged();
public slots:
    void fileDialogOpen();
    void addDirToList();
    void remDirFromList();
    void setAddButtonEnabled(QString currentDir);
    void listSelectionChanged(QListWidgetItem* currentItem, QListWidgetItem*);
    void saveSettings();

    // QWidget interface
protected:
    void showEvent(QShowEvent *);
};

#endif // SETTINGSWINDOW_H
