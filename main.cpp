#include "connection.h"
//#include "magasins.h"
#include "InterfacePer.h" // Ajoute l'interface principale
#include <QApplication>
#include <QMessageBox>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Connection c;
    bool test = c.createconnect();

    if (test) {
        QMessageBox::information(nullptr, QObject::tr("Connexion"),
                                 QObject::tr("Connexion réussie à la base !\nCliquez sur OK pour continuer."),
                                 QMessageBox::Ok);

        // Affichage de l'interface
        interfacePer w;
        w.show();

        return a.exec();
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Connexion"),
                              QObject::tr("Échec de connexion !\nCliquez sur Annuler pour quitter."),
                              QMessageBox::Cancel);
        return 1; // Quitter l'application en cas d'échec de connexion
    }
}
