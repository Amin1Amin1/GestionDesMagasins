#include "interfaceper.h"
#include "ui_interfaceper.h"
#include "magasins.h"
// #include "connection.h"
//#include "metiers.h" // Inclure l'en-tête du dialogue
#include <QMessageBox>
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QTextDocument>
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>






interfacePer::interfacePer(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::interfacePer)
{
    ui->setupUi(this);
    connect(ui->pushButton9_ajouter, &QPushButton::clicked, this, &interfacePer::on_pushButton9_ajouter_clicked);
    connect(ui->pushButton_10_afficher, &QPushButton::clicked, this, &interfacePer::on_pushButton_10_afficher_clicked);
    connect(ui->pushButton_12_supprimer, &QPushButton::clicked, this, &interfacePer::on_pushButton_12_supprimer_clicked);
    connect(ui->pushButton_modifier, &QPushButton::clicked, this, &interfacePer::on_pushButton_modifier_clicked);
    //***
    connect(ui->pushButton_rechercher, &QPushButton::clicked, this, &interfacePer::on_pushButton_rechercher_clicked);
    connect(ui->pushButton_trier, &QPushButton::clicked, this, &interfacePer::on_pushButton_trier_clicked);
    connect(ui->pushButton_pdf, &QPushButton::clicked, this, &interfacePer::on_pushButton_pdf_clicked);
    connect(ui->pushButton_statistique, &QPushButton::clicked, this, &interfacePer::on_pushButton_statistique_clicked);



}

interfacePer::~interfacePer()
{
    delete ui;
}

void interfacePer::on_pushButton9_ajouter_clicked()
{
    qDebug() << "Bouton Ajouter pressé !";

    bool idOk, surfaceOk, telOk;
    int id_magasin = ui->lineEdit_8->text().toInt(&idOk);
    double surface = ui->lineEdit_16->text().toDouble(&surfaceOk);
    QString telephone = ui->lineEdit_18->text();


    QString nom_magasin = ui->lineEdit_7->text();
    QString type_magasin = ui->lineEdit_15->text();
    QString emplacement = ui->lineEdit_17->text();
    QTime ouverture = ui->timeEdit_6->time();
    QTime fermeture = ui->timeEdit_5->time();
    //QString employes = ui->comboBox_3->currentText();
    QDate date_creation = ui->dateEdit_3->date();

    Magasins M(id_magasin, nom_magasin, type_magasin, emplacement, surface, telephone,
               ouverture, fermeture, date_creation);

    // Ajout à la base de données
    bool test = M.ajouter();
    if (test) {
        QMessageBox::information(this, "Succès", "Ajout effectué !");
       // qDebug() << "Le magasin a été ajouté avec succès.";
    } else {
        QMessageBox::critical(this, "Erreur", "Ajout non effectué !");
        //qDebug() << "Erreur lors de l'ajout du magasin.";
    }
}


void interfacePer::on_pushButton_10_afficher_clicked()

{
    Magasins magasin;
    QSqlQueryModel* model = magasin.afficher();

    if (model) {
        // Associe le modèle de données au QTableView
        ui->tableView->setModel(model);

        // Définit les en-têtes des colonnes
        model->setHeaderData(0, Qt::Horizontal, tr("ID Magasin"));
        model->setHeaderData(1, Qt::Horizontal, tr("Nom Magasin"));
        model->setHeaderData(2, Qt::Horizontal, tr("Type de Magasin"));
        model->setHeaderData(3, Qt::Horizontal, tr("Emplacement"));
        model->setHeaderData(4, Qt::Horizontal, tr("Surface"));
        model->setHeaderData(5, Qt::Horizontal, tr("Heures Ouverture"));
        model->setHeaderData(6, Qt::Horizontal, tr("Heures Fermeture"));
        model->setHeaderData(7, Qt::Horizontal, tr("Date Création"));
        model->setHeaderData(8, Qt::Horizontal, tr("Téléphone"));

        // Ajuste la taille des colonnes pour s'adapter au contenu
        ui->tableView->resizeColumnsToContents();
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible d'afficher les magasins.");
    }
}

