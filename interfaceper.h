#ifndef INTERFACEPER_H
#define INTERFACEPER_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class interfacePer;
}
QT_END_NAMESPACE

class Metiers; // Déclaration anticipée

class interfacePer : public QMainWindow
{
    Q_OBJECT

public:
    interfacePer(QWidget *parent = nullptr);
    ~interfacePer();

private slots:
    void on_pushButton9_ajouter_clicked();
    void on_pushButton_10_afficher_clicked();
    void on_pushButton_12_supprimer_clicked();
    void on_pushButton_modifier_clicked();
    void on_pushButton_rechercher_clicked();
    void on_pushButton_trier_clicked();
    void on_pushButton_pdf_clicked();
    void on_pushButton_statistique_clicked();
    void on_calendarWidget_selectionChanged();
    void on_Commandes_clicked();
    void on_pushButton_gererCommandes_clicked(); // Nouveau slot



private:
    Ui::interfacePer *ui;
    Metiers *metiersDialog; // Pointeur vers le dialogue
};

#endif // INTERFACEPER_H
