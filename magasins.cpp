#include "magasins.h"
// Inclut le fichier d'en-tête de la classe Magasins, qui définit les attributs et les prototypes des méthodes.

#include <QDebug>
// Fournit des outils pour afficher des messages de débogage dans la console.

#include <QSqlQuery>
// Permet d'exécuter des requêtes SQL pour interagir avec la base de données.

#include <QSqlError>
// Gère les erreurs liées aux requêtes SQL.

#include <QRegularExpression>
// Fournit des outils pour valider les données (par exemple, le numéro de téléphone).

#include <QMessageBox>
// Permet d'afficher des boîtes de dialogue pour informer ou alerter l'utilisateur.

// Constructeur par défaut
Magasins::Magasins()
    : id(0),
    NomMagasin(""),
    TypeDeMagasin(""),
    Emplacement(""),
    Surface(0.0),
    Telephone(""),
    HeureOuverture(QTime(0, 0)),
    HeureFermeture(QTime(0, 0)),
    DateDeCreation(QDate::currentDate())
// Initialise les attributs avec des valeurs par défaut :
// - id à 0
// - Chaînes vides pour les noms, type, emplacement et téléphone
// - Surface à 0.0
// - Heures d'ouverture/fermeture à 00:00
// - Date de création à la date actuelle
{
}

// Constructeur paramétré
Magasins::Magasins(int id, QString nomMagasin, QString typeDeMagasin, QString emplacement,
                   double surface, QString telephone, QTime heureOuverture, QTime heureFermeture,
                   QDate dateDeCreation)
    : id(id),
    NomMagasin(nomMagasin),
    TypeDeMagasin(typeDeMagasin),
    Emplacement(emplacement),
    Surface(surface),
    Telephone(telephone),
    HeureOuverture(heureOuverture),
    HeureFermeture(heureFermeture),
    DateDeCreation(dateDeCreation)
// Initialise les attributs avec les valeurs fournies lors de la création de l'objet.
{
}

// Ajouter un magasin
bool Magasins::ajouter()
{
    // Contrôles de saisie
    if (id <= 0) {
        QMessageBox::warning(nullptr, "Erreur", "L'ID du magasin doit être un nombre positif.");
        return false;
    }
    // Vérifie que l'ID est positif.

    if (NomMagasin.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le nom du magasin est obligatoire.");
        return false;
    }
    // Vérifie que le nom du magasin n'est pas vide (après suppression des espaces).

    if (TypeDeMagasin.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le type de magasin est obligatoire.");
        return false;
    }
    // Vérifie que le type de magasin n'est pas vide.

    if (Emplacement.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "L'emplacement est obligatoire.");
        return false;
    }
    // Vérifie que l'emplacement n'est pas vide.

    if (Surface <= 0) {
        QMessageBox::warning(nullptr, "Erreur", "La surface doit être un nombre positif.");
        return false;
    }
    // Vérifie que la surface est positive.

    if (!HeureOuverture.isValid() || !HeureFermeture.isValid() || HeureOuverture >= HeureFermeture) {
        QMessageBox::warning(nullptr, "Erreur", "L'heure d'ouverture doit être valide et avant l'heure de fermeture.");
        return false;
    }
    // Vérifie que les heures d'ouverture et de fermeture sont valides et que l'ouverture précède la fermeture.

    if (!DateDeCreation.isValid() || DateDeCreation > QDate::currentDate()) {
        QMessageBox::warning(nullptr, "Erreur", "La date de création doit être valide et non dans le futur.");
        return false;
    }
    // Vérifie que la date de création est valide et non future.

    QRegularExpression regexTel(QStringLiteral("^\\d{8}$"));
    if (!regexTel.match(Telephone).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur", "Le numéro de téléphone doit contenir exactement 8 chiffres.");
        return false;
    }
    // Vérifie que le numéro de téléphone contient exactement 8 chiffres.

    QSqlQuery query;
    query.prepare(QStringLiteral(
        "INSERT INTO MAGASINS (ID_MAGASIN, NOM_MAGASIN, TYPE_DE_MAGASIN, EMPLACEMENT, SURFACE, "
        "HEURES_OUVERTURE, HEURES_FERMETURE, DATE_CREATION, TELEPHONE) "
        "VALUES (:id, :nom, :type, :emplacement, :surface, :ouverture, :fermeture, :date_creation, :telephone)"
        ));
    // Prépare une requête SQL pour insérer un magasin dans la table MAGASINS.

    query.bindValue(":id", id);
    query.bindValue(":nom", NomMagasin);
    query.bindValue(":type", TypeDeMagasin);
    query.bindValue(":emplacement", Emplacement);
    query.bindValue(":surface", Surface);
    query.bindValue(":ouverture", HeureOuverture.toString("HH:mm"));
    query.bindValue(":fermeture", HeureFermeture.toString("HH:mm"));
    query.bindValue(":date_creation", DateDeCreation);
    query.bindValue(":telephone", Telephone);
    // Lie les valeurs des attributs aux paramètres de la requête.

    bool success = query.exec();
    // Exécute la requête et stocke le résultat (succès ou échec).

    /*if (!success) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de l'ajout : " + query.lastError().text());
    } else {
        QMessageBox::information(nullptr, "Succès", "Magasin ajouté avec succès !");
    }*/
    // Code commenté : aurait affiché un message de succès ou d'erreur (géré dans interfacePer.cpp).

    return success;
    // Retourne true si l'insertion a réussi, false sinon.
}

