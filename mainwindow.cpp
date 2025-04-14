#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QSqlQueryModel>
#include <QTableView>
#include <QSqlError>
#include <QPdfWriter>
#include <QFileDialog>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSlice>
#include <QSqlQuery>
#include <QDebug>
#include <QVBoxLayout>
#include<QSqlTableModel>
#include "produit.h"
#include "magasins.h"
#include "interfaceper.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Assuming your QTableView is named tableView
    connect(ui->tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(onItemSelected()));


    //setting up buttons
    connect(ui->ajouter, &QPushButton::clicked, this, &MainWindow::ajouter_produit);
    connect(ui->supprimer, &QPushButton::clicked, this, &MainWindow::supprimer_produit);
    connect(ui->modifier, &QPushButton::clicked, this, &MainWindow::modifier_produit);
    connect(ui->chercher, &QPushButton::clicked, this, &MainWindow::chercher_produit);
    connect(ui->tri, &QPushButton::clicked, this, &MainWindow::trier_produit);
    connect(ui->statistique, &QPushButton::clicked, this, &MainWindow::statistics);
    connect(ui->pdf, &QPushButton::clicked, this, &MainWindow::pdf_generator);
    connect(ui->verifier, &QPushButton::clicked, this, &MainWindow::verifier_stock);
    connect(ui->history, &QPushButton::clicked, this, &MainWindow::showHistory);
    connect(ui->clear_history, &QPushButton::clicked, this, &MainWindow::clearHistory);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadProduitData()
{
    QSqlQueryModel *model = produit.afficher();
    if (model) {
        ui->tableView->setModel(model);
        connect(ui->tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(onItemSelected()));
    } else {
        QMessageBox::warning(this, "Erreur", "Aucune donnée de produit disponible.");
    }
}

void MainWindow::ajouter_produit()
{
    QDate today = QDate::currentDate();
    QDate date=  ui->date_exp->date();
    if (date <= today)
    {
        QMessageBox::warning(this, "Date Invalide", "Merci de selectionner une date plus tard qu'aujourd'hui.");
    }
    else
    {
        double prix = ui->prix->text().toDouble();
        if (prix<=0)
        {
            QMessageBox::warning(this, "Prix Invalide", "Merci de selectionner un prix superieur à 0.");
        }
        else
        {
            int stock = ui->quantite->text().toInt();
            if (stock<0)
            {
                QMessageBox::warning(this, "Quantité Invalide", "Merci de selectionner une quantité positive.");
            }
            else
            {
                QString nom = ui->nom->text();

                QSqlQuery query;
                query.prepare("SELECT COUNT(*) FROM PRODUITS WHERE nom_p = :nom");
                query.bindValue(":nom", nom);

                if (!query.exec()){qDebug() << "Error checking for name existence:" << query.lastError();}
                else
                {
                    query.next();
                    int count = query.value(0).toInt();

                    if (count > 0)
                    {
                        QMessageBox::warning(this, "Nom Invalide", "Ce produit existe deja.");
                    }
                    else
                    {

                    QString date_exp =date.toString("dd-MM-yyyy");
                    //checkboxes
                    QCheckBox *checkboxLactose = ui->lactose;
                    QCheckBox *checkboxBio = ui->bio;
                    QCheckBox *checkboxSugar = ui->sugar;
                    QCheckBox *checkboxGluten = ui->gluten;
                    QCheckBox *checkboxEco = ui->eco;

                    Produit produit(nom, date_exp, prix, stock, checkboxLactose, checkboxBio, checkboxSugar, checkboxGluten, checkboxEco);

                    if (produit.ajouter())
                    {
                        QMessageBox::information(this, "Success", "Product added successfully!");
                        recordAction("Ajout du produit",nom);
                    } else
                        QMessageBox::warning(this, "Error", "Failed to add product.");
                    loadProduitData();
                    }
                }
            }
        }
    }
}


