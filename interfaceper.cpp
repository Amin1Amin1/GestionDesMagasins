#include "interfaceper.h"
// Inclut le fichier d'en-tête de la classe interfacePer, qui définit la structure de la classe.

#include "ui_interfaceper.h"
// Inclut l'interface utilisateur générée automatiquement par Qt Designer pour interfacePer.

#include "magasins.h"
// Inclut la classe Magasins, probablement utilisée pour gérer les données des magasins.

#include <QMessageBox>
// Permet d'afficher des boîtes de dialogue pour informer ou alerter l'utilisateur.

#include <QDebug>
// Fournit des outils pour le débogage (affichage de messages dans la console).

#include <QSqlQueryModel>
// Classe pour représenter les résultats d'une requête SQL dans une table.

#include <QSqlError>
// Gère les erreurs liées aux requêtes SQL.

#include <QPdfWriter>
// Permet de créer des fichiers PDF.

#include <QPainter>
// Fournit des outils pour dessiner sur des surfaces comme un PDF.

#include <QFileDialog>
// Ouvre une boîte de dialogue pour sélectionner des fichiers.

#include <QTextDocument>
// Gère les documents texte formatés (non utilisé directement ici).

#include <QtCharts>
// Inclut la bibliothèque Qt Charts pour créer des graphiques.

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
// Classes spécifiques pour afficher des graphiques (camemberts, vues de graphiques).

#include <QVBoxLayout>
// Gère la disposition verticale des widgets.

#include <QSqlQuery>
// Permet d'exécuter des requêtes SQL.

#include <QDate>
// Représente et manipule des dates.

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
// Classes pour gérer les requêtes HTTP (pour les appels API).

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
// Classes pour manipuler des données JSON.

#include <cmath>
// Fournit des fonctions mathématiques (utilisé pour calculer des similarités).

#include <QTimer>
// Permet de programmer des événements temporisés (non utilisé ici).

#include <QProcess>
// Permet d'exécuter des programmes externes, comme un script Python.
#include <QSerialPortInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

interfacePer::interfacePer(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::interfacePer), networkManager(new QNetworkAccessManager(this)),
    serialPort(new QSerialPort(this))
// Constructeur de la classe interfacePer, hérite de QMainWindow.
// Initialise l'interface utilisateur (ui) et le gestionnaire de requêtes réseau (networkManager).

{
    ui->setupUi(this);
    setupSerialPort();
    // Configure l'interface utilisateur définie dans le fichier .ui.

    connect(ui->pushButton9_ajouter, &QPushButton::clicked, this, &interfacePer::on_pushButton9_ajouter_clicked);
    // Connecte le clic du bouton "Ajouter" à la fonction correspondante.

    connect(ui->pushButton_10_afficher, &QPushButton::clicked, this, &interfacePer::on_pushButton_10_afficher_clicked);
    // Connecte le bouton "Afficher" pour afficher les magasins.

    connect(ui->pushButton_12_supprimer, &QPushButton::clicked, this, &interfacePer::on_pushButton_12_supprimer_clicked);
    // Connecte le bouton "Supprimer" pour supprimer un magasin.

    connect(ui->pushButton_modifier, &QPushButton::clicked, this, &interfacePer::on_pushButton_modifier_clicked);
    // Connecte le bouton "Modifier" pour modifier un magasin.

    connect(ui->pushButton_rechercher, &QPushButton::clicked, this, &interfacePer::on_pushButton_rechercher_clicked);
    // Connecte le bouton "Rechercher" pour effectuer une recherche.

    connect(ui->pushButton_trier, &QPushButton::clicked, this, &interfacePer::on_pushButton_trier_clicked);
    // Connecte le bouton "Trier" pour trier les magasins.

    connect(ui->pushButton_pdf, &QPushButton::clicked, this, &interfacePer::on_pushButton_pdf_clicked);
    // Connecte le bouton "PDF" pour générer un fichier PDF.

    connect(ui->pushButton_statistique, &QPushButton::clicked, this, &interfacePer::on_pushButton_statistique_clicked);
    // Connecte le bouton "Statistique" pour afficher un graphique.

    connect(ui->calendarWidget, &QCalendarWidget::selectionChanged, this, &interfacePer::on_calendarWidget_selectionChanged);
    // Met à jour l'affichage des commandes lorsque l'utilisateur sélectionne une date.

    connect(ui->pushButton_ajouterCommande, &QPushButton::clicked, this, &interfacePer::on_pushButton_ajouterCommande_clicked);
    // Connecte le bouton pour ajouter une commande.

    connect(ui->pushButton_supprimerCommande, &QPushButton::clicked, this, &interfacePer::on_pushButton_supprimerCommande_clicked);
    // Connecte le bouton pour supprimer une commande.

    connect(ui->pushButton_modifierCommande, &QPushButton::clicked, this, &interfacePer::on_pushButton_modifierCommande_clicked);
    // Connecte le bouton pour modifier une commande.

    connect(ui->pushButtonAfficherCARegion, &QPushButton::clicked, this, &interfacePer::afficherChiffreAffairesParRegion);
    // Connecte le bouton pour afficher le chiffre d'affaires par région.

    connect(ui->pushButtonAfficherCAMagasin, &QPushButton::clicked, this, &interfacePer::afficherChiffreAffairesParMagasin);
    // Connecte le bouton pour afficher le chiffre d'affaires par magasin.

    connect(ui->pushButton_recommendProducts, &QPushButton::clicked, this, &interfacePer::on_pushButton_recommendProducts_clicked);
    // Connecte le bouton pour recommander des produits via un script Python.

    connect(serialPort, &QSerialPort::readyRead, this, &interfacePer::readSerialData);
    connect(ui->pushButton_afficherCabines, &QPushButton::clicked, this, &interfacePer::on_pushButton_afficherCabines_clicked);

}

