#ifndef INTERFACEPER_H
#define INTERFACEPER_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QTime>
#include <QDate>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QProcess> // Ajout pour QProcess
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui {
class interfacePer;
}
QT_END_NAMESPACE

class interfacePer : public QMainWindow
{
    Q_OBJECT

public:
    explicit interfacePer(QWidget *parent = nullptr);
    ~interfacePer();

public slots:
    void readSerialData(); // Lire les données de l'Arduino
    void on_pushButton_afficherCabines_clicked(); // Afficher l'historique
    void on_pushButton9_ajouter_clicked();
    void on_pushButton_10_afficher_clicked();
    void on_pushButton_12_supprimer_clicked();
    void on_pushButton_modifier_clicked();
    void on_pushButton_rechercher_clicked();
    void on_pushButton_trier_clicked();
    void on_pushButton_pdf_clicked();
    void on_pushButton_statistique_clicked();
    void on_calendarWidget_selectionChanged();
    void on_pushButton_ajouterCommande_clicked();
    void on_pushButton_supprimerCommande_clicked();
    void on_pushButton_modifierCommande_clicked();
    void on_pushButton_recommendProducts_clicked();
    void handleRecommendationReply(QNetworkReply *reply);
    void handleRecommendationReplySimulated(const QByteArray &response, const QStringList &produits); // Nouvelle déclaration

private:
    Ui::interfacePer *ui;
    QNetworkAccessManager *networkManager;
    QSerialPort *serialPort; // Port série pour Arduino
    void setupSerialPort();
    void afficherLesCommandesParDate(const QDate& date);
    QSqlQueryModel* obtenirChiffreAffairesParMagasin();
    void afficherChiffreAffairesParMagasin();
    QSqlQueryModel* obtenirChiffreAffairesParRegion();
    void afficherChiffreAffairesParRegion();
};

#endif // INTERFACEPER_H
