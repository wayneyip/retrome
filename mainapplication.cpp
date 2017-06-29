#include <QPainter>

#include "mainapplication.h"
#include <iostream>

MainApplication::MainApplication(DataStore* ds) :
	ds_(ds)
{
	initializeComponents();
	setupLayout();
	styleLayout();
	connectEvents();

	this->setLayout(overallLayout);
}

MainApplication::~MainApplication()
{

}

void MainApplication::initializeComponents()
{
	this->setWindowTitle("RetroMe");
	overallLayout = new QVBoxLayout();

	// First row: title
	firstRowLayout = new QHBoxLayout();
	titleLabel = new QLabel("RetroMe");

	// Second row: display + selection
	secondRowLayout = new QHBoxLayout();
	avatarDisplayLayout = new QVBoxLayout();
	avatarContainer = new QLabel();
	
	selectionLayout = new QVBoxLayout();
	selectionCategoryTabMenu = new QTabWidget();
	selectionCategoryBodyList = new QListWidget();

	selectionCategoryClothingList = new QListWidget();
	selectionCategoryPropsList = new QListWidget();
	selectionCategoryBackdropList = new QListWidget();

	selectedCategoryLabel = new QLabel("Hair");
	selectionItemList = new QListWidget();
	selectionItemList->setViewMode(QListWidget::IconMode);
	selectionItemList->setIconSize(QSize(200,200));
	selectionItemList->setResizeMode(QListWidget::Adjust);
	selectionItemList->setMovement(QListWidget::Static);
}

void MainApplication::setupLayout()
{
	// First row: title
	overallLayout->addLayout(firstRowLayout);
	firstRowLayout->addWidget(titleLabel);

	// Second row: avatar display + category/item selection
	overallLayout->addLayout(secondRowLayout);

	// Second row left-side: avatar display
	secondRowLayout->addLayout(avatarDisplayLayout);
	avatarDisplayLayout->addWidget(avatarContainer);

	// Second row right-upper: category selection
	secondRowLayout->addLayout(selectionLayout);
	selectionLayout->addWidget(selectionCategoryTabMenu);
	fillCategoryTabMenu();

	// Second row right-lower: item selection
	selectionLayout->addWidget(selectedCategoryLabel);
	selectionLayout->addWidget(selectionItemList);

	setupAvatar();
}

void MainApplication::fillCategoryTabMenu()
{
	selectionCategoryTabMenu->addTab(selectionCategoryBodyList, "Body");
	selectionCategoryTabMenu->addTab(selectionCategoryClothingList, "Clothing");
	selectionCategoryTabMenu->addTab(selectionCategoryPropsList, "Props");
	selectionCategoryTabMenu->addTab(selectionCategoryBackdropList, "Backdrop");

	selectionCategoryBodyList->addItem("Hair");
	selectionCategoryBodyList->addItem("Eyebrows");
	selectionCategoryBodyList->addItem("Eyes");
	selectionCategoryBodyList->addItem("Nose");
	selectionCategoryBodyList->addItem("Mouth");
	selectionCategoryBodyList->addItem("Ears");	
}

void MainApplication::styleLayout()
{
	
}

void MainApplication::connectEvents()
{
	connect(selectionCategoryBodyList, SIGNAL(currentRowChanged(int)), this, SLOT(changeCategory()));
	connect(selectionItemList, SIGNAL(currentRowChanged(int)), this, SLOT(selectItem()));

}

void MainApplication::changeCategory()
{
	// Clear the item list
	selectionItemList->clear();
	itemList_.clear();

	// Update category label
	selectedCategoryLabel->setText(selectionCategoryBodyList->currentItem()->text());
	
	// Get the item list for currently selected category
	std::string currentType 
		= selectionCategoryTabMenu->tabText(selectionCategoryTabMenu->currentIndex()).toStdString();
	std::string currentCategory
		= selectionCategoryBodyList->item(selectionCategoryBodyList->currentRow())->text().toStdString();

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
	// Do nothing if only category is changed
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
	
	// Get head and body sprites, then paint them on
	QPainter p(&avatar);
	QImage head("img/body_head_1.png");
	p.drawImage(QPoint(0, 0), head);
	QImage body("img/body_body_1.png");
	p.drawImage(QPoint(0, 0), body);
	p.end();

	// Scale up and display image
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
	QPixmap avatar = setupAvatar();

	// Paint each currently equipped item onto pixmap
	std::vector<std::string> equippedItems = ds_->getAllEquippedItems();
	for (unsigned int i=0; i < equippedItems.size(); i++)
	{
		paintSprite(avatar, equippedItems[i]);
	}
	scaleAvatar(avatar);
}

void MainApplication::paintSprite(QPixmap& avatar, std::string spriteName)
{
 	QPainter p(&avatar);
 	QImage sprite(QString::fromStdString(spriteName));
 	p.drawImage(QPoint(0, 0), sprite);
 	p.end();
 	std::cout << "painted " + spriteName << std::endl;
}