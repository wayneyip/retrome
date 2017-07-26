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

	QPixmap setupAvatar();
	void getAvatarSize();
	void scaleAvatar(QPixmap& avatar);
	void updateAvatar();
	void paintSprite(QPixmap& avatar, std::string spriteName, Color* color);

	void PRINT(std::string statement);

private slots:
	void changeType();
	void changeCategory();
	void changeColor();
	void selectItem(QListWidgetItem* item);
	void randomSelect();
	void saveAvatar();
	void quit();

private:
	// Data members
	DataStore* ds_;
	std::vector<Item*> itemList_;
	std::vector<Color*> colorList_;
	QListWidgetItem* selectedItem_;

	// Overall layout
	QVBoxLayout* overallLayout;

	// First row: title
	QHBoxLayout* firstRowLayout;
	QLabel* titleContainer;

	// Second row: display + selection
	QHBoxLayout* secondRowLayout;
	
	// Second row left-side: display panel
	QVBoxLayout* avatarDisplayLayout;
	QLabel* avatarContainer;
	QSize AVATAR_SIZE;
	
	// Second row right-upper: category selection menu
	QVBoxLayout* selectionLayout;
	QTabWidget* selectionCategoryTabMenu;
	QListWidget* selectionCategoryBodyList;
	QListWidget* selectionCategoryClothingList;
	QListWidget* selectionCategoryPropsList;
	QListWidget* selectionCategoryBackdropList;

	// Second row right-lower: item selection menu
	QLabel* selectedCategoryLabel;
	QListWidget* selectionColorList;
	QListWidget* selectionItemList;

	// Third row: random select + save + quit 
	QHBoxLayout* thirdRowLayout;
	QPushButton* randomSelectButton;
	QPushButton* saveButton;
	QPushButton* quitButton;
};

#endif