interfacePer::~interfacePer()
{

    delete serialPort;

    delete networkManager;
    // Libère la mémoire du gestionnaire de requêtes réseau.

    delete ui;
    // Libère la mémoire de l'interface utilisateur.
}

void interfacePer::on_pushButton9_ajouter_clicked()
{
    qDebug() << "Bouton Ajouter (Magasin) pressé !";
    // Affiche un message de débogage.

    bool idOk, surfaceOk;
    int id_magasin = ui->lineEdit_8->text().toInt(&idOk);
    // Convertit l'ID du magasin en entier et vérifie sa validité.

    double surface = ui->lineEdit_16->text().toDouble(&surfaceOk);
    // Convertit la surface en double et vérifie sa validité.

    QString telephone = ui->lineEdit_18->text();
    QString nom_magasin = ui->lineEdit_7->text();
    QString type_magasin = ui->lineEdit_15->text();
    QString emplacement = ui->lineEdit_17->text();
    QTime ouverture = ui->timeEdit_6->time();
    QTime fermeture = ui->timeEdit_5->time();
    QDate date_creation = ui->dateEdit_3->date();
    // Récupère les autres champs de l'interface utilisateur.

    Magasins M(id_magasin, nom_magasin, type_magasin, emplacement, surface, telephone,
               ouverture, fermeture, date_creation);
    // Crée un objet Magasins avec les données saisies.

    bool test = M.ajouter();
    // Appelle la méthode ajouter() pour insérer le magasin dans la base de données.

    if (test) {
        QMessageBox::information(this, "Succès", "Ajout de magasin effectué !");
    } else {
        QMessageBox::critical(this, "Erreur", "Ajout de magasin non effectué !");
    }
    // Affiche un message de succès ou d'erreur.

    on_pushButton_10_afficher_clicked();
    // Rafraîchit l'affichage des magasins.
}

void interfacePer::on_pushButton_10_afficher_clicked()
{
    Magasins magasin;
    QSqlQueryModel* model = magasin.afficher();
    // Récupère les données des magasins via la méthode afficher().

    if (model) {
        ui->tableView->setModel(model);
        // Définit le modèle de données pour le tableView.

        model->setHeaderData(0, Qt::Horizontal, tr("ID Magasin"));
        model->setHeaderData(1, Qt::Horizontal, tr("Nom Magasin"));
        model->setHeaderData(2, Qt::Horizontal, tr("Type de Magasin"));
        model->setHeaderData(3, Qt::Horizontal, tr("Emplacement"));
        model->setHeaderData(4, Qt::Horizontal, tr("Surface"));
        model->setHeaderData(5, Qt::Horizontal, tr("Heures Ouverture"));
        model->setHeaderData(6, Qt::Horizontal, tr("Heures Fermeture"));
        model->setHeaderData(7, Qt::Horizontal, tr("Date Création"));
        model->setHeaderData(8, Qt::Horizontal, tr("Téléphone"));
        // Définit les en-têtes des colonnes.

        ui->tableView->resizeColumnsToContents();
        // Ajuste la taille des colonnes au contenu.
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible d'afficher les magasins.");
        // Affiche une erreur si le modèle est invalide.
    }
}

void interfacePer::on_pushButton_12_supprimer_clicked()
{
    bool idOk;
    int id_magasin = ui->lineEdit_id_supprimer->text().toInt(&idOk);
    // Convertit l'ID saisi en entier et vérifie sa validité.

    if (!idOk) {
        QMessageBox::warning(this, "Erreur de saisie", "Veuillez entrer un ID de magasin valide !");
        return;
    }
    // Affiche un avertissement si l'ID est invalide.

    Magasins magasin;
    bool success = magasin.supprimer(id_magasin);
    // Appelle la méthode supprimer() pour supprimer le magasin.

    if (success) {
        QMessageBox::information(this, "Succès", "Magasin supprimé avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression du magasin.");
    }
    // Affiche un message de succès ou d'erreur.

    on_pushButton_10_afficher_clicked();
    // Rafraîchit l'affichage.
}

