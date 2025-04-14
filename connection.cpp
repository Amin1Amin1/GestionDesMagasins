#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include "connection.h"

Connection::Connection() {}

bool Connection::createconnect() {
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    db.setDatabaseName("Source_Projet2A");
    db.setUserName("amin");
    db.setPassword("admin");

    if (db.open()) {
        test = true;
        qDebug() << " Connexion réussie à la base de données !";
    } else {
        qDebug() << " Erreur de connexion : " << db.lastError().text();
        qDebug() << "Vérifiez que votre DSN est bien configuré dans ODBC.";
    }

    return test;
}

void Connection::closeConnection(){db.close();}