void interfacePer::on_pushButton_12_supprimer_clicked()
{
    bool idOk;
    int id_magasin = ui->lineEdit_id_supprimer->text().toInt(&idOk);

    if (!idOk) {
        QMessageBox::warning(this, "Erreur de saisie", "Veuillez entrer un ID valide !");
        return;
    }

    Magasins magasin;
    bool success = magasin.supprimer(id_magasin);

    if (success) {
        QMessageBox::information(this, "Succès", "Magasin supprimé avec succès !");
        qDebug() << "Le magasin a été supprimé avec succès.";
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression du magasin.");
        qDebug() << "Erreur lors de la suppression du magasin.";
    }

    // Rafraîchit l'affichage des magasins
    on_pushButton_10_afficher_clicked();
}


void interfacePer::on_pushButton_modifier_clicked()
{
    // Récupère l'ID du magasin à modifier
    bool idOk;
    int id_magasin = ui->lineEdit_id_supprimer->text().toInt(&idOk); // Change here

    // Vérifie si l'ID est valide
    if (!idOk) {
        QMessageBox::warning(this, "Erreur de saisie", "Veuillez entrer un ID valide !");
        return;
    }

    // Récupère les nouvelles valeurs des champs
    QString nom_magasin = ui->lineEdit_7->text();
    QString type_magasin = ui->lineEdit_15->text();
    QString emplacement = ui->lineEdit_17->text();
    double surface = ui->lineEdit_16->text().toDouble();
    QString telephone = ui->lineEdit_18->text();
    QTime ouverture = ui->timeEdit_6->time();
    QTime fermeture = ui->timeEdit_5->time();
    QDate date_creation = ui->dateEdit_3->date();

    // Crée une instance de Magasins avec les nouvelles valeurs
    Magasins M(id_magasin, nom_magasin, type_magasin, emplacement, surface, telephone,
               ouverture, fermeture, date_creation);

    // Met à jour le magasin dans la base de données
    bool success = M.modifier(id_magasin);

    if (success) {
        QMessageBox::information(this, "Succès", "Magasin modifié avec succès !");
        qDebug() << "Le magasin a été modifié avec succès.";
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification du magasin.");
        qDebug() << "Erreur lors de la modification du magasin.";
    }

    // Rafraîchit l'affichage des magasins
    on_pushButton_10_afficher_clicked();
}
// recherche
void interfacePer::on_pushButton_rechercher_clicked()
{
    // Récupérer le critère de recherche et la valeur saisie
    QString critere = ui->comboBox_critere->currentText(); // Exemple : "nom", "type", "emplacement"
    QString valeur = ui->lineEdit_recherche->text();

    if (critere.isEmpty() || valeur.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un critère et entrer une valeur.");
        return;
    }

    // Appel de la méthode de recherche
    Magasins magasin;
    QSqlQueryModel* model = magasin.rechercher(critere, valeur);

    if (model && model->rowCount() > 0) {
        // Si des résultats sont trouvés, les afficher dans la table
        ui->tableView->setModel(model);
        ui->tableView->resizeColumnsToContents();
    } else {
        // Aucun résultat trouvé
        QMessageBox::warning(this, "Aucun résultat", "Aucun magasin ne correspond à votre recherche.");
    }
}



//tri
void interfacePer::on_pushButton_trier_clicked()
{
    QString critere = ui->comboBox_trie->currentText(); // Par exemple : "nom", "type", "emplacement"

    // Vérifier que le critère de tri est valide
    QString columnName;
    if (critere == "nom") {
        columnName = "NOM_MAGASIN";
    } else if (critere == "type") {
        columnName = "TYPE_DE_MAGASIN";
    } else if (critere == "emplacement") {
        columnName = "EMPLACEMENT";
    } else {
        QMessageBox::warning(this, "Erreur", "Critère de tri invalide.");
        return;
    }

    // Créer une nouvelle requête avec tri
    QString queryStr = QString("SELECT * FROM magasins ORDER BY %1").arg(columnName);

    // Créer un modèle et y appliquer la requête
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(queryStr);

    // Vérifier s'il y a des erreurs dans la requête
    if (model->lastError().isValid()) {
        qDebug() << "Erreur de requête SQL : " << model->lastError().text();
        return;
    }

    // Appliquer le modèle trié à la vue
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
}