void interfacePer::on_pushButton_modifier_clicked()
{
    bool idOk;
    int id_magasin = ui->lineEdit_id_supprimer->text().toInt(&idOk);
    // Vérifie la validité de l'ID saisi.

    if (!idOk) {
        QMessageBox::warning(this, "Erreur de saisie", "Veuillez entrer un ID de magasin valide !");
        return;
    }
    // Affiche un avertissement si l'ID est invalide.

    QString nom_magasin = ui->lineEdit_7->text();
    QString type_magasin = ui->lineEdit_15->text();
    QString emplacement = ui->lineEdit_17->text();
    double surface = ui->lineEdit_16->text().toDouble();
    QString telephone = ui->lineEdit_18->text();
    QTime ouverture = ui->timeEdit_6->time();
    QTime fermeture = ui->timeEdit_5->time();
    QDate date_creation = ui->dateEdit_3->date();
    // Récupère les données modifiées.

    Magasins M(id_magasin, nom_magasin, type_magasin, emplacement, surface, telephone,
               ouverture, fermeture, date_creation);
    // Crée un objet Magasins avec les nouvelles données.

    bool success = M.modifier(id_magasin);
    // Appelle la méthode modifier() pour mettre à jour le magasin.

    if (success) {
        QMessageBox::information(this, "Succès", "Magasin modifié avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification du magasin.");
    }
    // Affiche un message de succès ou d'erreur.

    on_pushButton_10_afficher_clicked();
    // Rafraîchit l'affichage.
}

void interfacePer::on_pushButton_rechercher_clicked()
{
    QString critere = ui->comboBox_critere->currentText();
    QString valeur = ui->lineEdit_recherche->text();
    // Récupère le critère et la valeur de recherche.

    if (critere.isEmpty() || valeur.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un critère et entrer une valeur.");
        return;
    }
    // Vérifie que les champs ne sont pas vides.

    Magasins magasin;
    QSqlQueryModel* model = magasin.rechercher(critere, valeur);
    // Recherche les magasins selon le critère et la valeur.

    if (model && model->rowCount() > 0) {
        ui->tableView->setModel(model);
        ui->tableView->resizeColumnsToContents();
    } else {
        QMessageBox::warning(this, "Aucun résultat", "Aucun magasin ne correspond à votre recherche.");
        on_pushButton_10_afficher_clicked();
    }
    // Affiche les résultats ou un message si aucun résultat n'est trouvé.
}

void interfacePer::on_pushButton_trier_clicked()
{
    QString critere = ui->comboBox_trie->currentText();
    // Récupère le critère de tri.

    Magasins magasin;
    QSqlQueryModel* model = magasin.trier(critere);
    // Trie les magasins selon le critère.

    if (model) {
        ui->tableView->setModel(model);
        ui->tableView->resizeColumnsToContents();
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible de trier les magasins.");
        on_pushButton_10_afficher_clicked();
    }
    // Affiche les magasins triés ou un message d'erreur.
}

void interfacePer::on_pushButton_pdf_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer le fichier PDF", "", "Fichiers PDF (*.pdf)");
    // Ouvre une boîte de dialogue pour choisir le chemin du fichier PDF.

    if (filePath.isEmpty())
        return;
    // Quitte si aucun fichier n'est sélectionné.

    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize::A4);
    pdfWriter.setResolution(300);
    // Configure le générateur de PDF.

    QPainter painter(&pdfWriter);
    painter.setFont(QFont("Times", 16, QFont::Bold));
    painter.drawText(QRect(0, 0, pdfWriter.width(), 100), Qt::AlignCenter, "Liste des Magasins");
    // Dessine un titre centré.

    QAbstractItemModel *model = ui->tableView->model();
    if (!model) {
        QMessageBox::warning(this, "Erreur", "Aucune donnée à exporter !");
        return;
    }
    // Vérifie que des données sont disponibles.

    painter.setFont(QFont("Helvetica", 10));
    int y = 150;
    int rowHeight = 25;
    // Configure la police et les positions initiales.

    for (int col = 0; col < model->columnCount(); ++col) {
        painter.drawText(50 + col * 150, y, model->headerData(col, Qt::Horizontal).toString());
    }
    y += rowHeight;
    // Dessine les en-têtes des colonnes.

    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            painter.drawText(50 + col * 150, y, model->data(model->index(row, col)).toString());
        }
        y += rowHeight;
        if (y > pdfWriter.height() - 100) {
            pdfWriter.newPage();
            y = 100;
        }
    }
    // Dessine chaque ligne de données, passe à une nouvelle page si nécessaire.

    painter.end();
    QMessageBox::information(this, "Exportation", "PDF généré avec succès !");
    // Termine le dessin et informe de la réussite.
}

