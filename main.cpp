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
	bool parsedCategories = itemParser->parseCategories(*ds, argv[1]);
	bool parsedItems = itemParser->parseItems(*ds, "img", "sprites", "icons");
	if (parsedCategories && parsedItems)
	{
		cout << "Successfully parsed all categories & items" << endl;
	}
	QApplication app(argc, argv);
	MainApplication main_app(ds);
	main_app.show();

	return app.exec();
}