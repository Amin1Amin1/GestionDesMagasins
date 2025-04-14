#include "magasins.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegularExpression>
#include <QMessageBox>

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

    // Vérification du numéro de téléphone
    QRegularExpression regexTel(QStringLiteral("^\\d{8}$"));
    if (!regexTel.match(Telephone).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur", "Le numéro de téléphone doit contenir exactement 8 chiffres.");
        return false;
    }

    // Requête SQL
    QSqlQuery query;
    query.prepare(QStringLiteral(
        "INSERT INTO MAGASINS (ID_MAGASIN, NOM_MAGASIN, TYPE_DE_MAGASIN, EMPLACEMENT, SURFACE, "
        "HEURES_OUVERTURE, HEURES_FERMETURE, DATE_CREATION, TELEPHONE) "
        "VALUES (:id, :nom, :type, :emplacement, :surface, :ouverture, :fermeture, :date_creation, :telephone)"
        ));

    query.bindValue(":id", id);
    query.bindValue(":nom", NomMagasin);
    query.bindValue(":type", TypeDeMagasin);
    query.bindValue(":emplacement", Emplacement);
    query.bindValue(":surface", Surface);
    query.bindValue(":ouverture", HeureOuverture.toString("HH:mm"));
    query.bindValue(":fermeture", HeureFermeture.toString("HH:mm"));
    query.bindValue(":date_creation", DateDeCreation);
    query.bindValue(":telephone", Telephone);

    bool success = query.exec();
    /*if (!success) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de l'ajout : " + query.lastError().text());
    } else {
        QMessageBox::information(nullptr, "Succès", "Magasin ajouté avec succès !");
    }*/
    return success;
}

// Supprimer un magasin
bool Magasins::supprimer(int id)
{
    if (id <= 0) {
        QMessageBox::warning(nullptr, "Erreur", "L'ID du magasin doit être un nombre positif.");
        return false;
    }

    // Vérifier si l'ID existe
    QSqlQuery checkQuery;
    checkQuery.prepare(QStringLiteral("SELECT COUNT(*) FROM MAGASINS WHERE ID_MAGASIN = :id"));
    checkQuery.bindValue(":id", id);
    if (!checkQuery.exec() || !checkQuery.next() || checkQuery.value(0).toInt() == 0) {
        QMessageBox::warning(nullptr, "Erreur", "L'ID du magasin n'existe pas.");
        return false;
    }

    // Suppression
    QSqlQuery query;
    query.prepare(QStringLiteral("DELETE FROM MAGASINS WHERE ID_MAGASIN = :id"));
    query.bindValue(":id", id);

    bool success = query.exec();
    if (!success) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la suppression : " + query.lastError().text());
    } else {
        QMessageBox::information(nullptr, "Succès", "Magasin supprimé avec succès !");
    }

    return success;
}

// Afficher tous les magasins
QSqlQueryModel* Magasins::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();

    QSqlQuery query;
    query.prepare(QStringLiteral("SELECT * FROM MAGASINS"));
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'affichage :" << query.lastError().text();
        delete model;
        return nullptr;
    }

    model->setQuery(std::move(query));
    return model;
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

    QRegularExpression regexTel(QStringLiteral("^\\d{8}$"));
    if (!regexTel.match(Telephone).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur", "Le numéro de téléphone doit contenir exactement 8 chiffres.");
        return false;
    }

    // Vérifier si l'ID existe
    QSqlQuery checkQuery;
    checkQuery.prepare(QStringLiteral("SELECT COUNT(*) FROM MAGASINS WHERE ID_MAGASIN = :id"));
    checkQuery.bindValue(":id", id);
    if (!checkQuery.exec() || !checkQuery.next() || checkQuery.value(0).toInt() == 0) {
        QMessageBox::warning(nullptr, "Erreur", "L'ID du magasin n'existe pas.");
        return false;
    }

    // Modification
    QSqlQuery query;
    query.prepare(QStringLiteral(
        "UPDATE MAGASINS SET NOM_MAGASIN = :nom, TYPE_DE_MAGASIN = :type, EMPLACEMENT = :emplacement, "
        "SURFACE = :surface, TELEPHONE = :telephone, HEURES_OUVERTURE = :ouverture, HEURES_FERMETURE = :fermeture, "
        "DATE_CREATION = :date_creation WHERE ID_MAGASIN = :id"
        ));

    query.bindValue(":id", id);
    query.bindValue(":nom", NomMagasin);
    query.bindValue(":type", TypeDeMagasin);
    query.bindValue(":emplacement", Emplacement);
    query.bindValue(":surface", Surface);
    query.bindValue(":telephone", Telephone);
    query.bindValue(":ouverture", HeureOuverture.toString("HH:mm"));
    query.bindValue(":fermeture", HeureFermeture.toString("HH:mm"));
    query.bindValue(":date_creation", DateDeCreation);

    bool success = query.exec();
    if (!success) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la modification : " + query.lastError().text());
    } else {
        QMessageBox::information(nullptr, "Succès", "Magasin modifié avec succès !");
    }

    return success;
}

// Trier les magasins
QSqlQueryModel* Magasins::trier(const QString& critere)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    // Whitelist of allowed columns for sorting
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

    // Sanitize critere
    QString sanitizedCritere = critere.toUpper();
    if (!allowedColumns.contains(sanitizedCritere)) {
        qDebug() << "Critère de tri invalide :" << critere;
        QMessageBox::warning(nullptr, "Erreur", "Critère de tri invalide.");
        delete model;
        return nullptr;
    }

    QSqlQuery query;
    query.prepare(QString("SELECT * FROM MAGASINS ORDER BY %1").arg(sanitizedCritere));
    if (!query.exec()) {
        qDebug() << "Erreur lors du tri :" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Échec du tri : " + query.lastError().text());
        delete model;
        return nullptr;
    }

    model->setQuery(std::move(query));
    return model;
}


QSqlQueryModel* Magasins::rechercher(const QString& critere, const QString& valeur)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    // Mapping user-friendly criteria to database columns
    QMap<QString, QString> critereMap = {
        {"nom", "NOM_MAGASIN"},
        {"type", "TYPE_DE_MAGASIN"},
        {"emplacement", "EMPLACEMENT"},
        {"telephone", "TELEPHONE"},
        {"id", "ID_MAGASIN"}
    };

    QString key = critere.trimmed().toLower();
    if (!critereMap.contains(key)) {
        qDebug() << "Critère invalide :" << critere;
        QMessageBox::warning(nullptr, "Erreur", "Critère de recherche invalide.");
        delete model;
        return nullptr;
    }

    QString dbColumn = critereMap[key];

    if (valeur.trimmed().isEmpty()) {
        qDebug() << "Valeur de recherche vide.";
        QMessageBox::warning(nullptr, "Erreur", "Veuillez entrer une valeur à rechercher.");
        delete model;
        return nullptr;
    }

    QSqlQuery query;
    query.prepare(QString("SELECT * FROM MAGASINS WHERE %1 LIKE :valeur").arg(dbColumn));
    query.bindValue(":valeur", "%" + valeur + "%");

    if (!query.exec()) {
        qDebug() << "Erreur SQL :" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la recherche : " + query.lastError().text());
        delete model;
        return nullptr;
    }

    model->setQuery(std::move(query));

    if (model->rowCount() == 0) {
        qDebug() << "Aucun résultat trouvé.";
        QMessageBox::information(nullptr, "Résultat", "Aucun résultat trouvé pour la recherche.");
        delete model;
        return nullptr;
    }

    return model;
}