void interfacePer::on_pushButton_statistique_clicked()
{
    QPieSeries *series = new QPieSeries();
    // Crée une série de données pour un graphique en camembert.

    QSqlQuery query;
    if (query.exec("SELECT TYPE_DE_MAGASIN, COUNT(*) FROM MAGASINS GROUP BY TYPE_DE_MAGASIN")) {
        while (query.next()) {
            QString type = query.value(0).toString();
            int count = query.value(1).toInt();
            series->append(type, count);
        }
    } else {
        qDebug() << "Erreur SQL (statistiques): " << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération des statistiques.");
        return;
    }
    // Récupère le nombre de magasins par type et remplit la série.

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des types de magasins");
    chart->legend()->setAlignment(Qt::AlignRight);
    // Crée un graphique avec la série et configure le titre et la légende.

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    // Crée une vue pour afficher le graphique avec lissage.

    if (ui->chartViewWidget->layout()) {
        QLayoutItem *item;
        while ((item = ui->chartViewWidget->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete ui->chartViewWidget->layout();
    }
    // Supprime l'ancien layout du widget, s'il existe.

    QVBoxLayout *layout = new QVBoxLayout(ui->chartViewWidget);
    layout->addWidget(chartView);
    ui->chartViewWidget->setLayout(layout);
    // Ajoute la vue du graphique dans un nouveau layout vertical.
}

void interfacePer::on_calendarWidget_selectionChanged()
{
    QDate selectedDate = ui->calendarWidget->selectedDate();
    afficherLesCommandesParDate(selectedDate);
    // Appelle la fonction pour afficher les commandes de la date sélectionnée.
}

void interfacePer::on_pushButton_ajouterCommande_clicked()
{
    int id_commande = ui->lineEdit_idCommande->text().toInt();
    QString nom_client = ui->lineEdit_nomClient->text();
    QString produit = ui->lineEdit_produit->text();
    QString etat = ui->comboBox_etat->currentText();
    QDate date_commande = ui->dateEdit_dateCommande->date();
    // Récupère les données de la commande.

    qDebug() << "--- Ajouter Commande ---";
    qDebug() << "ID Commande saisi:" << id_commande;
    qDebug() << "Nom Client saisi:" << nom_client;
    qDebug() << "Produit saisi:" << produit;
    qDebug() << "Etat sélectionné:" << etat;
    qDebug() << "Date Commande saisie:" << date_commande;
    // Affiche les données pour le débogage.

    if (nom_client.isEmpty() || produit.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs de la commande.");
        return;
    }
    // Vérifie que les champs obligatoires ne sont pas vides.

    QSqlQuery query;
    query.prepare("INSERT INTO commandes (id_commande, nom_client, produit, etat, date_commande) "
                  "VALUES (:id, :nom, :produit, :etat, :date)");
    query.bindValue(":id", id_commande);
    query.bindValue(":nom", nom_client);
    query.bindValue(":produit", produit);
    query.bindValue(":etat", etat);
    query.bindValue(":date", date_commande);
    // Prépare et lie les valeurs pour une requête d'insertion SQL.

    qDebug() << "Requête INSERT préparée:" << query.lastQuery();
    QVariantList boundValuesList = query.boundValues();
    qDebug() << "Valeurs liées (par index):" << boundValuesList;
    // Affiche la requête et les valeurs liées pour le débogage.

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Commande ajoutée avec succès !");
        qDebug() << "Requête INSERT exécutée avec succès.";
        ui->calendarWidget->setSelectedDate(date_commande);
        afficherLesCommandesParDate(date_commande);
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout de la commande : " + query.lastError().text());
        qDebug() << "Erreur SQL (ajout commande): " << query.lastError().text();
    }
    // Exécute la requête et affiche un message de succès ou d'erreur.
}

void interfacePer::afficherLesCommandesParDate(const QDate& date)
{
    qDebug() << "\n--- Afficher Commandes Pour Date ---";
    qDebug() << "Date demandée:" << date;
    // Affiche des informations de débogage.

    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT id_commande, nom_client, produit, etat, date_commande FROM commandes WHERE date_commande = :date");
    query.bindValue(":date", date);
    // Prépare une requête pour sélectionner les commandes d'une date spécifique.

    qDebug() << "Requête SELECT préparée:" << query.lastQuery();
    QVariantList boundValuesList = query.boundValues();
    qDebug() << "Valeurs liées (par index):" << boundValuesList;
    // Affiche la requête et les valeurs liées.

    if (query.exec()) {
        qDebug() << "Requête SELECT exécutée avec succès.";
        model->setQuery(query);
        qDebug() << "Nombre de commandes récupérées:" << model->rowCount();
        // Exécute la requête et définit le modèle.

        if (model->lastError().isValid()) {
            qDebug() << "Erreur de requête (afficher commandes):" << model->lastError().text();
            QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération des commandes.");
            delete model;
            return;
        }
        ui->tableView->setModel(model);
        ui->tableView->resizeColumnsToContents();
    } else {
        qDebug() << "Erreur d'exécution (afficher commandes):" << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'exécution de la requête des commandes.");
        delete model;
    }
    // Affiche les commandes ou une erreur, et ajuste les colonnes.
}

void interfacePer::on_pushButton_supprimerCommande_clicked()
{
    int id_commande = ui->lineEdit_idCommande->text().toInt();
    QSqlQuery query;
    query.prepare("DELETE FROM commandes WHERE id_commande = :id");
    query.bindValue(":id", id_commande);
    // Prépare une requête pour supprimer une commande par ID.

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Commande supprimée avec succès !");
        afficherLesCommandesParDate(ui->calendarWidget->selectedDate());
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression de la commande : " + query.lastError().text());
        qDebug() << "Erreur SQL (supprimer commande):" << query.lastError().text();
    }
    // Exécute la requête et affiche un message de succès ou d'erreur.
}