void MainWindow::supprimer_produit()
{
    Produit p;
    QString nom_p = ui->nom->text();
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM PRODUITS WHERE nom_p = :nom");
    query.bindValue(":nom", nom_p);
    //verification de l'existance
    if (!query.exec()){qDebug() << "Error checking for name existence:" << query.lastError();}
    else
    {
        query.next();
        int count = query.value(0).toInt();

        if (count == 0)
        {
            QMessageBox::warning(this, "Nom Invalide", "Ce produit n'existe pas.");
        }
        else
        {
            if (p.supprimer(nom_p))
            {
                QMessageBox::information(this, "Succès", "Produit supprimé avec succès.");
                loadProduitData();
                recordAction("Suppression du produit",nom_p);
            }
            else
            {
                QMessageBox::warning(this, "Erreur", "Erreur lors de la suppression du produit.");
            }
        }
    }
}


void MainWindow::onItemSelected() {
    // Get the selected index
    QModelIndex selectedIndex = ui->tableView->selectionModel()->currentIndex();

    // Ensure a valid index is selected
    if (selectedIndex.isValid()) {
        // Get the row of the selected index
        int row = selectedIndex.row();

        // Retrieve data from the model (assuming you are using QSqlTableModel)
        QString nom = ui->tableView->model()->data(ui->tableView->model()->index(row, 0)).toString();
        QString date_exp = ui->tableView->model()->data(ui->tableView->model()->index(row, 1)).toString();
        QString prix = ui->tableView->model()->data(ui->tableView->model()->index(row, 2)).toString();
        int quantite = ui->tableView->model()->data(ui->tableView->model()->index(row, 3)).toInt();
        int lactose = ui->tableView->model()->data(ui->tableView->model()->index(row, 4)).toInt();
        int bio = ui->tableView->model()->data(ui->tableView->model()->index(row, 5)).toInt();
        int sugar = ui->tableView->model()->data(ui->tableView->model()->index(row, 6)).toInt();
        int gluten = ui->tableView->model()->data(ui->tableView->model()->index(row, 7)).toInt();
        int eco = ui->tableView->model()->data(ui->tableView->model()->index(row, 8)).toInt();

        // Populate the input fields with the retrieved data
        ui->nom->setText(nom);
        ui->date_exp->setDate(QDate::fromString(date_exp, "MM-dd-yyyy"));
        ui->prix->setText(prix);
        ui->quantite->setValue(quantite);
        ui->lactose->setChecked(lactose == 1);
        ui->bio->setChecked(bio == 1);
        ui->sugar->setChecked(sugar == 1);
        ui->gluten->setChecked(gluten == 1);
        ui->eco->setChecked(eco == 1);

    }
}


void MainWindow::modifier_produit()
{
    QDate today = QDate::currentDate();
    QDate date=  ui->date_exp->date();
    if (date <= today)
    {
        QMessageBox::warning(this, "Date Invalide", "Merci de selectionner une date plus tard qu'aujourd'hui.");
    }
    else
    {
        double prix = ui->prix->text().toDouble();
        if (prix<=0)
        {
            QMessageBox::warning(this, "Prix Invalide", "Merci de selectionner un prix superieur à 0.");
        }
        else
        {
            int stock = ui->quantite->text().toInt();
            if (stock<0)
            {
                QMessageBox::warning(this, "Quantité Invalide", "Merci de selectionner une quantité positive.");
            }
            else
            {
                QString nom = ui->nom->text();

                QSqlQuery query;
                query.prepare("SELECT COUNT(*) FROM PRODUITS WHERE nom_p = :nom");
                query.bindValue(":nom", nom);

                if (!query.exec()){qDebug() << "Error checking for name existence:" << query.lastError();}
                else
                {
                    query.next();
                    int count = query.value(0).toInt();

                    if (count == 0)
                    {
                        QMessageBox::warning(this, "Nom Invalide", "Ce produit n'existe pas.");
                    }
                    else
                    {

                        QString date_exp =date.toString("dd-MM-yyyy");
                        //checkboxes
                        QCheckBox *checkboxLactose = ui->lactose;
                        QCheckBox *checkboxBio = ui->bio;
                        QCheckBox *checkboxSugar = ui->sugar;
                        QCheckBox *checkboxGluten = ui->gluten;
                        QCheckBox *checkboxEco = ui->eco;

                        Produit produit(nom, date_exp, prix, stock, checkboxLactose, checkboxBio, checkboxSugar, checkboxGluten, checkboxEco);

                        if (produit.modifier())
                        {
                            QMessageBox::information(this, "Success", "Product modified successfully!");
                        } else
                            QMessageBox::warning(this, "modified", "Failed to modify product.");
                        loadProduitData();
                        recordAction("modification du produit",nom);
                    }
                }
            }
        }

    }
}


