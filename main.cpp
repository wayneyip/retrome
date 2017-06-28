#include <iostream>
#include <QApplication>
#include "datastore.h"
#include "item_parser.h"
#include "mainapplication.h"

using namespace std;

int main(int argc, char* argv[])
{
	DataStore* ds = new DataStore();
	ItemParser* itemParser = new ItemParser();
	bool success = itemParser->parse(*ds, "img", "sprites", "icons");
	if (success)
	{
		cout << "Successfully parsed all items" << endl;
	}
	QApplication app(argc, argv);
	MainApplication main_app(ds);
	main_app.show();

	return app.exec();
}