void interfacePer::on_pushButton_modifierCommande_clicked()
{
    int id_commande = ui->lineEdit_idCommande->text().toInt();
    QString nom_client = ui->lineEdit_nomClient->text();
    QString produit = ui->lineEdit_produit->text();
    QString etat = ui->comboBox_etat->currentText();
    QDate date_commande = ui->dateEdit_dateCommande->date();
    // Récupère les données modifiées de la commande.

    qDebug() << "Modification Commande - ID:" << id_commande << ", Nom:" << nom_client << ", Produit:" << produit << ", Etat:" << etat << ", Date:" << date_commande;
    // Affiche les données pour le débogage.

    if (nom_client.isEmpty() || produit.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs de la commande.");
        return;
    }
    // Vérifie que les champs obligatoires ne sont pas vides.

    QSqlQuery query;
    query.prepare("UPDATE commandes SET nom_client = :nom, produit = :produit, etat = :etat, date_commande = :date "
                  "WHERE id_commande = :id");
    query.bindValue(":id", id_commande);
    query.bindValue(":nom", nom_client);
    query.bindValue(":produit", produit);
    query.bindValue(":etat", etat);
    query.bindValue(":date", date_commande);
    // Prépare une requête pour mettre à jour la commande.

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Commande modifiée avec succès !");
        afficherLesCommandesParDate(ui->calendarWidget->selectedDate());
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la modification de la commande : " + query.lastError().text());
        qDebug() << "Erreur SQL (modifier commande):" << query.lastError().text();
    }
    // Exécute la requête et affiche un message de succès ou d'erreur.
}

QSqlQueryModel* interfacePer::obtenirChiffreAffairesParMagasin()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT m.ID_MAGASIN, m.NOM_MAGASIN, COALESCE(SUM(p.QUANTITE_VENDUE * p.PRIX_VENTE_UNITAIRE), 0) AS chiffre_affaires_total "
                  "FROM MAGASINS m "
                  "LEFT JOIN PRODUITS p ON m.ID_MAGASIN = p.ID_MAGASIN "
                  "GROUP BY m.ID_MAGASIN, m.NOM_MAGASIN "
                  "ORDER BY m.ID_MAGASIN");
    // Prépare une requête pour calculer le chiffre d'affaires par magasin.

    qDebug() << "Requête SQL (CA par Magasin - avant exécution): " << query.lastQuery();
    if (query.exec()) {
        qDebug() << "Requête SQL (CA par Magasin) exécutée avec succès.";
        model->setQuery(query);
        qDebug() << "Nombre de lignes retournées (CA par Magasin):" << model->rowCount();
    } else {
        qDebug() << "Erreur d'exécution de la requête (CA par Magasin):" << query.lastError().text();
    }
    // Exécute la requête et définit le modèle.

    return model;
    // Retourne le modèle avec les résultats.
}

void interfacePer::afficherChiffreAffairesParMagasin()
{
    QSqlQueryModel* model = obtenirChiffreAffairesParMagasin();
    if (model) {
        ui->tableView->setModel(model);
        ui->tableView->resizeColumnsToContents();
        model->setHeaderData(0, Qt::Horizontal, tr("ID Magasin"));
        model->setHeaderData(1, Qt::Horizontal, tr("Nom Magasin"));
        model->setHeaderData(2, Qt::Horizontal, tr("Chiffre d'Affaires Total"));
    }
    // Affiche le chiffre d'affaires par magasin dans le tableView.
}