void MainWindow::chercher_produit() {
    QString nom_p = ui->nom->text();

    Produit  p;
    QSqlQueryModel* model = p.chercherProduitParNom(nom_p);

    if (model->rowCount() == 0) {
        QMessageBox::information(this, "Résultat", "Aucun produit trouvé  avec ce nom.");
    } else {
        ui->tableView->setModel(model);  // Afficher les résultats dans un QTableView
    }
}


void MainWindow::trier_produit() {
    // Get the selected criterion
    QString critic = ui->critere->currentText();
    QString order = ui->ordre->currentText();

    // Map criterion to database columns
    QString column;
    if (critic == "Alphabétique") {
        column = "nom_p"; // Assuming 'nom_p' is the column for product names
    } else if (critic == "Prix") {
        column = "prix"; // Replace with your actual column name
    } else if (critic == "Stock") {
        column = "stock"; // Replace with your actual column name
    } else {
        QMessageBox::warning(this, "Error", "Invalid criterion selected.");
        return;
    }

    // Map order to SQL syntax
    QString sqlOrder = (order == "Ascendant") ? "ASC" : "DESC";
    if (order == "Ascendant")
    {
        sqlOrder = "ASC"; // Assuming 'nom_p' is the column for product names
    }
    else if (order == "Descendant")
    {
        sqlOrder = "DESC"; // Replace with your actual column name
    }
    else
    {
        QMessageBox::warning(this, "Error", "Invalid order selected.");
        return;
    }

    // Construct the SQL query
    QString queryStr = QString("SELECT * FROM PRODUITS ORDER BY %1 %2")
                           .arg(column)
                           .arg(sqlOrder);

    // Execute the query
    QSqlQueryModel *model = new QSqlQueryModel(this);
    QSqlQuery query;
    if (query.exec(queryStr)) {
        model->setQuery(std::move(query));
        ui->tableView->setModel(model); // Assuming you display data in a QTableView
    } else {
        QMessageBox::critical(this, "Error", "Failed to perform sorting: " + query.lastError().text());
    }
}


