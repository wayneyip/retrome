#include <QApplication>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QPainter>
#include <QFont>

#include "mainapplication.h"
#include <iostream>

MainApplication::MainApplication(DataStore* ds) :
	ds_(ds)
{
	initializeComponents();
	setupLayout();
	styleLayout();
	connectEvents();

	this->setWindowTitle("RetroMe");
	this->setLayout(overallLayout);
}

MainApplication::~MainApplication()
{

}

void MainApplication::initializeComponents()
{
	overallLayout = new QVBoxLayout();

	// First row: title
	firstRowLayout = new QHBoxLayout();
	titleContainer = new QLabel();
	QImage title("img/title_1.png");
	// title = title.scaled(2 * title.size());
	titleContainer->setPixmap(QPixmap::fromImage(title));

	// Second row: avatar display + category/item selection
	secondRowLayout = new QHBoxLayout();

	// Second row left-side: avatar display
	avatarDisplayLayout = new QVBoxLayout();
	avatarContainer = new QLabel();
	
	// Second row right-upper: category selection
	selectionLayout = new QVBoxLayout();
	selectionCategoryTabMenu = new QTabWidget();
	selectionCategoryBodyList = new QListWidget();
	selectionCategoryClothingList = new QListWidget();
	selectionCategoryPropsList = new QListWidget();
	selectionCategoryBackdropList = new QListWidget();

	// Second row right-lower: item selection
	selectedCategoryLabel = new QLabel("");
	selectionItemList = new QListWidget();

	// Third row: save + quit
	thirdRowLayout = new QHBoxLayout();
	saveButton = new QPushButton("Save Avatar");
	quitButton = new QPushButton("Quit");
}

void MainApplication::setupLayout()
{
	// First row: title
	overallLayout->addLayout(firstRowLayout);
	firstRowLayout->addWidget(titleContainer);

	// Second row: avatar display + category/item selection
	overallLayout->addLayout(secondRowLayout);

	// Second row left-side: avatar display
	secondRowLayout->addLayout(avatarDisplayLayout);
	avatarDisplayLayout->addWidget(avatarContainer);
	setupAvatar();

	// Second row right-upper: category selection
	secondRowLayout->addLayout(selectionLayout);
	selectionLayout->addWidget(selectionCategoryTabMenu);
	fillCategoryTabMenu();

	// Second row right-lower: item selection
	selectionLayout->addWidget(selectedCategoryLabel);
	selectionLayout->addWidget(selectionItemList);
	selectionItemList->setViewMode(QListWidget::IconMode);
	selectionItemList->setIconSize(QSize(200,200));
	selectionItemList->setResizeMode(QListWidget::Adjust);
	selectionItemList->setMovement(QListWidget::Static);

	// Third row: save + quit	
	overallLayout->addLayout(thirdRowLayout);
	thirdRowLayout->addWidget(saveButton);
	thirdRowLayout->addWidget(quitButton);
}

void MainApplication::fillCategoryTabMenu()
{
	selectionCategoryTabMenu->addTab(selectionCategoryBodyList, "Body");
	selectionCategoryBodyList->addItem("Hair");
	selectionCategoryBodyList->addItem("Eyebrows");
	selectionCategoryBodyList->addItem("Eyes");
	selectionCategoryBodyList->addItem("Nose");
	selectionCategoryBodyList->addItem("Mouth");
	selectionCategoryBodyList->addItem("Ears");	

	selectionCategoryTabMenu->addTab(selectionCategoryClothingList, "Clothing");
	selectionCategoryClothingList->addItem("Top");
	selectionCategoryClothingList->addItem("Bottom");
	selectionCategoryClothingList->addItem("Shoes");

	selectionCategoryTabMenu->addTab(selectionCategoryPropsList, "Props");
	selectionCategoryTabMenu->addTab(selectionCategoryBackdropList, "Backdrop");
}

void MainApplication::styleLayout()
{
	// Center app on screen
	QRect screenGeometry = QApplication::desktop()->screenGeometry();
	int x = (screenGeometry.width()-this->width()) / 2;
	int y = (screenGeometry.height()-this->height()) / 2;
	this->move(x, y);

	// Center title
	firstRowLayout->setAlignment(titleContainer, Qt::AlignCenter);


}

void MainApplication::connectEvents()
{
	connect(selectionCategoryTabMenu, SIGNAL(currentChanged(int)), this, SLOT(changeType()));
	connect(selectionCategoryBodyList, SIGNAL(currentRowChanged(int)), this, SLOT(changeCategory()));
	connect(selectionCategoryClothingList, SIGNAL(currentRowChanged(int)), this, SLOT(changeCategory()));
	connect(selectionItemList, SIGNAL(currentRowChanged(int)), this, SLOT(selectItem()));
	connect(saveButton, SIGNAL(clicked()), this, SLOT(saveAvatar()));
	connect(quitButton, SIGNAL(clicked()), this, SLOT(quit()));

}