QSqlQueryModel* interfacePer::obtenirChiffreAffairesParRegion()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT m.EMPLACEMENT AS region, "
                  "COALESCE(SUM(p.QUANTITE_VENDUE * p.PRIX_VENTE_UNITAIRE), 0) AS chiffre_affaires_total "
                  "FROM MAGASINS m "
                  "LEFT JOIN PRODUITS p ON m.ID_MAGASIN = p.ID_MAGASIN "
                  "GROUP BY m.EMPLACEMENT "
                  "ORDER BY m.EMPLACEMENT");
    // Prépare une requête pour calculer le chiffre d'affaires par région.

    qDebug() << "Requête SQL (CA par Région - avant exécution): " << query.lastQuery();
    if (query.exec()) {
        qDebug() << "Requête SQL (CA par Région) exécutée avec succès.";
        model->setQuery(query);
        qDebug() << "Nombre de lignes retournées (CA par Région):" << model->rowCount();
    } else {
        qDebug() << "Erreur d'exécution de la requête (CA par Région):" << query.lastError().text();
    }
    // Exécute la requête et définit le modèle.

    return model;
    // Retourne le modèle avec les résultats.
}

void interfacePer::afficherChiffreAffairesParRegion()
{
    QSqlQueryModel* model = obtenirChiffreAffairesParRegion();
    if (model) {
        ui->tableView->setModel(model);
        ui->tableView->resizeColumnsToContents();
        model->setHeaderData(0, Qt::Horizontal, tr("Région"));
        model->setHeaderData(1, Qt::Horizontal, tr("Chiffre d'Affaires Total"));
    }
    // Affiche le chiffre d'affaires par région dans le tableView.
}

void interfacePer::on_pushButton_recommendProducts_clicked()
{
    bool idOk, surfaceOk;
    int id_magasin = ui->lineEdit_8->text().toInt(&idOk);
    QString type_magasin = ui->lineEdit_15->text();
    QString emplacement = ui->lineEdit_17->text();
    double surface = ui->lineEdit_16->text().toDouble(&surfaceOk);
    // Récupère les informations du magasin.

    if (!idOk || !surfaceOk || type_magasin.isEmpty() || emplacement.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs.");
        return;
    }
    // Vérifie que tous les champs sont remplis.

    QString magasin_desc = QString("Magasin de type %1 à %2, surface %3 m²")
                               .arg(type_magasin, emplacement, QString::number(surface));
    // Crée une description textuelle du magasin.

    QStringList produits;
    QSqlQuery query;
    if (query.exec("SELECT DISTINCT produit FROM COMMANDES WHERE ROWNUM <= 5")) {
        while (query.next()) {
            produits << query.value(0).toString();
        }
    } else {
        qDebug() << "Erreur SQL (récupération produits): " << query.lastError().text();
        produits << "Lait" << "Pain" << "Vêtements" << "Électronique" << "Fruits";
    }
    // Récupère jusqu'à 5 produits distincts ou utilise une liste par défaut.

    QJsonArray inputs;
    inputs.append(magasin_desc);
    for (const QString &produit : produits) {
        inputs.append(produit);
    }
    QString inputsJson = QJsonDocument(inputs).toJson(QJsonDocument::Compact);
    // Crée un tableau JSON avec la description du magasin et les produits.

    QProcess process;
    QString pythonScriptPath = "C:/projet_QT/scripts/generate_embeddings.py";
    process.start("python", QStringList() << pythonScriptPath << inputsJson);
    // Lance le script Python avec les données JSON.

    if (!process.waitForStarted()) {
        QMessageBox::critical(this, "Erreur", "Impossible de démarrer le script Python : " + process.errorString());
        return;
    }
    // Vérifie que le script a démarré.

    if (!process.waitForFinished(30000)) {
        QMessageBox::critical(this, "Erreur", "Le script Python a dépassé le temps d'exécution.");
        process.kill();
        return;
    }
    // Attend la fin du script (timeout de 30 secondes).

    QByteArray output = process.readAllStandardOutput();
    QByteArray error = process.readAllStandardError();
    // Récupère la sortie standard et les erreurs.

    if (!error.isEmpty()) {
        qDebug() << "Erreur script Python :" << error;
        QMessageBox::critical(this, "Erreur", "Erreur dans le script Python : " + QString(error));
        return;
    }
    // Affiche une erreur si le script a échoué.

    handleRecommendationReplySimulated(output, produits);
    // Traite la sortie du script pour générer des recommandations.
}