void MainWindow::statistics() {
    QSqlQuery query;
    int totalProducts = 0;
    int glutenFreeCount = 0;
    int sugarFreeCount = 0;
    int lactoseFreeCount = 0;
    int organicCount = 0;
    int bioCount = 0;

    // Step 1: Get the total number of products
    query.prepare("SELECT COUNT(*) FROM PRODUITS");
    if (query.exec() && query.next()) {
        totalProducts = query.value(0).toInt();
    } else {
        QMessageBox::warning(this, "Database Error", "Failed to fetch total products: " + query.lastError().text());
        return;
    }

    if (totalProducts == 0) {
        QMessageBox::warning(this, "No Data", "The products table is empty.");
        return;
    }

    // Step 2: Get counts for each category
    query.prepare("SELECT COUNT(*) FROM PRODUITS WHERE gluten = 1");
    if (query.exec() && query.next()) {
        glutenFreeCount = query.value(0).toInt();
    }

    query.prepare("SELECT COUNT(*) FROM PRODUITS WHERE sugar = 1");
    if (query.exec() && query.next()) {
        sugarFreeCount = query.value(0).toInt();
    }

    query.prepare("SELECT COUNT(*) FROM PRODUITS WHERE lactose = 1");
    if (query.exec() && query.next()) {
        lactoseFreeCount = query.value(0).toInt();
    }

    query.prepare("SELECT COUNT(*) FROM PRODUITS WHERE eco = 1");
    if (query.exec() && query.next()) {
        organicCount = query.value(0).toInt();
    }

    query.prepare("SELECT COUNT(*) FROM PRODUITS WHERE bio = 1");
    if (query.exec() && query.next()) {
        bioCount = query.value(0).toInt();
    }

    // Step 3: Calculate percentages
    double glutenFreePercentage = (glutenFreeCount * 100.0) / totalProducts;
    double sugarFreePercentage = (sugarFreeCount * 100.0) / totalProducts;
    double lactoseFreePercentage = (lactoseFreeCount * 100.0) / totalProducts;
    double organicPercentage = (organicCount * 100.0) / totalProducts;
    double bioPercentage = (bioCount * 100.0) / totalProducts;

    // Step 4: Display results
    QString message = QString(
                          "Statistics:\n"
                          "Gluten-Free: %1%\n"
                          "Sugar-Free: %2%\n"
                          "Lactose-Free: %3%\n"
                          "Organic: %4%\n"
                          "Bio: %5%")
                          .arg(glutenFreePercentage, 0, 'f', 2)
                          .arg(sugarFreePercentage, 0, 'f', 2)
                          .arg(lactoseFreePercentage, 0, 'f', 2)
                          .arg(organicPercentage, 0, 'f', 2)
                          .arg(bioPercentage, 0, 'f', 2);

    QMessageBox::information(this, "Product Statistics", message);
}

void MainWindow::pdf_generator() {
    // Step 1: Define the file path
    QString filePath = "C:/Users/MSI/Desktop/Validation Projet ney/projet/produits.pdf"; // Save in the user's home directory

    // Step 2: Setup PDF writer
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize::A4);
    pdfWriter.setResolution(300);

    QPainter painter(&pdfWriter);
    int yPosition = 100; // Initial position for text

    // Step 3: Title of the PDF
    QFont titleFont("Helvetica", 18, QFont::Bold);
    painter.setFont(titleFont);
    painter.drawText(50, yPosition, "Product Details");
    yPosition += 150;

    // Step 4: Table Header with more space
    QFont headerFont("Helvetica", 12, QFont::Bold);
    painter.setFont(headerFont);
    painter.drawText(50, yPosition, "Nom");
    painter.drawText(400, yPosition, "Date_Exp");
    painter.drawText(650, yPosition, "Prix");
    painter.drawText(1000, yPosition, "Stock");
    painter.drawText(1400, yPosition, "Lactose-Free");
    painter.drawText(1500, yPosition, "Bio");
    painter.drawText(1600, yPosition, "Sugar-Free");
    painter.drawText(1700, yPosition, "Gluten-Free");
    painter.drawText(1800, yPosition, "Eco");
    yPosition += 150;

    painter.drawLine(50, yPosition, 1000, yPosition); // Underline header
    yPosition += 150;

    // Step 5: Fetch Data from Database
    QSqlQuery query("SELECT nom_p, date_exp, prix, stock, lactose, bio, sugar, gluten, eco FROM PRODUITS");
    QFont dataFont("Helvetica", 10);
    painter.setFont(dataFont);

    while (query.next()) {
        // Fetch each column value
        QString nom_p = query.value(0).toString();
        QString date_exp = query.value(1).toString();
        double prix = query.value(2).toDouble();
        int stock = query.value(3).toInt();
        bool lactose = query.value(4).toBool();
        bool bio = query.value(5).toBool();
        bool sugar = query.value(6).toBool();
        bool gluten = query.value(7).toBool();
        bool eco = query.value(8).toBool();

        // Adjusting X positions for better spacing
        painter.drawText(50, yPosition, nom_p);
        painter.drawText(400, yPosition, date_exp);
        painter.drawText(650, yPosition, QString::number(prix, 'f', 2)); // 2 decimal places
        painter.drawText(1000, yPosition, QString::number(stock));
        painter.drawText(1400, yPosition, lactose ? "Yes" : "No");
        painter.drawText(1500, yPosition, bio ? "Yes" : "No");
        painter.drawText(1600, yPosition, sugar ? "Yes" : "No");
        painter.drawText(1700, yPosition, gluten ? "Yes" : "No");
        painter.drawText(1800, yPosition, eco ? "Yes" : "No");

        yPosition += 150;

        // Add a new page if needed
        if (yPosition > pdfWriter.height() - 50) {
            pdfWriter.newPage();
            yPosition = 100; // Reset y-position
        }
    }

    // Step 6: End the Painter and Notify
    painter.end();
    QMessageBox::information(this, "PDF Created", "PDF has been successfully created at:\n" + filePath);
    recordAction("Export PDF");

}


