#ifndef DIALOGSELECTSYMBOL_H
#define DIALOGSELECTSYMBOL_H

#include <QDialog>

namespace Ui {
    class DialogSelectSymbol;
}

class DialogSelectSymbol : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSelectSymbol(QWidget *parent = 0);
    ~DialogSelectSymbol();
    QString getSelectedSymbolName();
    QString source;
    QString iconName;
    void setSelectedIcon(QString name);
    QIcon getIconByName(QString name);

private:
    Ui::DialogSelectSymbol *ui;

private slots:
    void on_comboBox_currentIndexChanged(QString );
};

#endif // DIALOGSELECTSYMBOL_H