void interfacePer::handleRecommendationReplySimulated(const QByteArray &response, const QStringList &produits)
{
    QJsonDocument doc = QJsonDocument::fromJson(response);
    if (doc.isNull() || !doc.isArray()) {
        qDebug() << "Réponse JSON invalide :" << response;
        QMessageBox::critical(this, "Erreur", "Réponse invalide du script Python.");
        return;
    }
    // Vérifie que la réponse est un tableau JSON valide.

    QJsonArray embeddings = doc.array();
    if (embeddings.isEmpty()) {
        qDebug() << "Aucun embedding retourné.";
        QMessageBox::critical(this, "Erreur", "Aucun résultat du script Python.");
        return;
    }
    // Vérifie que des embeddings sont retournés.

    QJsonArray magasin_embedding = embeddings[0].toArray();
    if (magasin_embedding.isEmpty()) {
        qDebug() << "Embedding du magasin vide.";
        QMessageBox::critical(this, "Erreur", "Erreur dans les données du script Python.");
        return;
    }
    // Récupère l'embedding du magasin (premier élément).

    struct ProductScore {
        QString name;
        double score;
    };
    QList<ProductScore> product_scores;
    // Structure pour stocker les scores des produits.

    for (int i = 1; i < embeddings.size() && i <= produits.size(); ++i) {
        QJsonArray product_embedding = embeddings[i].toArray();
        if (product_embedding.isEmpty()) {
            qDebug() << "Embedding du produit vide à l'index" << i;
            continue;
        }
        // Récupère l'embedding de chaque produit.

        double sum_diff_squared = 0.0;
        int min_length = qMin(magasin_embedding.size(), product_embedding.size());
        for (int j = 0; j < min_length; ++j) {
            double diff = magasin_embedding[j].toDouble() - product_embedding[j].toDouble();
            sum_diff_squared += diff * diff;
        }
        double score = 1.0 / (1.0 + std::sqrt(sum_diff_squared));
        qDebug() << "Produit:" << produits[i-1] << ", Score calculé:" << score;
        product_scores.append({produits[i-1], score});
    }
    // Calcule un score de similarité pour chaque produit (distance euclidienne inversée).

    if (product_scores.isEmpty()) {
        qDebug() << "Aucune similarité calculée.";
        QMessageBox::warning(this, "Avertissement", "Aucune recommandation calculée.");
        return;
    }
    // Vérifie que des scores ont été calculés.

    std::sort(product_scores.begin(), product_scores.end(),
              [](const ProductScore &a, const ProductScore &b) {
                  return a.score > b.score;
              });
    // Trie les produits par score décroissant.

    QStandardItemModel *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({tr("Produit Recommandé"), tr("Score")});
    // Crée un modèle pour afficher les recommandations.

    int max_recommendations = qMin(3, product_scores.size());
    for (int i = 0; i < max_recommendations; ++i) {
        QList<QStandardItem*> row;
        QStandardItem *itemName = new QStandardItem(product_scores[i].name);
        QStandardItem *itemScore = new QStandardItem(QString::number(product_scores[i].score, 'f', 2));
        row << itemName << itemScore;
        model->appendRow(row);
    }
    // Ajoute jusqu'à 3 produits recommandés au modèle.

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    QMessageBox::information(this, "Recommandations", QString("Produits recommandés affichés (%1 éléments).").arg(max_recommendations));
    // Affiche les recommandations dans le tableView.
}