void MainApplication::changeType()
{
	changeCategory();
}

void MainApplication::changeCategory()
{
	// Get currently active type
	QListWidget* categoryList = (QListWidget*)selectionCategoryTabMenu->currentWidget();

	// Clear the item list
	selectionItemList->clear();
	itemList_.clear();

	// Do nothing if current type has no category selected 
	if (categoryList->currentItem() == NULL) return;

	// Update category label
	selectedCategoryLabel->setText(categoryList->currentItem()->text());
	
	// Get the item list for currently selected category
	std::string currentType 
		= selectionCategoryTabMenu->tabText(selectionCategoryTabMenu->currentIndex()).toStdString();
	std::string currentCategory
		= categoryList->item(categoryList->currentRow())->text().toStdString();

	itemList_ = ds_->getCategoryItems(currentType, currentCategory);

	// Display each item's icons
	for (unsigned int i=0; i < itemList_.size(); i++)
	{
		selectionItemList->addItem(
			new QListWidgetItem(QIcon(QString::fromStdString(itemList_[i]->getIconName())), NULL));
	}

	// Highlight the item in this category currently equipped by avatar, if any
	Item* equippedItem = ds_->findEquippedItem(currentType, currentCategory);
	if (equippedItem)
	{
		for (unsigned int i=0; i < itemList_.size(); i++)
		{
			if (itemList_[i] == equippedItem)
			{
				selectionItemList->setCurrentRow(i);
			}
		}
	}
}

void MainApplication::selectItem()
{
	// Do nothing if only the category was changed
	if (selectionItemList->currentRow() < 0) return;

	ds_->selectItem(itemList_[selectionItemList->currentRow()]);

	updateAvatar();
}

QPixmap MainApplication::setupAvatar()
{
	// Set up transparent pixmap
	getAvatarSize();
	QPixmap avatar(AVATAR_SIZE);
	avatar.fill(Qt::transparent);
	
	// Get body and head sprites, then paint them on
	QPainter p(&avatar);
	QImage body("img/body_body_1.png");
	p.drawImage(QPoint(0, 0), body);
	QImage head("img/body_head_1.png");
	p.drawImage(QPoint(0, 0), head);
	p.end();

	// Scale up avatar for display
	scaleAvatar(avatar);
	return avatar;
}

void MainApplication::getAvatarSize()
{
	QImage body("img/body_body_1.png");
	AVATAR_SIZE = body.size();
}

void MainApplication::scaleAvatar(QPixmap& avatar)
{
	QPixmap avatar_scaled;
	avatar_scaled.fill(Qt::transparent);
	avatar_scaled = avatar.scaled(2*AVATAR_SIZE, Qt::KeepAspectRatio);
	avatarContainer->setPixmap(avatar_scaled);
}

void MainApplication::updateAvatar()
{
	// Set up avatar's head and body
	QPixmap avatar = setupAvatar();

	// Paint each currently equipped item onto avatar
	DataStore::equippedItemHeap equippedItems = ds_->getAllEquippedItems();
	int equippedItemCount = equippedItems.size();
	for (int i=0; i < equippedItemCount; i++)
	{
		std::string equippedItem = equippedItems.top()->getSpriteName();
		paintSprite(avatar, equippedItem);
		equippedItems.pop();
	}

	// Paint right hand onto avatar
 	QPainter p(&avatar);
 	QImage sprite("img/body_hand_1.png");
 	p.drawImage(QPoint(0, 0), sprite);
 	p.end();	

	// Scale up avatar for display
	scaleAvatar(avatar);
}

void MainApplication::paintSprite(QPixmap& avatar, std::string spriteName)
{
	QPainter p(&avatar);
	QImage sprite(QString::fromStdString(spriteName));
	p.drawImage(QPoint(0, 0), sprite);
	p.end();
}

void MainApplication::saveAvatar()
{
	// Ensure user provides filename in PNG
	QString filename = QFileDialog::getSaveFileName();
	if (! filename.endsWith(".png", Qt::CaseInsensitive))
	{
		filename = filename + ".png";
	}

	// Resize avatar pixmap (to regular size) and save
	QPixmap avatarToSave = avatarContainer->pixmap()
							->scaled(AVATAR_SIZE, Qt::KeepAspectRatio);
	avatarToSave.save(filename, "PNG");
}

void MainApplication::quit()
{
	QApplication::exit();
}

void MainApplication::PRINT(std::string statement)
{
	std::cout << statement << std::endl;
}