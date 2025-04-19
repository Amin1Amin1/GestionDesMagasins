#ifndef COMMANDES_H
#define COMMANDES_H

#include <QDialog>
#include <QDate>
#include <QSqlQueryModel>

namespace Ui {
class Commandes;
}

class Commandes : public QDialog
{
    Q_OBJECT

public:
    explicit Commandes(QWidget *parent = nullptr); // constructeur pour interface
    ~Commandes();

    // Constructeurs métiers
    Commandes(); // constructeur vide
    Commandes(int id, QString nom_client, QString produit, QString etat, QDate date_commande);

    // Méthodes CRUD
    bool ajouter(); // Create
    QSqlQueryModel* afficher(const QString& dateStr); // Read par date
    QSqlQueryModel* afficher(); // Read tout
    bool supprimer(int id); // Delete
    bool modifier(int id); // Update

private:
    Ui::Commandes *ui;

    // Attributs métier
    int id_commande;
    QString nom_client;
    QString produit;
    QString etat;
    QDate date_commande;
};

#endif // COMMANDES_H