void interfacePer::handleRecommendationReply(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Erreur réseau :" << reply->errorString();
        QString errorMsg = reply->errorString();
        if (errorMsg.contains("429")) {
            errorMsg = "Quota de l'API dépassé. Veuillez réessayer plus tard.";
        }
        QMessageBox::critical(this, "Erreur", "Erreur lors de la communication avec l'API : " + errorMsg);
        reply->deleteLater();
        return;
    }
    // Gère les erreurs réseau (par exemple, quota dépassé).

    QByteArray response = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(response);
    if (doc.isNull() || !doc.isArray()) {
        qDebug() << "Réponse JSON invalide :" << response;
        QMessageBox::critical(this, "Erreur", "Réponse invalide de l'API.");
        reply->deleteLater();
        return;
    }
    // Vérifie que la réponse de l'API est un tableau JSON valide.

    QJsonArray embeddings = doc.array();
    if (embeddings.isEmpty()) {
        qDebug() << "Aucun embedding retourné.";
        QMessageBox::critical(this, "Erreur", "Aucun résultat de l'API.");
        reply->deleteLater();
        return;
    }
    // Vérifie que des embeddings sont retournés.

    QJsonArray magasin_embedding = embeddings[0].toArray();
    if (magasin_embedding.isEmpty()) {
        qDebug() << "Embedding du magasin vide.";
        QMessageBox::critical(this, "Erreur", "Erreur dans les données de l'API.");
        reply->deleteLater();
        return;
    }
    // Récupère l'embedding du magasin.

    struct ProductScore {
        QString name;
        double score;
    };
    QList<ProductScore> product_scores;
    // Structure pour stocker les scores des produits.

    QStringList produits;
    QSqlQuery query;
    if (query.exec("SELECT DISTINCT produit FROM COMMANDES WHERE ROWNUM <= 5")) {
        while (query.next()) {
            produits << query.value(0).toString();
        }
    } else {
        qDebug() << "Erreur SQL (récupération produits):" << query.lastError().text();
        produits << "Lait" << "Pain" << "Vêtements" << "Électronique" << "Fruits";
    }
    // Récupère jusqu'à 5 produits distincts ou utilise une liste par défaut.

    for (int i = 1; i < embeddings.size() && i <= produits.size(); ++i) {
        QJsonArray product_embedding = embeddings[i].toArray();
        if (product_embedding.isEmpty()) {
            continue;
        }
        double sum_diff_squared = 0.0;
        int min_length = qMin(magasin_embedding.size(), product_embedding.size());
        for (int j = 0; j < min_length; ++j) {
            double diff = magasin_embedding[j].toDouble() - product_embedding[j].toDouble();
            sum_diff_squared += diff * diff;
        }
        double score = 1.0 / (1.0 + std::sqrt(sum_diff_squared));
        product_scores.append({produits[i-1], score});
    }
    // Calcule un score de similarité pour chaque produit.

    std::sort(product_scores.begin(), product_scores.end(),
              [](const ProductScore &a, const ProductScore &b) {
                  return a.score > b.score;
              });
    // Trie les produits par score décroissant.

    QStandardItemModel *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({tr("Produit Recommandé"), tr("Score")});
    int max_recommendations = qMin(3, product_scores.size());
    for (int i = 0; i < max_recommendations; ++i) {
        QList<QStandardItem*> row;
        row << new QStandardItem(product_scores[i].name)
            << new QStandardItem(QString::number(product_scores[i].score, 'f', 2));
        model->appendRow(row);
    }
    // Ajoute jusqu'à 3 produits recommandés au modèle.

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    QMessageBox::information(this, "Recommandations", "Produits recommandés affichés.");
    // Affiche les recommandations.

    reply->deleteLater();
    // Libère la mémoire de la réponse réseau.
}
void interfacePer::setupSerialPort()
{
    // Trouver le port de l'Arduino
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
        if (info.manufacturer().contains("Arduino", Qt::CaseInsensitive)) {
            serialPort->setPortName(info.portName());
            break;
        }
    }

    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (!serialPort->open(QIODevice::ReadWrite)) {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le port série : " + serialPort->errorString());
    }
}

void interfacePer::readSerialData()
{
    static QByteArray buffer;
    buffer += serialPort->readAll();
    while (buffer.contains('\n')) {
        int index = buffer.indexOf('\n');
        QByteArray line = buffer.left(index);
        buffer = buffer.mid(index + 1);
        QJsonDocument doc = QJsonDocument::fromJson(line);
        if (doc.isNull() || !doc.isObject()) {
            qDebug() << "Données JSON invalides :" << line;
            continue;
        }
        QJsonObject obj = doc.object();
        QString cabine1Etat = obj["cabine1"].toString();
        QString cabine2Etat = obj["cabine2"].toString();
        QString attenteEtat = obj["attente"].toString();
        QSqlQuery query;
        query.prepare("INSERT INTO CABINES (date_, cabine1_etat, cabine2_etat, attente_etat) "
                      "VALUES (SYSTIMESTAMP, :cabine1, :cabine2, :attente)");
        query.bindValue(":cabine1", cabine1Etat);
        query.bindValue(":cabine2", cabine2Etat);
        query.bindValue(":attente", attenteEtat);
        if (!query.exec()) {
            qDebug() << "Erreur SQL :" << query.lastError().text();
        }
        ui->labelCabine1->setText("Cabine 1 : " + cabine1Etat);
        ui->labelCabine2->setText("Cabine 2 : " + cabine2Etat);
        ui->labelAttente->setText("Zone d'attente : " + attenteEtat);
    }
}

void interfacePer::on_pushButton_afficherCabines_clicked()
{
    QSqlQueryModel* model = new QSqlQueryModel(ui->tableView);
    QSqlQuery query;
    query.prepare("SELECT date_, cabine1_etat, cabine2_etat, attente_etat FROM CABINES ORDER BY date_ DESC");
    if (query.exec()) {
        model->setQuery(query);
        ui->tableView->setModel(model);
        ui->tableView->resizeColumnsToContents();
        model->setHeaderData(0, Qt::Horizontal, tr("Date"));
        model->setHeaderData(1, Qt::Horizontal, tr("État Cabine 1"));
        model->setHeaderData(2, Qt::Horizontal, tr("État Cabine 2"));
        model->setHeaderData(3, Qt::Horizontal, tr("Zone d'attente"));
    } else {
        qDebug() << "Erreur SQL :" << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Impossible d'afficher les données des cabines.");
    }
}