// Supprimer un magasin
bool Magasins::supprimer(int id)
{
    if (id <= 0) {
        QMessageBox::warning(nullptr, "Erreur", "L'ID du magasin doit être un nombre positif.");
        return false;
    }
    // Vérifie que l'ID est positif.

    QSqlQuery checkQuery;
    checkQuery.prepare(QStringLiteral("SELECT COUNT(*) FROM MAGASINS WHERE ID_MAGASIN = :id"));
    checkQuery.bindValue(":id", id);
    if (!checkQuery.exec() || !checkQuery.next() || checkQuery.value(0).toInt() == 0) {
        QMessageBox::warning(nullptr, "Erreur", "L'ID du magasin n'existe pas.");
        return false;
    }
    // Vérifie si l'ID existe dans la base de données.

    QSqlQuery query;
    query.prepare(QStringLiteral("DELETE FROM MAGASINS WHERE ID_MAGASIN = :id"));
    query.bindValue(":id", id);
    // Prépare une requête SQL pour supprimer le magasin avec l'ID spécifié.

    bool success = query.exec();
    if (!success) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la suppression : " + query.lastError().text());
    } else {
        QMessageBox::information(nullptr, "Succès", "Magasin supprimé avec succès !");
    }
    // Exécute la requête et affiche un message de succès ou d'erreur.

    return success;
    // Retourne true si la suppression a réussi, false sinon.
}

// Afficher tous les magasins
QSqlQueryModel* Magasins::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    // Crée un modèle pour stocker les résultats de la requête.

    QSqlQuery query;
    query.prepare(QStringLiteral("SELECT * FROM MAGASINS"));
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'affichage :" << query.lastError().text();
        delete model;
        return nullptr;
    }
    // Prépare et exécute une requête pour sélectionner tous les magasins.

    model->setQuery(std::move(query));
    // Associe la requête au modèle.

    return model;
    // Retourne le modèle contenant les données des magasins.
}

// Modifier un magasin
bool Magasins::modifier(int id)
{
    if (id <= 0) {
        QMessageBox::warning(nullptr, "Erreur", "L'ID du magasin doit être un nombre positif.");
        return false;
    }
    if (NomMagasin.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le nom du magasin est obligatoire.");
        return false;
    }
    if (TypeDeMagasin.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le type de magasin est obligatoire.");
        return false;
    }
    if (Emplacement.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "L'emplacement est obligatoire.");
        return false;
    }
    if (Surface <= 0) {
        QMessageBox::warning(nullptr, "Erreur", "La surface doit être un nombre positif.");
        return false;
    }
    if (!HeureOuverture.isValid() || !HeureFermeture.isValid() || HeureOuverture >= HeureFermeture) {
        QMessageBox::warning(nullptr, "Erreur", "L'heure d'ouverture doit être valide et avant l'heure de fermeture.");
        return false;
    }
    if (!DateDeCreation.isValid() || DateDeCreation > QDate::currentDate()) {
        QMessageBox::warning(nullptr, "Erreur", "La date de création doit être valide et non dans le futur.");
        return false;
    }
    // Vérifie les mêmes contraintes que pour l'ajout.

    QRegularExpression regexTel(QStringLiteral("^\\d{8}$"));
    if (!regexTel.match(Telephone).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur", "Le numéro de téléphone doit contenir exactement 8 chiffres.");
        return false;
    }
    // Vérifie le format du numéro de téléphone.

    QSqlQuery checkQuery;
    checkQuery.prepare(QStringLiteral("SELECT COUNT(*) FROM MAGASINS WHERE ID_MAGASIN = :id"));
    checkQuery.bindValue(":id", id);
    if (!checkQuery.exec() || !checkQuery.next() || checkQuery.value(0).toInt() == 0) {
        QMessageBox::warning(nullptr, "Erreur", "L'ID du magasin n'existe pas.");
        return false;
    }
    // Vérifie si l'ID existe dans la base de données.

    QSqlQuery query;
    query.prepare(QStringLiteral(
        "UPDATE MAGASINS SET NOM_MAGASIN = :nom, TYPE_DE_MAGASIN = :type, EMPLACEMENT = :emplacement, "
        "SURFACE = :surface, TELEPHONE = :telephone, HEURES_OUVERTURE = :ouverture, HEURES_FERMETURE = :fermeture, "
        "DATE_CREATION = :date_creation WHERE ID_MAGASIN = :id"
        ));
    // Prépare une requête SQL pour mettre à jour les informations du magasin.

    query.bindValue(":id", id);
    query.bindValue(":nom", NomMagasin);
    query.bindValue(":type", TypeDeMagasin);
    query.bindValue(":emplacement", Emplacement);
    query.bindValue(":surface", Surface);
    query.bindValue(":telephone", Telephone);
    query.bindValue(":ouverture", HeureOuverture.toString("HH:mm"));
    query.bindValue(":fermeture", HeureFermeture.toString("HH:mm"));
    query.bindValue(":date_creation", DateDeCreation);
    // Lie les valeurs des attributs aux paramètres de la requête.

    bool success = query.exec();
    if (!success) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la modification : " + query.lastError().text());
    } else {
        QMessageBox::information(nullptr, "Succès", "Magasin modifié avec succès !");
    }
    // Exécute la requête et affiche un message de succès ou d'erreur.

    return success;
    // Retourne true si la modification a réussi, false sinon.
}

