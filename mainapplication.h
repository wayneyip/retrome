#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include "datastore.h"

class MainApplication : public QWidget
{
	Q_OBJECT

public:
	MainApplication(DataStore* ds);
	~MainApplication();
	void initializeComponents();
	void setupLayout();
	void fillCategoryTabMenu();
	void styleLayout();
	void connectEvents();
	void setupAvatar();
	void updateAvatar();

private slots:
	void changeCategory();
	void selectItem();

private:
	// Data members
	DataStore* ds_;
	std::vector<Item*> itemList_;

	// Overall layout
	QVBoxLayout* overallLayout;

	// First row: title
	QHBoxLayout* firstRowLayout;
	QLabel* titleLabel;

	// Second row: display + selection
	QHBoxLayout* secondRowLayout;
	
	// Second row left-side: display panel
	QVBoxLayout* avatarDisplayLayout;
	QLabel* avatarContainer;
	
	// Second row right-upper: category selection menu
	QVBoxLayout* selectionLayout;
	QTabWidget* selectionCategoryTabMenu;
	QListWidget* selectionCategoryBodyList;
	QListWidget* selectionCategoryClothingList;
	QListWidget* selectionCategoryPropsList;
	QListWidget* selectionCategoryBackdropList;

	// Second row right-lower: item selection menu
	QLabel* selectedCategoryLabel;
	QListWidget* selectionItemList;
};

#endif