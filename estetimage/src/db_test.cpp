#include <QCoreApplication>
#include <QtSql>
#include <iostream>

#define q2c(string) string.toStdString()

int main(int countArg, char **listArg)
{
    QCoreApplication app(countArg, listArg);
    std::cout << std::flush;

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
	db.setUserName("root");
	db.setPassword("      ");
	db.setDatabaseName("tuto");
	if(db.open())
	{
	    std::cout << "Vous êtes maintenant connecté à " << q2c(db.hostName()) << std::endl;
	    db.close();
	}
	else
	{
	    std::cout << "La connexion a échouée, désolé" << std::endl;
	}

    return app.exec();
}