// Trier les magasins
QSqlQueryModel* Magasins::trier(const QString& critere)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    // Crée un modèle pour stocker les résultats triés.

    QStringList allowedColumns = {
        "ID_MAGASIN",
        "NOM_MAGASIN",
        "TYPE_DE_MAGASIN",
        "EMPLACEMENT",
        "SURFACE",
        "TELEPHONE",
        "HEURES_OUVERTURE",
        "HEURES_FERMETURE",
        "DATE_CREATION"
    };
    // Liste des colonnes valides pour le tri.

    QString sanitizedCritere = critere.toUpper();
    if (!allowedColumns.contains(sanitizedCritere)) {
        qDebug() << "Critère de tri invalide :" << critere;
        QMessageBox::warning(nullptr, "Erreur", "Critère de tri invalide.");
        delete model;
        return nullptr;
    }
    // Vérifie que le critère de tri est valide (protection contre les injections SQL).

    QSqlQuery query;
    query.prepare(QString("SELECT * FROM MAGASINS ORDER BY %1").arg(sanitizedCritere));
    if (!query.exec()) {
        qDebug() << "Erreur lors du tri :" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Échec du tri : " + query.lastError().text());
        delete model;
        return nullptr;
    }
    // Prépare et exécute une requête pour trier les magasins selon le critère.

    model->setQuery(std::move(query));
    // Associe la requête au modèle.

    return model;
    // Retourne le modèle avec les données triées.
}

QSqlQueryModel* Magasins::rechercher(const QString& critere, const QString& valeur)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    // Crée un modèle pour stocker les résultats de la recherche.

    QMap<QString, QString> critereMap = {
        {"nom", "NOM_MAGASIN"},
        {"type", "TYPE_DE_MAGASIN"},
        {"emplacement", "EMPLACEMENT"},
        {"telephone", "TELEPHONE"},
        {"id", "ID_MAGASIN"}
    };
    // Mappage des critères conviviaux vers les noms de colonnes de la base de données.

    QString key = critere.trimmed().toLower();
    if (!critereMap.contains(key)) {
        qDebug() << "Critère invalide :" << critere;
        QMessageBox::warning(nullptr, "Erreur", "Critère de recherche invalide.");
        delete model;
        return nullptr;
    }
    // Vérifie que le critère de recherche est valide.

    QString dbColumn = critereMap[key];
    // Récupère le nom de la colonne correspondante.

    if (valeur.trimmed().isEmpty()) {
        qDebug() << "Valeur de recherche vide.";
        QMessageBox::warning(nullptr, "Erreur", "Veuillez entrer une valeur à rechercher.");
        delete model;
        return nullptr;
    }
    // Vérifie que la valeur de recherche n'est pas vide.

    QSqlQuery query;
    query.prepare(QString("SELECT * FROM MAGASINS WHERE %1 LIKE :valeur").arg(dbColumn));
    query.bindValue(":valeur", "%" + valeur + "%");
    // Prépare une requête pour rechercher les magasins avec une correspondance partielle.

    if (!query.exec()) {
        qDebug() << "Erreur SQL :" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la recherche : " + query.lastError().text());
        delete model;
        return nullptr;
    }
    // Exécute la requête.

    model->setQuery(std::move(query));
    // Associe la requête au modèle.

    if (model->rowCount() == 0) {
        qDebug() << "Aucun résultat trouvé.";
        QMessageBox::information(nullptr, "Résultat", "Aucun résultat trouvé pour la recherche.");
        delete model;
        return nullptr;
    }
    // Si aucun résultat n'est trouvé, informe l'utilisateur et retourne nullptr.

    return model;
    // Retourne le modèle avec les résultats de la recherche.
}

QSqlQueryModel* Magasins::rechercherParDate(const QDate& date)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    // Crée un modèle et une requête pour rechercher des commandes par date.

    query.prepare("SELECT id_commande, nom_client, produit, etat, date_commande FROM commandes WHERE date_commande = :date");
    query.bindValue(":date", date);
    // Prépare une requête pour sélectionner les commandes à une date spécifique.

    if (!query.exec()) {
        qDebug() << "Erreur dans rechercherParDate:" << query.lastError().text();
    }
    // Exécute la requête et affiche une erreur en cas d'échec.

    model->setQuery(query);
    // Associe la requête au modèle.

    return model;
    // Retourne le modèle avec les commandes correspondantes.
}