void MainWindow::verifier_stock()
{
    int qte = ui->qte_limite->text().toInt();
    if (qte<=0)
    {
        QMessageBox::warning(this, "Quantité invalide!", "Merci de choisir une quantité positive.");
    }
    else
    {
        QSqlQuery query;
        // SQL query to fetch products with stock < 10
        query.prepare("SELECT nom_p, stock FROM produits WHERE stock < :qte");
        query.bindValue(":qte", qte);
        if (!query.exec()) {
            qDebug() << "Erreur lors de l'exécution de la requête de vérification du stock:" << query.lastError().text();
            return;
        }

        QStringList lowStockProducts;
        while (query.next()) {
            QString productName = query.value(0).toString(); // nom_p
            int stock = query.value(1).toInt();             // stock
            lowStockProducts.append(QString("%1 (Stock: %2)").arg(productName).arg(stock));
        }

        if (!lowStockProducts.isEmpty()) {
            QString notificationMessage = "Attention! Les produits suivants ont un stock inférieur à 10:\n\n";
            notificationMessage += lowStockProducts.join("\n");

            // Show notification using QMessageBox
            QMessageBox::warning(this, "Alerte Stock Faible", notificationMessage);
        } else {
            qDebug() << "Tous les produits ont un stock suffisant.";
            QMessageBox::information(this, "No Low Stock", "Tous les produits ont un stock suffisant.");
        }
        recordAction("Verification Du Stock");

    }
}

void MainWindow::recordAction(const QString& actionType, const QString& productName)
{
    QFile file("C:/Users/MSI/Desktop/Validation Projet ney/projet/history.txt");

    // Vérification du chemin
    qDebug() << "File path: " << file.fileName();

    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Erreur", "Impossible d'ouvrir le fichier d'historique.");
        return;
    } else {
        qDebug() << "File opened successfully.";
    }

    QTextStream out(&file);

    // Log the details based on the action type
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << " | " << actionType;

    if (!productName.isEmpty()) {
        out << " | Product: " << productName;
    }
    out << "\n";

    // Flush and close
    out.flush();
    file.close();
}

void MainWindow::showHistory() {
    QFile file("C:/Users/MSI/Desktop/Validation Projet ney/projet/history.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier d'historique.");
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    // Set the content of the existing QTextEdit named 'historique'
    ui->historique->setPlainText(content);  // Assuming 'historique' is the name of your QTextEdit in the UI
}

void MainWindow::clearHistory() {
    QFile file("C:/Users/MSI/Desktop/Validation Projet ney/projet/history.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Erreur", "Impossible d'effacer l'historique.");
        return;
    }

    file.resize(0);  // Clear the file content
    file.close();

    // Clear the QTextEdit (historique)
    ui->historique->clear();

    QMessageBox::information(this, "Succès", "Historique effacé avec succès.");
}
