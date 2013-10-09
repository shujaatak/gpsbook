#ifndef DIALOGSELECTSYMBOL_H
#define DIALOGSELECTSYMBOL_H

#include <QtGlobal>
#if ( QT_VERSION >= QT_VERSION_CHECK(5, 0, 0) )
#include <QtWidgets/QDialog>
#else
#include <QDialog>
#endif

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
