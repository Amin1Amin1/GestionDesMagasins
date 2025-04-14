#include "test.h"
#include <QDebug>
#include <QtSql/QSqlDatabase>

test::test() {
    qDebug() << QSqlDatabase::drivers();
}

int main() {
    test t;  // Création d'un objet test pour appeler le constructeur
    return 0;
}
