#include <iostream>
#include <QApplication>
#include "datastore.h"
#include "item_parser.h"
#include "mainwindow.h"
#include "mainapplication.h"


using namespace std;

int main(int argc, char* argv[])
{
    string categories = "categories.txt";

	DataStore* ds = new DataStore();
	ItemParser* itemParser = new ItemParser();
	bool categoryParsingError = itemParser->parseCategories(*ds, categories);
	bool itemParsingError = itemParser->parseItems(*ds, "img", "sprites", "icons");
	if (categoryParsingError)
	{
		cerr << "Error parsing categories!" << endl;
		return 1;
	}
	else if (itemParsingError)
	{
		cerr << "Error parsing items!" << endl;
		return 1;
	}
	else
	{
		cout << "Successfully parsed all categories & items" << endl;
	}

	QApplication app(argc, argv);

    MainWindow main_app(ds);
	main_app.show();

	return app.exec();
}

