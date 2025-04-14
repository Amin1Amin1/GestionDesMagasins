#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPdfWriter>
#include <QSqlQueryModel>
#include <QPainter>
#include <QSqlTableModel>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QPdfWriter>
#include <QPainter>
#include <QSqlQuery>
#include <QFileDialog>

#include "produit.h"
#include "interfaceper.h"
#include "magasins.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadProduitData();



private:
    Ui::MainWindow *ui;
    Produit produit;
    interfacePer *interfacePer;

private slots:
    void onItemSelected();

    void ajouter_produit();
    void supprimer_produit();
    void modifier_produit();

    void chercher_produit();
    void trier_produit();
    void statistics();
    void pdf_generator();
    void verifier_stock();
    void recordAction(const QString& actionType, const QString& productName = "");
    void showHistory();
    void clearHistory();





    void on_label_5_linkActivated(const QString &link);

};


#endif // MAINWINDOW_H
