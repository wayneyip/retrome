#include <iostream>
#include <QApplication>
#include "datastore.h"
#include "item_parser.h"
#include "mainapplication.h"

using namespace std;

int main(int argc, char* argv[])
{
    if(argc < 2) {
        cerr << "Please specify a categories file" << endl;
        return 1;
    }

	DataStore* ds = new DataStore();
	ItemParser* itemParser = new ItemParser();
	bool categoryParsingError = itemParser->parseCategories(*ds, argv[1]);
	bool itemParsingError = itemParser->parseItems(*ds, "img", "sprites", "icons");
	if (categoryParsingError)
	{
		cerr << "Error parsing categories!" << endl;
	}
	else if (itemParsingError)
	{
		cerr << "Error parsing items!" << endl;
	}
	else
	{
		cout << "Successfully parsed all categories & items" << endl;
	}

	QApplication app(argc, argv);
	MainApplication main_app(ds);
	main_app.show();

	return app.exec();
}