void interfacePer::on_pushButton_pdf_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer le fichier PDF", "", "Fichiers PDF (*.pdf)");

    if (filePath.isEmpty())
        return;

    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize::A4);
    pdfWriter.setResolution(300);

    QPainter painter(&pdfWriter);

    // Titre
    painter.setFont(QFont("Times", 16, QFont::Bold));
    painter.drawText(QRect(0, 0, pdfWriter.width(), 100), Qt::AlignCenter, "Liste des Magasins");

    // Récupération des données depuis la tableView
    QAbstractItemModel *model = ui->tableView->model();

    if (!model) {
        QMessageBox::warning(this, "Erreur", "Aucune donnée à exporter !");
        return;
    }

    painter.setFont(QFont("Helvetica", 10));
    int y = 150;
    int rowHeight = 25;

    // En-têtes
    for (int col = 0; col < model->columnCount(); ++col) {
        painter.drawText(50 + col * 150, y, model->headerData(col, Qt::Horizontal).toString());
    }

    y += rowHeight;

    // Données
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

    painter.end();

    QMessageBox::information(this, "Exportation", "PDF généré avec succès !");
}

void interfacePer::on_pushButton_statistique_clicked()
{
    QPieSeries *series = new QPieSeries();  // Pas besoin de QtCharts:: si les includes sont bons

    QSqlQuery query;
    if (query.exec("SELECT TYPE_DE_MAGASIN, COUNT(*) FROM MAGASINS GROUP BY TYPE_DE_MAGASIN")) {
        while (query.next()) {
            QString type = query.value(0).toString();
            int count = query.value(1).toInt();
            series->append(type, count);
        }
    } else {
        qDebug() << "Erreur SQL : " << query.lastError().text();
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des types de magasins");
    chart->legend()->setAlignment(Qt::AlignRight);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Nettoyage et ajout au widget
    if (ui->chartViewWidget->layout()) {
        QLayoutItem *item;
        while ((item = ui->chartViewWidget->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete ui->chartViewWidget->layout();
    }

    QVBoxLayout *layout = new QVBoxLayout(ui->chartViewWidget);
    layout->addWidget(chartView);
    ui->chartViewWidget->setLayout(layout);
}

void interfacePer::on_calendarWidget_selectionChanged()
{
    QDate selectedDate = ui->calendarWidget->selectedDate();
    qDebug() << "Date sélectionnée :" << selectedDate.toString("yyyy-MM-dd");
}

void interfacePer::on_Commandes_clicked()
{
    QDate selectedDate = ui->calendarWidget->selectedDate();
    QString dateStr = selectedDate.toString("yyyy-MM-dd");

    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    // Utilisation de TO_DATE pour Oracle
    query.prepare("SELECT ID_COMMANDE, NOM_CLIENT, PRODUIT, ETAT "
                  "FROM COMMANDES "
                  "WHERE DATE_COMMANDE = TO_DATE(:date, 'YYYY-MM-DD')");
    query.bindValue(":date", dateStr);

    if (query.exec()) {
        model->setQuery(std::move(query));

        // Définir les en-têtes
        model->setHeaderData(0, Qt::Horizontal, "ID");
        model->setHeaderData(1, Qt::Horizontal, "Client");
        model->setHeaderData(2, Qt::Horizontal, "Produit");
        model->setHeaderData(3, Qt::Horizontal, "État");

        ui->tableView_commandes->setModel(model);
        ui->tableView_commandes->resizeColumnsToContents();

        if (model->rowCount() == 0) {
            QMessageBox::information(this, "Information", "Aucune commande trouvée pour cette date.");
        }
    } else {
        qDebug() << "Erreur requête:" << query.lastError().text();
        QMessageBox::critical(this, "Erreur",
                              "Impossible de charger les commandes: " + query.lastError().text());
        delete model;
    }
}
