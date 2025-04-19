// magasins.h
#ifndef MAGASINS_H
#define MAGASINS_H

#include <QString>
#include <QDate>
#include <QTime>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Magasins
{
public:
    Magasins();
    // Constructor declaration
    Magasins(int id, QString nomMagasin, QString typeDeMagasin, QString emplacement,
             double surface, QString telephone, QTime heureOuverture, QTime heureFermeture,
             QDate dateDeCreation);

    // Methods
    bool ajouter();
    bool supprimer(int id);
    QSqlQueryModel* afficher();
    bool modifier(int id);
    //  Recherche
    QSqlQueryModel* rechercher(const QString& critere, const QString& valeur);

    //  Tri
    QSqlQueryModel* trier(const QString& critere);

    QSqlQueryModel* rechercherParDate(const QDate& date);

private:
    int id;
    QString NomMagasin;
    QString TypeDeMagasin;
    QString Emplacement;
    double Surface;
    QString Telephone;
    QTime HeureOuverture;
    QTime HeureFermeture;
    //QString ListeDesEmployes;
    QDate DateDeCreation;
};

#endif // MAGASINS_H
